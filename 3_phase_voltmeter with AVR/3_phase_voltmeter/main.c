/*
 * 3_phase_voltmeter.c
 *
 * Created: 5/2/2021 11:56:38 PM
 * Author : Paramjeet singh
 */ 

#ifndef F_CPU
#define F_CPU 8000000UL
#endif

#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include "lcd-routines.h"

int adc;
char buf[20];
float vm = 0;

void adc_init()
{
	//AREF = AVcc
	ADMUX = (1<<REFS0); //for enabling reference voltage 
	ADCSRA = (1<<ADEN)|(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0);  //ADC enable and set prescaler
}

uint16_t adc_read(uint8_t ch){
	
	ch &= 0b00000111; 
	ADMUX = (ADMUX & 0xF8)|ch;  //1111 1000 0xF8 
	ADCSRA |= (1<<ADSC);
	while(ADCSRA & (1<<ADSC)); // wait until conversion  completes; ADSC=0 means Complete
	
	return (ADCW);
}


int adc_phase(int ch)
{
	uint16_t adc_int[100];
	int i = 0, Max = 0;
	
	for (i = 0; i <= 79; i++) {
		adc_int[i] = adc_read(ch);      // read adc value at PORTC.0
	}
	
	Max = adc_int[0];
	
	for (i = 0; i <= 80; i++) {
		if (Max < adc_int[i])
		Max = adc_int[i];
	}
	
	adc = Max;
	//itoa(Max, buf, 10);
	
	_delay_ms(500);
	
	vm = (float)(adc * 0.2763637188); // 283/1024 = 0.2763637188
	adc = (int)vm;
	
	itoa(adc, buf, 10);
	
	return *buf;
	
}


int main(void)
{	
	lcd_init();
	lcd_setcursor(0, 1);
	lcd_string("AC VOLTMETER..");
	_delay_ms(500);
	lcd_clear();
	lcd_string("intialising ADC..");
	
	adc_init();
	
	while(1){
		
		for(int i=0; i<=2; i++)
		{
			adc_phase(i);  //Phase1, Phase2, Phase3
			/*_delay_ms(1000);*/
			
			if(i==0)
			{
				lcd_clear();
				lcd_setcursor(0,1);
				lcd_string("Phase1: ");
				lcd_setcursor(0,8);
				lcd_string((const char*)buf);
			}	
			
			else if(i==1)
			{
				lcd_clear();
				lcd_setcursor(0,1);
				lcd_string("Phase2: ");
				lcd_setcursor(0,8);
				lcd_string((const char*)buf);
			}
			
			else if(i==2)
			{
				lcd_clear();
				lcd_setcursor(0,1);
				lcd_string("Phase3: ");
				lcd_setcursor(0,8);
				lcd_string((const char*)buf);
			}
			
			else
			{
				return 0;
			}
			/*_delay_ms(500);*/
		}
	}
}