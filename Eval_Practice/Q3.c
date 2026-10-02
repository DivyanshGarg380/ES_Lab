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
    Write a C program to Switch ON/OFF alternate LED patterns
*/

#include <LPC17xx.h>

void delay(void) {
    unsigned int i;
    for(i = 0; i < 100000; i++);
}

int main(void) {
    SystemInit();
    SystemCoreClockUpdate();

    LPC_PINCON->PINSEL0 = 0;
    LPC_GPIO0->FIODIR |= 0xFF0;

    LPC_PINCON->PINSEL4 = 0;
    LPC_GPIO2->FIODIR |= (1 << 12);

    while(1) {
        if(LPC_GPIO2->FIOPIN & (1 << 12)) {
            LPC_GPIO0->FIOCLR = 0xFF0;
            LPC_GPIO0->FIOSET = 0x550;
        } else {
            LPC_GPIO0->FIOCLR = 0xFF0;
            LPC_GPIO0->FIOSET = 0xAA0;
        }
    }
}

