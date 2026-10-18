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

unsigned int row, col, temp;
unsigned char key;

int main(void) {
    SystemInit();
    SystemCoreClockUpdate();

    LPC_PINCON->PINSEL4 = 0;
    LPC_GPIO2->FIODIR = 0x00003C00;

    LPC_PINCON->PINSEL3 = 0;
    LPC_GPIO1->FIODIR = ~0x07800000;

    while(1) {
        for(row = 0; row < 4; ++row) {
            LPC_GPIO2->FIOCLR = 0x00003C00;
            LPC_GPIO2->FIOSET = (1 << (10 + row));

            temp = LPC_GPIO1->FIOPIN & 0x07800000;
            if(temp != 0) {
                if(temp == (1 << 23)) col = 0;
                else if(temp == (1 << 24)) col = 1;
                else if(temp == (1 << 25)) col = 2;
                else if(temp == (1 << 26)) col = 3;
                else continue;

                key = row * 4 + col;

                while((LPC_GPIO1->FIOPIN & 0x07800000) != 0);
            }
        }
    }
}