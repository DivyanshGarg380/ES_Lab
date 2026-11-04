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
    Write an LPC1768 program to read analog voltages from ADC channel 4 (P1.30) and ADC channel 5 (P1.31) using POT4 and POT1 respectively.
    
    - Read both ADC channels continuously.
    - Calculate the absolute difference between their ADC readings.
    - Display the difference as a 4-digit decimal value on the second line of the LCD.
    - Example: DIFF: 1234
    - Refresh the LCD continuously as the potentiometers are adjusted.

    IT-C Question (04/11/2026)
*/

#include <LPC17xx.h>

#define RS_CTRL 0x08000000
#define EN_CTRL 0x10000000
#define DT_CTRL 0x07800000

unsigned int temp1 = 0, temp2 = 0, i;
unsigned char flag1 = 0, flag2 = 0;

unsigned long int init_command[] = {0x30, 0x30, 0x30, 0x20, 0x28, 0x0C, 0x06, 0x01, 0x80};

unsigned int adc4, adc5, diff;

void delay_lcd(unsigned int r1) {
    unsigned int r;
    for(r = 0; r < r1; ++r);
}

void port_write(void) {
    LPC_GPIO0->FIOPIN = temp2;
    if(!flag1) LPC_GPIO0->FIOCLR = RS_CTRL;
    else LPC_GPIO0->FIOSET = RS_CTRL;

    LPC_GPIO0->FIOSET = EN_CTRL;
    delay_lcd(25);
    LPC_GPIO0->FIOCLR = EN_CTRL;
    delay_lcd(3000);
}

void lcd_write(void) {
    flag2 = (flag1 == 1) ? 0 : ((temp1 == 0x30) || (temp1 == 0x20)) ? 1 : 0;
    temp2 = temp1 & 0xF0;
    temp2 <<= 19;
    port_write();

    if(!flag2) {
        temp2 = temp1 & 0x0F;
        temp2 <<= 23;
        port_write();
    }
}

int main(void) {
    unsigned char msg[] = "DIFF: 0000";

    LPC_SC->PCONP |= (1 << 12);
    LPC_PINCON->PINSEL3 = 0xF0000000;

    LPC_PINCON->PINSEL1 = 0;
    LPC_PINCON->PINSEL4 = 0;
    LPC_GPIO0->FIODIR |= RS_CTRL | EN_CTRL | DT_CTRL;

    flag1 = 0;
    for(i = 0; i < 9; ++i) {
        temp1 = init_command[i];
        lcd_write();
    }

    while(1) {
        LPC_ADC->ADCR = (1 << 4) | (1 << 21) | ( 1 << 24);
        while (!(LPC_ADC->ADGDR & (1 << 31)));
        adc4 = (LPC_ADC->ADGDR >> 4) & 0xFFF;


        LPC_ADC->ADCR = (1 << 5) | (1 << 21) | ( 1 << 24);
        while (!(LPC_ADC->ADGDR & (1 << 31)));
        adc5 = (LPC_ADC->ADGDR >> 4) & 0xFFF;

        if(adc4 > adc5) diff = adc4 - adc5;
        else diff = adc5 - adc4;

        msg[6] = (diff / 1000) + '0';
        msg[7] = ((diff / 100) % 10) + '0';
        msg[8] = ((diff / 10) % 10) + '0';
        msg[9] = (diff % 10) + '0';

        flag1 = 0;
        temp1 = 0x80;
        lcd_write();

        flag1 = 1;
        for (i = 0; msg[i] != '\0'; i++){
            temp1 = msg[i];
            lcd_write();
        }
    } 
}   