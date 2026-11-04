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
    Write an LPC1768 program to read the analog voltage from ADC channel 5 (P1.31) using POT1 and display 
    the corresponding ADC value on the 4-digit Seven Segment Display.
    
    - Read the ADC value continuously (range: 0–4095).
    - Display the value as a 4-digit decimal number.
    - Example: ADC value 275 should be displayed as 0275.
    - Use multiplexing to keep all four digits visible.
    - Update the display as the potentiometer is adjusted.
*/

#include <LPC17xx.h>

#define FIRST_SEG 0xF87FFFFF
#define SECOND_SEG 0xF8FFFFFF
#define THIRD_SEG 0xF97FFFFF
#define FOURTH_SEG 0xF9FFFFFF

unsigned int seg_codes[] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F};

unsigned int adc_value;
unsigned int d1, d2, d3, d4, i, j;

void delay(void) {
    for(j = 0; j < 1000; ++j);
}

void display(void) {
    LPC_GPIO1->FIOPIN = FIRST_SEG;
    LPC_GPIO0->FIOPIN = seg_codes[d1] << 4;
    delay();

    LPC_GPIO1->FIOPIN = SECOND_SEG;
    LPC_GPIO0->FIOPIN = seg_codes[d2] << 4;
    delay();

    LPC_GPIO1->FIOPIN = THIRD_SEG;
    LPC_GPIO0->FIOPIN = seg_codes[d3] << 4;
    delay();

    LPC_GPIO1->FIOPIN = FOURTH_SEG;
    LPC_GPIO0->FIOPIN = seg_codes[d4] << 4;
    delay();
}

int main(void) {
    LPC_SC->PCONP |= (1 << 12);
    LPC_PINCON->PINSEL3 = 0xC0000000;

    LPC_PINCON->PINSEL0 = 0;
    LPC_GPIO0->FIODIR |= 0x00000FF0;
    LPC_GPIO1->FIODIR |= 0x07800000;

    while(1) {
        LPC_ADC->ADCR = (1 << 5) | (1 << 21) | (1 << 24);
        while (!(LPC_ADC->ADGDR & (1 << 31)));
        adc_value = (LPC_ADC->ADGDR >> 4) & 0xFFF;


        d1 = adc_value / 1000;
        d2 = (adc_value / 100) % 10;
        d3 = (adc_value / 10) % 10;
        d4 = adc_value % 10;

        for (i = 0; i < 200; i++) display();
    }
}