#include "i2c.h"
#include "gpio.h"


void sdInit()
{
    uint8_t empty;
    const uint8_t initSize = 5;
    const uint8_t initSeq[initSize] = { (uint8_t)COMMAND_CONTROL_BYTE,
                                        (uint8_t)
                           (uint8_t)
                                        }
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
    i2c_transfer7(DISPI2C, SD_ADDR, initSeq, initSize, *empty, 0)
//             dma send seq
//             обработчик по окончанию (так же структура состояние отслеживать придется или по регистрам только удастся если да то круто)
}

void fromDotMatrixToData()
{
    for(int i=0 ; i<BUFFER_SIZE)
}

void flashlight()
{

}
