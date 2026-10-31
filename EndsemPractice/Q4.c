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
    Write an Embedded C program to perform the following operations:
    1. Accept a two-digit decimal number using the 4×4 matrix keypad.
    2. Display the entered number on the first two 7-segment displays (U8 and U9).
    3. On the remaining two displays (U10 and U11), implement an even-number up counter from 00 up to the entered number.
    4. When SW2 is pressed, run the counter downwards in steps of 2.
    5. The counter must never display odd numbers.
    
    Assumption: SW2 selects the down-count direction while pressed; releasing it resumes up-counting.
*/

#include <LPC17xx.h>

#define FIRST_SEG   0xF87FFFFF
#define SECOND_SEG  0xF8FFFFFF
#define THIRD_SEG   0xF97FFFFF
#define FOURTH_SEG  0xF9FFFFFF

#define ROW_MASK  0x00780000   // P0.19-P0.22
#define COL_MASK  0x00078000   // P0.15-P0.18

unsigned int i, j, row, col;
unsigned int input_num = 0;
unsigned int count = 0;
unsigned int max_count = 0;

unsigned int temp1, temp2, dig1, dig2, dig3, dig4;
unsigned char key;

unsigned char array_dec[10] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F};

unsigned char keypad[4][4] =
{
    {'1','2','3','A'},
    {'4','5','6','B'},
    {'7','8','9','C'},
    {'*','0','#','D'}
};

// Rows on P0.19-P0.22
unsigned int row_value[4] = {0x00080000, 0x00100000, 0x00200000, 0x00400000};

void delay(void);
unsigned char keypad_read(void);
void Display(void);

int main(void) {
    SystemInit();
    SystemCoreClockUpdate();

    LPC_PINCON->PINSEL0 = 0;
    LPC_PINCON->PINSEL1 = 0;
    LPC_PINCON->PINSEL3 = 0; // P1.23-P1.26
    LPC_PINCON->PINSEL4 = 0; // P2.10-P2.13

    LPC_GPIO0->FIODIR |= 0x00000FF0; // P0.4-P0.11 data lines

    LPC_GPIO1->FIODIR |= 0x07800000; // selection lines P1.23-1.26
    LPC_GPIO0->FIODIR |= ROW_MASK; // keypad rows output
    LPC_GPIO2->FIODIR &= ~(1 << 0); // SW2 as input at P2.0

    LPC_GPIO0->FIODIR &= ~COL_MASK; // keypad cols input

    // read first digit
    key = keypad_read();

    input_num = (key - '0') * 10;

    // read second digit
    key = keypad_read();

    input_num += (key - '0');

    max_count = input_num;

    if(max_count % 2 != 0) max_count--;
    
    count = 0;

    dig1 = input_num / 10;
    dig2 = input_num % 10;

    while(1) {
        dig3 = count / 10;
        dig4 = count % 10;

        for(i = 0; i < 1000; ++i) Display();

        if(LPC_GPIO2->FIOPIN & (1 << 0)) {
            // SW2 pressed -> UP counter
            if(count < max_count) count += 2;
        } else {
            // SW2 released -> Down counter
            if(count >= 2) count -= 2;
        }
    }
}

unsigned char keypad_read(void) {
    unsigned char key_val = 0;
    while(1) {
        for(row = 0; row < 4; ++row) {
            LPC_GPIO0->FIOCLR = ROW_MASK; // clear all rows
            LPC_GPIO0->FIOSET = row_value[row]; // activate one row

            for(j = 0; j < 1000; ++j);

            // read cols
            temp1 = LPC_GPIO0->FIOPIN & COL_MASK;
            if(temp1 != 0) {
                if(temp1 & 0x00008000) col = 0;
                else if(temp1 & 0x00010000) col = 1;
                else if(temp1 & 0x00020000) col = 2;
                else if(temp1 & 0x00040000) col = 3;

                key_val = keypad[row][col];
                while(LPC_GPIO0->FIOPIN & COL_MASK); // wait until key released

                LPC_GPIO2->FIOCLR = ROW_MASK;

                return key_val;
            }
        }
    }
}

void Display(void) {
    LPC_GPIO1->FIOPIN = FIRST_SEG;
    temp2 = array_dec[dig1];
    LPC_GPIO0->FIOCLR = 0x00000FF0;
    LPC_GPIO0->FIOSET = temp2 << 4;
    for(j = 0; j < 500; j++);
    LPC_GPIO0->FIOCLR = 0x00000FF0;

    LPC_GPIO1->FIOPIN = SECOND_SEG;
    temp2 = array_dec[dig2];
    LPC_GPIO0->FIOCLR = 0x00000FF0;
    LPC_GPIO0->FIOSET = temp2 << 4;
    for(j = 0; j < 500; j++);
    LPC_GPIO0->FIOCLR = 0x00000FF0;

    LPC_GPIO1->FIOPIN = THIRD_SEG;
    temp2 = array_dec[dig3];
    LPC_GPIO0->FIOCLR = 0x00000FF0;
    LPC_GPIO0->FIOSET = temp2 << 4;
    for(j = 0; j < 500; j++);
    LPC_GPIO0->FIOCLR = 0x00000FF0;

    LPC_GPIO1->FIOPIN = FOURTH_SEG;
    temp2 = array_dec[dig4];
    LPC_GPIO0->FIOCLR = 0x00000FF0;
    LPC_GPIO0->FIOSET = temp2 << 4;
    for(j = 0; j < 500; j++);
    LPC_GPIO0->FIOCLR = 0x00000FF0;
}