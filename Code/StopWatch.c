/******************************************************************************
 *
 * Module: StopWatch
 *
 * File Name: StopWatch.c
 *
 * Description: Source file for the StopWatch.
 *
 * Author: Jana Wael
 *
 *******************************************************************************/

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

int second = 0;
int minute = 0;
int hour = 0;
int clock[6];

void timer1_CTC(){
	TCNT1 = 0;
	OCR1A = 1000;
	TCCR1A  = (1 << FOC1A);
	TCCR1B = (1 << WGM12) | (1 << CS10) | (1 << CS12);
	TIMSK |= (1 << OCIE1A);

}
ISR(TIMER1_COMPA_vect){
	clock[0]++;//sec1
     if(clock[0]==10){
         clock[0]=0;
         clock[1]++;//sec2
         if(clock[1] == 6 && clock[0] == 0){
             clock[1]=0;
             clock[2]++;//min1
         }
     }
     if(clock[2]==10){
         clock[2]=0;
         clock[3]++;//min2
          if(clock[3]==6 && clock[2]==0){
              clock[3]=0;
              clock[4]++;//hour1
          }
     }
     if(clock[4]==10){
         clock[4]=0;
         clock[5]++;//hour2
         if(clock[5]==10 && clock[4]==0){
             for(int i = 0; i < 6; i++){
                 clock[i]=0;
             }
             TCNT1 = 0;
         }
     }
}

void INT0_Init(){
	DDRC &= (1 << PD2);
	PORTD |= (1 << PD2);
	MCUCR |= (1 << ISC01);
	GICR |= (1 << INT0);
}

void INT1_Init(){
	DDRC &= (1 << PD3);
	MCUCR |= (1 << ISC11) | (1 << ISC10);
	GICR |= (1 << INT1);
}
void INT2_Init(){
	DDRC &= (1 << PB2);
	PORTB |= (1 << PB2);
	MCUCSR |= (1 << ISC2);
	GICR |= (1 << INT2);
}
//Reset interrupt
ISR(INT0_vect){
	SREG |= (1 << 7);
	second = 0;
	minute = 0;
	hour = 0;
	TCNT1 = 0;
}

//pause interrupt
ISR(INT1_vect){
	SREG |= (1 << 7);
	TCCR1B &= (~(1 << CS10)) & (~(1 << CS12));
}

//resume interrupt
ISR(INT2_vect){
	SREG |= (1 << 7);
	TCCR1B |= (1 << CS10) | (1 << CS12);
}

void init_7segment(void){
	DDRC = 0x0F;
	PORTC = 0xF0;
	DDRA = 0X3F;
	PORTA = 0x01;
}

int main(void){
	SREG|=(1<<7);

	INT0_Init();

	INT1_Init();

	INT2_Init();

	timer1_CTC();

	init_7segment();


	while(1){
		for(int j=0;j<6;j++){
				PORTA=1<<j;
				PORTC=clock[j];
				_delay_ms(3);
			}

	}

}
