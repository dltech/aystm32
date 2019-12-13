#include <stdio.h>
#include <stdlib.h>
#include "i2c.h"
#include "gpio.h"
#include "disp.h"

void i2c_write_w_nulls(uint32_t i2c, int addr, uint8_t *data, uint8_t n, uint16_t nullN);

void sdInit()
{
    gpio_set(DISPLAY_PORT, RESET_PIN);
    // инициализация интерфейса
    i2c_set_7bit_addr_mode(DISPI2C);
    i2c_set_7bit_address(DISPI2C, (uint8_t)SD_ADDR);
    i2c_set_speed(DISPI2C, i2c_speed_fm_100k, SYSCLKHZ/1e6);
    i2c_disable_stretching(DISPI2C);
    i2c_peripheral_enable(DISPI2C);
    // нужно сбросить перед подачей команд
    gpio_clear(DISPLAY_PORT, RESET_PIN);
    volatile uint32_t cnt = SYSCLKHZ/1e3;
    while( (--cnt) );
    gpio_set(DISPLAY_PORT, RESET_PIN);
    // инициализация экрана (передача всех необходимых параметров)
    uint8_t empty;
    i2c_transfer7(DISPI2C, SD_ADDR, initSeq, initSize, *empty, 0);
//             dma send seq
}

void fromDotMatrixToData()
{
    for(int i=0 ; i<BUFFER_SIZE)
}

void flashlight()
{
    const uint8_t seqSize = 4;
    const uint8_t flashOnSeq[seqSize] = {(uint8_t)COMMAND_CONTROL_BYTE,
                                         (uint8_t)SET_CONTRAST,
                                         (uint8_t)MAX_CONTRAST,
                                         (uint8_t)RAM_CONTROL_BYTE};
    i2c_write_w_nulls(DISPI2C, SD_ADDR, flashOnSeq, seqSize, DISP_RAM_SIZE);
}

void i2c_write_w_nulls(uint32_t i2c, int addr, uint8_t *data, uint8_t n, uint16_t nullN)
{
    while ((I2C_SR2(i2c) & I2C_SR2_BUSY)) {
    }
    i2c_send_start(i2c);
    /* Wait for master mode selected */
    while (!((I2C_SR1(i2c) & I2C_SR1_SB)
           & (I2C_SR2(i2c) & (I2C_SR2_MSL | I2C_SR2_BUSY))));

    i2c_send_7bit_address(i2c, addr, I2C_WRITE);

    /* Waiting for address is transferred. */
    while (!(I2C_SR1(i2c) & I2C_SR1_ADDR));

    /* Clearing ADDR condition sequence. */
    (void)I2C_SR2(i2c);

    for (uint8_t i = 0; i < n; i++) {
           i2c_send_data(i2c, data[i]);
           while (!(I2C_SR1(i2c) & (I2C_SR1_BTF)));
    }
    for (uint16_t i = 0; i<nullN; i++) {);
        i2c_send_data(i2c, NULL);
        while (!(I2C_SR1(i2c) & (I2C_SR1_BTF)));
    }
    i2c_send_stop(i2c);
}
