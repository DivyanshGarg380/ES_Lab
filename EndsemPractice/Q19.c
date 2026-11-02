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
    Design a program to control eight LEDs using a 4×4 keypad.
    - Press 1: Turn OFF all LEDs.
    - Press 2: Turn ON the first four LEDs.
    - Press 3: Turn ON the last four LEDs.
    - Press 4: Turn ON all eight LEDs.
    - Any other key should turn OFF all LEDs.
*/

#include <LPC17xx.h>

char keys[4][4] = {
    {'1','2','3','A'},
    {'4','5','6','B'},
    {'7','8','9','C'},
    {'*','0','#','D'}
};

void delay(void) {
    unsigned int i;
    for(i = 0; i < 10000; i++);
}

char get_key(void) {
    unsigned int row, col, value;
    while(1) {
        for(row = 0; row < 4; row++) {
            LPC_GPIO2->FIOCLR = 0x00003C00;
            LPC_GPIO2->FIOSET = (1 << (10 + row));

            value = LPC_GPIO1->FIOPIN & 0x07800000;
            if(value != 0) {
                if(value == (1 << 23)) col = 0;
                else if(value == (1 << 24)) col = 1;
                else if(value == (1 << 25)) col = 2;
                else col = 3;

                while(LPC_GPIO1->FIOPIN & 0x07800000);
                return keys[row][col];
            }
        }
    }
}

int main(void) {
    char key;
    LPC_PINCON->PINSEL3 = 0;
    LPC_PINCON->PINSEL4 = 0;

    LPC_GPIO2->FIODIR |= 0x00003C00;
    LPC_GPIO1->FIODIR &= 0xF8788888;

    LPC_PINCON->PINSEL0 = 0;
    LPC_GPIO0->FIODIR |= 0x00000FF0;

    while(1) {
        key = get_key();
        LPC_GPIO0->FIOCLR = 0x00000FF0;

        if(key == '1') LPC_GPIO0->FIOCLR = 0x00000FF0;
        else if(key == '2') LPC_GPIO0->FIOSET = 0x000000F0;
        else if(key == '3') LPC_GPIO0->FIOSET = 0x00000F00;
        else if(key == '4') LPC_GPIO0->FIOSET = 0x00000FF0;
    }
}