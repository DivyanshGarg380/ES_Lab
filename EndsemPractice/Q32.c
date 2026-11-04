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
    Write an LPC1768 program to generate a PWM signal using PWM1.4 (P1.23) and display its current duty cycle on the LCD.
    
    - Start with a duty cycle of 20%.
    - Increase the duty cycle by 20% every 2 seconds.
    - After reaching 100%, reset it to 20% and repeat.
    - Display the current duty cycle on the second line of the LCD, e.g. DUTY: 40%.
    - Keep the PWM output and LCD display updated continuously.
*/

#include <LPC17xx.h>

#define RS_CTRL 0x08000000
#define EN_CTRL 0x10000000
#define DT_CTRL 0x07800000

unsigned long int temp1 = 0, temp2 = 0, i, j, r;
unsigned char flag1 = 0, flag2 = 0;

unsigned int init_commands[] = {0x30, 0x30, 0x30, 0x20, 0x28, 0x0C, 0x06, 0x01, 0x80};

unsigned int duty = 20;
unsigned int mr_value;

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

void lcd_string(unsigned char *str) {
    flag1 = 1;
    while(*str) {
        temp1 = *str++;
        lcd_write();
    }
}

int main(void) {
    unsigned char msg[] = "DUTY: 000%";

    LPC_SC->PCONP |= (1 << 6);
    LPC_PINCON->PINSEL3 &= ~(3 << 14);
    LPC_PINCON->PINSEL3 |= (2 << 14);

    LPC_PINCON->PINSEL1 = 0;
    LPC_PINCON->PINSEL4 = 0;

    LPC_GPIO0->FIODIR |= RS_CTRL | EN_CTRL | DT_CTRL;

    flag1 = 0;
    for(i = 0; i < 9; ++i) {
        temp1 = init_commands[i];
        lcd_write();
    }

    LPC_PWM1->PR = 0;
    LPC_PWM1->PCR = (1 << 12);
    LPC_PWM1->MCR = 0x03;
    LPC_PWM1->MR0 = 30000;

    mr_value = (30000 * duty) / 100;
    LPC_PWM1->MR4 = mr_value;
    LPC_PWM1->LER = (1 << 0) | (1 << 4);

    LPC_PWM1->TCR = 0x02;
    LPC_PWM1->TCR = 0x09;

    while(1) {
        msg[6] = (duty / 100) + '0';
        msg[7] = ((duty / 10) % 10) + '0';
        msg[8] = (duty % 10) + '0';

        flag1 = 0;
        temp1 = 0x80;
        lcd_write();

        lcd_string(msg);
        for(j = 0; j < 200; ++j) delay_lcd(2000);

        duty += 20;
        if(duty > 100) duty = 20;

        mr_value = (30000 * duty) / 100;
        LPC_PWM1->MR4 = mr_value;
        LPC_PWM1->LER = (1 << 4);
    }
}
