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
    Write an LPC1768 program to generate a PWM signal using PWM1.4 (P1.23) and display its duty cycle on the 4-digit Seven Segment Display.
    
    - Start with a duty cycle of 10%.
    - Increase the duty cycle by 10% every 2 seconds.
    - When it reaches 100%, restart from 10%.
    - Display the duty cycle as a 4-digit number:
        - 10% → 0010
        - 50% → 0050
        - 100% → 0100
    - Use multiplexing to keep the display continuously visible.
*/

#include <LPC17xx.h>

#define FIRST_SEG 0xF87FFFFF
#define SECOND_SEG 0xF8FFFFFF
#define THIRD_SEG 0xF97FFFFF
#define FOURTH_SEG 0xF9FFFFFF

unsigned int duty = 10;
unsigned int mr_value;
unsigned int d1, d2, d3, d4;
unsigned int i, j;

unsigned int seg_codes[] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F};

void delay(void) {
    for(j = 0; j < 1000; ++j);
}

void display(void) {
    LPC_GPIO2->FIOPIN = FIRST_SEG;
    LPC_GPIO0->FIOPIN = seg_codes[d1] << 4;
    delay();

    LPC_GPIO2->FIOPIN = SECOND_SEG;
    LPC_GPIO0->FIOPIN = seg_codes[d2] << 4;
    delay();

    LPC_GPIO2->FIOPIN = THIRD_SEG;
    LPC_GPIO0->FIOPIN = seg_codes[d3] << 4;
    delay();

    LPC_GPIO2->FIOPIN = FOURTH_SEG;
    LPC_GPIO0->FIOPIN = seg_codes[d4] << 4;
    delay();
}

int main(void) {
    LPC_SC->PCONP |= (1 << 6);
    LPC_PINCON->PINSEL3 &= ~(3 << 14);
    LPC_PINCON->PINSEL3 |= (2 << 14);

    LPC_PINCON->PINSEL0 = 0;
    LPC_GPIO0->FIODIR |= 0x00000FF0;
    LPC_GPIO2->FIODIR |= 0x07800000;

    LPC_PWM1->PR = 0;
    LPC_PWM1->PCR = (1 << 12);
    LPC_PWM1->MCR = 0x03;
    LPC_PWM1->MR0 = 30000;
    
    mr_value = (30000 * duty) / 100;
    LPC_PWM1->MR4 = mr_value;
    LPC_PWM1->LER = (1 << 0) | (1 << 4);
    LPC_PWM1->TCR = 0x02;
    LPC_PWM1->TCR = 0x09;

    while(1) {
        d1 = duty / 1000;
        d2 = (duty / 100) % 10;
        d3 = (duty / 10) % 10;
        d4 = duty % 10;

        for(i = 0; i < 500; ++i) display();

        duty += 10;
        if(duty > 100) duty = 10;

        mr_value = (30000 * duty) / 100;
        LPC_PWM1->MR4 = mr_value;
        LPC_PWM1->LER = (1 << 4);
    }
}
