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
    Dynamic 8-bit LED Controller

    Write an Embedded C program for LPC1768 using P0.4–P0.11 as 8 LEDs and a switch connected to P2.12.

    The program must behave as follows:

    - Initially, display an 8-bit up counter
    - If the switch is HIGH, the counter continues UP.
    - If the switch becomes LOW, the counter changes direction and starts counting DOWN
    - The counter must wrap around:
    - UP: 255 → 0
    - DOWN: 0 → 255
    - Twist: The switch may change state at any time.
    
    You must check the switch during every iteration, not just once before starting the counter.
    - Display the value on P0.4–P0.11.
*/

#include <LPC17xx.h>

void delay(void) {
    unsigned int i;
    for(i = 0; i < 100000; i++);
}

int main(void) {
    unsigned int count = 0;
    SystemInit();
    SystemCoreClockUpdate();

    LPC_PINCON->PINSEL0 = 0;
    LPC_GPIO0->FIODIR = 0xFF0;

    LPC_PINCON->PINSEL4 = 0;
    LPC_GPIO2->FIODIR = ~(1 << 12);

    while(1) {
        LPC_GPIO0->FIOCLR = 0xFF0;
        LPC_GPIO0->FIOSET = (count << 4);

        delay();
        if(LPC_GPIO2->FIOPIN & (1 << 12)) {
            // UP Counting
            if(count == 255) count = 0;
            else count++;
        } else {
            // DOWN Counting
            if(count == 0) count = 255;
            else count--;
        }
    }
}