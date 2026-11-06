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
    Write an Embedded C program for LPC1768 to read the analog voltage from ADC channel 5 (POT1) and illuminate one of the four seven segment displays depending on the ADC value:
    0 - 1023 : illuminate the 1st display (U8)
    1024 - 2047 : illuminate the 2nd display (U9)
    2048 - 3071 : illuminate the 3rd display (U10)
    3072 - 4095 : illuminate the 4th display (U11)
    Only one display should be ON at a time.

    CCE-A Question on 06/11/2026 (Question already present in this REPO 😂)
*/

#include <LPC17xx.h>

#define FIRST_SEG 0xF87FFFFF
#define SECOND_SEG 0xF8FFFFFF
#define THIRD_SEG 0xF97FFFFF
#define FOURTH_SEG 0xF9FFFFFF

void delay(void) {
    unsigned int i;
    for(i = 0; i < 1000; ++i);
}

int main(void) {
    unsigned long adc_val;
    unsigned int index;

    SystemInit();
    SystemCoreClockUpdate();

    LPC_SC->PCONP |= (1 << 12);
    LPC_PINCON->PINSEL0 = 0;
    LPC_PINCON->PINSEL3 = 0xC0000000;

    LPC_GPIO0->FIODIR |= 0x00000FF0;
    LPC_GPIO1->FIODIR |= 0x07800000;

    while(1) {
        LPC_ADC->ADCR = (1 << 5) | (1 << 21) | (1 << 24);
        while(!(LPC_ADC->ADGDR & (1 << 31)));
        adc_val = (LPC_ADC->ADGDR >> 4) & 0xFFF;

        if(adc_val <= 1023) {
            LPC_GPIO1->FIOPIN = FIRST_SEG;
            LPC_GPIO0->FIOCLR = 0x00000FF0;
            LPC_GPIO0->FIOSET = (0x7F << 4); // displaying 8 
            // delay();
            // LPC_GPIO0->FIOCLR = 0x00000FF0;
        } else if(adc_val >= 1024 && adc_val <= 2048) {
            LPC_GPIO1->FIOPIN = SECOND_SEG;
            LPC_GPIO0->FIOCLR = 0x00000FF0;
            LPC_GPIO0->FIOSET = (0x7F << 4); // displaying 8 
            // delay();
            // LPC_GPIO0->FIOCLR = 0x00000FF0;
        } else if(adc_val >= 2049 && adc_val <= 3071) {
            LPC_GPIO1->FIOPIN = THIRD_SEG;
            LPC_GPIO0->FIOCLR = 0x00000FF0;
            LPC_GPIO0->FIOSET = (0x7F << 4); // displaying 8 
            // delay();
            // LPC_GPIO0->FIOCLR = 0x00000FF0;
        } else if(adc_val >= 3072 && adc_val <= 4095) {
            LPC_GPIO1->FIOPIN = FOURTH_SEG;
            LPC_GPIO0->FIOCLR = 0x00000FF0;
            LPC_GPIO0->FIOSET = (0x7F << 4); // displaying 8 
            // delay();
            // LPC_GPIO0->FIOCLR = 0x00000FF0;
        }
    }
}
