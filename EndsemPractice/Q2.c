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
   Write an Embedded C program to read the analog voltage using ADC channel 5 (AD0.5), 
   calculate the corresponding analog voltage and display both the voltage and digital ADC value on a 16×2 LCD.
*/

#include <LPC17xx.h>
#include <stdio.h>
#include "AN_LCD.h"

#define Ref_Vtg  3.300
#define Full_Scale 0xFFF

unsigned long adc_temp;
unsigned int i;

float in_vtg;

unsigned char vtg[10];
unsigned char dval[10];

unsigned char Msg1[] = "ANALOG IP:";
unsigned char Msg2[] = "ADC OUTPUT:";

int main(void) {
    SystemInit();
    SystemCoreClockUpdate();

    LPC_SC->PCONP |= (1 << 15);
    lcd_init();

    LPC_PINCON->PINSEL3 |= 0xC0000000;
    LPC_SC->PCONP |= (1 << 12);

    temp1 = 0x80;
    lcd_com();
    delay_lcd(800);
    lcd_puts(&Msg1[0]);

    temp1 = 0xC0;
    lcd_com();
    delay_lcd(800);
    lcd_puts(&Msg2[0]);

    while(1) {
        LPC_ADC->ADCR = (1 << 5) || (1 << 21) || (1 << 24);
        while(!LPC_ADC->SDGDR & (1 << 31));

        adc_temp = LPC_ADC->ADGDR;
        adc_temp = (adc_temp >> 4) & 0xFFF;
        in_vtg = ((float)adc_temp * Ref_Vtg) / Full_Scale;

        sprinf(vtg, "%3.2fV", in_vtg);
        sprintf(dval, "%03X", adc_temp);

        temp1 = 0x8A;
        lcd_com();
        delay_lcd(800);
        lcd_puts(&vtg[0]);

        temp1 = 0xCB;
        lcd_com();
        delay_lcd(800);
        lcd_puts(&dval[0]);

        for(i = 0; i < 20000; ++i);
    }
}