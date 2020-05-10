#include <stdio.h>
#include <stdlib.h>
#include "../libopencm3/include/libopencm3/stm32/i2c.h"
#include "../libopencm3/include/libopencm3/stm32/gpio.h"
#include "board.h"
#include "disp.h"

void i2c_write_w_ones(uint32_t i2c, int addr, uint8_t *data, uint8_t n, uint16_t fillSize);

void sdInit()
{
    gpio_set(DISPLAY_PORT, DISP_RESET);
    // инициализация интерфейса
    i2c_set_clock_frequency(DISPI2C, I2C_CR2_FREQ_36MHZ);
    i2c_set_standard_mode(DISPI2C);
    i2c_set_ccr(DISPI2C, (uint16_t)0xb4);
    i2c_set_trise(DISPI2C, 36+1);

    // i2c_set_clock_frequency(DISPI2C, I2C_CR2_FREQ_36MHZ); // 400кГц наверно не схавает
    // i2c_set_fast_mode(DISPI2C);
    // i2c_set_ccr(DISPI2C, (uint16_t)0x1e);
    // i2c_set_dutycycle(DISPI2C, I2C_CCR_DUTY_DIV2);
    // i2c_set_trise(DISPI2C, 36+1);

    i2c_set_own_7bit_slave_address(DISPI2C, (uint8_t)SD_ADDR);
    i2c_peripheral_enable(DISPI2C);

    // нужно сбросить перед подачей команд
    gpio_clear(DISPLAY_PORT, DISP_RESET);
    volatile uint32_t cnt = SYSCLKHZ/1e3;
    while( (--cnt) );
    gpio_set(DISPLAY_PORT, DISP_RESET);
    // инициализация экрана (передача всех необходимых параметров)

    #define initSize    41
    uint8_t initSeq[initSize] = { (uint8_t)COMMAND_CONTROL_BYTE,
                                  (uint8_t)SET_MUX_RATIO,
                                  (uint8_t)MIN_MUX_RATIO,
                                  (uint8_t)COMMAND_CONTROL_BYTE,
                                  (uint8_t)SET_DISPLAY_CLOCK,
                                  (uint8_t)DISPLAY_MAX_FREQ,
                                  (uint8_t)COMMAND_CONTROL_BYTE,
                                        (uint8_t)SET_DISPLAY_OFFSET,
                                        (uint8_t)DISP_NULL,
                                        (uint8_t)COMMAND_CONTROL_BYTE,
                                        (uint8_t)SET_NULL_START_LINE,
                                        (uint8_t)COMMAND_CONTROL_BYTE,
                                        (uint8_t)SET_NULL_PAGE,
                                        (uint8_t)COMMAND_CONTROL_BYTE,
                                        (uint8_t)NO_COL_REMAP,
                                        (uint8_t)COMMAND_CONTROL_BYTE,
                                        (uint8_t)SET_NORMAL_SCAN_DIR,
                                        (uint8_t)COMMAND_CONTROL_BYTE,
                                        (uint8_t)SET_COM_PIN_CONFIG,
                                        (uint8_t)DEFAULT_COM_PIN,
                                        (uint8_t)COMMAND_CONTROL_BYTE,
                                        (uint8_t)SET_CONTRAST,
                                        (uint8_t)INIT_CONTRAST,
                                        (uint8_t)COMMAND_CONTROL_BYTE,
                                        (uint8_t)SET_DESELECT_LEVEL,
                                        (uint8_t)DEFAULT_DESELECT,
                                        (uint8_t)COMMAND_CONTROL_BYTE,
                                        (uint8_t)SET_MEMORY_ADDRESSING_MODE,
                                        (uint8_t)HORISONTAL_ADDRESSING,
                                        (uint8_t)COMMAND_CONTROL_BYTE,
                                        (uint8_t)SET_DISPLAY_PRE_CHARGE,
                                        (uint8_t)DEFAULT_PRE_CHARGE,
                                        (uint8_t)COMMAND_CONTROL_BYTE,
                                        (uint8_t)DISPLAY_ON,
                                        (uint8_t)COMMAND_CONTROL_BYTE,
                                        (uint8_t)SET_NON_INVERSE,
                                        (uint8_t)COMMAND_CONTROL_BYTE,
                                        (uint8_t)SET_CHARGE_PUMP,
                                        (uint8_t)ENABLE_CHARGE_PUMP,
                                        (uint8_t)COMMAND_CONTROL_BYTE,
                                        (uint8_t)SET_NORMAL_MODE };
    uint8_t empty;
    i2c_transfer7(DISPI2C, SD_ADDR, initSeq, initSize, &empty, 0);
//             dma send seq
}

/*
void fullSizeXbmToDispHorAddr(uint8_t *in, uint8_t *out)
{
    const uint8_t xCol = 16;
    uint8_t outN = 0;
    uint8_t inN = 0;
    for(int page=0 ; page<DISP_PAGE ; ++page)
    {
        for(int col=0 ; col<DISP_COL ; ++col) {
            outN = page*DISP_COL+col;
            inN  = xCol*page*8;
            for(int i=0; i<8; ++i) out[outN] |= in[inN]
        }
    }
    for(int i=0 ; i<DISP_RAM_SIZE ; ++i) {
        for(int j=0 ; j<8 ; ++j) dispBuffer[i] |= array[xCol*j + i] & (1 << );
    }
    ;
}

void dispTransfer()
{
    ;
}

*/

void flashlight()
{
    const uint8_t seqSize = 4;
    uint8_t flashOnSeq[seqSize];
    flashOnSeq[0] = (uint8_t)COMMAND_CONTROL_BYTE;
    flashOnSeq[1] = (uint8_t)SET_CONTRAST;
    flashOnSeq[2] = (uint8_t)MAX_CONTRAST;
    flashOnSeq[3] = (uint8_t)RAM_CONTROL_BYTE;
    i2c_write_w_ones(DISPI2C, SD_ADDR, flashOnSeq, seqSize, DISP_RAM_SIZE);
}

void i2c_write_w_ones(uint32_t i2c, int addr, uint8_t *data, uint8_t n, uint16_t fillSize)
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
    for (uint16_t i = 0; i<fillSize; i++) {
        i2c_send_data(i2c, 0xff);
        while (!(I2C_SR1(i2c) & (I2C_SR1_BTF)));
    }
    i2c_send_stop(i2c);
}
