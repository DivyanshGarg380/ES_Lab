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
    1. Accept ADC channel selection using the matrix keypad.
    - Press 4 to select AD0.4.
    - Press 5 to select AD0.5.
    2. Read the corresponding analog voltage.
    3. Calculate its digital ADC value and analog voltage.
    4. Display the selected channel on the first LCD line.
    5. Continuously display the digital ADC value and voltage on the second LCD line.
*/

#include <LPC17xx.h>
#include <stdio.h>

#define RS_CTRL 0x08000000
#define EN_CTRL 0x10000000
#define DT_CTRL 0x07800000

#define ROW_MASK 0x00780000
#define COL_MASK 0x00078000

#define Ref_Vtf 3.300
#define Full_Scale 4095

unsigned int i, j, row, col, temp1, channel;
unsigned long adc_val;
float voltage;
unsigned char key;

unsigned char line1[17], line2[17];

unsigned char keypad[4][4] = {
    {'1','2','3','A'},
    {'4','5','6','B'},
    {'7','8','9','C'},
    {'*','0','#','D'}
};

unsigned int row_value[4] = {0x00080000, 0x00100000, 0x00200000, 0x00400000};

void delay_lcd(unsigned int r1);
void lcd_init(void);
void lcd_com(unsigned char cmd);
void lcd_data(unsigned char data);
void lcd_puts(unsigned char *str);
void lcd_send_nibble(unsigned char nibble);
unsigned char keypad_read(void);

int main(void) {
    SystemInit();
    SystemCoreClockUpdate();

    LPC_PINCON->PINSEL0 = 0;
    LPC_PINCON->PINSEL1 = 0;

    LPC_PINCON->PINSEL3 = 0xF0000000; // AD0.4 and AD0.5
    LPC_GPIO0->FIODIR |= ROW_MASK;
    LPC_GPIO0->FIODIR &= ~COL_MASK;

    LPC_GPIO0->FIODIR |= DT_CTRL | RS_CTRL | EN_CTRL;
    LPC_SC->PCONP |= (1 << 12); // enable ADC

    lcd_init();
    while(1) {
        key = keypad_read();
        if(key == '4') channel = 4;
        else if(key == '5') channel = 5;
        else continue;

        sprintf((char *)line1, "CHANNEL: AD0.%u ", channel);
        lcd_com(0x80);
        lcd_puts(line1);

        LPC_ADC->ADCR = (1 << channel) | (1 << 21) | (1 << 24);
        while(!(LPC_ADC->ADGDR & (1 << 31)));

        adc_val = (LPC_ADC->ADGDR >> 4) & 0xFFF;
        voltage = ((float)adc_val * Ref_Vtf) / Full_Scale;

        sprintf((char *)line2, "%04X %1.2fV   ", (unsigned int)adc_val, voltage);
        lcd_com(0xC0);
        lcd_puts(line2);
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
    lcd_send_nibble(cmd & 0x0F);
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
    while(str[k] != '\0') {
        lcd_data(str[k++]);
    }
}

void delay_lcd(unsigned int r1) {
    unsigned int r;
    for(r = 0; r < r1; ++r);
}

unsigned char keypad_read(void) {
    unsigned char key_val;
    while(1) {
        for(row = 0; row < 4; row++) {
            LPC_GPIO0->FIOCLR = ROW_MASK;
            LPC_GPIO0->FIOSET = row_value[row];
            for(j = 0; j < 1000; j++);
            temp1 = LPC_GPIO0->FIOPIN & COL_MASK;

            if(temp1 != 0) {
                if(temp1 & 0x00008000) col = 0;
                else if(temp1 & 0x00010000) col = 1;
                else if(temp1 & 0x00020000) col = 2;
                else if(temp1 & 0x00040000) col = 3;
                else continue;

                key_val = keypad[row][col];
                while(LPC_GPIO0->FIOPIN & COL_MASK);

                return key_val;
            }
        }
    }
}
