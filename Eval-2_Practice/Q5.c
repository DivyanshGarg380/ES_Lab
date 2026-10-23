/*
    Write an Embedded C program for LPC1768 to generate a PWM waveform on PWM1.4 (P1.23) with the following specifications:
    1. PWM period match register: MR0 = 50000.
    2. Generate a 35% duty cycle initially.
    3. After configuring PWM, change the duty cycle to 65%.
    4. No interrupts and no delay loops are required.
*/

#include <LPC17xx.h>

int main(void) {
    SystemInit();
    SystemCoreClockUpdate();

    LPC_SC->PCONP = (1 << 6);
    LPC_PINCON->PINSEL3 &= ~(3 << 14);
    LPC_PINCON->PINSEL3 |= (2 << 14);

    LPC_PWM1->PR = 0;
    LPC_PWM1->PCR = (1 << 12);
    LPC_PWM1->MCR = (1 << 1);

    LPC_PWM1->MR0 = 50000;
    LPC_PWM1->MR4 = 17500; // set to 35% at first

    LPC_PWM1->LER = (1 << 0) | (1 << 4);

    LPC_PWM1->TCR = 1 << 1;
    LPC_PWM1->TCR = (1 << 0) | (1 << 3);

    LPC_PWM1->MCR4 = 32500; // set to 65% now
    LPC_PWM1->LER = (1 << 4);

    while(1);
}