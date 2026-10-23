/*
    Write an Embedded C program to generate a PWM signal with 75% duty cycle on PWM1.4 (P1.23) using LPC1768. 
    Assume MR0 = 20000. Use the PWM peripheral without interrupts.
*/

#include <LPC17xx.h>

int main(void) {
    SystemInit();
    SystemCoreClockUpdate();

    LPC_SC->PCONP |= (1 << 6);

    LPC_PINCON->PINSEL3 &= ~(3 << 14);
    LPC_PINCON->PINSEL3 |= (2 << 14);

    LPC_PWM1->PR = 0;
    LPC_PWM1->PCR = (1 << 12);

    LPC_PWM1->MCR = (1 << 1);
    LPC_PWM1->MR0 = 20000;

    LPC_PWM1->MR4 = 15000;

    LPC_PWM1->LER = (1 << 0) | (1 << 4);
    LPC_PWM1->TCR = (1 << 1);

    LPC_PWM1->TCR = (1 << 0) | (1 << 3);
    while(1);
}