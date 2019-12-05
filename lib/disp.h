#include "inttypes.h"

#define SD_ADDR     0xff
#define COMMAND_CONTROL_BYTE    0x80
#define RAM_CONTROL_BYTE        0xc0

/* A8 3f, D3 00, 40, A0/A1, C0/C8, DA, 81 7f, A4 (включение), A6, D5, 8D 14, AF */

#define INIT_COMMANDS 10
uint8_t initSeq[INIT_COMMANDS] = {};

#define MATRIX_ROW  64
#define MATRIX_COL  4
volatile uint32_t dispBuffer[MATRIX_COL][MATRIX_ROW];

#define BUFFER_SIZE 1016
uint8_t dispData[BUFFER_SIZE];
