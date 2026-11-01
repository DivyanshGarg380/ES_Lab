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
    Write an Embedded C program to:
    1. Read analog input from POT1 using ADC channel 5.
    2. Calculate the input voltage.
    3. Display the voltage on the first LCD line.
    4. If voltage exceeds 2.0 V, display HIGH VOLTAGE on the second line and turn ON an LED.
    5. Otherwise display NORMAL and turn OFF the LED.
*/

#include <LPC17xx.h>
#include <stdio.h>

#define RS_CTRL 0x08000000
#define EN_CTRL 0x10000000
#define DT_CTRL 0x07800000

#define LED 0x00000010

#define Ref_Vtg  3.300
#define Full_Scale 4095

unsigned long adc_value;
float voltage;

unsigned int i;

unsigned char line1[17];
unsigned char line2[17];

void delay_lcd(unsigned int r1);
void lcd_init(void);
void lcd_com(unsigned char cmd);
void lcd_data(unsigned char data);
void lcd_puts(unsigned char *str);
void lcd_send_nibble(unsigned char nibble);

int main(void) {
    SystemInit();
    SystemCoreClockUpdate();

    LPC_PINCON->PINSEL0 = 0;
    LPC_PINCON->PINSEL1 = 0;

    LPC_PINCON->PINSEL3 |= 0xC0000000;
    LPC_GPIO0->FIODIR |= LED;

    LPC_GPIO0->FIODIR |= DT_CTRL | RS_CTRL | EN_CTRL;

    LPC_SC->PCONP |= (1 << 12); // enable ADC
    lcd_init();

    while(1) {
        LPC_ADC->ADCR = (1 << 5) | (1 << 21) | (1 << 24);
        while(!(LPC_ADC->ADGDR & (1 << 31)));
        adc_value = (LPC_ADC->ADGDR >> 4) & 0xFFF;

        voltage = ((float)adc_value * Ref_Vtg) / Full_Scale;
        sprintf((char *)line1, "VOLTAGE:%1.2fV", voltage);
        lcd_com(0x80);
        lcd_puts(line1);

        if(voltage > 2.0) {
            sprintf((char *)line2, "HIGH VOLTAGE");
            LPC_GPIO0->FIOSET = LED;
        } else {
            sprintf((char *)line2, "NORMAL");
            LPC_GPIO0->FIOCLR = LED;
        }

        lcd_com(0xC0);
        lcd_puts(line2);
        for(i = 0; i < 20000; ++i);
    }
}

void lcd_init(void) {
    delay_lcd(50000);

    lcd_send_nibble(0x03);
    delay_lcd(50000);

    lcd_send_nibble(0x03);
    delay_lcd(10000);

    lcd_send_nibble(0x03);
    delay_lcd(10000);

    lcd_send_nibble(0x02);

    lcd_com(0x28);
    lcd_com(0x0C);
    lcd_com(0x06);
    lcd_com(0x01);
    lcd_com(0x80);
}

void lcd_send_nibble(unsigned char nibble) {
    LPC_GPIO0->FIOCLR = DT_CTRL;
    LPC_GPIO0->FIOSET = ((nibble & 0x0F) << 23);

    LPC_GPIO0->FIOSET = EN_CTRL;
    delay_lcd(1000);

    LPC_GPIO0->FIOCLR = EN_CTRL;
    delay_lcd(1000);
}

void lcd_com(unsigned char cmd) {
    LPC_GPIO0->FIOCLR = RS_CTRL;
    lcd_send_nibble(cmd >> 4);
    lcd_send_nibble(cmd & 0X0F);
    delay_lcd(2000);
}   

void lcd_data(unsigned char data) {
    LPC_GPIO0->FIOSET = RS_CTRL;
    lcd_send_nibble(data >> 4);
    lcd_send_nibble(data & 0x0F);
    delay_lcd(2000);
}

void lcd_puts(unsigned char *str) {
    unsigned int k = 0;
    while(str[k] != '\0'){
        lcd_data(str[k++]);
    }
}

void delay_lcd(unsigned int r1) {
    unsigned int r;
    for(r = 0; r < r1; r++);
}
