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
    Write an Embedded C program for LPC1768 to implement a hexadecimal calculator using a 4×4 matrix keypad and a 16×2 LCD.
    - Enter the first two-digit hexadecimal number using the keypad.
    - Select an arithmetic operator:
    - A → Addition
    - B → Subtraction
    - C → Multiplication
    - D → Division
    - Enter the second two-digit hexadecimal number.
    - Perform the selected operation and display the result on the LCD in hexadecimal.
    - Handle division by zero and negative subtraction results.

    IT-D1 Question
*/

#include <LPC17xx.h>
#include <stdio.h>

#define RS_CTRL 0x08000000
#define EN_CTRL 0x10000000
#define DT_CTRL 0x07800000

unsigned long int temp1 = 0, temp2 = 0;
unsigned int i, j;
unsigned char flag1 = 0, flag2 = 0;

unsigned long int init_command[] = {0x30, 0x30, 0x30, 0x20, 0x28, 0x0C, 0x06, 0x01, 0x80};

char keys[4][4] = {
    {'1','2','3','A'},
    {'4','5','6','B'},
    {'7','8','9','C'},
    {'*','0','#','D'}
};

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
    delay_lcd(300000);
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
    unsigned int row, col;
    unsigned long int val;

    while(1) {
        for(row = 0; row < 4; ++row) {
            LPC_GPIO2->FIOCLR = 0x00003C00;
            LPC_GPIO2->FIOSET = (1 << (row + 10));
            delay_lcd(1000);

            val = LPC_GPIO1->FIOPIN & 0x07800000;
            if(val != 0) {
                if(val == 0x00800000) col = 0;
                else if(val == 0x01000000) col = 1;
                else if(val == 0x02000000) col = 2;
                else if(val == 0x04000000) col = 3;
                else continue;

                while(LPC_GPIO1->FIOPIN & 0x07800000);
                return keys[row][col];
            }
        }
    }
}


unsigned int hex_value(char key) {
    if(key >= '0' && key <= '9') return key - '0';
    if(key >= 'A' && key <= 'D') return key - 'A' + 10;
    if(key == '*') return 14;
    return 15; // '#'
}

unsigned int read_hex(void) {
    unsigned int number = 0;
    unsigned int digit, k;
    for(k = 0; k < 2; ++k) {
        char key = get_key();
        digit = hex_value(key);
        number = (number << 4) | digit;
    }
    return number;
}

int main(void) {
    SystemInit();
    SystemCoreClockUpdate();

    unsigned int num1, num2;
    unsigned long int result;
    char op;

    char display[17];

    LPC_GPIO0->FIODIR |= RS_CTRL | EN_CTRL | DT_CTRL;
    LPC_PINCON->PINSEL3 = 0;
    LPC_PINCON->PINSEL4 = 0;

    LPC_GPIO2->FIODIR |= 0x00003C00;
    LPC_GPIO1->FIODIR &= ~0x07800000;

    flag1 = 0;
    for(i = 0; i < 9; ++i) {
        temp1 = init_command[i];
        lcd_write();
    }

    while(1) {
        flag1 = 0;
        temp1 = 0x80;
        lcd_write();

        flag1 = 1;
        char msg[] = "FIRST HEX: ";
        for(i = 0; msg[i] != '\0'; ++i) {
            temp1 = msg[i];
            lcd_write();
        }

        num1 = read_hex();

        flag1 = 0;
        temp1 = 0x01; // clear
        lcd_write();

        temp1 = 0x80;
        lcd_write();

        flag1 = 1;
        char msg1[] = "OP: ";
        for(i = 0; msg1[i] != '\0'; ++i) {
            temp1 = msg1[i];
            lcd_write();
        }

        op = get_key();

        flag1 = 0;
        temp1 = 0x01;
        lcd_write();
        temp1 = 0x80;
        lcd_write();

        flag1 = 1;
        char msg2[] = "SECOND HEX: ";
        for(i = 0; msg2[i] != '\0'; ++i) {
            temp1 = msg2[i];
            lcd_write();
        }

        num2 = read_hex();

        if(op == 'A') {
            result = num1 + num2;
            sprintf(display, "%lX", result);
        } else if(op == 'B') {
            if(num1 >= num2) sprintf(display, "%X", num1 - num2);
            else sprintf(display, "%X", num2 - num1);
        } else if(op == 'C') {
            result = num1 * num2;
            sprintf(display, "%lX", result);
        } else if(op == 'D') {
            if(num2 == 0) sprintf(display, "DIVISION BY ZERO");
            else {
                result = num1 / num2;
                sprintf(display, "%lX", result);
            }
        } else sprintf(display, "INVALID OPERATOR");

        flag1 = 0;
        temp1 = 0x01;
        lcd_write();

        temp1 = 0x80;
        lcd_write();

        flag1 = 1;
        for(i = 0; display[i] != '\0'; ++i) {
            temp1 = display[i];
            lcd_write();
        }

        delay_lcd(100000);
    }
}