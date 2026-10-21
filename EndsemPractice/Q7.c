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
   Write an Embedded C program to display a four-digit BCD counter on the multiplexed seven-segment display. 
   Use a switch to select the direction of counting: HIGH means count up and LOW means count down. 
   Update the count once per second.
*/

#include <LPC17xx.h>

#define FIRST_SEG   0xF87FFFFF
#define SECOND_SEG  0xF8FFFFFF
#define THIRD_SEG   0xF97FFFFF
#define FOURTH_SEG  0xF9FFFFFF

unsigned int dig1 = 0, dig2 = 0, dig3 = 0, dig4 = 0, dig_count = 0;
unsigned int i, j, count_delay;
unsigned int direction;

unsigned char array_dec[10] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F};

void display(void);
void delay(void);
void increment(void);
void decrement(void);

int main(void) {
    SystemInit();
    SystemCoreClockUpdate();

    LPC_PINCON->PINSEL0 = 0;
    LPC_PINCON->PINSEL3 = 0;
    LPC_PINCON->PINSEL4 = 0;

    LPC_GPIO0->FIODIR |= 0xFF0;
    LPC_GPIO1->FIODIR |= 0x07800000;
    LPC_GPIO2->FIODIR &= ~(1 << 12);

    while(1) {
        for(j = 0; j < 1000; ++j) {
            display();
        }

        direction = LPC_GPIO2->FIOPIN & (1 << 12);
        if(direction) increment();
        else decrement();

        delay();
    }
}

void increment(void) {
    dig1++;
    if(dig1 == 10) {
        dig1 = 0;
        dig2++;
        if(dig2 == 10) {
            dig2 = 0;
            dig3++;
            if(dig3 == 10) {
                dig3 = 0;
                dig4++;
                if(dig4 == 10) dig4 = 0;
            }
        }
    }
}

void decrement(void) {
    if (dig1 > 0) dig1--;
    else {
        dig1 = 9;
        if (dig2 > 0) dig2--;
        else {
            dig2 = 9;
            if (dig3 > 0) dig3--;
            else {
                dig3 = 9;
                if (dig4 > 0) dig4--;
                else dig4 = 9;
            }
        }
    }
}

void display(void) {
    unsigned int digit;
    dig_count++;
    if(dig_count == 1) {
        digit = dig1;
        LPC_GPIO1->FIOPIN = FIRST_SEG;
    } else if(dig_count == 2) {
        digit = dig2;
        LPC_GPIO1->FIOPIN = SECOND_SEG;
    } else if(dig_count == 3) {
        digit = dig3;
        LPC_GPIO1->FIOPIN = THIRD_SEG;
    } else {
        digit = dig4;
        LPC_GPIO1->FIOPIN = FOURTH_SEG;
        dig_count = 0;
    }

    LPC_GPIO0->FIOPIN = array_dec[digit] << 4;
    for (i = 0; i < 500; i++);

    LPC_GPIO0->FIOCLR = 0xFF0;
}

