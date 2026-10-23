/*
    Write an Embedded C program for LPC1768 to control the LED intensity using PWM1.4 (P1.23). 
    Use row 0 of the keyboard to select the following intensity levels, as specified in Lab 11 of your manual.

    key pressed	            LED intensity
        0	                    10%
        1	                    25%
        2	                    50%
        3	                    75%
*/

#include <LPC17xx.h>

unsigned int key, row, col;
unsigned long int temp;


int main(void) {
    SystemInit();
    SystemCoreClockUpdate();

    LPC_SC->PCONP |= (1 << 6);

    LPC_PINCON->PINSEL3 &= ~(3 << 14);
    LPC_PINCON->PINSEL3 |= (2 << 14);

    // Rows P2.10-P2.13
    LPC_PINCON->PINSEL4 = 0;
    LPC_GPIO02->FIODIR |= 0x00003C00;

    // Cols P0.19-P0.22
    LPC_PINCON->PINSEL1 = 0;
    LPC_GPIO0->FOIDIR &= ~0x00780000;

    LPC_PWM1->PR = 0;
    LPC_PWM1->PCR = (1 << 12);
    LPC_PWM1->MCR = (1 << 1);
    LPC_PWM1->MR0 = 30000;
    LPC_PWM1->MR4 = 3000; // initially 10%
    LPC_PWM1->LER = (1 << 0) | (1 << 4);

    //  reset and start PWM
    LPC_PWM1->TCR = (1 << 1);
    LPC_PWM1->TCR = (1 << 0) | (1 << 3);

    while(1) {
        // active row 0 of keyboard
        LPC_GPIO2->FIOCLR = 0x00003C00;
        LPC_GPIO2->FIOSET = (1 << 10);

        temp = LPC_GPIO0->FIOPIN & 0x00780000;

        if(temp != 0) {
            if(temp == (1 << 19)) key = 0;
            else if(temp == (1 << 20)) key= 1;
            else if(temp == (1 << 21)) key = 2;
            else if(temp == (1 << 22)) key = 3;
            else key = 99;

            if (key == 0) LPC_PWM1->MR4 = 3000;
            else if(key == 1) LPC_PWM1->MR4 = 7500;
            else if(key == 2) LPC_PWM1->MR4 = 15000;
            else if(key == 3) LPC_PWM1->MR4 = 22500;
            
            LPC_PWM1->LER = (1 << 4);

            while((LPC_GPIO0->FIOPIN & 0x00780000) != 0);
        }
    }
}