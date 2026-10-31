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
    1. Accept a two-digit threshold percentage using the matrix keypad.
    2. Read analog voltage from POT1 using ADC channel 5.
    3. Calculate the corresponding percentage.
    4. Display the entered threshold on the first LCD line.
    5. Display the live ADC percentage on the second LCD line.
    6. Display LIMIT REACHED when the ADC percentage equals or exceeds the entered threshold.
*/

#include <LPC17xx.h>
#include <stdio.h>
#include "AN_LCD.h"

#define ROW_MASK  0x00780000
#define COL_MASK  0x00078000

unsigned int i, j, row, col;
unsigned int temp1, temp2;

unsigned int threshold = 0;
unsigned int adc_value, percentage;

unsigned char key;

unsigned char keypad[4][4] = {
    {'1','2','3','A'},
    {'4','5','6','B'},
    {'7','8','9','C'},
    {'*','0','#','D'}
};

unsigned int row_value[4] = {0x00080000, 0x00100000, 0x00200000, 0x00400000};

unsigned char msg1[17];
unsigned char msg2[17];

unsigned char keypad_read(void);

int main(void) {
    SystemInit();
    SystemCoreClockUpdate();

    LPC_PINCON->PINSEL0 = 0;
    LPC_PINCON->PINSEL1 = 0;

    LPC_GPIO0->FIODIR |= ROW_MASK;
    LPC_GPIO0->FIODIR &= ~COL_MASK;

    LPC_PINCON->PINSEL3 |= 0xC0000000;
    LPC_SC->PCONP |= (1 << 12); // for ADC initializtion

    LPC_SC->PCONP |= (1 << 15); // for LCD initializtion
    lcd_init();

    key = keypad_read();
    threshold = (key - '0') * 10;
    key = keypad_read();
    threshold += (key - '0');

    while(1) {
        LPC_ADC->ADCR = (1 << 5) || (1 << 21) || (1 << 24);
        while(!(LPC_ADC->ADGDR & (1 << 31)));

        adc_value = LPC_ADC->ADGDR;
        adc_value = (adc_value >> 4) & 0xFFF;

        percentage = (adc_value * 100) / 4095;

        sprintf(msg1, "SET:%02u%%", threshold);
        
        if(percentage >= threshold) sprintf(msg2, "LIMIT REACHED");
        else sprintf(msg2, "ADC:%02u%%", percentage);

        temp1 = 0x80;
        lcd_com();
        delay_lcd(800);
        lcd_puts(msg1);

        temp1 = 0xC0;
        lcd_com();
        delay_lcd(800);
        lcd_puts(msg2);
    } 
}

unsigned char keypad_read(void) {
    unsigned char key_val;
    while(1) {
        for(row = 0; row < 4; ++row) {
            LPC_GPIO0->FIOCLR = ROW_MASK;
            LPC_GPIO0->FIOSET = row_value[row];

            for(j = 0; j < 1000; ++j);

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