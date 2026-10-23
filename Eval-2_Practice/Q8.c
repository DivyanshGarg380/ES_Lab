/*
   Write an Embedded C program for LPC1768 to control LED brightness using an analog input connected to ADC0.5 (P1.31) and PWM output PWM1.4 (P1.23).

    Requirements:
    1. Configure ADC0.5 and PWM1.4.
    2. Set MR0 = 30000.
    3. Continuously read the ADC value (12-bit, range 0–4095).
    4. Scale the ADC reading to set PWM duty cycle from 0% to 100%.
    5. No interrupts.
*/

#include <LPC17xx.h>

unsigned int adc_value;
unsigned int duty;

int main(void) {
    SystemInit();
    SystemCoreClockUpdate();

    LPC_SC->PCONP = (1 << 6) | (1 << 12);

    // PWM1.4 
    LPC_PINCON->PINSEL3 &= ~(3 << 14); 
    LPC_PINCON->PINSEL3 |=  (2 << 14);

    // ADC0.5
    LPC_PINCON->PINSEL3 &= ~(3 << 30);
    LPC_PINCON->PINSEL3 |=  (3 << 30);

    LPC_ADC->ADCR = (1 << 5) | (1 << 21);

    LPC_PWM1->PR  = 0;
    LPC_PWM1->PCR = (1 << 12);
    LPC_PWM1->MCR = (1 << 1);

    LPC_PWM1->MR0 = 30000;
    LPC_PWM1->MR4 = 0; // start with 0%
    LPC_PWM1->LER = (1 << 0) | (1 << 4);

    LPC_PWM1->TCR = (1 << 1);

    LPC_PWM1->TCR = (1 << 0) | (1 << 3);

    while(1) {
        // start ADC on channel 5
        LPC_ADC->ADCR &= ~(7 << 24);
        LPC_ADC->ADCR |= (1 << 24);

        while(!(LPC_ADC->ADGDR & (1 << 31)));

        adc_value = (LPC_ADC->ADGDR >> 4) & 0xFFF;

        duty = (adc_value * 30000) / 4095;
        LPC_PWM1->MR4 = duty;
        LPC_PWM1->LER = (1 << 4);
    }
}