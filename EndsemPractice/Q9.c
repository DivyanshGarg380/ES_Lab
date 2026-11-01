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
    Write an Embedded C program to:
    1. Accept a two-digit duty-cycle value (00–99) using the 4×4 matrix keypad.
    2. Generate PWM using PWM1.4.
    3. Control the brightness of onboard LED LD10 according to the entered duty cycle.
    4. Continuously accept new two-digit values and update the brightness
*/

#include <LPC17xx.h>

unsigned int i, j, row, col, temp1;
unsigned int percentage = 0, duty = 0;
unsigned char key;
unsigned char keypad[4][4] = {
    {'1','2','3','A'},
    {'4','5','6','B'},
    {'7','8','9','C'},
    {'*','0','#','D'}
};

#define ROW_MASK 0x00780000
#define COL_MASK 0x00078000

unsigned int row_value[4] = {0x00080000, 0x00100000, 0x00200000, 0x00400000};

unsigned char keypad_read(void);
void pwm_init(void);

int main(void) {
    SystemInit();
    SystemCoreClockUpdate();

    LPC_PINCON->PINSEL0 = 0;
    LPC_PINCON->PINSEL1 = 0;

    LPC_GPIO0->FIODIR |= ROW_MASK;
    LPC_GPIO0->FIODIR &= ~COL_MASK;

    pwm_init();
    while(1) {
        key = keypad_read();
        percentage = (key - '0') * 10;
        key = keypad_read();
        percentage += (key - '0');

        duty = (percentage * 30000) / 100;
        LPC_PWM1->MR4 = duty;
        LPC_PWM1->LER = (1 << 4);
    }
}

void pwm_init(void) {
    LPC_SC->PCONP |= (1 << 6);

    LPC_PINCON->PINSEL3 &= ~0x0000C000;
    LPC_PINCON->PINSEL3 |= 0x00008000;

    LPC_PWM1->PCR = 0x00001000;
    LPC_PWM1->MCR = 0x00000003;
    LPC_PWM1->MR0 = 30000;
    LPC_PWM1->MR4 = 0;
    LPC_PWM1->LER = 0x11;
    LPC_PWM1->TCR = 0x02;
    LPC_PWM1->TCR = 0X09;
}

unsigned char keypad_read(void) {
    unsigned char key_val;
    while(1) {
        for(row = 0; row < 4; ++row) {
            LPC_GPIO0->FIOCLR = ROW_MASK;
            LPC_GPIO0->FIOSET = row_value[row];
            for(j = 0; j < 1000; ++j);

            temp1 = LPC_GPIO0->FIOPIN & COL_MASK;

            if(temp1 != 0) {
                if(temp1 & 0x00008000) col = 0;
                else if(temp1 & 0x00010000) col = 1;
                else if(temp1 & 0x00020000) col = 2;
                else if(temp1 & 0x00040000) col = 3;
                else continue;

                key_val = keypad[row][col];
                while(LPC_GPIO0->FIOPIN & COL_MASK);

                return key_val;
            }   
        }
    }
}