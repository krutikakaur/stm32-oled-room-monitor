#include "ssd1306.h"

extern I2C_HandleTypeDef hi2c1;

#define SSD1306_ADDR (0x3C << 1)

static uint8_t fb[SSD1306_WIDTH * SSD1306_HEIGHT / 8];

/* Control byte 0x00 = command, 0x40 = data */
static HAL_StatusTypeDef cmd(uint8_t c)
{
    return HAL_I2C_Mem_Write(&hi2c1, SSD1306_ADDR, 0x00, 1, &c, 1, 100);
}

HAL_StatusTypeDef ssd1306_init(void)
{
    HAL_Delay(100); /* let the display power up */

    static const uint8_t seq[] = {
        0xAE,             /* display off */
        0xD5, 0x80,       /* clock divide / oscillator */
        0xA8, 0x3F,       /* multiplex ratio: 64 rows */
        0xD3, 0x00,       /* display offset */
        0x40,             /* start line 0 */
        0x8D, 0x14,       /* charge pump on */
        0x20, 0x00,       /* horizontal addressing mode */
        0xA1,             /* segment remap */
        0xC8,             /* COM scan direction */
        0xDA, 0x12,       /* COM pins config */
        0x81, 0xCF,       /* contrast */
        0xD9, 0xF1,       /* pre-charge */
        0xDB, 0x40,       /* VCOMH level */
        0xA4,             /* show RAM contents */
        0xA6,             /* normal (not inverted) */
        0xAF              /* display on */
    };

    for (uint32_t i = 0; i < sizeof(seq); i++)
    {
        HAL_StatusTypeDef r = cmd(seq[i]);
        if (r != HAL_OK) return r;
    }

    ssd1306_fill(0);
    return ssd1306_update();
}

void ssd1306_fill(uint8_t on)
{
    for (uint32_t i = 0; i < sizeof(fb); i++)
        fb[i] = on ? 0xFF : 0x00;
}

void ssd1306_set_pixel(uint8_t x, uint8_t y, uint8_t on)
{
    if (x >= SSD1306_WIDTH || y >= SSD1306_HEIGHT) return;
    uint16_t idx = x + (y / 8) * SSD1306_WIDTH;
    if (on) fb[idx] |=  (1 << (y % 8));
    else    fb[idx] &= ~(1 << (y % 8));
}

HAL_StatusTypeDef ssd1306_update(void)
{
    HAL_StatusTypeDef r;
    if ((r = cmd(0x21)) != HAL_OK) return r; /* column range */
    if ((r = cmd(0x00)) != HAL_OK) return r;
    if ((r = cmd(SSD1306_WIDTH - 1)) != HAL_OK) return r;
    if ((r = cmd(0x22)) != HAL_OK) return r; /* page range */
    if ((r = cmd(0x00)) != HAL_OK) return r;
    if ((r = cmd(7)) != HAL_OK) return r;

    return HAL_I2C_Mem_Write(&hi2c1, SSD1306_ADDR, 0x40, 1,
                             fb, sizeof(fb), 1000);
}
