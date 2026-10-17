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
#define SW2 (1 << 12)

unsigned long int temp1, temp2, i;
unsigned char flag1;
unsigned long int init_command[] = {0x30, 0x30, 0x30, 0x20, 0x28, 0x0C, 0x06, 0x01, 0x80};

void lcd_write(void);
void port_write(void);
void delay_lcd(unsigned int r1);

int main(void) {
    unsigned int dice = 0;
    SystemInit();
    SystemCoreClockUpdate();

    LPC_GPIO0->FIODIR = DT_CTRL | RS_CTRL | EN_CTRL;
    LPC_PINCON->PINSEL4 = 0;
    LPC_GPIO2->FIODIR = ~SW2;

    flag1 = 0; // initialize LCD
    for(i = 0; i < 9; ++i) {
        temp1 = init_command[i];
        lcd_write();
    }

    // display message
    flag1 = 1;
    temp1 = 'D';
    lcd_write();
    temp1 = 'i';
    lcd_write();
    temp1 = 'c';
    lcd_write();
    temp1 = 'e';
    lcd_write();
    temp1 = ':';
    lcd_write();

    while(1) {
        dice++;
        if(dice > 6) dice = 0;
        delay_lcd(30000);

        // when switch pressed
        if(LPC_GPIO2->FIOPIN & SW2) {
            temp1 = 0xC0;
            flag1 = 0;
            lcd_write();

            flag1 = 1;
            temp1 = dice + '0';
            lcd_write();

            while(LPC_GPIO2->FIOPIN & SW2);
        }
    }
}

void lcd_write(void) {
    unsigned char flag2;
    flag2 = (flag1 == 1) ? 0 : ((temp1 == 0x30) || (temp1 == 0x20)) ? 1 : 0;
    temp2 = (temp1& 0xF0) << 19;
    port_write();

    if(flag2 == 0) {
        temp2 = (temp1 & 0x0F) << 23;
        port_write();
    }
}

void port_write(void) {
    LPC_GPIO0->FIOPIN = temp2;
    if(flag1 == 0) LPC_GPIO0->FIOCLR = RS_CTRL;
    else LPC_GPIO0->FIOSET =  RS_CTRL;
    LPC_GPIO0->FIOSET = EN_CTRL;
    delay_lcd(25);
    LPC_GPIO0->FIOCLR = EN_CTRL;
    delay_lcd(30000);
}

void delay_lcd(unsigned int r1) {
    unsigned int r;
    for(r = 0; r < r1; ++r);
}

