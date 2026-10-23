/*
    Write an Embedded C program to generate PWM with 50% duty cycle on the LED using LPC1768.
*/

#include <LPC17xx.h>

int main(void) {
    SystemInit();
    SystemCoreClockUpdate();

    // Select PWM1.4 on P1.23
    LPC_SC->PCONP |= (1 << 6);
    LPC_PINCON->PINSEL3 &= ~(3 << 14);
    LPC_PINCON->PINSEL3 |= 2 << 14;
    
    // Configure PWM
    LPC_PWM1->PR = 0;
    LPC_PWM1->PCR = (1 << 12);
    LPC_PWM1->MCR = (1 << 1);

    LPC_PWM1->MR0 = 30000;
    LPC_PWM1->MR4 = 15000;

    LPC_PWM1->LER = (1 << 0) | (1 << 4);

    LPC_PWM1->TCR = (1 << 1); // reset
    LPC_PWM1->TCR = (1 << 0) | (1 << 3); // enable

    while(1);
}