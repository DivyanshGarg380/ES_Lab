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
  Write an Embedded C program to control LED intensity using PWM1.4 on P1.23. 
  A switch connected to P2.12 selects between four intensity levels: 10%, 25%, 50% and 75%, according to its input state combined with a software mode counter.
*/

#include <LPC17xx.h>

unsigned int i, mode = 0, previous_switch = 0;

void pwn_init(void);
void set_intensity(unsigned int);

int main(void) {
    SystemInit();
    SystemCoreClockUpdate();

    LPC_PINCON->PINSEL = 0;
    LPC_GPIO2->FIODIR &= ~(1 << 12);

    pwn_init(); // initialize PWM
    
    while(1) {
        unsigned int curr_switch = (LPC_GPIO2->FIOPIN >> 12) & 1;
        
        // change mode on a new HIGH state
        if(curr_switch == 1 && previous_switch == 0) {
            mode++;
            if(mode == 4) mode = 0;

            if(mode == 0) set_intensity(10);
            else if(mode == 1) set_intensity(25);
            else if(mode == 2) set_intensity(50);
            else set_intensity(75);
        }

        previous_switch = curr_switch;

        // delay
        for(i = 0; i < 10000; ++i);
    }
}

void pwn_init(void) {
    LPC_SC->PCONP |= (1 << 6); // power up PWM1

    LPC_PINOCN->PINSEL3 &= ~(3 << 14);
    LPC_PINCON->PINSEL3 |= (2 << 14);

    LPC_PWM1->PCR = (1 << 12); // single edge PWM mode

    // reset counter on match with MR0
    LPC_PWM1->MCR = (1 << 1);

    // set PWM period
    LPC_PWM1->MR0 = 30000;

    // start with 10% intensity
    LPC_PWM1->MR4 = 3000;

    // load match values
    LPC_PWM1->LER = (1 << 0) | (1 << 4);

    // reset PWM counter and prescaler
    LPC_PWM1->TCR = (1 << 1);

    // enable PWN and counter
    LPC_PWM1->TCR = (1 << 0) | (1 << 3);
}

void set_intensity(unsigned int perc) {
    LPC_PWM1->MR4 = (LPC_PWM1->MR0 * perc) / 100;
    LPC_PWM1->LER = (1 << 0) | (1 << 4);
}