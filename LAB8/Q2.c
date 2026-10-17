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

unsigned int i, j;
unsigned int count = 0;

unsigned char seg[10] = {
    0x3F, 0x06, 0x5B, 0x4F, 0x66,
    0x6D, 0x7D, 0x07, 0x7F, 0x6F
};

int main(void) {
    SystemInit();
    SystemCoreClockUpdate();

    LPC_PINCON->PINSEL0 = 0;
    LPC_PINCON->PINSEL3 = 0;
    LPC_GPIO0->FIODIR |= 0x00000FF0;
    LPC_GPIO1->FIODIR |= 0x07800000;

    LPC_PINCON->PINSEL4 = 0;
    LPC_GPIO2->FIODIR = ~(1 << 10);

    while(1) {
        unsigned int d1 = count % 10;
        unsigned int d2 = (count / 10) % 10;
        unsigned int d3 = (count / 100) % 10;
        unsigned int d4 = (count / 1000) % 10;

        for(i = 0; i < 100; ++i) {
            LPC_GPIO1->FIOPIN = 0xF87FFFFF;
            LPC_GPIO0->FIOPIN = seg[d1] << 4;
            for (j = 0; j < 500; j++);

            LPC_GPIO1->FIOPIN = 0xF8FFFFFF;
            LPC_GPIO0->FIOPIN = seg[d2] << 4;
            for (j = 0; j < 500; j++);

            LPC_GPIO1->FIOPIN = 0xF97FFFFF;
            LPC_GPIO0->FIOPIN = seg[d3] << 4;
            for (j = 0; j < 500; j++);

            LPC_GPIO1->FIOPIN = 0xF9FFFFFF;
            LPC_GPIO0->FIOPIN = seg[d4] << 4;
            for (j = 0; j < 500; j++);

            LPC_GPIO0->FIOCLR = 0x00000FF0;
        }

        // 1 sec delay
        for(i = 0; i < 1000; ++i) {
            for(j = 0; j < 1000; ++j);
        }

        if(LPC_GPIO2->FIOPIN & (1 << 10)) {
            count++;
            if(count > 9999) count = 0;
        } else {
            if(count == 0) count = 9999;
            else count--;
        }
    }
}