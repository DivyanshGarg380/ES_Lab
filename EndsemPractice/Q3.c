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
    A square-wave signal with 50% duty cycle is connected to P2.12 (EINT2, Function 1). 
    Write an Embedded C program using an external hardware interrupt to generate a square wave at P0.4 with a frequency one-sixth of the input frequency and a duty cycle of 50%.
*/

#include <LPC17xx.h>

unsigned int interrupt_count = 0;
unsigned int output_state = 0;

int main(void) {
    SystemInit();
    SystemCoreClockUpdate();

    LPC_PINCON->PINSEL0 = 0;
    LPC_GPIO0->FIODIR |= (1 << 4);

    LPC_PINCON->PINSEL4 &= ~(3 << 24);
    LPC_PINCON->PINSEL4 |= (1 << 24);

    LPC_SC->EXTMODE |= (1 << 2);
    LPC_SC->EXTPOLAR &= ~(1 << 2);

    NVIC_EnableIRQ(EINT2_IRQn);
    while(1);
}

void EINT2_IRQHandler(void) {
    interrupt_count++;

    if(interrupt_count == 3) {
        interrupt_count = 0;
        if(output_state == 0) {
            LPC_GPIO0->FIOSET = 1 << 4;
            output_state = 1;
        } else {
            LPC_GPIO0->FIOCLR = 1 << 4;
            output_state = 0;
        }
    }

    LPC_SC->EXTINT = 1 << 2;
}