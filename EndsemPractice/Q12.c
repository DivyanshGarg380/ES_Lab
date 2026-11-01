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
    1. Generate PWM using PWM1.4.
    2. Initially set LED brightness to 10%.
    3. Each time SW2 is pressed, change the brightness in this sequence: 10% → 25% → 50% → 75% → 100% → 10%.
    4. Continue indefinitely.
*/

#include <LPC17xx.h>
unsigned int level = 0, duty;

unsigned int intensity[5] = {3000, 7500, 15000, 22500, 30000};

int main(void) {
    SystemInit();
    SystemCoreClockUpdate();

    LP_SC->PCONP |= (1 << 6); // enable PWM1

    LPC_PINCON->PINSEL3 &= ~0x003FC000;
    LPC_PINCON->PINSEL3 |= 0x00008000;

    LPC_GPIO1->FIODIR &= ~(1 << 26);
    LPC_PWM1->PCR = 0x00001000;
    LPC_PWM1->MCR = 0x00000003;
    LPC_PWM1->MR0 = 30000;
    LPC_PWM1->MR4 = intensity[0];
    LPC_PWM1->LER = 0x11;
    LPC_PWM1->TCR = 0x02;
    LPC_PWM1->TCR = 0x09;

    while(1) {
        if(!(LPC_GPIO1->FIODIR & (1 << 26))) {
            level++;
            if(level >= 5) level = 0;
            duty = intensity[level];
            LPC_PWM1->MR4 = duty;
            LPC_PWM1->LER = (1 << 4);

            while(!(LPC_GPIO1->FIOPIN & (1 << 26)));
            for(duty = 0; duty < 50000; ++duty);
        }
    }
}

