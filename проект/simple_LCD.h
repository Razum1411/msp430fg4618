#include <stdint.h>
#include <msp430.h>
void init_lcd(void);
void LCDchar(int ch, int pos);
void LCDdigit(uint16_t val, int pos);
