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
   Write an Embedded C program to implement a 4-digit BCD down counter on the multiplexed 7-segment display. 
   Use a delay of approximately one second between successive counts.
    The counter should run: 9999 → 9998 → 9997 → ... → 0001 → 0000 → 9999
*/

#include <LPC17xx.h>

#define FIRST_SEG   0xF87FFFFF
#define SECOND_SEG  0xF8FFFFFF
#define THIRD_SEG   0xF97FFFFF
#define FOURTH_SEG  0xF9FFFFFF

unsigned int dig1 = 9, dig2 = 9, dig3 = 9, dig4 = 9;

unsigned int dig_count = 0;
unsigned int temp1, temp2, i;

unsigned char array_dec[10] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F};

void Display(void);
void Delay(void);

int main(void) {
    SystemInit();
    SystemCoreClockUpdate();

    LPC_PINCON->PINSEL0 = 0; // segment data pins
    LPC_PINCON->PINSEL3 = 0; // enable pins

    LPC_GPIO0->FIODIR |= 0X00000FF0;
    LPC_GPIO1->FIODIR |= 0x07800000;

    while(1) {
        for(i = 0; i < 1000; i++) Display();
        Delay();

        // decrement counter
        if(dig1 > 0) dig1--;
        else {
            dig1 = 9;
            if(dig2 > 0) dig2--;
            else {
                dig2 = 9;
                if(dig3 > 0) dig3--;
                else {
                    dig3 = 9;
                    if(dig4 > 0) dig4--;
                    else dig4 = 9;
                }
            }
        }
    }
}

void Display(void) {
    temp1 = dig1;
    LPC_GPIO1->FIOPIN = FIRST_SEG;
    temp2 = array_dec[temp1];
    LPC_GPIO0->FIOPIN = temp2 << 4;
    for(i = 0; i < 500; ++i);
    LPC_GPIO0->FIOCLR = 0x00000FF0;

    temp1 = dig2;
    LPC_GPIO1->FIOPIN = SECOND_SEG;
    temp2 = array_dec[temp1];
    LPC_GPIO0->FIOPIN = temp2 << 4;
    for(i = 0; i < 500; i++);
    LPC_GPIO0->FIOCLR = 0x00000FF0;

    temp1 = dig3;
    LPC_GPIO1->FIOPIN = THIRD_SEG;
    temp2 = array_dec[temp1];
    LPC_GPIO0->FIOPIN = temp2 << 4;
    for(i = 0; i < 500; i++);
    LPC_GPIO0->FIOCLR = 0x00000FF0;

    temp1 = dig4;
    LPC_GPIO1->FIOPIN = FOURTH_SEG;
    temp2 = array_dec[temp1];
    LPC_GPIO0->FIOPIN = temp2 << 4;
    for(i = 0; i < 500; i++);
    LPC_GPIO0->FIOCLR = 0x00000FF0;
}

void Delay(void) {
    unsigned int j;
    for(j = 0; j < 5000; j++);
}

