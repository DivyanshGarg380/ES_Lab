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

#define RS_CTRL 0x08000000
#define EN_CTRL 0x10000000
#define DT_CTRL 0x07800000

unsigned long int temp1, temp2, i;
unsigned char flag1;
unsigned int row, col, key;
char keys[] = "0123456789ABCDEF";

unsigned long int init_command[] = {0x30, 0x30, 0x30, 0x20, 0x28, 0x0C, 0x06, 0x01, 0x80};

void lcd_write(void);
void port_write(void);
void delay_lcd(unsigned int r1);

int main(void) {
    unsigned long int temp;
    SystemInit();
    SystemCoreClockUpdate();

    LPC_GPIO0->FIODIR |= DT_CTRL | RS_CTRL | EN_CTRL;
    flag1 = 0;
    for(i = 0; i < 9; ++i) {
        temp1 = init_command[i];
        lcd_write();
    }

    LPC_PINCON->PINSEL4 = 0;
    LPC_GPIO2->FIODIR = 0x00003C00;

    LPC_PINCON->PINSEL3 = 0;
    LPC_GPIO1->FIODIR = ~0x07800000;

    while(1) {
        for(row = 0; row < 4; ++row) {
            LPC_GPIO2->FIOCLR = 0x00003C00;
            LPC_GPIO2->FIOSET = (1 << (10 + row));

            delay_lcd(100);
            temp = LPC_GPIO1->FIOPIN & 0x07800000;
            if(temp != 0) {
                if(temp == 1 << 23) col = 0;
                else if(temp == 1 << 24) col = 1;
                else if(temp == 1 << 25) col = 2;
                else if(temp == 1 << 26) col = 3;
                else continue;
                
                key = row * 4 + col;
                flag1 = 0;
                temp1 = 0x01;
                lcd_write();

                flag1 = 1;
                temp1 = keys[key];
                lcd_write();

                while ((LPC_GPIO1->FIOPIN & 0x07800000) != 0);
            }
        }
    }
}

void lcd_write(void) {
    unsigned char flag2;
    flag2 = (flag1 == 1) ? 0 : ((temp1 == 0x30) || (temp1 == 0x20)) ? 1 : 0;

    temp2 = (temp1 & 0xF0) << 19;
    port_write();

    if (flag2 == 0) {
        temp2 = (temp1 & 0x0F) << 23;
        port_write();
    }
}

void port_write(void) {
    LPC_GPIO0->FIOPIN = temp2;

    if (flag1 == 0) LPC_GPIO0->FIOCLR = RS_CTRL;
    else LPC_GPIO0->FIOSET = RS_CTRL;

    LPC_GPIO0->FIOSET = EN_CTRL;
    delay_lcd(25);
    LPC_GPIO0->FIOCLR = EN_CTRL;
    delay_lcd(30000);
}

void delay_lcd(unsigned int r1) {
    unsigned int r;
    for (r = 0; r < r1; r++);
}