#define F_CPU	8000000UL	//--- CPU Clock Frequency

#include<avr/io.h>
#include<util/delay.h>
#include<stdio.h>
#include<stdbool.h>
#include<avr/interrupt.h>
#include "I2C_Master_C_file.c"	/* Include I2C header file */

#define Slave_Write_Address		0x20
#define Slave_Read_Address		0x21

#define rs PB2
#define en PB3
#define pr PB0
#define Rows PORTA //Pc0,pc1,pc2,pc3
bool pause = false;
bool isWriting = false;
int k;

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
int press_key();
void show_menu();


const char ITEMS[12][10] = {
	"Beef",
	"Pizza",
	"Kabab",
	"Chicken",
	"Soup",
	"Tofu",
	"Sushi",
	"Steak",
	"Soda",
	"*",
	"Salad",
	"#"
};
const char KEYS[12] = {
	'1',
	'2',
	'3',
	'4',
	'5',
	'6',
	'7',
	'8',
	'9',
	'*',
	'0',
	'#'
};

bool ordered_items[12] = {
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false
};

int main(void)
{
	DDRB=0xFF;
	DDRC=0xFF;
	PORTC=0xFF;

	I2C_Init();

	
	lcd_init();
	


	
	dis_print("Welcome");
	second_line();
	dis_print("to our restaurant");
	_delay_ms(50);
	lcd_clear();
	//show_menu();	
	sei();
	
	_delay_ms(50);
	lcd_clear();
	
	
	while(1)
	{	
		k = press_key();
		

		
		if(k && isWriting == false){
			GICR = (1<<INT0);
			isWriting = true;
			
		}
		
		GICR = (0<<INT0);
		isWriting = false;
		
	}
	
	
}

ISR (INT0_vect){

	if (k==9){
		lcd_clear();
		dis_print("press a key");
		pause = false;
		
	}else if(k==11){
		lcd_clear();
		dis_print("Finished");
		
		I2C_Start_Wait(Slave_Write_Address);
		for(int i = 0; i<12; i++){
			if(ordered_items[i]==true){
				I2C_Write(KEYS[i]);
				_delay_ms(50);
			}
		}
					
		PORTB = (1<<pr);
		_delay_ms(20);
		PORTB = (0<<pr);

		
	}else{
		if(!pause){
			lcd_clear();
			char str[16];
			first_line();
			sprintf(str,"%c: %s",KEYS[k],ITEMS[k]);
			dis_print(str);
			second_line();
			dis_print("*:add, #:finish");
			first_line();
			
			ordered_items[k]=true;
			
			pause = true;
			
		}
	}
}

int press_key()
{
	unsigned char c,r;
	
	DDRA=0xf0;
	PORTA=0xef;
	int k[4][3]={{0,1,2},{3,4,5},{6,7,8},{9,10,11}};
	
	do{
		PORTA &= 0x0f;
		r = PINA & 0x0f;
	} while(r!=0x0f);
		
	do{
		r = PINA & 0x0f;
	} while(r==0x0f);
		
	while(1){
		PORTA = 0xef;
		r = PINA & 0x0f;
		if(r!=0x0f){
			c=0;
			break;
		}
		PORTA = 0xdf;
		r = PINA & 0x0f;
		if(r!=0x0f){
			c=1;
			break;
		}
		PORTA = 0xbf;
		r=PINA&0x0f;
		if(r!=0x0f)
		{
			c=2;
			break;
		}
	}
	if(r==0x0e) return k[0][c];
	else if(r==0x0d) return k[1][c];
	else if(r==0x0b) return k[2][c];
	else return k[3][c];
}

void show_menu(){
	dis_print("***** MENU *****");
	_delay_ms(30);
	for(int i =0; i<9; i+=2){
		lcd_clear();
		char str[16];
		sprintf(str,"%s: press %c",ITEMS[i],KEYS[i-1]);
		dis_print(str);
		second_line();
		if(i==8)i++;
		sprintf(str,"%s: press %c",ITEMS[i+1],KEYS[i]);
		 dis_print(str);
		_delay_ms(40);
		first_line();
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
