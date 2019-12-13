#include "../libopencm3/include/libopencm3/stm32/rcc.h"
#include "st_usbfs.h"

void sysClk(void);
void usbOn(void);

void rough_delay_us(uint16_t us)
{
    const uint32_t sysclkMhz = 72;
    volatile uint32_t cnt = ((uint32_t)us)*sysclkMhz;
    while(cnt-- > 0);
}

void sysClk()
{
    // включаем кварц, ждем пока прогреется
    RCC_CR |= RCC_CR_HSEON;
    uint32_t timeout = 9e6;
    while ( ((RCC_CR & RCC_CR_HSERDY) == 0) && (--timeout > 1) );
    // рассчет на тактирование от кварца 12 мГц, на максимальную частоту в 72 мГц
    // AHB, APB2 столько же (USB мощный, электричества не жалко),
    // APB2 36 мГц (для таймеров 72), на АЦП забили, не используем.
    uint32_t cfgr = (RCC_CFGR_PLLMUL_PLL_CLK_MUL6 << RCC_CFGR_PLLMUL_SHIFT) \
                  | (RCC_CFGR_USBPRE_PLL_CLK_DIV1_5 << RCC_CFGR_USBPRE)    \
                  | (RCC_CFGR_PLLXTPRE_HSE_CLK << RCC_CFGR_PLLXTPRE)       \
                  | (RCC_CFGR_PLLSRC_HSE_CLK << RCC_CFGR_PLLSRC)           \
                  | (RCC_CFGR_PPRE2_HCLK_NODIV << RCC_CFGR_PPRE2_SHIFT)    \
                  | (RCC_CFGR_PPRE1_HCLK_DIV2 << RCC_CFGR_PPRE1_SHIFT)     \
                  | (RCC_CFGR_HPRE_SYSCLK_NODIV << RCC_CFGR_HPRE_SHIFT);
    RCC_CFGR = cfgr;

    // передергиваем PLL, что бы точно все включилось
    timeout = 9e6;
    if ( (RCC_CFGR & RCC_CFGR_SWS) == RCC_CFGR_SWS_PLL )
    {
        RCC_CFGR &= ~RCC_CFGR_SW;
        while ( ((RCC_CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_HSI) && \
                (--timeout > 1) );
    }
    RCC_CR &= (uint32_t)(~RCC_CR_PLLON);
    timeout = 9e6;
    while( ((RCC_CR & RCC_CR_PLLRDY) != 0) && (--timeout > 1) );
    RCC_CR |= RCC_CR_PLLON;
    timeout = 9e6;
    while( ((RCC_CR & RCC_CR_PLLRDY) == 0) && (--timeout > 1) );
    // включаем sysclk, ждем
    RCC_CFGR |= (uint32_t)(RCC_CFGR_SW_PLL << RCC_CFGR_SW_SHIFT);
    timeout = 9e6;
    while( ((RCC_CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_PLL) && (--timeout > 1) );
}

void boardInit()
{
    sysClk();
    RCC_AHBENR  |= BOARD_AHB;
    RCC_APB1ENR |= BOARD_APB1;
    RCC_APB2ENR |= BOARD_APB2;
    usbOn();
}

void photoButtonsInit(void)
{

}

void usbOn()
{
    // лапки к usb подключаются сами (трогать их не надо), тактирование,
    // я надеюсь, включено выше и больше там никаких приколов не будет
    USB_CNTR_REG &= ~(uint32_t)USB_CNTR_PWDN;
    rough_delay_us(1);
    USB_ISTR_REG = 0;
    USB_CNTR_REG &= ~(uint32_t)USB_CNTR_FRES;
}
