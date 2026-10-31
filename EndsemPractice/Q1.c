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
   Write an Embedded C program to display an 8-bit up/down counter on eight LEDs. Use switch SW2 to select the counting direction.
    - SW2 = 1: Up counter (0 to 255).
    - SW2 = 0: Down counter (255 to 0).
    - Introduce a suitable delay between successive counts.
*/

#include<LPC17xx.h>

unsigned int j, count;

int main(void) {
    SystemInit();
    SystemCoreClockUpdate();

    LPC_PINCON->PINSEL0 = 0; // as GPIO
    LPC_PINCON->PINSEL3 = 0; // as GPIO for SW2

    LPC_GPIO0->FIODIR |= 0x00000FF0;
    LPC_GPIO1->FIODIR &= ~(1 << 26);

    count = 0;
    while(1) {
        // read SW2
        if(LPC_GPIO1->FIOPIN & (1 << 26)) {
            // SW2 = 1 -> Up counter
            LPC_GPIO0->FIOCLR = 0x00000FF0;
            LPC_GPIO0->FIOSET = (count << 4);
            count++;
            if(count > 255) count = 0;
        } else {
            // SW2 = 0 -> Down counter
            LPC_GPIO0->FIOCLR = 0x00000FF0;
            LPC_GPIO0->FIOSET = (count << 4);
            if(count == 0) count = 255;
            else count--;
        }

        for(j = 0; j < 50000; ++j);
    }
}