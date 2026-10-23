/*
   Write an Embedded C program for LPC1768 to control LED brightness using a switch connected to P2.12 and PWM output PWM1.4 (P1.23).

    Requirements:
    1. Configure P2.12 as a GPIO input.
    2. Configure P1.23 as PWM1.4 output.
    3. Set MR0 = 30000.
    4. When the switch is pressed (P2.12 = 0), set duty cycle to 75%.
    5. When the switch is released (P2.12 = 1), set duty cycle to 25%.
    6. No interrupts.
    Assume the switch is active-low.
*/

#include <LPC17xx.h>

int main(void) {
    SystemInit();
    SystemCoreClockUpdate();

    LPC_SC->PCONP |= (1 << 6);
    LPC_PINCON->PINSEL3 &= ~(3 << 14);
    LPC_PINCON->PINSEL3 |=  (2 << 14);


    LPC_PINCON->PINSEL4 = 0;
    LPC_GPIO2->FIODIR &= ~(1 << 12);

    LPC_PWM1->PR = 0;
    LPC_PWM1->PCR = (1 << 12);
    LPC_PWM1->MCR = (1 << 1);

    LPC_PWM1->MR0 = 30000;
    LPC_PWM1->MR4 = 7500;

    LPC_PWM1->LER = (1 << 0) | (1 << 4);

    LPC_PWM1->TCR = (1 << 1);

    LPC_PWM1->TCR = (1 << 0) | (1 << 3);

    while(1) {
        if(!(LPC_GPIO2->FIOPIN & (1 << 12))) LPC_PWM1->MR4 = 22500;
        else LPC_PWM1->MR4 = 7500;

        LPC_PWM1->LER = (1 << 4);
    }
}