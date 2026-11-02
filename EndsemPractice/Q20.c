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
    Design a program to read a key pressed on the 4×4 matrix keypad and display the corresponding character on the LCD.
    - Display KEY PRESSED: on the first LCD line.
    - Display the pressed key on the second line.
    - Continue accepting new keypresses and update the LCD.
*/

#include <LPC17xx.h>

#define RS_CTRL 0x08000000
#define EN_CTRL 0x10000000
#define DT_CTRL 0x07800000

unsigned long int temp1, temp2, i;
unsigned char flag1 = 0, flag2 = 0;

unsigned long int init_command[] = {0x30, 0x30, 0x30, 0x20, 0x28, 0x0C, 0x06, 0x01, 0x80};

char keys[4][4] = {
    {'1','2','3','A'},
    {'4','5','6','B'},
    {'7','8','9','C'},
    {'*','0','#','D'}
};

void lcd_write(void);
void port_write(void);
void delay_lcd(unsigned int);

char get_key(void) {
    unsigned int row, col, value;
    while(1) {
        for(row = 0; row < 4; row++) {
            LPC_GPIO2->FIOCLR = 0x00003C00;
            LPC_GPIO2->FIOSET = (1 << (10 + row));

            value = LPC_GPIO1->FIOPIN & 0x07800000;

            if(value != 0) {
                if(value == (1 << 23)) col = 0;
                else if(value == (1 << 24)) col = 1;
                else if(value == (1 << 25)) col = 2;
                else col = 3;

                while(LPC_GPIO1->FIOPIN & 0x07800000);
                return keys[row][col];
            }
        }
    }
}

int main(void) {
    char key;
    char msg[] = "KEY PRESSED:";

    SystemInit();
    SystemCoreClockUpdate();

    LPC_GPIO0->FIODIR |= DT_CTRL | RS_CTRL | EN_CTRL;
    flag1 = 0;
    for(i = 0; i < 9; ++i) {
        temp1 = init_command[i];
        lcd_write();
    }

    LPC_PINCON->PINSEL3 = 0;
    LPC_PINCON->PINSEL4 = 0;

    LPC_GPIO2->FIODIR |= 0x00003C00;
    LPC_GPIO1->FIODIR &= 0xF87FFFFF;

    while(1) {
        flag1 = 0;
        temp1 = 0x80;
        lcd_write();

        flag1 = 1;
        for(i = 0; msg[i] != '\0'; ++i) {
            temp1 = msg[i];
            lcd_write();
        }

        key = get_key();
        flag1 = 0;
        temp1 = 0xC0;
        lcd_write();
        
        flag1 = 1;
        temp1 = key;
        lcd_write();
    }
}

void lcd_write(void) {
    flag2 = (flag1 == 1) ? 0: ((temp1 == 0x30) || (temp1 == 0x20)) ? 1 : 0;
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
    if(!flag1) LPC_GPIO0->FIOCLR = RS_CTRL;
    else LPC_GPIO0->FIOSET = RS_CTRL;
    
    LPC_GPIO0->FIOSET = EN_CTRL;
    delay_lcd(25);
    LPC_GPIO0->FIOCLR = EN_CTRL;
    delay_lcd(30000);
}

void delay_lcd(unsigned int r1) {
    unsigned int r;
    for(r = 0; r < r1; r++);
}

