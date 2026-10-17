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
unsigned int d1 = 0, d2 = 0, d3 = 0, d4 = 0;

unsigned char seg[10] = {
    0x3F, 0x06, 0x5B, 0x4F, 0x66,
    0x6D, 0x7D, 0x07, 0x7F, 0x6F
};

int main(void) {
    SystemInit();
    SystemCoreClockUpdate();

    LPC_PINCON->PINSEL0 = 0;
    LPC_GPIO0->FIODIR = 0x00000FF0;

    LPC_PINCON->PINSEL3 = 0;
    LPC_GPIO1->FIODIR = 0x07800000;

    while(1) {
        for(i = 0; i < 100; ++i) {
            LPC_GPIO1->FIOPIN = 0xF87FFFFF;
            LPC_GPIO0->FIOPIN = seg[d1] << 4;
            for(j = 0; j < 500; ++j);

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

        d1++;
        if(d1 == 10) {
            d1 = 0;
            d2++;
            if(d2 == 10) {
                d2 = 0;
                d3++;
                if(d3 == 10) {
                    d3 = 0;
                    d4++;
                    if(d4 == 10) d4 = 0;
                }
            }
        }
    }
}