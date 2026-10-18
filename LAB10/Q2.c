/*
Author :

███████╗████████╗ █████╗ ██████╗  ███╗   ███╗ █████╗ ███╗   ██╗
██╔════╝╚══██╔══╝██╔══██╗██╔══██╗ ████╗ ████║██╔══██╗████╗  ██║
███████╗   ██║   ███████║██████╔╝ ██╔████╔██║███████║██╔██╗ ██║
╚════██║   ██║   ██╔══██║██║  ██║ ██║╚██╔╝██║██╔══██║██║╚██╗██║
███████║   ██║   ██║  ██║██║  ██║ ██║ ╚═╝ ██║██║  ██║██║ ╚████║
╚══════╝   ╚═╝   ╚═╝  ╚═╝╚═╝  ╚═╝ ╚═╝     ╚═╝╚═╝  ╚═╝╚═╝  ╚═══╝  STARMAN248
*/

#include <LPC17xx.h>
#include <stdio.h>
#include "AN_LCD.h"

#define Ref_Vtg 3.300
#define Full_Scale 0xFFF

int main(void) {
    unsigned long adc4, adc5;
    unsigned long diff;
    unsigned int i;
    
    float v4, v5, difference;
    unsigned char vtg[10], dval[10];
    unsigned char Msg1[] = "VOLT DIFFER:";
    unsigned char Msg2[] = "ADC DIFFER:";

    SystemInit();
    SystemCoreClockUpdate();

    LPC_SC->PCONP |= (1 << 15);
    lcd_init();

    LPC_PINCON->PINSEL3 |= (3 << 28);
    LPC_PINCON->PINSEL3 |= (3 << 30);

    LPC_SC->PCONP |= (1 << 12);
    temp1 = 0x80;
    lcd_com();
    lcd_puts(Msg1);

     temp1 = 0xC0;
    lcd_com();
    lcd_puts(Msg2);

    while(1) {
        LPC_ADC->ADCR = (1 <<4) | (1 << 21) | (1 << 24);
        while((LPC_ADC->ADGDR & (1 << 31)) == 0);

        adc4 = (LPC_ADC->ADGDR >> 4) & 0xFFF;

        LPC_ADC->ADCR = (1 << 5) | (1 << 21) | (1 << 24);
        while ((LPC_ADC->ADGDR & (1 << 31)) == 0);

        adc5 = (LPC_ADC->ADGDR >> 4) & 0xFFF;

        if(adc4 >= adc5) diff = adc4 - adc5;
        else diff = adc5 - adc4;

        v4 = ((float)adc4 * Ref_Vtg) / Full_Scale;
        v5 = ((float)adc5 * Ref_Vtg) / Full_Scale;

        if (v4 >= v5) difference = v4 - v5;
        else difference = v5 - v4;

        sprintf((char *)vtg, "%3.2fV", difference);
        sprintf((char *)dval, "%lX", diff);

        temp1 = 0x8C;
        lcd_com();
        lcd_puts(vtg);

        temp1 = 0xCB;
        lcd_com();
        lcd_puts(dval);

        for(i = 0; i < 20000; ++i);
    }
}