#include "dht22.h"

extern TIM_HandleTypeDef htim2;   /* must run at 1 MHz (prescaler 83) */

#define DHT_PORT GPIOA
#define DHT_PIN  GPIO_PIN_1

static inline uint32_t now_us(void)
{
    return __HAL_TIM_GET_COUNTER(&htim2);
}

/* Wait for the pin to reach 'level'. Returns microseconds waited, or -1 on timeout. */
static int wait_level(GPIO_PinState level, uint32_t timeout_us)
{
    uint32_t start = now_us();
    while (HAL_GPIO_ReadPin(DHT_PORT, DHT_PIN) != level)
    {
        if ((now_us() - start) > timeout_us) return -1;
    }
    return (int)(now_us() - start);
}

void dht22_init(void)
{
    /* Open-drain output: writing 1 releases the line, and we can still read it */
    GPIO_InitTypeDef g = {0};
    HAL_GPIO_WritePin(DHT_PORT, DHT_PIN, GPIO_PIN_SET);
    g.Pin   = DHT_PIN;
    g.Mode  = GPIO_MODE_OUTPUT_OD;
    g.Pull  = GPIO_NOPULL;
    g.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(DHT_PORT, &g);

    HAL_TIM_Base_Start(&htim2);
}

dht22_status_t dht22_read(int16_t *temp_x10, uint16_t *hum_x10)
{
    uint8_t d[5] = {0};

    /* Start signal: pull low for more than 1 ms, then release */
    HAL_GPIO_WritePin(DHT_PORT, DHT_PIN, GPIO_PIN_RESET);
    HAL_Delay(2);

    __disable_irq();   /* timing-critical for about 5 ms */
    HAL_GPIO_WritePin(DHT_PORT, DHT_PIN, GPIO_PIN_SET);

    /* Sensor response: low ~80 us, high ~80 us, then the first bit's low phase */
    if (wait_level(GPIO_PIN_RESET, 200) < 0) goto timeout;
    if (wait_level(GPIO_PIN_SET,   150) < 0) goto timeout;
    if (wait_level(GPIO_PIN_RESET, 150) < 0) goto timeout;

    /* 40 bits: each is ~50 us low, then a high of ~27 us (0) or ~70 us (1) */
    for (int i = 0; i < 40; i++)
    {
        if (wait_level(GPIO_PIN_SET, 100) < 0) goto timeout;
        int high_us = wait_level(GPIO_PIN_RESET, 150);
        if (high_us < 0) goto timeout;

        d[i / 8] <<= 1;
        if (high_us > 50) d[i / 8] |= 1;
    }
    __enable_irq();

    /* Checksum: low 8 bits of the sum of the first four bytes */
    if (((d[0] + d[1] + d[2] + d[3]) & 0xFF) != d[4])
        return DHT22_ERR_CHECKSUM;

    *hum_x10 = ((uint16_t)d[0] << 8) | d[1];

    int16_t t = ((uint16_t)(d[2] & 0x7F) << 8) | d[3];
    if (d[2] & 0x80) t = -t;        /* top bit is the sign */
    *temp_x10 = t;

    return DHT22_OK;

timeout:
    __enable_irq();
    return DHT22_ERR_TIMEOUT;
}
