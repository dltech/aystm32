#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "board.h"
#include "lm4811.h"
#include "ayconn.h"

#include "../libopencm3/include/libopencm3/stm32/pwr.h"
#include "../libopencm3/include/libopencm3/stm32/gpio.h"
#include "../libopencm3/include/libopencm3/stm32/rcc.h"
#include "../libopencm3/include/libopencm3/stm32/f1/bkp.h"
#include "../libopencm3/include/libopencm3/stm32/timer.h"

int main(void)
{
    boardInit();
    initAy();
    init4811(16);

// отладка чтоб пищало
    aySendBlocking(5, 0x01);
    rough_delay_us(1000);
    aySendBlocking(4, 0xff);
    rough_delay_us(1000);
    aySendBlocking(3, 0x01);
    rough_delay_us(1000);
    aySendBlocking(2, 0xff);
    rough_delay_us(1000);
    aySendBlocking(1, 0x01);
    rough_delay_us(1000);
    aySendBlocking(0, 0xff);
    rough_delay_us(1000);
    aySendBlocking(10, 0x00);
    rough_delay_us(1000);
    aySendBlocking(11, 0x00);
    rough_delay_us(1000);
    aySendBlocking(12, 0x00);
    rough_delay_us(1000);
    aySendBlocking(7, 0xf8);
    rough_delay_us(1000);
    aySendBlocking(0xd, 0x0d);
    rough_delay_us(1000);

    aySendBlocking(0xb, 0x01);
    rough_delay_us(1000);
    aySendBlocking(0xc, 0x01);
    rough_delay_us(1000);

    aySendBlocking(0x8, 0x10);
    rough_delay_us(1000);
    aySendBlocking(0x9, 0x10);
    rough_delay_us(1000);
    aySendBlocking(0xa, 0x10);
    rough_delay_us(1000);

    while(1);
}
