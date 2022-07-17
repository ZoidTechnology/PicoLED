#ifndef LED_H
#define LED_H

#include "hardware/pio.h"

void led_init(PIO pio, uint sm);
void led_write(PIO pio, uint sm, uint pin, uint8_t *data, uint length);

#endif