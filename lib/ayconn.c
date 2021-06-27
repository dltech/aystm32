#include "inttypes.h"

#include "../libopencm3/include/libopencm3/stm32/gpio.h"
#include "../libopencm3/include/libopencm3/stm32/timer.h"
#include "../libopencm3/include/libopencm3/stm32/f1/nvic.h"

#include "board.h"
#include "ayconn.h"

enum ayStates {
    LATCH_ADDR = 1,
    WRITE_DELAY,
    WRITE,
    READ,
    END,
    COMPLETE
};

volatile struct ayStruct {
    uint8_t addr;
    uint8_t data;
    uint8_t state;
} ayData = {0,0,COMPLETE};


void ayInterface(void);

void initAy()
{
    // сперва порты
    gpio_set_mode(AY_DATA_PORT, GPIO_MODE_OUTPUT_50_MHZ, GPIO_CNF_OUTPUT_PUSHPULL, AY_DATA_IOS);
    gpio_set_mode(AY_SERV_PORT, GPIO_MODE_OUTPUT_50_MHZ, GPIO_CNF_OUTPUT_PUSHPULL, AY_BC1 | AY_BDIR | AY_RESET);
    gpio_set_mode(AY_SERV_PORT, GPIO_MODE_OUTPUT_50_MHZ, GPIO_CNF_OUTPUT_ALTFN_PUSHPULL, AY_CLOCK);

    gpio_clear(AY_SERV_PORT, AY_RESET);

    // ay clock
    TIM1_CR1   = (uint32_t) TIM_CR1_CKD_CK_INT;
    TIM1_PSC   = (uint32_t) (TIMERSHZ/AYHZ/2) - 1;
    TIM1_ARR   = (uint32_t) 2 - 1;
    TIM1_CCR2  = (uint32_t) 1;
    TIM1_CCMR1 = (uint32_t) (TIM_CCMR1_OC2M_PWM2 | TIM_CCMR1_OC2PE);
    TIM1_CCER  = (uint32_t) TIM_CCER_CC2NE;
    TIM1_BDTR  = (uint32_t) TIM_BDTR_MOE;
    TIM1_CR1  |= (uint32_t) TIM_CR1_CEN;
    // data clock
    TIM2_CR1   = (uint32_t) TIM_CR1_CKD_CK_INT;
    TIM2_PSC   = (uint32_t) (((uint32_t)TIMERSHZ)/((uint32_t)COMMUNICATIONHZ)) - 1;
    TIM2_ARR   = (uint32_t) 1;
    TIM2_DIER  = (uint32_t) TIM_DIER_UIE;
    TIM2_CR1  |= (uint32_t) TIM_CR1_CEN;
    nvic_enable_irq(NVIC_TIM2_IRQ);
    nvic_set_priority(NVIC_TIM2_IRQ, 0x00);

    gpio_set(AY_SERV_PORT, AY_RESET);
}

/* отправка данных в AY, естесственно без контроля, если предыдущие данные
   не ушли, то они пропали */
void aySend(uint8_t addr, uint8_t data)
{
    if( ayData.state != COMPLETE ) return;

    nvic_disable_irq(NVIC_TIM2_IRQ);

    if(ayData.addr != addr) { // что бы повторно не передавать адрес если он такой же
        ayData.state = LATCH_ADDR;
    } else {
        ayData.state = WRITE;
    }
    ayData.addr = 0x0f & addr;
    ayData.data = data;

    nvic_enable_irq(NVIC_TIM2_IRQ);
}

void aySendBlocking(uint8_t addr, uint8_t data)
{
    uint32_t timeout = (uint32_t)1e7;
    while( (ayData.state != COMPLETE) && (--timeout > 1)  );
    aySend(addr,data);
}

void ayInterface()
{
    switch (ayData.state)
    {
        case LATCH_ADDR:
            gpio_port_write(AY_DATA_PORT, ayData.addr);
            gpio_set(AY_SERV_PORT, AY_BDIR | AY_BC1);
            ayData.state = WRITE_DELAY;
            break;
        case WRITE_DELAY:
            gpio_clear(AY_SERV_PORT, AY_BDIR | AY_BC1);
            ayData.state = WRITE;
            break;
        case WRITE:
            gpio_port_write(AY_DATA_PORT, ayData.data);
            gpio_set(AY_SERV_PORT, AY_BDIR);
            gpio_clear(AY_SERV_PORT, AY_BC1);
            ayData.state = END;
            break;
        case END:
            gpio_clear(AY_SERV_PORT, AY_BDIR | AY_BC1);
            ayData.state = COMPLETE;
            break;
        case COMPLETE:
            break;
        default:
            ayData.state = END;
            break;
    }
}

void tim2_isr()
{
    TIM2_CR1  &= (uint32_t) ~TIM_CR1_CEN;
    TIM2_SR = 0;
    ayInterface();
    TIM2_CR1  |= (uint32_t) TIM_CR1_CEN;
}
