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
    Write an LPC1768 program to read the analog voltage from ADC channel 5 (P1.31) and control the brightness 
    of the onboard LED LD10 using PWM1.4 (P1.23).
    
    - When ADC value is below 1365, set brightness to 25%.
    - When ADC value is between 1365 and 2730, set brightness to 50%.
    - When ADC value is above 2730, set brightness to 75%.
    - Continuously monitor the ADC and update the PWM duty cycle.
*/

#include <LPC17xx.h>

unsigned int adc_val, duty;

int main(void) {
    SystemInit();
    SystemCoreClockUpdate();

    LPC_SC->PCONP |= (1 << 12) | (1 << 6); // enable ADC and PWM1
    LPC_PINCON->PINSEL3 = 0xC0008000;

    LPC_PWM1->PR = 0;
    LPC_PWM1->PCR = (1 << 12);
    LPC_PWM1->MCR = 0x03;
    LPC_PWM1->MR0 = 30000;
    LPC_PWM1->MR4 = 7500;
    LPC_PWM1->LER = (1 << 0) | (1 << 4);

    LPC_PWM1->TCR = 0x02;
    LPC_PWM1->TCR = 0x09;

    while(1) {
        LPC_ADC->ADCR = (1 << 5) | (1 << 21) | (1 << 24);
        while(!(LPC_ADC->ADGDR & (1 << 31)));

        adc_val = (LPC_ADC->ADGDR >> 4) & 0xFFF;

        if(adc_val < 1365) duty = 7500;
        else if(adc <= 2730) duty = 15000;
        else duty = 22500;

        LPC_PWM1->MR4 = duty;
        LPC_PWM1->LER = (1 << 4);
    }
}