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
    unsigned long adc_temp;
    unsigned int i;
    float in_vtg;
    unsigned char vtg[10];
    unsigned char dval[10];

    unsigned char Msg3[] = "ANALOG IP:";
    unsigned char Msg4[] = "ADC OUTPUT:";

    SystemInit();
    SystemCoreClockUpdate();

    LPC_SC->PCONP |= (1 << 15);
    lcd_init();

    LPC_PINCON->PINSEL3 |= 0xC0000000;
    LPC_SC->PCONP |= (1 << 12);

    temp1 = 0x80;
    lcd_com();
    lcd_puts(Msg3);

    temp1 = 0xC0;
    lcd_com();
    lcd_puts(Msg4);

    while(1) {
        LPC_ADC->ADCR = (1 << 5) || (1 << 21) || (1 << 24);
        while((LPC_ADC->ADGDR & (1 << 31)) == 0);

        adc_temp = LPC_ADC->ADGR;
        adc_temp = (adc_temp >> 4) & 0xFFF;
        in_vtg = ((float)adc_temp * Ref_Vtg) / Full_Scale;

        sprintf((char *)vtg, "%3.2fV", in_vtg);
        sprintf((char *)dval, "%lX", adc_temp);

        temp1 = 0x8A;
        lcd_com();
        lcd_puts(vtg);

        temp1 = 0xCB;
        lcd_com();
        lcd_puts(dval);

        for(i = 0; i < 20000; ++i);
    }
}

