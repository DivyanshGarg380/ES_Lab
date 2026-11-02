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
    Design a PWM program using PWM1.4.
    - Generate PWM output on LD10.
    - Start with a relatively slow pulse frequency.
    - Gradually increase the frequency in five steps.
    - Maintain approximately 50% duty cycle at every step.
    - After reaching the highest frequency, restart from the lowest frequency.
*/

#include <LPC17xx.h>

void delay(void) {
    unsigned int i;
    for(i = 0; i < 500000; ++i);
}

int main(void) {
    unsigned int period;

    LPC_PINCON->PINSEL3 &= ~0x0000C000;
    LPC_PINCON->PINSEL3 |= 0x00008000;

    LPC_SC->PCONP |= (1 << 6);
    LPC_PWM1->PR = 0;
    LPC_PWM1->PCR = (1 << 12);

    LPC_PWM1->MCR = 0x03;
    LPC_PWM1->MR0 = 30000;
    LPC_PWM1->MR4 = 15000;

    LPC_PWM1->LER = (1 << 0) | (1 << 4);
    LPC_PWM1->TCR = 0x02;
    LPC_PWM1->TCR = 0x09;

    while(1) {
        for(period = 30000; period >= 10000; period -= 5000) {
            LPC_PWM1->MR0 = period;
            LPC_PWM1->MR4 = period / 2;
            LPC_PWM1->LER = (1 << 0) | (1 << 4);
            delay();
        }
    }
}3