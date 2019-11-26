#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "board.h"

int main(void)
{
    clkInit();

    delay();
    displayInit();
    fanInit();
    menuInit();
    tl_init();
    while(1)
    {
        ;
    }
}
