/*
   Write an Embedded C program for LPC1768 to control LED brightness using PWM1.4 (P1.23) and a switch connected to P2.12.

    Requirements:
    1. MR0 = 30000.
    2. Each time the switch is pressed, cycle through duty cycles 25% → 50% → 75% → 100% → 25%.
    3. Use GPIO polling only; no interrupts.
    4. Assume the switch is active-low.
    5. Use a delay loop to avoid registering the same press repeatedly
*/

#include <LPC17xx.h>

unsigned int duty_index = 0;
unsigned int duty_values[4] = {7500, 15000, 22500, 30000};
unsigned int i;

int main(void) {
    SystemInit();
    SystemCoreClockUpdate();

    LPC_SC->PCONP |= (1 << 6);
    LPC_PINCON->PINSEL3 &= ~(3 << 14);
    LPC_PINCON->PINSEL3 |= (2 << 14);

    LPC_PINCON->PINSEL4 = 0;
    LPC_GPIO2->FIODIR &= ~(1 << 12);

    LPC_PWM1->PR = 0;
    LPC_PWM1->PCR = (1 << 12);
    LPC_PWM1->MCR = (1 << 1);

    LPC_PWM1->MR0 = 30000;
    LPC_PWM1->MR4 = duty_values[duty_index];

    LPC_PWM1->LER = (1 << 0) | (1 << 4);

    LPC_PWM1->TCR = (1 << 1);
    LPC_PWM1->TCR = (1 << 0) | (1 << 3);

    while(1) {
        if(!(LPC_GPIO2->FIOPIN & (1 << 12))) {
            duty_index = (duty_index + 1) % 4;

            LPC_PWM1->MR4 = duty_values[duty_values];
            LPC_PWM1->LER = (1 << 4);

            while ((LPC_GPIO2->FIOPIN & (1 << 12)) == 0);
            
            for(i = 0; i < 50000; ++i); // delay
        }
    }
}