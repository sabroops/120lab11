/*	Author: Sabrina Carlos
 *      Partner(s) Name: 
 *	Lab Section: 021
 *	Assignment: Lab #11  Exercise #2
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif
#include<avr/interrupt.h>
#include "timer.h"
#include "bit.h"
#include "keypad.h"
#include "scheduler.h"
#include "io.h"

unsigned char x = 0x00;
unsigned char leg[] = "CS120B is Legend... wait for it DARY!               "; //barney boi
unsigned char out[16] = ""; //fit on screen
unsigned char count = 0x00;

enum Screen_states { Start, disp };
int ScreenSMTick(int state) {
	switch(state) {
		case Start:
			state = disp;
			break;
		case disp:
		       	state = disp;
			break;
		default: state = Start;
			 break;
	}
	switch (state) {
		case Start: 
			break;
		case disp:
			for (unsigned int i = 0; i < 16; i++) {
				out[i] = leg[(count + i) % 52];	
			}
			LCD_DisplayString(1, out);
			count++;
			if (count > 52) { 
				count = 0;
			}
			break;
		default:
			break;
	}
	return state;
}
int main(void) {
    /* Insert DDR and PORT initializations */
	DDRD = 0xFF; PORTD = 0x00;
	DDRB = 0xFF; PORTB = 0x00;
    /* Insert your solution below */
	static task task1;
	task *tasks[] = {&task1};
	const unsigned short numTasks = sizeof(tasks)/sizeof(task*);
	//const char start = -1;
	task1.state = Start;
	task1.period = 1500;
	task1.elapsedTime = task1.period;
	task1.TickFct = &ScreenSMTick;

	TimerSet(1);
	TimerOn();
	LCD_init();
	unsigned short i;
    while (1) {
	for (i=0; i<numTasks; i++){
		if (tasks[i]->elapsedTime == tasks[i]->period){
			tasks[i]->state = tasks[i]->TickFct(tasks[i]->state);
			tasks[i]->elapsedTime = 0;
		}
		tasks[i]->elapsedTime += 10;
	}
	while (!TimerFlag);
	TimerFlag = 0;
    }
    //return 1;
}
