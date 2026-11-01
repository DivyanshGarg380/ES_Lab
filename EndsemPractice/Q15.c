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
   Design a temperature-style ADC indicator using POT1 and the 7-segment display.
    - Read the analog value from ADC channel 5.
    - Convert the 12-bit ADC reading into a value from 0 to 99.
    - Display the value on the last two 7-segment digits; keep the first two digits OFF.
    - Continuously update the display as POT1 changes.
*/

#include <LPC17xx.h>

#define FIRST_SEG   0xF87FFFFF
#define SECOND_SEG  0xF8FFFFFF
#define THIRD_SEG   0xF97FFFFF
#define FOURTH_SEG  0xF9FFFFFF

unsigned int seg_code[] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F};

void delay(void) {
    unsigned int i;
    for(i = 0; i < 3000; ++i);
}

void display(unsigned int value) {
    unsigned int tens = value / 10;
    unsigned int ones = value % 10;

    LPC_GPIO1->FIOPIN = FIRST_SEG;
    delay();

    LPC_GPIO1->FIOPIN = SECOND_SEG;
    delay();

    LPC_GPIO0->FIOPIN = seg_code[tens] << 4;
    LPC_GPIO1->FIOPIN = THIRD_SEG;
    delay();

    LPC_GPIO1->FIOPIN = 0xFFFFFFFF;
    LPC_GPIO0->FIOPIN = seg_code[ones] << 4;
    LPC_GPIO1->FIOPIN = FOURTH_SEG;
    delay();

    LPC_GPIO1->FIOPIN = 0xFFFFFFFF;
}

int main(void) {
    unsigned int adc, value;
    LPC_PINCON->PINSEL3 = 0xC0000000;
    LPC_GPIO0->FIODIR |= 0x00000FF0;
    LPC_GPIO1->FIODIR |= 0x07800000;

    LPC_SC->PCONP |= (1 << 12);
    
    while(1) {
        LPC_ADC->ADCR = (1 << 5) | (1 << 21) | (1 << 24);
        while(!(LPC_ADC->ADGDR & (1 << 31)));
        adc = (LPC_ADC->ADGDR >> 4) & 0xFFF;
        value = (adc * 99) / 4095;
        display(value); 
    }
}