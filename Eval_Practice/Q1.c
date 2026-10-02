/*
Author :

███████╗████████╗ █████╗ ██████╗  ███╗   ███╗ █████╗ ███╗   ██╗
██╔════╝╚══██╔══╝██╔══██╗██╔══██╗ ████╗ ████║██╔══██╗████╗  ██║
███████╗   ██║   ███████║██████╔╝ ██╔████╔██║███████║██╔██╗ ██║
╚════██║   ██║   ██╔══██║██║  ██║ ██║╚██╔╝██║██╔══██║██║╚██╗██║
███████║   ██║   ██║  ██║██║  ██║ ██║ ╚═╝ ██║██║  ██║██║ ╚████║
╚══════╝   ╚═╝   ╚═╝  ╚═╝╚═╝  ╚═╝ ╚═╝     ╚═╝╚═╝  ╚═╝╚═╝  ╚═══╝  STARMAN248
*/

/*
    Write a C program to display an 8-bit binary down counter on LEDs
*/

#include <LPC17xx.h>

void delay(void) {
    unsigned int i;
    for(i = 0; i < 100000; i++);
}

int main(void) {
    unsigned int count;
    SystemInit();
    SystemCoreClockUpdate();

    LPC_PINCON->PINSEL0 = 0;
    LPC_GPIO0->FIODIR = 0xFF0;

    while(1) {
        for(count = 255; i >= 0; --i) {
            LPC_GPIO0->FIOCLR = 0xFF0;
            LPC_GPIO0->FIOSET = (count << 4);
            delay();
        }

        LPC_GPIO0->FIOCLR = 0xFF0;
        delay();
    }
}

