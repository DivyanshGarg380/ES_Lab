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
    Write an Embedded C program for the LPC1768 microcontroller to interface ADC Channel 5 (AD0.5) with eight LEDs connected to Port 2. 
    Based on the analog input voltage, glow the corresponding number of LEDs sequentially, such that only one LED remains ON at a time, with a suitable delay between consecutive LEDs. 
    and LEDs connector to CNB.

    CCE-C2 Question
*/

#include <LPC17xx.h>

void delay(void) {
    unsigned int i;
    for(i = 0; i < 500000; i++);
}

int main(void) {
    unsigned int adc_val, count, i;
    unsigned long LED;
    SystemInit();
    SystemCoreClockUpdate();

    LPC_PINCON->PINSEL3 = 0xC0000000;
    LPC_PINCON->PINSEL0 = 0; 
    LPC_GPIO0->FIODIR |= 0x00000FF0;

    LPC_SC->PCONP |= (1 << 12);
    while(1) {
        LPC_ADC->ADCR = (1 << 5) | (1 << 21) | (1 << 24);
        while(!(LPC_ADC->ADGDR & (1 << 31)));

        adc_val = (LPC_ADC->ADGDR >> 4) & 0xFFF;

        LED = 0x00000010;

        count = (adc_val * 8) / 4096;
        for(i = 0; i < count; ++i) {
            LPC_GPIO0->FIOCLR = 0x00000FF0;
            LPC_GPIO0->FIOSET = LED;
            LED <<= 1;
            delay();
        }

        LPC_GPIO0->FIOCLR = 0x00000FF0;
    }
}
