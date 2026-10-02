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
    Write a C program to implement an 8-bit ring counter using LEDs. 
    When SW2 = 1, the illuminated LED should move from P0.4 toward P0.11. 
    When SW2 = 0, it should move in the opposite direction.
*/

#include <LPC17xx.h>

void delay(void) {
    unsigned int i;
    for(i = 0; i < 100000; i++);
}

int main(void) {
    unsigned int led = 0x10;
    SystemInit();
    SystemCoreClockUpdate();

    LPC_PINCON->PINSEL0 = 0;
    LPC_GPIO0->FIODIR |= 0xFF0;

    LPC_PINCON->PINSEL4 = 0;
    LPC_GPIO2->FIODIR = ~(1 << 12);

    while(1) {
        LPC_GPIO0->FIOCLR = 0xFF0;
        LPC_GPIO0->FIOSET = led;
        delay();

        if(LPC_GPIO2->FIOPIN & (1 << 12)) {
            // SW2 = 1 -> move towards P0.11
            led <<= 1;
            if(led > 0x800) led = 0x10;
        } else {
            // SW2 = 0 -> move towards P0.4
            led >>= 1;
            if(led < 0x10) led = 0x800;
        }
    }
}