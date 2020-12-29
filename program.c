/*****************************************************
This program was produced by the
CodeWizardAVR V2.05.3 Standard
Automatic Program Generator
� Copyright 1998-2011 Pavel Haiduc, HP InfoTech s.r.l.
http://www.hpinfotech.com

Project : 
Version : 
Date    : 12/29/2020
Author  : M
Company : 
Comments: 


Chip type               : ATmega8
Program type            : Application
AVR Core Clock frequency: 8.000000 MHz
Memory model            : Small
External RAM size       : 0
Data Stack size         : 256
*****************************************************/

#include <mega8.h>

// Alphanumeric LCD functions
#include <alcd.h>
#include <stdio.h>
#include <delay.h>

// Declare your global variables here
#define Rows PORTC   //Pc0,pc1,pc2,pc3
#define Columns PIND //PD4,PD5,PD6

unsigned char upperNibble, keyCode, keyPressed,k;
unsigned char orderedItem[20];

char press_key()
{
    unsigned char i;
    DDRC = 0x0f;
    PORTC = 0x0f;
    PORTD = 0xf0;
    k = 1;
    while (k == 1)
    {

        upperNibble = 0xff;

        for (i = 0; i < 4; i++)
        {
            delay_ms(1);
            Rows = ~(0x01 << i);
            delay_ms(1);
            upperNibble = Columns | 0x0f;

            if (upperNibble != 0xff)
            {
                delay_ms(20); //key debouncing delay
                upperNibble = Columns | 0x0f;
                if (upperNibble == 0xff)
                    goto OUT;

                keyCode = (upperNibble & 0xf0) | (0x0f & ~(0x01 << i));

                while (upperNibble != 0xff)
                    upperNibble = Columns | 0x0f;

                delay_ms(20); //key debouncing delay

                switch (keyCode)
                {
                case (0xee):
                    keyPressed = '1';
                    k = 0;
                    break;
                case (0xed):
                    keyPressed = '4';
                    k = 0;
                    break;
                case (0xeb):
                    keyPressed = '7';
                    k = 0;
                    break;
                case (0xe7):
                    keyPressed = '*';
                    k = 0;
                    break;
                case (0xde):
                    keyPressed = '2';
                    k = 0;
                    break;
                case (0xdd):
                    keyPressed = '5';
                    k = 0;
                    break;
                case (0xdb):
                    keyPressed = '8';
                    k = 0;
                    break;
                case (0xd7):
                    keyPressed = '0';
                    k = 0;
                    break;
                case (0xbe):
                    keyPressed = '3';
                    k = 0;
                    break;
                case (0xbd):
                    keyPressed = '6';
                    k = 0;
                    break;
                case (0xbb):
                    keyPressed = '9';
                    k = 0;
                    break;
                case (0xb7):
                    keyPressed = '#';
                    k = 0;
                    break;
                case (0x7e):
                    keyPressed = '/';
                    k = 0;
                    break;
                case (0x7d):
                    keyPressed = 'X';
                    k = 0;
                    break;
                case (0x7b):
                    keyPressed = '-';
                    k = 0;
                    break;
                case (0x77):
                    keyPressed = '+';
                    k = 0;
                    break;
                default:
                    keyPressed = 'X';
                    k = 0;
                }
            OUT:;
            }
        }
    }
    return keyPressed;
}



void main(void)
{
// Declare your local variables here

// Input/Output Ports initialization
// Port B initialization
// Func7=In Func6=In Func5=In Func4=In Func3=In Func2=In Func1=In Func0=In 
// State7=T State6=T State5=T State4=T State3=T State2=T State1=T State0=T 
PORTB=0x00;
DDRB=0x00;

// Port C initialization
// Func6=In Func5=In Func4=In Func3=In Func2=In Func1=In Func0=In 
// State6=T State5=T State4=T State3=T State2=T State1=T State0=T 
PORTC=0x00;
DDRC=0x00;

// Port D initialization
// Func7=In Func6=In Func5=In Func4=In Func3=In Func2=In Func1=In Func0=In 
// State7=T State6=T State5=T State4=T State3=T State2=T State1=T State0=T 
PORTD=0x00;
DDRD=0x00;

// Timer/Counter 0 initialization
// Clock source: System Clock
// Clock value: Timer 0 Stopped
TCCR0=0x00;
TCNT0=0x00;

// Timer/Counter 1 initialization
// Clock source: System Clock
// Clock value: Timer1 Stopped
// Mode: Normal top=0xFFFF
// OC1A output: Discon.
// OC1B output: Discon.
// Noise Canceler: Off
// Input Capture on Falling Edge
// Timer1 Overflow Interrupt: Off
// Input Capture Interrupt: Off
// Compare A Match Interrupt: Off
// Compare B Match Interrupt: Off
TCCR1A=0x00;
TCCR1B=0x00;
TCNT1H=0x00;
TCNT1L=0x00;
ICR1H=0x00;
ICR1L=0x00;
OCR1AH=0x00;
OCR1AL=0x00;
OCR1BH=0x00;
OCR1BL=0x00;

// Timer/Counter 2 initialization
// Clock source: System Clock
// Clock value: Timer2 Stopped
// Mode: Normal top=0xFF
// OC2 output: Disconnected
ASSR=0x00;
TCCR2=0x00;
TCNT2=0x00;
OCR2=0x00;

// External Interrupt(s) initialization
// INT0: Off
// INT1: Off
MCUCR=0x00;

// Timer(s)/Counter(s) Interrupt(s) initialization
TIMSK=0x00;

// USART initialization
// USART disabled
UCSRB=0x00;

// Analog Comparator initialization
// Analog Comparator: Off
// Analog Comparator Input Capture by Timer/Counter 1: Off
ACSR=0x80;
SFIOR=0x00;

// ADC initialization
// ADC disabled
ADCSRA=0x00;

// SPI initialization
// SPI disabled
SPCR=0x00;

// TWI initialization
// TWI disabled
TWCR=0x00;

// Alphanumeric LCD initialization
// Connections are specified in the
// Project|Configure|C Compiler|Libraries|Alphanumeric LCD menu:
// RS - PORTB Bit 4
// RD - PORTB Bit 5
// EN - PORTB Bit 6
// D4 - PORTB Bit 0
// D5 - PORTB Bit 1
// D6 - PORTB Bit 2
// D7 - PORTB Bit 3
// Characters/line: 16
lcd_init(16);

while (1)
      {
      // Place your code here  
      char strbuff[20];     
      char keyPressed = press_key();         
      char* item = menuItem(keyPressed);
      sprintf(strbuff, "its: %s", orderedItem);
      
      lcd_puts(strbuff);
      delay_ms(100);

      }
}
