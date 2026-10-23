/*
    Write an Embedded C program for LPC1768 to generate a PWM waveform on PWM1.4 (P1.23) with these specifications:
    1. PWM frequency = 1 kHz
    2. Peripheral clock PCLK = 25MHz
    3. Duty cycle = 40%
    4. Use PWM1, with no interrupts.
    Calculate the required MR0 and MR4 values, then write the complete program.
*/

#include <LPC17xx.h>

int main(void) {
    SystemInit();
    SystemCoreClockUpdate();

    LPC_SC->PCONP |= (1 << 6);
    LPC_PINCON->PINSEL3 &= ~(3 << 14);
    LPC_PINCON->PINSEL3 |=  (2 << 14);

    LPC_PWM1->PR = 0;
    LPC_PWM1->PCR = (1 << 12);

    LPC_PWM1->MCR = (1 << 1);

    // MR0 = PCLK / Freq = 25000000/1000 = 25000
    LPC_PWM1->MR0 = 25000;

    LPC_PWM1->MR4 = 10000;

    LPC_PWM1->LER = (1 << 0) | (1 << 4);

    LPC_PWM1->TCR = (1 << 1);

    LPC_PWM1->TCR = (1 << 0) | (1 << 3);
    while(1);
}

