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
    1. Accept a two-digit threshold percentage using the 4×4 keypad.
    2. Display the entered threshold on U8 and U9.
    3. Read analog input from POT1 using ADC channel 5.
    4. Convert the ADC reading into a percentage (0–100).
    5. Display the live ADC percentage on U10 and U11.
*/

#include <LPC17xx.h>

#define FIRST_SEG   0xF87FFFFF
#define SECOND_SEG  0xF8FFFFFF
#define THIRD_SEG   0xF97FFFFF
#define FOURTH_SEG  0xF9FFFFFF

#define ROW_MASK  0x00780000
#define COL_MASK  0x00078000

unsigned int i, j, row, col;
unsigned int temp1, temp2;

unsigned int threshold = 0;
unsigned int adc_value, percentage;

unsigned int d1, d2, d3, d4;

unsigned char key;

unsigned char array_dec[10] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F};

unsigned char keypad[4][4] = {
    {'1','2','3','A'},
    {'4','5','6','B'},
    {'7','8','9','C'},
    {'*','0','#','D'}
};

unsigned int row_value[4] = {0x00080000, 0x00100000, 0x00200000, 0x00400000};

void Display(void);
unsigned char keypad_read(void);

int main(void) {
    SystemInit();
    SystemCoreClockUpdate();

    LPC_PINCON->PINSEL0 = 0;
    LPC_PINCON->PINSEL1 = 0;
    LPC_PINCON->PINSEL3 = 0xC0000000;

    LPC_GPIO0->FIODIR |= 0x00000FF0; // SSD data lines
    LPC_GPIO0->FIODIR |= ROW_MASK; // rows output
    LPC_GPIO0->FIODIR &= ~COL_MASK; // cols input
    LPC_GPIO1->FIODIR |= 0x07800000; // SSD selection lines 

    // enable ADC
    LPC_SC->PCONP |= (1 << 12);

    // read first digit
    key = keypad_read();
    threshold = (key - '0') * 10;

    // read second digit
    key = keypad_read();
    threshold += (key - '0');

    while(1) {
        // start ADC on channel 5
        LPC_ADC->ADCR = (1 << 5) || (1 << 21) || (1 << 24);
        while(!LPC_ADC->ADGDR && (1 << 31));

        adc_value = LPC_ADC->ADGDR;
        adc_value = (adc_value >> 4) & 0xFFF;

        percentage = (adc_value * 100) / 4095;

        // seperate digits
        d1 = threshold / 10;
        d2 = threshold % 10;
        d3 = percentage / 10;
        d4 = percentage % 10;

        // refresh display
        for(i = 0; i < 100; ++i) Display();
    }
}

unsigned char keypad_read(void) {
    unsigned char key_val;
    while(1) {
        for(row = 0; row < 4; ++row) {
            LPC_GPIO0->FIOCLR = ROW_MASK;
            LPC_GPIO0->FIOSET = row_value[row];

            for(j = 0; j < 1000; ++j);

            temp1 = LPC_GPIO0->FIOPIN & COL_MASK;
            if(temp1 != 0) {
                if(temp1 & 0x00008000) col = 0;
                else if(temp1 & 0x00010000) col = 1;
                else if(temp1 & 0x00020000) col = 2;
                else if(temp1 & 0x00040000) col = 3;
                else continue;

                key_val = keypad[row][col];

                while(LPC_GPIO0->FIOPIN & COL_MASK);
                return key_val;
            }
        }
    }
}

void Display(void) {
    LPC_GPIO1->FIOPIN = FIRST_SEG;
    LPC_GPIO0->FIOCLR = 0x00000FF0;
    LPC_GPIO0->FIOSET = array_dec[d1] << 4;
    for(j = 0; j < 500; j++);

    LPC_GPIO1->FIOPIN = SECOND_SEG;
    LPC_GPIO0->FIOCLR = 0x00000FF0;
    LPC_GPIO0->FIOSET = array_dec[d2] << 4;
    for(j = 0; j < 500; j++);

    LPC_GPIO1->FIOPIN = THIRD_SEG;
    LPC_GPIO0->FIOCLR = 0x00000FF0;
    LPC_GPIO0->FIOSET = array_dec[d3] << 4;
    for(j = 0; j < 500; j++);

    LPC_GPIO1->FIOPIN = FOURTH_SEG;
    LPC_GPIO0->FIOCLR = 0x00000FF0;
    LPC_GPIO0->FIOSET = array_dec[d4] << 4;
    for(j = 0; j < 500; j++);
}