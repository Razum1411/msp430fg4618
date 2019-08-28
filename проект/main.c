#include <msp430xG46x.h> 
#include "simple_LCD.h" 

void main(void) 
{ 
WDTCTL = WDTPW+WDTHOLD; // Stop WDT 
P5DIR |= BIT1; // Set P5.1 to be output 
P2SEL |= BIT4 + BIT5; // P2.4,5 = USCI_A0 RXD/TXD 
UCA0CTL1 |= UCSSEL_2; // SMCLK 
UCA0BR0 = 0x09; // 1MHz/115200 (lower byte) 
UCA0BR1 = 0x00; // 1MHz/115200 (upper byte) 
UCA0MCTL = 0x02; // Modulation (UCBRS0=0x01)(UCOS16=0) 
UCA0CTL1 &= ~UCSWRST; // **Initialize USCI state machine** 
IE2 |= UCA0RXIE; // Enable USCI_A0 RX interrupt 
TACTL = MC_2 + TASSEL_1 + ID_3; // The continuous timer mode of Timer A; ACLK; /8; freq = 4096 

// Setting the Timer A capture mode 

TACCTL0 = CAP + CM_3 + CCIE ; // Capture mode with timer A interrupt; both fronts; interrupt allowed 

//Timer A is set (отвечает за определение времени удержания кнопки) 


//инициализация правой нижней кнопки на msp430 
P1SEL |= 0x01; 
init_lcd(); 
_BIS_SR(LPM0_bits + GIE); // Enter LPM0, interrupts enabled 
} 
// Echo back RXed character, confirm TX buffer is ready first 


#pragma vector=USCIAB0RX_VECTOR 
__interrupt void USCIA0RX_ISR (void) 
{ 
while(!(IFG2&UCA0TXIFG)); // Wait until can transmit 
switch ( UCA0TXBUF ) { 
case 0x30: 
LCDdigit(0,5); 
break; 
case 0x31: 
LCDdigit(1,5); 
break; 
case 0x32: 
LCDdigit(2,5); 
break; 
case 0x33: 
LCDdigit(3,5); 
break; 
case 0x34: 
LCDdigit(4,5); 
break; 
case 0x35: 
LCDdigit(5,5); 
break; 
case 0x36: 
LCDdigit(6,5); 
break; 
case 0x37: 
LCDdigit(7,5); 
break; 
case 0x38: 
LCDdigit(8,5); 
break; 
case 0x39: 
LCDdigit(9,5); 
break; 
default: 
LCDdigit(0,5); 
break; 
} 
UCA0TXBUF = UCA0RXBUF; // TX -> RXed character 
// LCDdigit(0,5); // проверка, что вообще ловит нажатия
P5OUT^=BIT1; // просто включает LED4 для удобной индикации 
} 



#pragma vector=TIMERA0_VECTOR 
__interrupt void ButtonHandler (void) { 
//TBCTL = ~timerOn; 
TBCTL = MC_0; 
TBCTL |= BIT2; 
//P2OUT &= ~leftLED_dot; 
//P2OUT &= ~rightLED_dash; 
if (TACCTL0 & CCI) { // Проверка, была ли кнопка отпущена 
//releasedTime = TACCR0; // Сохранить текущее значение TACCR0 (время, когда кнопка была отпущена) 
// Запуск таймера B (отвечает за определение времени, в течение которого кнопка не нажимается) 
//TBCTL |= timerOn; 
//TBCTL = MC_2 + TASSEL_1 + ID_3; 
UCA0TXBUF = 'a';   // отправляем любой символ по нажатию.Если раскомментить все остальное, то мы устрним дребезжание кнопки при нажатии
} 
//else 
//if (flagContacts != 0) { 
// flagContacts = 0; 
// pressedTime = TACCR0; //Сохранить время нажатия на кнопку 
//} 
}
