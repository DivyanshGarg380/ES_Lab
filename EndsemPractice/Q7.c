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
    Write an Embedded C program to:
    1. Read analog input from POT1 using ADC channel 5.
    2. Control the brightness of LD10 using PWM1.4.
    3. Turn ON the first general-purpose LED when ADC value exceeds 2047.
    4. Turn OFF the LED otherwise.
    5. PWM brightness must vary proportionally with the ADC input.
*/

#include <LPC17xx.h>

unsigned long adc_value;
unsigned long duty, i;

int main(void) {
    SystemInit();
    SystemCoreClockUpdate();

    LPC_PINCON->PINSEL0 = 0; // P0.4 GPIO
    LPC_PINCON->PINSEL3 = 0xC0008000; // AD0.5 and PWM1.4
    LPC_GPIO0->FIODIR |= (1 << 4); // Led output

    LPC_SC->PCONP |= (1 << 12); // enable ADC
    LPC_SC->PCONP |= (1 << 6); // enable PWM

    LPC_PWM1->PCR = (1 << 12);
    LPC_PWM1->MCR = 0x03;

    LPC_PWM1->MR0 = 30000;
    LPC_PWM1->MR4 = 0;
    
    LPC_PWM1->LER = (1 << 0) | (1 << 4);
    LPC_PWM1->TCR = 0x02; // reset
    LPC_PWM1->TCR = 0x09; // enable counter 

    while(1) {
        LPC_ADC->ADCR = (1 << 5) | (1 << 21) | (1 << 24);
        while(!(LPC_ADC->ADGDR & (1 << 31)));

        adc_value = (LPC_ADC->ADGDR >> 4) & 0xFFF;
        duty = (adc_value * 29999) / 4095;

        // update PWM
        LPC_PWM1->MR4 = duty;
        LPC_PWM1->LER = (1 << 4);

        if(adc_value > 2047) LPC_GPIO0->FIOSET = (1 << 4);
        else LPC_GPIO0->FIOSET = (1 << 4);
    }
}

