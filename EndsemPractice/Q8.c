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
    1. Read analog voltage from POT1 using ADC channel 5.
    2. Read analog voltage from POT4 using ADC channel 4.
    3. Compare both ADC values.
    4. If ADC channel 4 is greater than ADC channel 5, vary LD10 brightness proportionally to their difference using PWM1.4.
    5. Otherwise, turn OFF LD10.
    6. Turn ON the first general-purpose LED whenever ADC channel 4 is greater than ADC channel 5.
*/

#include <LPC17xx.h>

unsigned long adc4_value;
unsigned long adc5_value;

unsigned long difference;
unsigned long duty;

unsigned int i;

int main(void) {
    SystemInit();
    SystemCoreClockUpdate();

    LPC_PINCON->PINSEL0 = 0;
    LPC_PINCON->PINSEL3 = 0xF0008000; // AD0.4, AD0.5 and PWM1.4

    LPC_GPIO0->FIODIR |= (1 << 4);

    LPC_SC->PCONP |= (1 << 12);

    LPC_SC->PCONP |= (1 << 6);
    LPC_PWM1->PCR = (1 << 12);

    LPC_PWM1->MCR = 0x03;
    LPC_PWM1->MR0 = 30000;
    LPC_PWM1->MR4 = 0;
    LPC_PWM1->LER = (1 << 0) | (1 << 4);
    LPC_PWM1->TCR = 0x02;
    LPC_PWM1->TCR = 0x09;

    while(1) {
        LPC_ADC->ADCR = (1 << 4) | (1 << 21) | (1 << 24);
        while(!(LPC_ADC->ADGDR & (1 << 31)));
        adc4_value = (LPC_ADC->ADGDR >> 4) & 0XFFF;
        
        LPC_ADC->ADCR = (1 << 5) | (1 << 21) | (1 << 24);
        while(!(LPC_ADC->ADGDR & (1 << 31)));
        adc5_value = (LPC_ADC->ADGDR >> 4) & 0xFFF;

        if(adc4_value > adc5_value) {
            difference = adc4_value - adc5_value;
            duty = (difference * 29999) / 4095;
            
            LPC_PWM1->MR4 = duty;
            LPC_PWM1->LER = (1 << 4);

            LPC_GPIO0->FIOSET = (1 << 4);
        } else {
            // turn off PWM output
            LPC_PWM1->MR4 = 0;
            LPC_PWM1->LER = (1 << 4);
            LPC_GPIO0->FIOCLR = (1 << 4);
        }
    }
}
