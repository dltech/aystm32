#include "../libopencm3/include/libopencm3/stm32/rcc.h"
#include "../libopencm3/include/libopencm3/stm32/flash.h"
#include "../libopencm3/include/libopencm3/stm32/st_usbfs.h"
#include "board.h"
#include "inttypes.h"

void sysClk(void);
void usbOn(void);
void photoButtonsInit(void);

void rough_delay_us(uint16_t us)
{
    volatile uint32_t cnt = (uint32_t)us*(uint32_t)4;
    while(cnt-- > 0);
}

void delay_s(uint16_t s)
{
    uint16_t cnt = s*20;
    while(cnt-- > 0) rough_delay_us(50000);
}

void sysClk()
{
    // включаем кварц, ждем пока прогреется
    RCC_CR |= (uint32_t)RCC_CR_HSEON;
    uint32_t timeout = 1e8;
    while ( ((RCC_CR & RCC_CR_HSERDY) == 0) && (--timeout > 1) );
    // рассчет на тактирование от кварца 8 мГц, на максимальную частоту в 72 мГц
    // AHB, APB1, APB2 36 (разрешенный максимум)
    // APB2 36 мГц (для таймеров 72), на АЦП забили, не используем.
    uint32_t cfgr = (RCC_CFGR_USBPRE_PLL_CLK_DIV1_5 << 22)                  \
                  | (RCC_CFGR_PLLMUL_PLL_CLK_MUL9 << RCC_CFGR_PLLMUL_SHIFT) \
                  | (RCC_CFGR_PLLXTPRE_HSE_CLK << 17)                       \
                  | (RCC_CFGR_PLLSRC_HSE_CLK << 16)                         \
                  | (RCC_CFGR_PPRE2_HCLK_NODIV << RCC_CFGR_PPRE2_SHIFT)     \
                  | (RCC_CFGR_PPRE1_HCLK_NODIV << RCC_CFGR_PPRE1_SHIFT)     \
                  | (RCC_CFGR_HPRE_SYSCLK_DIV2 << RCC_CFGR_HPRE_SHIFT);
    RCC_CFGR = cfgr;
    // что то с памятью, копипаста с функций stmhal
    FLASH_ACR |= (uint32_t)FLASH_ACR_PRFTBE;
    FLASH_ACR &= ((uint32_t)~FLASH_ACR_LATENCY_MASK);
    FLASH_ACR |= (uint32_t)FLASH_ACR_LATENCY_2WS;
    // передергиваем PLL, что бы точно все включилось
    timeout = 9e6;
    if ( (RCC_CFGR & RCC_CFGR_SWS) == (RCC_CFGR_SWS_SYSCLKSEL_PLLCLK << RCC_CFGR_SWS_SHIFT) )
    {
        RCC_CFGR &= ~((uint32_t)RCC_CFGR_SW);
        while ( ((RCC_CFGR & RCC_CFGR_SWS) != (RCC_CFGR_SWS_SYSCLKSEL_HSICLK << RCC_CFGR_SWS_SHIFT) ) && \
                (--timeout > 1) );
    }
    RCC_CR &= ~(uint32_t)(RCC_CR_PLLON);
    timeout = 9e6;
    while( ((RCC_CR & RCC_CR_PLLRDY) != 0) && (--timeout > 1) );
    RCC_CR |= (uint32_t)RCC_CR_PLLON;
    timeout = 9e6;
    while( ((RCC_CR & RCC_CR_PLLRDY) == 0) && (--timeout > 1) );

    // включаем sysclk, ждем
    RCC_CFGR |= (uint32_t)(RCC_CFGR_SW_SYSCLKSEL_PLLCLK << RCC_CFGR_SW_SHIFT);
    timeout = 9e6;
    while( ((RCC_CFGR & RCC_CFGR_SWS) != (RCC_CFGR_SWS_SYSCLKSEL_PLLCLK << RCC_CFGR_SWS_SHIFT)) && (--timeout > 1) );
}

void usbOn()
{
    // лапки к usb подключаются сами (трогать их не надо), тактирование,
    // я надеюсь, включено выше и больше там никаких приколов не будет
    (*USB_CNTR_REG) &= ((uint32_t)(~USB_CNTR_PWDN));
    rough_delay_us(1);
    (*USB_ISTR_REG) = ((uint32_t)0);
    (*USB_CNTR_REG) &= ((uint32_t)(~USB_CNTR_FRES));
}

void boardInit()
{
    sysClk();
    RCC_AHBENR  |= (uint32_t)BOARD_AHB;
    RCC_APB1ENR |= (uint32_t)BOARD_APB1;
    RCC_APB2ENR |= (uint32_t)BOARD_APB2;
    usbOn();
}

void photoButtonsInit()
{
    ;
}
