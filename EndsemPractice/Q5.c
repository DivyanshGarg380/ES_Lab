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
   Write an Embedded C program to accept an expression of the form A operator B = from the matrix keypad, where A and B are single-digit BCD numbers and the operator is + or -. 
   Display the result on the LCD.
*/

#include <LPC17xx.h>

#define RS_CTRL 0x08000000
#define EN_CTRL 0x10000000
#define DT_CTRL 0x07800000

unsigned long temp1, temp2;
unsigned int i, row, col, A, B;
int result;
char operation;
unsigned char key, flag1, flag2;

unsigned long init_command[] = {0x30, 0x30, 0x30, 0x20, 0x28, 0x0C, 0x06, 0x01, 0x80};

unsigned char keys[4][4] = {
    {'1', '2', '3', '+'},
    {'4', '5', '6', '-'},
    {'7', '8', '9', '='},
    {'C', '0', 'E', 'F'}
};

void delay_lcd(unsigned int);
void lcd_write(void);
void port_write(void);
void lcd_command(unsigned char);
void lcd_data(unsigned char);
void lcd_string(unsigned char *);
void keypad_init(void);
unsigned char get_key(void);
void display_number(int);

int main(void) {
    SystemInit();
    SystemCoreClockUpdate();

    LPC_PINCON->PINSEL1 = 0;
    LPC_GPIO0->FIODIR |= DT_CTRL | RS_CTRL | EN_CTRL;

    // initialize LCD
    flag1 = 0;
    for(i = 0; i < 9; ++i) {
        temp1 = init_command[i];
        lcd_write();
    }

    keypad_init();

    while(1) {
        lcd_command(0x01);
        lcd_string((unsigned char *)"Enter A op B");

        // read first operand
        key = get_key();
        A = key - '0';
        lcd_command(0xC0);
        lcd_data(key);

        // read operator
        key = get_key();
        operation = key;
        lcd_data(key);

        // read second operand
        key = get_key();
        B = key - '0';
        lcd_data(key);

        // "=" waali key ka input
        key = get_key();

        // calculate result
        if(operation == '+') result = A + B;
        else result = A - B;

        // display result on second LCD line
        lcd_command(0xC0);
        lcd_string((unsigned char *)"Result: ");
        display_number(result);

        delay_lcd(1000000);
    }
}

// config keypad row (P2.10 - P2.13)  and cols (P0.19 - P0.22)
void keypad_init(void) {
    LPC_PINCON->PINSEL4 = 0;
    LPC_PINCON->PINSEL1 = 0;

    LPC_GPIO2->FIODIR |= 0x00003C00;
    LPC_GPIO0->FIODIR &= ~0x00780000;
}

unsigned char get_key(void) {
    unsigned long row_mask, col_val;
    while(1) {
        for(row = 0; row < 4; ++row) {
            row_mask = (1 << (10 + row));
            LPC_GPIO2->FIOCLR = 0x00003C00;
            LPC_GPIO2->FIOSET = row_mask;
            delay_lcd(1000);

            col_val = LPC_GPIO0->FIOPIN & 0x00780000;
            if(col_val != 0) {
                if(col_val & (1 << 19)) col = 0;
                else if(col_val & (1 << 20)) col = 1;
                else if(col_val & (1 << 21)) col = 2;
                else col = 3;

                while ((LPC_GPIO0->FIOPIN & 0x00780000) != 0);

                return keys[row][col];
            }
        }
    }   
}

// send LCD command
void lcd_command(unsigned char command) {
    flag1 = 0; // command only
    temp1 = command;
    lcd_write();
}

// send one character to LCD
void lcd_data(unsigned char data) {
    flag1 = 1; // data only
    temp1 = data;
    lcd_write();
}

// display the string on LCD
void lcd_string(unsigned char *str) {
    while(*str != '\0') {
        lcd_data(*str);
        ++str;
    }
}

void display_number(int num) {
    unsigned int digit;
    unsigned char buff[6];
    unsigned int cnt = 0;
    int n = num;

    if(n < 0) {
        lcd_data('-');
        n = -n;
    }

    do {
        digit = n % 10;
        buff[cnt++] = digit + '0';
        n /= 10;
    } while(n > 0);

    while(cnt > 0) lcd_data(buff[--cnt]);
}

void lcd_write(void) {
    flag2 = (flag1 == 1) ? 0 : ((temp1 == 0x30) || (temp1 == 0x20)) ? 1 : 0;
    temp2 = temp1 & 0xF0;
    temp2 = temp2 << 19;
    port_write();

    if(!flag2) {
        temp2 = temp1 & 0x0F;
        temp2 = temp2 << 23;
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
    unsigned int x;
    for(x = 0; x < r1; ++x);
}