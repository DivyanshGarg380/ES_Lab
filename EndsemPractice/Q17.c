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
    Design an LED pattern using all eight onboard LEDs.
    - Initially, only the first LED should glow.
    - The glowing LED should move from left to right.
    - After reaching the last LED, it should move from right to left.
    - Repeat this pattern continuously.
*/

#include <LPC17xx.h>

void delay(void) {
    unsigned int i;
    for(i = 0; i < 500000; ++i);
}

int main(void) {
    unsigned int i;
    LPC_PINCON->PINSEL0 = 0;
    LPC_GPIO0->FIODIR |= 0x00000FF0;

    while(1) {
        for(i = 0; i < 8; ++i) {
            LPC_GPIO0->FIOCLR = 0x00000FF0;
            LPC_GPIO0->FIOSET = (1 << (i + 4));
            delay();
        }

        for(i = 6; i > 0; --i) {
            LPC_GPIO0->FIOCLR = 0x00000FF0;
            LPC_GPIO0->FIOSET = (1 << (i + 4));
            delay();
        }
    }
}