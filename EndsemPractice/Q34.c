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
    Write an LPC1768 program that accepts a two-digit number through the 4×4 keypad and displays it on the LCD.
    
    - The user enters two numeric keys (0–9).
    - Display the entered digits on the first line of the LCD.
    - Calculate and display the square of the entered number on the second line.
    - Ignore non-numeric keys (A–D, *, #).
    - After displaying the result, wait for the next two-digit input.
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

char keys[4][4] = {
    {'1','2','3','A'},
    {'4','5','6','B'},
    {'7','8','9','C'},
    {'*','0','#','D'}
};

char entered[2];
unsigned int row, col, count = 0;
unsigned int number, square;

char get_key(void) {
    unsigned int val;
    while(1) {
        for(row = 0; row < 4; ++row) {
            LPC_GPIO2->FIOCLR = 0x00003C00;
            LPC_GPIO2->FIOSET = (1 << (10 + row));

            val = LPC_GPIO1->FIOPIN & 0x07800000;
            if(val) {
                for(col = 0; col < 4; ++col) {
                    if(val & (1 << (23 + col))) {
                        delay_lcd(3000);
                        while(LPC_GPIO1->FIOPIN & 0x07800000);
                        return keys[row][col];
                    }
                }
            }
        }
    }
}


int main(void) {
    char msg1[] = "NUMBER: 00";
    char msg2[] = "SQUARE: 0000";

    LPC_PINCON->PINSEL3 = 0;
    LPC_PINCON->PINSEL4 = 0;
    LPC_GPIO2->FIODIR |= 0x3C00;
    LPC_GPIO1->FIODIR &= ~0x07800000;

    LPC_GPIO0->FIODIR |= RS_CTRL | EN_CTRL | DT_CTRL;

    flag1 = 0;
    for(i = 0; i < 9; ++i) {
        temp1 = init_command[i];
        lcd_write();
    }

    while(1) {
        count = 0;
        while(count < 2) {
            char key = get_key();
            if(key >= '0' && key <= '9') {
                entered[count] = key;
                count++;
            }
        }

        number = (entered[0] - '0') * 10 + (entered[1] - '0');
        square = number * number;

        msg1[8] = entered[0];
        msg1[9] = entered[1];

        msg2[8]  = (square / 1000) + '0';
        msg2[9]  = ((square / 100) % 10) + '0';
        msg2[10] = ((square / 10) % 10) + '0';
        msg2[11] = (square % 10) + '0';
        
        flag1 = 0;
        temp1 = 0x01;
        lcd_write();

        temp1 = 0x80;
        lcd_write();

        flag1 = 1;
        for (i = 0; msg1[i] != '\0'; i++) {
            temp1 = msg1[i];
            lcd_write();
        }

        flag1 = 0;
        temp1 = 0xC0;
        lcd_write();

        flag1 = 1;
        for (i = 0; msg2[i] != '\0'; i++) {
            temp1 = msg2[i];
            lcd_write();
        }

        delay_lcd(2000);
    }
}
