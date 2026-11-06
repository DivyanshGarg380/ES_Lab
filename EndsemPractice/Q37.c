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
    Write an Embedded C program for LPC1768 to implement a PIN lock using the 4x4 matrix keypad and 16x2 LCD.
    1. Stored PIN is "4729".
    2. Line 1 shows "ENTER PIN:". Line 2 shows a '*' for every digit typed.
    3. Only digits 0-9 are accepted. Pressing '*' clears the current entry and restarts input on line 2.
    4. After 4 digits are entered, compare with the stored PIN automatically.
    5. Correct PIN: display "ACCESS GRANTED" and stop.
    6. Wrong PIN: display "WRONG PIN" and "LEFT: n" (attempts remaining), then ask again.
    7. After 3 wrong attempts display "SYSTEM LOCKED" and halt forever.
*/

#include <LPC17xx.h>
#include <string.h>

#define RS_CTRL 0x08000000 // P0.27
#define EN_CTRL 0x10000000 // P0.28
#define DT_CTRL 0x07800000 // P0.23-P0.26

#define COL_MASK 0x07800000 // P1.23-P1.26 inputs
#define ROW_MASK 0x00003C00 // P2.10-P2.13 outputs

unsigned long int temp1 = 0, temp2 = 0;
unsigned char flag1 = 0, flag2 = 0;
unsigned long int init_commands[] = {0x30, 0x30, 0x30, 0x20, 0x28, 0x0C, 0x06, 0x01, 0x80};

unsigned char keys[4][4] = {
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
    if(flag1) LPC_GPIO0->FIOCLR = RS_CTRL;
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

void lcd_init(void) {
    unsigned int i;
    flag1 = 0;
    for(i = 0; i < 9; ++i) {
        temp1 = init_commands[i];
        lcd_write();
    }
}

unsigned char keypad_read(void) {
    unsigned int r, c, val;
    while(1) {
        for(r = 0; r < 4; ++r) {
            LPC_GPIO2->FIOCLR = ROW_MASK;
            LPC_GPIO2->FIOSET = (1 << (10 + r));
            delay_lcd(1000);
            val = LPC_GPIO1->FIOPIN & COL_MASK;
            if(val) {
                if(val & (1 << 23))  c = 0;
                else if(val & (1 << 24)) c = 1;
                else if(val & (1 << 25)) c = 2;
                else if(val & (1 << 26)) c = 3;
                else continue;

                while(LPC_GPIO1->FIOPIN & COL_MASK);
                return keys[r][c];
            }
        }
    }
}

void lcd_cmd(unsigned char c)  { flag1 = 0; temp1 = c; lcd_write(); }
void lcd_data(unsigned char c) { flag1 = 1; temp1 = c; lcd_write(); }
void lcd_puts(const char *s) { while(*s) lcd_data(*s++); }

int main(void) {
    unsigned char pin[5] = "4729", entry[5];
    unsigned int idx, attempts = 0;
    unsigned char key;

    SystemInit();
    SystemCoreClockUpdate();

    LPC_PINCON->PINSEL1 = 0;
    LPC_PINCON->PINSEL3 = 0;
    LPC_PINCON->PINSEL4 = 0;
    LPC_GPIO0->FIODIR |= DT_CTRL | EN_CTRL | RS_CTRL;
    LPC_GPIO2->FIODIR |= ROW_MASK;
    LPC_GPIO1->FIODIR &= ~COL_MASK;

    lcd_init();
    while(attempts < 3) {
        idx = 0;
        lcd_cmd(0x01);
        lcd_puts("ENTER PIN:");
        lcd_cmd(0xC0);

        while(idx < 4) {
            key = keypad_read();
            if(key >= '0' && key <= '9') {
                entry[idx++] = key;
                lcd_data('*');
            } else if(key == '*') {
                idx = 0;
                 lcd_cmd(0xC0);
                lcd_puts("                ");
                lcd_cmd(0xC0);
            }
        }
        entry[4] = '\0';
        lcd_cmd(0x01);

        if(strcmp((char *)entry, (char *)pin) == 0) {
            lcd_puts("ACCESS GRANTED");
            while(1);
        }

        attempts++;
        lcd_puts("WRONG PIN");
        lcd_cmd(0xC0);
        lcd_puts("LEFT: ");
        lcd_data('0' + (3 - attempts));
        delay_lcd(2000000);
    }

    lcd_cmd(0x01);
    lcd_puts("SYSTEM LOCKED");
    while(1);
}