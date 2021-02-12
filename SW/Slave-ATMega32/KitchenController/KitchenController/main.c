

#include <avr/io.h>
#include <stdio.h>
#include "I2C_Slave_C_File.c"

#define Slave_Address		0x20
#define F_CPU	8000000UL

#include <util/delay.h>

void lcd_init();
void dis_cmd(char);
void dis_data(char);
void lcdcmd(char);
void lcddata(char);
void dis_print(char*);
void lcd_clear();
void lcd_clear_animation();
void second_line();
void first_line();

#define rs PB2
#define en PB3
#define pr PB0

int main(void)
{
	DDRB = 0xFF;		//--- Defining PORT A as output
	
	lcd_init();
	lcd_clear();
	first_line();
	
	I2C_Slave_Init(Slave_Address);

	int cnt = 0;
	while (1){
		int8_t s = 0;
		int8_t wr =I2C_Slave_Listen(); 
		switch(wr){
			case 0:
			{
				while(1){
					char str[16];
					s =  I2C_Slave_Receive();
					if(s==-1)break;
					sprintf(str,"%c ",(char) s);
					if(cnt>15)second_line();
					dis_print(str);
					cnt +=2;
				}
				//lcd_clear();

			}
			default:
					break;
		}
	}
}

void second_line(){
	dis_cmd(0xC0);
}

void first_line(){
	dis_cmd(0x80);
}

void lcd_clear_animation(){
	for(int x=0;x<16;x++)
	{
		dis_cmd(0x1c);
		_delay_ms(5);
	}
	lcd_clear();
}

void lcd_clear(){
	dis_cmd(0x01);
}

void lcd_init()
{
	dis_cmd(0x02);		// to initialize LCD in 4-bit mode.
	dis_cmd(0x28);
	dis_cmd(0x0C);
	dis_cmd(0x06);
	dis_cmd(0x83);
}

void dis_cmd(char cmd_value)
{
	char cmd_value1;
	
	cmd_value1 = cmd_value & 0xF0;		//mask lower nibble because PA4-PA7 pins are used.
	lcdcmd(cmd_value1);			// send to LCD
	
	cmd_value1 = ((cmd_value<<4) & 0xF0);	//shift 4-bit and mask
	lcdcmd(cmd_value1);			// send to LCD
}

void dis_data(char data_value)
{
	char data_value1;
	
	data_value1=data_value&0xF0;
	lcddata(data_value1);
	
	data_value1=((data_value<<4)&0xF0);
	lcddata(data_value1);
}

void lcdcmd(char cmdout)
{
	PORTB=cmdout;
	PORTB&=~(1<<rs);
	PORTB|=(1<<en);
	_delay_ms(1);
	PORTB&=~(1<<en);
}

void lcddata(char dataout)
{
	PORTB=dataout;
	PORTB|=(1<<rs);
	PORTB|=(1<<en);
	_delay_ms(1);
	PORTB&=~(1<<en);
}

void dis_print(char* p){
	while(*p){
		dis_data(*p++);
	}
}