/*
Author :

███████╗████████╗ █████╗ ██████╗  ███╗   ███╗ █████╗ ███╗   ██╗
██╔════╝╚══██╔══╝██╔══██╗██╔══██╗ ████╗ ████║██╔══██╗████╗  ██║
███████╗   ██║   ███████║██████╔╝ ██╔████╔██║███████║██╔██╗ ██║
╚════██║   ██║   ██╔══██║██║  ██║ ██║╚██╔╝██║██╔══██║██║╚██╗██║
███████║   ██║   ██║  ██║██║  ██║ ██║ ╚═╝ ██║██║  ██║██║ ╚████║
╚══════╝   ╚═╝   ╚═╝  ╚═╝╚═╝  ╚═╝ ╚═╝     ╚═╝╚═╝  ╚═╝╚═╝  ╚═══╝  STARMAN248
*/

#include <LPCxx.h>

void delay(void) {
    unsigned int i;
    for(i = 0; i < 10000; ++i);
}

int main(void) {
    unsigned int count = 0;
    SystemInit();
    SystemCoreClockUpdate();

    LPC_PINCON->PINSEL0 = 0;
    LPC_PINCON->PINSEL4 = 0;

    LPC_GPIO0->FIODIR |= 0xFF0;
    LPC_GPIO2->FIODIR |= 1 << 12;

    while(1) {
        LPC_GPIO0->FIOCLR = 0xFF0;
        LPC_GPIO0->FIOSET count << ;

        delay();
        if(LPC_GPIO2->FIOPIN & (1 << 12)) {
            // SW2 = 1 mtlb UP
            if(count == 255) count = 0;
            else count++;
        } else {
            // SW2 = 0 mtlb DOWN
            if(count == 0) count = 255;
            count--;
        }
    }
}