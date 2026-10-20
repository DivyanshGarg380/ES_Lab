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
   Write an Embedded C program to read the analog input from ADC channel 5 (P1.31) 
   and display the eight most significant bits of the converted digital value on eight LEDs connected to P0.4–P0.11.
*/

#include <LPC17xx.h>

unsigned long adc_value;
unsigned int led_value;

int main(void) {
    SystemInit();
    SystemCoreClockUpdate();

    LPC_PINCON->PINSEL0 = 0;
    LPC_GPIO0->FIODIR |= 0x0FF0;

    LPC_PINCON->PINSEL3 |= 0xC0000000;
    LPC_SC->PCONP |= 1 << 12; // powering up the ADC

    while(1) {
        // select ADC channel 5 -> enable and start

        LPC_ADC->ADCR = (1 << 5) | (1 << 21) | (1 << 24);
        
        while((LPC_ADC->ADGDR & (1 << 31)) == 0);

        adc_value = LPC_ADC->ADGDR;
        adc_value = (adc_value >> 4) & 0XFFF;

        led_value = adc_value >> 4; // keeping 8 most significant bits
        LPC_GPIO0->FIOPIN = (led_value << 4);
    }
}

