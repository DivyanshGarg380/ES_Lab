/*
Author :

███████╗████████╗ █████╗ ██████╗  ███╗   ███╗ █████╗ ███╗   ██╗
██╔════╝╚══██╔══╝██╔══██╗██╔══██╗ ████╗ ████║██╔══██╗████╗  ██║
███████╗   ██║   ███████║██████╔╝ ██╔████╔██║███████║██╔██╗ ██║
╚════██║   ██║   ██╔══██║██║  ██║ ██║╚██╔╝██║██╔══██║██║╚██╗██║
███████║   ██║   ██║  ██║██║  ██║ ██║ ╚═╝ ██║██║  ██║██║ ╚████║
╚══════╝   ╚═╝   ╚═╝  ╚═╝╚═╝  ╚═╝ ╚═╝     ╚═╝╚═╝  ╚═╝╚═╝  ╚═══╝  STARMAN248
*/

#include <LPC17xx.h>

unsigned int key, duty;

void pwn_init(void);
unsigned int read_key(void);
void delay(void);

int main(void) {
    SystemInit();
    SystemCoreClockUpdate();

    pwn_init();
    LPC_PINCON->PINSEL4 = 0;
    LPC_GPIO2->FIODIR |= 1 << 10;

    LPC_PINCON->PINSEL1 = 0;
    LPC_GPIO0->FIODIR = ~(0x0F << 19);

    while(1) {
        key = read_key();
        if(key == 0) duty = 3000;
        else if(key == 1) duty = 7500;
        else if(key == 2) duty = 15000;
        else if(key == 3) duty = 22500;
        else continue;

        LPC_PWM1->MR4 = duty;
        LPC_PWM1->LER = (1 << 0) | (1 << 4);
    }
}

void pwn_init(void) {
    LPC_SC->PCONP |= (1 << 6);
    LPC_PINCON->PINSEL3 &= ~(3 << 14);
    LPC_PINCON->PINSEL3 |= (2 << 14);

    LPC_PWM1->PCR = (1 << 12);     
    LPC_PWM1->MCR = (1 << 1); 

    LPC_PWM1->MR0 = 30000;           
    LPC_PWM1->MR4 = 3000;         

    LPC_PWM1->LER = (1 << 0) | (1 << 4);

    LPC_PWM1->TCR = (1 << 1);   
    LPC_PWM1->TCR = (1 << 0) | (1 << 3); 
}

unsigned int read_key(void) {
    unsigned long int cols;

    while (1) {
        LPC_GPIO2->FIOCLR = (0x0F << 10);
        LPC_GPIO2->FIOSET = (1 << 10);

        cols = LPC_GPIO0->FIOPIN & (0x0F << 19);

        if (cols != 0) {
            delay();

            if (cols & (1 << 19)) key = 0;
            else if (cols & (1 << 20)) key = 1;
            else if (cols & (1 << 21)) key = 2;
            else if (cols & (1 << 22)) key = 3;
            else continue;

            while ((LPC_GPIO0->FIOPIN & (0x0F << 19)) != 0);

            delay();
            return key;
        }
    }
}

void delay(void) {
    unsigned int i;
    for (i = 0; i < 10000; i++);
}

