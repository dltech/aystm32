#include "inttypes.h"

#include "../libopencm3/include/libopencm3/stm32/gpio.h"
#include "../libopencm3/include/libopencm3/stm32/rcc.h"
#include "../libopencm3/include/libopencm3/stm32/timer.h"
#include "../libopencm3/include/libopencm3/stm32/dma.h"

#include "board.h"
#include "ayconn.h"


volatile struct ayStruct {
    uint32_t addr;
    uint32_t data;
} ayData = {0,0};

void initAy()
{
    // сперва порты
    gpio_set_mode(AY_DATA_PORT, GPIO_MODE_OUTPUT_50_MHZ, GPIO_CNF_OUTPUT_PUSHPULL, AY_DATA_IOS);
    gpio_set_mode(AY_SERV_PORT, GPIO_MODE_OUTPUT_50_MHZ, GPIO_CNF_OUTPUT_PUSHPULL, AY_BC1 | AY_BDIR | AY_RESET);
    gpio_set_mode(AY_DATA_PORT, GPIO_MODE_OUTPUT_50_MHZ, GPIO_CNF_OUTPUT_ALTFN_PUSHPULL, AY_CLOCK);

    // таймер для лапки тактирования и запуск передачи данных по тактам
    TIM1_CR1   = (uint32_t) TIM_CR1_CKD_CK_INT;
//    TIM1_CR2   = (uint32_t) TIM_CR2_CCDS;
    TIM1_PSC   = (uint32_t) SYSCLKHZ/AYHZ/1024 - 1;
    TIM1_ARR   = (uint32_t) 1024 - 1;
    TIM1_CCR2  = (uint32_t) 512;
//    TIM1_DIER  = (uint32_t) TIM_DIER_UDE;
    TIM1_CCER  = (uint32_t) TIM_CCER_CC2NE;
    TIM1_CCMR1 = (uint32_t) (TIM_CCMR1_OC2M_PWM2 | TIM_CCMR1_OC1PE);
    TIM1_BDTR  = (uint32_t) TIM_BDTR_MOE;
    TIM1_CR1  |= (uint32_t) TIM_CR1_CEN;

    // DMA для данных
    DMA1_CPAR2  = (uint32_t) &GPIOA_BSRR;
    DMA1_CMAR2  = (uint32_t) &ayData;
    DMA1_CNDTR2 = (uint32_t) 2;
    // конфиг DMA
    uint32_t ccr = DMA_CCR_MINC | DMA_CCR_MSIZE_32BIT | DMA_CCR_PSIZE_32BIT;
    ccr |= DMA_CCR_PL_LOW | DMA_CCR_DIR | DMA_CCR_CIRC;
    ccr |= DMA_CCR_TEIE;
    DMA1_CCR3 = ccr;
}

/* отправка данных в AY, естесственно без контроля, если предыдущие данные
   не ушли, то они пропали */
void aySend(uint8_t addr, uint8_t data)
{
    ayData.addr = 0xffff0000 | (0x000000ff & (uint32_t)addr);
    ayData.data = 0xffff0000 | (0x000000ff & (uint32_t)data);
}
