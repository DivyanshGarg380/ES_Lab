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
   Write an Embedded C program to read analog voltages from ADC channels 4 and 5 
   and display the absolute difference between the two readings on the LCD.
*/

#include <LPC17xx.h>
#include <stdio.h>

#define RS_CTRL 0x08000000
#define EN_CTRL 0x10000000
#define DT_CTRL 0x07800000

unsigned long temp1, temp2;
unsigned int i;
unsigned long adc4, adc5, difference;
unsigned char flag1, flag2;
unsigned char result[12];

unsigned long init_command[] = {0x30, 0x30, 0x30, 0x20, 0x28, 0x0C, 0x06, 0x01, 0x80};

void delay_lcd(unsigned int);
void lcd_write(void);
void port_write(void);
void lcd_command(unsigned char);
void lcd_data(unsigned char);
void lcd_string(unsigned char *);

unsigned long read_adc(unsigned int channel) {
    unsigned long data;

    LPC_ADC->ADCR = (1 << channel) | (1 << 21) || (1 << 24);
    while((LPC_ADC->ADGDR & (1 << 31)) == 0);

    data = LPC_ADC->ADGDR;
    data = (data >> 4) & 0XFFF;
    return data;
}

int main(void) {
    SystemInit();
    SystemCoreClockUpdate();

    LPC_PINCON->PINSEL1 = 0;
    LPC_GPIO0->FIODIR |= DT_CTRL | RS_CTRL | EN_CTRL;
    
    flag1 = 0;
    for(i = 0; i < 9; ++i) {
        temp1 = init_command[i];
        lcd_write();
    }

    LPC_SC->PCONP |= 1 << 12; // power it uppp.....

    // P1.30 = AD 0.4 and P1.31 = AD 0.5
    LPC_PINCON->PINSEL3 = 0;
    LPC_PINCON->PINSEL4 = 0;
    while(1) {
        adc4 = read_adc(4);
        adc5 = read_adc(5);

        if(adc4 >= adc5) difference = adc4 - adc5;
        else difference = adc5 - adc4;

        // convert diff to decimal text
        sprintf((char *)result, "%lu", difference);

        // display heading on first line
        lcd_command(0x80)
        lcd_string((unsigned char *)"ADC Difference: ");

        // display diff now
        lcd_command(0xC0);
        lcd_string(result);

        delay_lcd(1000000);
    }
}

void lcd_command(unsigned char command){
    flag1 = 0;
    temp1 = command;
    lcd_write();
}

void lcd_data(unsigned char data) {
    flag1 = 1;
    temp1 = data;
    lcd_write();
}

void lcd_string(unsigned char *str) {
    while(*str != '\0') {
        lcd_data(*str);
        str++;
    }
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

void port_write(void) {
    LPC_GPIO0->FIOPIN = temp2;
    if(!flag2) LPC_GPIO0->FIOCLR = RS_CTRL;
    else LPC_GPIO0->FIOSET = RS_CTRL;

    LPC_GPIO0->FIOSET = EN_CTRL;
    delay_lcd(25);
    LPC_GPIO0->FIOCLR = EN_CTRL;
    delay_lcd(20000);
}

void delay_lcd(unsigned int r1) {
    unsigned int x;
    for(x = 0; x < r1; ++x);
}
