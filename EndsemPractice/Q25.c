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
    Write an Embedded C program for LPC1768 to read the analog voltage from ADC channel AD0.5 (P1.31) and control 8 LEDs connected to P0.4–P0.11.
    
    - 0–1.1 V: LEDs glow one by one from left to right.
    - 1.1–2.2 V: LEDs glow one by one from right to left.
    - 2.2–3.3 V: All LEDs blink simultaneously.
    - The pattern should change automatically when the ADC voltage changes.
*/

#include <LPC17xx.h>

#define LED_MASK 0x00000FF0

void delay(void) {
    unsigned int i;
    for(i = 0; i < 300000; i++);
}

unsigned int read_adc(void) {
    LPC_ADC->ADCR = (1 << 5) | (1 << 21) | (1 << 24);
    while(!(LPC_ADC->ADGDR & (1 << 31)));
    return (LPC_ADC->ADGDR >> 4) & 0xFFF;
}

int main(void) {
    unsigned int adc, i;
    LPC_PINCON->PINSEL0 = 0;
    LPC_GPIO0->FIODIR |= LED_MASK;

    LPC_PINCON->PINSEL3 &= ~(3 << 30);
    LPC_PINCON->PINSEL3 |= (3 << 30);

    LPC_SC->PCONP |= (1 << 12);
    while(1) {
        adc = read_adc();
        LPC_GPIO0->FIOCLR = LED_MASK;

        if(adc < 1365) {
            for(i = 4; i <= 11; ++i) {
                LPC_GPIO0->FIOCLR = LED_MASK;
                LPC_GPIO0->FIOSET = (1 << i);
                delay();
            }
        } else if(adc < 2730) {
            for(i = 11; i >= 4; --i) {
                LPC_GPIO0->FIOCLR = LED_MASK;
                LPC_GPIO0->FIOSET = (1 << i);
                delay();
            }
        } else {
            LPC_GPIO0->FIOSET = LED_MASK;
            delay();
            LPC_GPIO0->FIOCLR = LED_MASK;
            delay();
        }
    }
}
