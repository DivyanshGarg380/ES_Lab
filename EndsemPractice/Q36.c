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
    Write an Embedded C program to simulate an 8-bit Johnson counter using the LPC1768 microcontroller.
    
    - Display the 8-bit counter output on the 8 LEDs.
    - Simultaneously display the corresponding 8-bit binary representation on the LCD.
    - Generate the counter sequence continuously with a suitable delay.
*/

#include <LPC17xx.h>

#define LED_MASK 0x00000FF0
#define RS_CTRL 0x08000000
#define EN_CTRL 0x10000000
#define DT_CTRL 0x07800000

unsigned int i, j, temp1 = 0, temp2 = 0;
unsigned int count = 0;
unsigned char flag1 = 0, flag2 = 0;

unsigned long int init_commands[] = {0x30, 0x30, 0x30, 0x20, 0x28, 0x0C, 0x06, 0x01, 0x80};

void delay_lcd(unsigned int r1) {
    unsigned int r;
    for(r = 0; r < r1; ++r);
}

void port_write(void) {
    LPC_GPIO0->FIOCLR = DT_CTRL;
    LPC_GPIO0->FIOSET = temp2;

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

void display_binary(unsigned char val) {
    flag1 = 0;
    temp1 = 0xC0;
    lcd_write();

    int k;
    flag1 = 1;
    for(k = 7; k >= 0; --k) {
        if(val & (1 << k)) temp1 = '1';
        else temp1 = '0';
        lcd_write();
    }
}

int main(void) {
    SystemInit();
    SystemCoreClockUpdate();

    LPC_PINCON->PINSEL0 = 0;
    LPC_PINCON->PINSEL1 = 0;

    LPC_GPIO0->FIODIR |= LED_MASK | DT_CTRL | RS_CTRL | EN_CTRL;

    flag1 = 0;
    for(i = 0; i < 9; ++i) {
        temp1 = init_commands[i];
        lcd_write();
    }

    flag1 = 1;
    temp1 = 'J';
    lcd_write();
    temp1 = 'O';
    lcd_write();
    temp1 = 'H';
    lcd_write();
    temp1 = 'N';
    lcd_write();
    temp1 = 'S';
    lcd_write();
    temp1 = 'O';
    lcd_write();
    temp1 = 'N';
    lcd_write();

    while(1) {
        LPC_GPIO0->FIOCLR = LED_MASK;
        LPC_GPIO0->FIOSET = count << 4;

        display_binary(count);
        for(j = 0; j < 10000; ++j);

        count = (count << 1) | ((count & 0x80) ? 0 : 1);
    }
}