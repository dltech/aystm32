#include "inttypes.h"
#include "../libopencm3/include/libopencm3/stm32/pwr.h"
#include "../libopencm3/include/libopencm3/stm32/gpio.h"
#include "../libopencm3/include/libopencm3/stm32/rcc.h"
#include "../libopencm3/include/libopencm3/stm32/f1/bkp.h"
#include "board.h"
#include "lm4811.h"

volatile uint8_t volumeGl = 0;

void delay4811(void);
void upDn(uint8_t n, uint8_t up);


void delay4811()
{
    volatile uint32_t cnt = DELAYCLKS;
    while(cnt-- > 0);
}

void upDn(uint8_t n, uint8_t up)
{
    if(n > SOUNDMAX) n = SOUNDMAX;

    if( up ) {
        gpio_set(AUDIO_PORT, AUDIO_UPDN);
    } else {
        gpio_clear(AUDIO_PORT, AUDIO_UPDN);
    }
    for(uint8_t i=0 ; i<n; ++i) {
        gpio_clear(AUDIO_PORT, AUDIO_CLK);
        delay4811();
        gpio_set(AUDIO_PORT, AUDIO_CLK);
        delay4811();
    }
    gpio_clear(AUDIO_PORT, AUDIO_CLK | AUDIO_UPDN);
}


void init4811(uint8_t volume)
{
    // что бы порты 14 и 15 никто не беспокоил
    PWR_CR |= (uint32_t)PWR_CR_DBP;
    RCC_BDCR = (uint32_t)0;
    BKP_CR = (uint32_t)0;
    BKP_RTCCR = (uint32_t)0;
    BKP_CSR = (uint32_t)0;
    RCC_BDCR |= (uint32_t)RCC_BDCR_BDRST;

    gpio_set_mode(AUDIO_PORT, GPIO_MODE_OUTPUT_50_MHZ, GPIO_CNF_OUTPUT_PUSHPULL, AUDIO_UPDN | AUDIO_CLK);
    gpio_clear(AUDIO_PORT, AUDIO_CLK | AUDIO_UPDN);

    volumeGl = 16;
    setVolume(0);
    volumeGl = 0;
    setVolume(volume);
}


void setVolume(uint8_t volume)
{
    if(volume == volumeGl) return;

    if(volumeGl < volume) {
        upDn(volume - volumeGl, 1);
    } else {
        upDn(volumeGl - volume, 0);
    }
}
