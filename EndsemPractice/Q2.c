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
    Write an Embedded C program to display a four-digit BCD down counter from 9999 to 0000 on the multiplexed seven-segment display. 
    Introduce a delay between successive counts and restart from 9999 after reaching 0000.
*/

#include <LPC17xx.h>

#define FIRST_SEG 0xF87FFFFF
#define SECOND_SEG 0xF8FFFFFF
#define THIRD_SEG 0xF97FFFFF
#define FOURTH_SEG 0xF9FFFFFF

unsigned int dig1 = 9, dig2 = 9, dig3 = 9, dig4 = 9, dig_count = 0, i, j;

unsigned char array_dec[10] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F};

void display(void);
void delay(void);

int main(void) {
    SystemInit();
    SystemCoreClockUpdate();

    LPC_PINCON->PINSEL0 = 0;
    LPC_PINCON->PINSEL3 = 0;

    LPC_GPIO0->FIODIR |= 0x00000FF0;
    LPC_GPIO1->FIODIR |= 0x07800000;

    while(1) {
        display();
        delay();

        // BCD down counting
        if(dig1 == 0 && dig2 == 0 && dig3 == 0 && dig4 == 0) {
            dig1 = 9; dig2 = 9; dig3 = 9; dig4 = 9;
        } else {
            if(dig1 > 0) {
                dig1--;
            } else {
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
}

void display(void) {
    LPC_GPIO1->FIOPIN = FIRST_SEG;
    LPC_GPIO0->FIOPIN = array_dec[dig1] << 4;
    for(j = 0; j < 500; ++j);

    LPC_GPIO1->FIOPIN = SECOND_SEG;
    LPC_GPIO0->FIOPIN = array_dec[dig2] << 4;
    for (j = 0; j < 500; j++);

    LPC_GPIO1->FIOPIN = THIRD_SEG;
    LPC_GPIO0->FIOPIN = array_dec[dig3] << 4;
    for (j = 0; j < 500; j++);

    LPC_GPIO1->FIOPIN = FOURTH_SEG;
    LPC_GPIO0->FIOPIN = array_dec[dig4] << 4;
    for (j = 0; j < 500; j++);
}

void delay(void) {
    for (i = 0; i < 1000; i++);
}