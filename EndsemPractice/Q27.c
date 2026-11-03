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
    Write an Embedded C program for LPC1768 using a 4×4 keypad, ADC0.5 and a 16×2 LCD.
    
    - Press 1 to take 1 ADC sample.
    - Press 2 to take 4 ADC samples.
    - Press 3 to take 8 ADC samples.
    - Calculate the average ADC value for the selected number of samples.
    - Display the average 12-bit ADC value and corresponding voltage on the LCD.
    - Keep accepting keypad inputs to change the sampling count.
*/

#include <LPC17xx.h>

#define RS_CTRL 0x08000000
#define EN_CTRL 0x10000000
#define DT_CTRL 0x07800000

unsigned long int temp1 = 0, temp2 = 0;
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
    for(r = 0; r < r1; r++);
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
        for(row = 0; row < 4; row++) {
            LPC_GPIO2->FIOCLR = 0x00003C00;
            LPC_GPIO2->FIOSET = (1 << (row + 10));

            delay_lcd(1000);

            value = LPC_GPIO1->FIOPIN & 0x07800000;

            if(value != 0) {
                for(col = 0; col < 4; col++) {
                    if(value & (1 << (col + 23))) {
                        char key = keys[row][col];
                        while(LPC_GPIO1->FIOPIN & (1 << (col + 23)));
                        delay_lcd(30000);
                        return key;
                    }
                }
            }
        }
    }
}

unsigned int read_adc(void) {
    LPC_ADC->ADCR = (1 << 5) | (1 << 21) | (1 << 24);
    while(!(LPC_ADC->ADGDR & (1 << 31)));
    return (LPC_ADC->ADGDR >> 4) & 0xFFF; 
}

int main(void) {
    SystemInit();
    SystemCoreClockUpdate();

    unsigned int i, count, adc, sum, avg, voltage;
    char key;

    char line1[] = "N=8 ADC=0000";
    char line2[] = "V=0.00V";

    LPC_PINCON->PINSEL1 = 0;
    LPC_GPIO0->FIODIR |= RS_CTRL | EN_CTRL | DT_CTRL;

    flag1 = 0;
    for(i = 0; i < 9; ++i) {
        temp1 = init_command[i];
        lcd_write();
    }

    LPC_PINCON->PINSEL3 = 0;
    LPC_PINCON->PINSEL4 = 0;
    LPC_GPIO1->FIODIR &= ~0x07800000;
    LPC_GPIO2->FIODIR |= 0x00003C00;

    LPC_PINCON->PINSEL3 |= 0xC0000000;
    LPC_SC->PCONP |= (1 << 12);

    count = 8;
    while(1) {
        key = get_key();
        if(key == '1') count = 1;
        else if(key == '2') count = 4;
        else if(key == '3') count = 8;
        else continue;

        sum = 0;
        for(i = 0; i < count; ++i) {
            adc = read_adc();
            sum += adc;
            delay_lcd(10000);
        }

        avg = sum / count;
        voltage = (avg * 3300) / 4095;

        line1[2] = '0' + count;
        line1[9] = '0' + (avg / 1000);
        line1[10] = '0' + ((avg / 100) % 10);
        line1[11] = '0' + ((avg / 10) % 10);
        line1[12] = '0' + (avg % 10);

        line2[2] = '0' + (voltage / 1000);
        line2[4] = '0' + ((voltage % 1000) / 100);
        line2[5] = '0' + ((voltage % 100) / 10);

        flag1 = 0;
        temp1 = 0x80;
        lcd_write();

        flag1 = 1;

        for(i = 0; line1[i] != '\0'; i++) {
            temp1 = line1[i];
            lcd_write();
        }

        flag1 = 0;
        temp1 = 0xC0;
        lcd_write();

        flag1 = 1;

        for(i = 0; line2[i] != '\0'; i++) {
            temp1 = line2[i];
            lcd_write();
        }
    }
}
