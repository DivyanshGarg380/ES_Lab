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
    Write an Embedded C program for LPC1768 to implement a 4-digit decimal counter on the multiplexed seven-segment display.
    - Initially display 0000.
    - If SW2 is pressed, increment the count.
    - Otherwise, decrement the count.
    - Counter must wrap from 9999 → 0000 and 0000 → 9999.
    - Update the display continuously with a 1-second interval between counts.
*/

#include <LPC17xx.h>

#define FIRST_SEG   0xF87FFFFF
#define SECOND_SEG  0xF8FFFFFF
#define THIRD_SEG   0xF97FFFFF
#define FOURTH_SEG  0xF9FFFFFF
#define DISABLE_ALL 0xFA7FFFFF

unsigned int seg_codes[10] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F};

void delay(void) {
    unsigned int i;
    for(i = 0; i < 1000; ++i);
}

void display(unsigned int count) {
    unsigned int d1, d2, d3, d4, j;
    d1 = count / 1000;
    d2 = (count / 100) % 10;
    d3 = (count / 10) % 10;
    d4 = count % 10;

    for(i = 0; i < 500; ++i) {
        LPC_GPIO1->FIOPIN = FIRST_SEG;
        LPC_GPIO1->FIOPIN = seg_codes[d1] << 4;
        delay();

        LPC_GPIO1->FIOPIN = SECOND_SEG;
        LPC_GPIO0->FIOPIN = seg_code[d2] << 4;
        delay();

        LPC_GPIO1->FIOPIN = THIRD_SEG;
        LPC_GPIO0->FIOPIN = seg_code[d3] << 4;
        delay();

        LPC_GPIO1->FIOPIN = FOURTH_SEG;
        LPC_GPIO0->FIOPIN = seg_code[d4] << 4;
        delay();

        LPC_GPIO1->FIOPIN = DISABLE_ALL;
    }
}

int main(void) {
    unsigned int count = 0, i;
    LPC_PINCON->PINSEL0 = 0;
    LPC_PINCON->PINSEL3 = 0;

    LPC_GPIO0->FIODIR |= 0x00000FF0;
    LPC_GPIO1->FIODIR |= 0x07800000;

    while(1) {
        display(count);
        if(LPC_GPIO2->FIOPIN & (1 << 12)) {
            count++;
            if(count > 9999) count = 0;
        } else {
            if(count == 0) count = 9999;
            else count--;
        }

        for(i = 0; i < 1000; ++i);
    }
}