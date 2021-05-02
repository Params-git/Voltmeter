/*
 * 3_phase_voltmeter.c
 *
 * Created: 5/2/2021 11:56:38 PM
 * Author : Paramjeet singh
 */ 

#include <avr/io.h>
#define F_CPU 8000000UL
#include <util/delay.h>
#include <stdlib.h>
#include "lcd-routines.h"

#ifndef F_CPU
#define F_CPU 8000000UL
#endif

/*int adc_read(int ch);
int adc_read2(int ch);
int adc_read3(int ch);*/

int adc;
int adc2;
int adc3;

char buf[20];
char buf2[20];
char buf3[20];

float vm = 0;
float vm2 = 0;
float vm3 = 0;

void adc_init()
{
	// AREF = AVcc
	ADMUX = (1<<REFS0); //for enabling reference voltage 
	ADCSRA = (1<<ADEN)|(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0);  //ADC enable and set prescaler
}

// read adc value
uint16_t adc_read(uint8_t ch){
	
	ch &= 0b00000111; 
	ADMUX = (ADMUX & 0xF8)|ch;  //1111 1000 0xF8 
	ADCSRA |= (1<<ADSC);
	while(ADCSRA & (1<<ADSC)); // wait until conversion  completes; ADSC=0 means Complete
	
	return (ADCW);
}

int main(void)
{
	uint16_t adc_int[41];
	uint16_t adc_int2[41];
	uint16_t adc_int3[41];
	int i = 0, Max = 0, Max2 = 0, Max3 = 0;
	
	lcd_init();
	lcd_setcursor(0, 1);
	lcd_string("AC VOLTMETER..");
	_delay_ms(1000);
	lcd_clear();
	lcd_string("intialising...");
	
	
	adc_init();
	
	while(1){
		
		_delay_ms(100);

		for (i = 0; i <= 39; i++) {
			adc_int[i] = adc_read(0);      // read adc value at PORTC.0
			adc_int2[i] = adc_read(1);
			adc_int3[i] = adc_read(2);
		}

		
		Max = adc_int[0];
		Max2 = adc_int2[0];
		Max3 = adc_int3[0];
		
		for (i = 0; i <= 40; i++) {
			if (Max < adc_int[i])
			Max = adc_int[i];
			
			if (Max2 < adc_int2[i])
			Max2 = adc_int2[i];
			
			if (Max3 < adc_int3[i])
			Max3 = adc_int3[i];
		}

		adc = Max;
		adc2 = Max2;
		adc3 = Max3;
		
		itoa(Max, buf, 10);
		itoa(Max2, buf2, 10);
		itoa(Max3, buf3, 10);
		
		_delay_ms(1000);
		
		vm = (float)(adc * 0.27734375); // 283/1024 = 0.2763637188
		vm2 = (float)(adc2 * 0.27734375); // 283/1024 = 0.2763637188
		vm3 = (float)(adc3 * 0.27734375); // 283/1024 = 0.2763637188
		
		adc = (int)vm;
		adc2 = (int)vm2;
		adc3 = (int)vm3;
		
		itoa(adc, buf, 10);
		itoa(adc2, buf2, 10);
		itoa(adc3, buf3, 10);
		
		_delay_ms(1000);
		
		lcd_clear();
		lcd_setcursor(0,1);
		lcd_string("Phase1: ");
		lcd_setcursor(0,8);
		lcd_string((const char*)buf);
		_delay_ms(1000);
		
		lcd_clear();
		lcd_setcursor(0,1);
		lcd_string("Phase2: ");
		lcd_setcursor(0,8);
		lcd_string((const char*)buf2);
		_delay_ms(1000);
		
		lcd_clear();
		lcd_setcursor(0,1);
		lcd_string("Phase3: ");
		lcd_setcursor(0,8);
		lcd_string((const char*)buf3);
		_delay_ms(1000);
	}
	return 0;
}