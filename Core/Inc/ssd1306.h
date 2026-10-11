#ifndef SSD1306_H
#define SSD1306_H

#include "main.h"

#define SSD1306_WIDTH  128
#define SSD1306_HEIGHT 64

HAL_StatusTypeDef ssd1306_init(void);
void ssd1306_fill(uint8_t on);
void ssd1306_set_pixel(uint8_t x, uint8_t y, uint8_t on);
HAL_StatusTypeDef ssd1306_update(void);

#endif
