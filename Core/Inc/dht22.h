#ifndef DHT22_H
#define DHT22_H

#include "main.h"

typedef enum {
    DHT22_OK = 0,
    DHT22_ERR_TIMEOUT,
    DHT22_ERR_CHECKSUM
} dht22_status_t;

void dht22_init(void);

/* Values are in tenths: 253 = 25.3 degC / 25.3 %RH */
dht22_status_t dht22_read(int16_t *temp_x10, uint16_t *hum_x10);

#endif
