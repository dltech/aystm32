#include "gpio.h"

#define SYSCLKHZ    72000000

#define AY_DATA_PORT GPIOA
#define AY_DATA_IOS  GPIO0 | GPIO1 | GPIO2 | GPIO3 | GPIO4 | GPIO5 | GPIO6 | GPIO7
#define AY_SERV_PORT GPIOB
#define AY_BC1       GPIO0
#define AY_BDIR      GPIO1
#define AY_RESET     GPIO13
#define AY_CLOCK     GPIO14

#define DISPLAY_PORT GPIOB
#define DISP_PINS    GPIO6 | GPIO7

#define BUTTON_PORT  GPIOB
#define BUTTON_L     GPIO4
#define BUTTON_R     GPIO5

#define BUTTON_CLK_PORT GPIOA
#define BUTTON_CLK_PIN  GPIO8

#define AUDIO_PORT GPIOC
#define AUDIO_UPDN GPIO14
#define AUDIO_CLK  GPIO15

#define USBPORT    GPIOA
#define USBDM      GPIO11
#define USBDP      GPIO12

// сразу задаю всё тактирование используемой периферии
#define BOARD_AHB   RCC_AHBENR_DMA1EN
#define BOARD_APB1  RCC_APB1ENR_USBEN
#define BOARD_APB2  RCC_APB2ENR_IOPAEN | RCC_APB2ENR_IOPBEN