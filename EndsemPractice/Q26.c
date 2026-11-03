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
    Write an Embedded C program for LPC1768 to control the brightness of an LED using a 4×4 matrix keypad and PWM1.4.
    
    - Press 1 to set brightness to 25%.
    - Press 2 to set brightness to 50%.
    - Press 3 to set brightness to 75%.
    - Press 4 to set brightness to 100%.
    - Display the selected brightness percentage on a 16×2 LCD.
    - The system must continuously accept new keypad inputs.
*/

#include <LPC17xx.h>

#define RS_CTRL 0x08000000
#define EN_CTRL 0x10000000
#define DT_CTRL 0x07800000

#define COL_MASK 0x00078000    // P0.15 - P0.18
#define ROW_MASK 0x00780000    // P0.19 - P0.22

unsigned long int temp1 = 0, temp2 = 0;
unsigned char flag1 = 0, flag2 = 0;

unsigned long int init_commands[] = {0x30, 0x30, 0x30, 0x20, 0x28, 0x0C, 0x06, 0x01, 0x80};

char keys[4][4] = {
    {'1','2','3','A'},
    {'4','5','6','B'},
    {'7','8','9','C'},
    {'*','0','#','D'}
};

void delay_lcd(unsigned int r1) {
    unsigned int t;
    for(t = 0; t < r1; ++t);
}

void port_write(void) {
    LPC_GPIO0->FIOPIN = temp2;
    if(!flag1) LPC_GPIO0->FIOCLR = RS_CTRL;
    else LPC_GPIO0->FIOSET = RS_CTRL;
    
    LPC_GPIO0->FIOSET = EN_CTRL;
    delay_lcd(25);
    LPC_GPIO0->FIOCLR = EN_CTRL;
    delay_lcd(2000);
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

char get_key(void) {
    unsigned int row, col, value;
    while(1) {
        for(row = 0; row < 4; ++row) {
            LPC_GPIO0->FIOCLR = ROW_MASK;
            LPC_GPIO0->FIOSET = (1 << (row + 19));
            delay_lcd(1000);

            value = LPC_GPIO0->FIOPIN & COL_MASK;
            if(value != 0) {
                for(col = 0; col < 4; ++col) {
                    if(value & (1 << (15 + col))) {
                        char key = keys[row][col];
                        while(LPC_GPIO0->FIOPIN & (1 << (15 + col)));

                        delay_lcd(3000);
                        return key;
                    }
                }
            }
        }
    }
}

int main(void) {
    unsigned int i, duty;
    char key;
    char message[] = "BRIGHTNESS: 00%";

    SystemInit();
    SystemCoreClockUpdate();

    LPC_PINCON->PINSEL1 = 0;
    LPC_GPIO0->FIODIR |= RS_CTRL | EN_CTRL | DT_CTRL;
    
    flag1 = 0;
    for(i = 0; i < 9; ++i) {
        temp1 = init_commands[i];
        lcd_write();
    }

    LPC_PINCON->PINSEL3 &= ~(3 << 14); // PWM1.4 on P1.23
    LPC_PINCON->PINSEL3 |= (2 << 14);

    LPC_PINCON->PINSEL0 = 0;
    LPC_PINCON->PINSEL1 = 0;
    LPC_GPIO0->FIODIR &= ~COL_MASK;
    LPC_GPIO0->FIODIR |= ROW_MASK;

    LPC_SC->PCONP |= (1 << 6);
    LPC_PWM1->PCR = (1 << 12);
    LPC_PWM1->MCR = 0x03;
    LPC_PWM1->MR0 = 30000;
    LPC_PWM1->MR4 = 7500;

    LPC_PWM1->LER = (1 << 0) | (1 << 4);
    LPC_PWM1->TCR = 0x02;
    LPC_PWM1->TCR = 0X09;

    while(1) {
        key = get_key();

        if(key >= '1' && key <= '4') {
            duty = (key - '0') * 25;
            LPC_PWM1->MR4 = (30000 * duty) / 100;
            LPC_PWM1->LER = (1 << 4);

            message[12] = '0' + (duty / 10);
            message[13] = '0' + duty % 10;

            flag1 = 0;
            temp1 = 0x01;
            lcd_write();
            temp1 = 0x80;
            lcd_write();

            flag1 = 1;
            for(i = 0; message[i] != '\0'; ++i) {
                temp1 = message[i];
                lcd_write();
            }
        }
    }
}