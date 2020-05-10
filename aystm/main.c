#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "board.h"
#include "lm4811.h"
#include "ayconn.h"


#define MUSIC_SIZE 32
uint8_t music[MUSIC_SIZE] = { 0,
                      1,
                      2,
                      3,
                      4,
                      5,
                      6,
                      7,
                      8,
                      9,
                      10,
                      11,
                      12,
                      13,
                      14,
                      15,
                      15,
                      14,
                      13,
                      12,
                      11,
                      10,
                      9,
                      8,
                      7,
                      6,
                      5,
                      4,
                      3,
                      2,
                      1 };

int main(void)
{
    boardInit();
    init4811(8);
    initAy();
    uint8_t i = 0;
    while(1) {
        aySendBl(10, 0x10 | music[i++]);
        if(i>=MUSIC_SIZE) i=0;
    }
}
