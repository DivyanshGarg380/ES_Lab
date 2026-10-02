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
    Write a C program to move a single ON LED from P0.4 to P0.11 and then back from P0.11 to P0.4 continuously
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

    while(1) {
        // move left -> right
        while(led < 0x800) {
            LPC_GPIO0->FIOCLR = 0x00000FF0;
            LPC_GPIO0->FIOSET = led;
            delay();
            led <<= 1;
        } 

        led = 0x400;
        // move right -> left
        while(led >= 0x10) {
            LPC_GPIO0->FIOCLR = 0x00000FF0;
            LPC_GPIO0->FIOSET = led;
            delay();
            led >>= 1;
        }
        led = 0x20;
    }
}