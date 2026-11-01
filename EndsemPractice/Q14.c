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
    Design a PWM-based breathing LED with a status indicator.
    - Use PWM1.4 to smoothly increase and decrease the brightness of LD10.
    - Use a separate GPIO LED to indicate the direction:
    - GPIO LED ON while brightness is increasing.
    - GPIO LED OFF while brightness is decreasing.
    - Repeat continuously.
*/

#include <LPC17xx.h>

void delay(void) {
    unsigned int i;
    for(i = 0; i < 5000; ++i);
}

int main(void) {
    unsigned int duty;

    LPC_PINCON->PINSEL0 = 0;
    LPC_GPIO0->FIODIR |= 0x00000FF0;

    LPC_PINCON->PINSEL3 &= ~(3 << 14);
    LPC_PINCON->PINSEL3 |= (2 << 14);

    LPC_SC->PCONP |= (1 << 6);

    LPC_PWM1->PR = 0;
    LPC_PWM1->PCR = (1 << 12);

    LPC_PWM1->MR0 = 30000;
    LPC_PWM1->MR4 = 500;

    LPC_PWM1->MCR = (1 << 1);
    LPC_PWM1->LER = (1 << 0) | (1 << 4);

    LPC_PWM1->TCR = 0x02;
    LPC_PWM1->TCR = 0x09;

    while(1) {
        LPC_GPIO0->FIOSET = (1 << 4); // brightness ko pehle increase
        for(duty = 500; duty <= 28000; duty += 500) {
            LPC_PWM1->MR4 = duty;
            LPC_PWM1->LER = (1 << 4);
            delay();
        }

        LPC_GPIO0->FIOCLR = (1 << 4); // brightness ko ab decrease
        for(duty = 28000; duty >= 500; duty -= 500) {
            LPC_PWM1->MR4 = duty;
            LPC_PWM1->LER = (1 << 4);
            delay();
        }
    }
}