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
    Design a digital voltage-level indicator using the ADC and 7-segment display.
    - Read the analog voltage from ADC channel 5 using POT1.
    - Display the voltage in millivolts on all four 7-segment digits (e.g., 1650 for 1.650 V).
    - If the voltage is below 1.0 V, display 0000.
    - If the voltage is above 3.0 V, display 3000.
    - Continuously update the display as the potentiometer changes.
*/

#include <LPC17xx.h>

#define FIRST_SEG   0xF87FFFFF
#define SECOND_SEG  0xF8FFFFFF
#define THIRD_SEG   0xF97FFFFF
#define FOURTH_SEG  0xF9FFFFFF

unsigned int seg_code[] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F};

void delay(void) {
    unsigned int u;
    for(u = 0; u < 3000; ++u);
}

void display(unsigned int value) {
    unsigned int d1, d2, d3, d4;
    d1 = value / 1000;
    d2 = (value / 100) % 10;
    d3 = (value / 10) % 10;
    d4 = value % 10;

    LPC_GPIO0->FIOPIN = seg_code[d1] << 4;
    LPC_GPIO1->FIOPIN = FIRST_SEG;
    delay();

    LPC_GPIO0->FIOPIN = seg_code[d2] << 4;
    LPC_GPIO1->FIOPIN = SECOND_SEG;
    delay();

    LPC_GPIO0->FIOPIN = seg_code[d3] << 4;
    LPC_GPIO1->FIOPIN = THIRD_SEG;
    delay();

    LPC_GPIO0->FIOPIN = seg_code[d4] << 4;
    LPC_GPIO1->FIOPIN = FOURTH_SEG;
    delay();

    LPC_GPIO1->FIOPIN = 0xFFFFFFFF;
}

int main(void) {
    unsigned int adc_value;
    unsigned int voltage;

    LPC_PINCON->PINSEL3 = 0xC0000000; // adc 5
    LPC_GPIO0->FIODIR |= 0x00000FF0;  // SSD data lines
    LPC_GPIO1->FIODIR |= 0x07800000;  // SSD enable lines

    LPC_SC->PCONP |= (1 << 12); // enable ADC
    
    while(1) {
        LPC_ADC->ADCR = (1 << 5) | (1 << 21) | (1 << 24);
        while(!(LPC_ADC->ADGDR & (1 << 31)));

        adc_value = (LPC_ADC->ADGDR >> 4) & 0xFFF;
        voltage = (adc_value * 3300) / 4095;

        if(voltage < 1000) voltage = 0;
        if(voltage > 3000) voltage = 3000;
        for(i = 0; i < 100; ++i) display(voltage);
    }
}

