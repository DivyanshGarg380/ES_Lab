/*
    Write an Embedded C program for LPC1768 to generate a PWM signal with 30% duty cycle on PWM1.4 (P1.23). 
    Use MR0 = 25000
*/

#include <LPC17xx.h>

int main(void) {
    SystemInit();
    SystemCoreClockUpdate();

    LPC_SC->PCONP = (1 << 6);
    LPC_PINCON->PINSEL3 &= ~(3 << 14);
    LPC_PINCON->PINSEL3 |=  (2 << 14);

    LPC_PWM1->PR = 0;
    LPC_PWM1->PCR = (1 << 12);

    LPC_PWM1->MCR = (1 << 1);

    LPC_PWM1->MR0 = 25000;

    LPC->PWM1->MR4 = 7500;

    LPC_PWM1->LER = (1 << 0) | (1 << 4);

    LPC_PWM1->TCR = (1 << 1);

    LPC_PWM1->TCR = (1 << 0) | (1 << 3);

    while(1);
}