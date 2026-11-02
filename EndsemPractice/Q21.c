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
    Design a system using ADC channel 5 and PWM1.4.
    - Read the analog voltage from POT1.
    - If the voltage is below 1.0 V, set LD10 brightness to 25%.
    - If the voltage is between 1.0 V and 2.0 V, set brightness to 50%.
    - If the voltage is above 2.0 V, set brightness to 75%.
    - Continuously update the brightness as POT1 changes.
*/

#include <LPC17xx.h>

int main(void) {
    unsigned int adc_val;
    SystemInit();
    SystemCoreClockUpdate();

    LPC_PINCON->PINSEL3 = 0xC0008000;
    LPC_SC->PCONP |= (1 << 12);
    LPC_SC->PCONP |= (1 << 6);

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

        if(adc_val< 1241) LPC_PWM1->MR4 = 7500;
        else if(adc_val < 2482) LPC_PWM1->MR4 = 15000;
        else LPC_PWM1->MR4 = 22500;
        
        LPC_PWM1->LER = (1 << 4);
    }
}