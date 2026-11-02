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
    Write an Embedded C program for LPC1768 that reads the analog input from ADC Channel 5 (AD0.5).
    Based on the ADC value, illuminate 1–8 LEDs connected to P0.4–P0.11.
    - If ADC value is below 2048, LEDs should fill from left to right.
    - If ADC value is 2048 or above, LEDs should fill from right to left.
    - Only the selected number of LEDs should remain ON.
    - Update the display continuously as the potentiometer changes.
*/

#include <LPC17xx.h>

void delay(void) {
    unsigned int i;
    for(i = 0; i < 50000; ++i);
}

int main(void) {
    SystemInit();
    SystemCoreClockUpdate();

    unsigned int adc_value, count, i, led_mask;
    
    LPC_PINCON->PINSEL0 = 0;
    LPC_GPIO0->FIODIR |= 0x00000FF0;

    LPC_PINCON->PINSEL3 &= ~(3 << 30);
    LPC_PINCON->PINSEL3 |= (3 << 30);

    LPC_SC->PCONP |= (1 << 12);
    while(1) {
        LPC_ADC->ADCR = (1 << 5) | (1 << 21) | (1 << 24);
        while(!(LPC_ADC->ADGDR & (1 << 31)));

        adc_value = (LPC_ADC->ADGDR >> 4) & 0xFFF;
        count = (adc_value * 8) / 4096;

        if(count == 0) count = 1;
        led_mask = 0;

        if(adc_value < 2048) {
            for(i = 0; i < count; ++i) led_mask |= (1 << (i + 4)); // from P0.4 to P0.11
        } else {
            for(i = 0; i < count; ++i) led_mask |= (1 << (11 - i)); // from P0.11 to P0.4
        } 

        LPC_GPIO0->FIOCLR = 0x00000FF0;
        LPC_GPIO0->FIOSET = led_mask;
        delay();
    }
}