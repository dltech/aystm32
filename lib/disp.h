#ifndef H_DISP
#define H_DISP

#include "inttypes.h"

/* только базовые настройки экрана, что бы увидеть все ищи другую библиотеку */
#define SD_ADDR                 0x3c
#define COMMAND_CONTROL_BYTE    0x80
#define RAM_CONTROL_BYTE        0xc0

#define SET_MEMORY_ADDRESSING_MODE  0x20
#define PAGE_ADRESSING              0x02
#define HORISONTAL_ADDRESSING       0x00
#define VERICAL_ADRESSING           0x01
#define SET_NULL_START_LINE         0x40
#define SET_CONTRAST                0x81
#define INIT_CONTRAST               0x7f
#define MAX_CONTRAST                0xff
#define SET_CHARGE_PUMP             0x8d
#define ENABLE_CHARGE_PUMP          0x14
#define NO_COL_REMAP                0xa0
#define FORCED_DISPLAY_ON           0xa5
#define DISPLAY_ON                  0xa4
#define SET_NON_INVERSE             0xa6
#define SET_INVERSE                 0xa7
// чем больше мультиплексор тем меньше частота Л - логика?
#define SET_MUX_RATIO               0xa8
#define MIN_MUX_RATIO               0x08
#define SET_NORMAL_MODE             0xaf
#define SET_SLEEP_MODE              0xae
#define SET_NULL_PAGE               0xb0
#define SET_NORMAL_SCAN_DIR         0xc0
#define SET_INVERSE_SCAN_DIR        0xc8
#define SET_DISPLAY_OFFSET          0xd3
#define DISP_NULL                   0x00
#define SET_DISPLAY_CLOCK           0xd5
#define DISPLAY_MAX_FREQ            0xf0
#define SET_DISPLAY_PRE_CHARGE      0xd9
#define DEFAULT_PRE_CHARGE          0x22
#define SET_COM_PIN_CONFIG          0xda
#define DEFAULT_COM_PIN             0x12
#define SET_DESELECT_LEVEL          0xdb
#define DEFAULT_DESELECT            0x00
#define DISPLAY_NOP                 0xe3

#define DISP_COL  128
#define DISP_PAGE 8
#define DISP_RAM_SIZE 1024
uint8_t dispData[DISP_RAM_SIZE];

void sdInit(void);
void flashlight(void);

#endif
