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
    Write an Embedded C program to display an 8-bit binary counter on LEDs connected to P0.4–P0.11. 
    Read the switch connected to P2.12. If the switch is HIGH, increment the counter; otherwise, decrement it. 
    Introduce a delay between successive counts.
*/

#include <LPC17xx.h>

unsigned int i, count = 0;

int main(void) {
    SystemInit();
    SystemCoreClockUpdate();

    LPC_PINCON->PINSEL0 = 0;
    LPC_PINCON->PINSEL4 = 0;
    LPC_GPIO0->FIODIR |= 0x0FF0;
    LPC_GPIO2->FIODIR &= ~(1 << 12);

    while(1) {
        LPC_GPIO0->FIOPIN = (count << 4);
        if(LPC_GPIO2->FIOPIN & (1 << 12)) {
            count++;
        } else count--;

        for(i = 0; i < 10000; ++i);
    }
}