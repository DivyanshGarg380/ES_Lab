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
    Write an Embedded C program for LPC1768 to display a 4-digit hexadecimal up counter on the multiplexed seven-segment display.
    - Start from 0000.
    - Increment the counter every second.
    - Display hexadecimal digits 0–9 and A–F.
    - After FFFF, wrap around to 0000.
    - Use multiplexing to display all four digits continuously.
*/

#include <LPC17xx.h>

#define FIRST_SEG   0xF87FFFFF
#define SECOND_SEG  0xF8FFFFFF
#define THIRD_SEG   0xF97FFFFF
#define FOURTH_SEG  0xF9FFFFFF
#define DISABLE_ALL 0xFA7FFFFF

unsigned int seg_code[16] = {
    0x3F, 0x06, 0x5B, 0x4F,
    0x66, 0x6D, 0x7D, 0x07,
    0x7F, 0x6F, 0x77, 0x7C,
    0x39, 0x5E, 0x79, 0x71
};

void delay(void) {
    unsigned int i;
    for(i = 0; i < 1000; ++i);
}

void display(unsigned int count) {
    unsigned int d1, d2, d3, d4, i;

    d1 = (count >> 12) & 0x0F;
    d2 = (count >> 8) & 0x0F;
    d3 = (count >> 4) & 0x0F;
    d4 = count & 0x0F;

    for(i = 0; i < 500; ++i) {
        LPC_GPIO1->FIOPIN = FIRST_SEG;
        LPC_GPIO0->FIOPIN = seg_code[d1] << 4;
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
        count++;
        if(count > 0xFFFF) count = 0;
        
        for(i = 0; i < 1000; ++i);
    }
}