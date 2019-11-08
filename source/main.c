/*	Author: akim106
 *  Partner(s) Name: 
 *	Lab Section:
 *	Assignment: Lab 10  Exercise 1
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#include <avr/interrupt.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

volatile unsigned char TimerFlag = 0;

unsigned long _avr_timer_M = 1;
unsigned long _avr_timer_cntcurr = 0;

void TimerOn() {
    TCCR1B = 0x0B;
    OCR1A = 125;
    TIMSK1 = 0x02;
    TCNT1 = 0;
    _avr_timer_cntcurr = _avr_timer_M;
    SREG |= 0x80;
}

void TimerOff() {
    TCCR1B = 0x00;
}

void TimerISR() {
    TimerFlag = 1;
}

ISR(TIMER1_COMPA_vect) {
    _avr_timer_cntcurr--;
    if (_avr_timer_cntcurr == 0) {
      TimerISR();
      _avr_timer_cntcurr = _avr_timer_M;
    }
}

void TimerSet(unsigned long M) {
    _avr_timer_M = M;
    _avr_timer_cntcurr = _avr_timer_M;
}

enum ThreeLEDStates{One, Two, Three} ThreeLEDState;
enum BlinkingLEDStates{Right, Left} BlinkingLEDState;
enum CombineLEDStates{Do} CombineLEDState;
unsigned char threeLED = 0;
unsigned char blinkingLED = 0;

void ThreeLEDTick() {
    switch(ThreeLEDState) { //Transitions
      case One:
        ThreeLEDState = Two;
        break;
      case Two:
        ThreeLEDState = Three;
        break;
      case Three:
        ThreeLEDState = One;
        break;
    }
    switch(ThreeLEDState) { //Actions
      case One:
        threeLED = 0x01;
        break;
      case Two:
        threeLED = 0x02;
        break;
      case Three:
        threeLED = 0x04;
        break;
    }
}

void BlinkingLEDTick() {
    switch(BlinkingLEDState) { //Transitions
      case Right:
        BlinkingLEDState = Left;
        break;
      case Left:
        BlinkingLEDState = Right;
        break;
    }
    switch(BlinkingLEDState) { //Actions
      case Right:
        blinkingLED = 0x04;
        break;
      case Left:
        blinkingLED = 0x01;
        break;
    }
}

void CombineLEDTick() {
    switch(CombineLEDState) {
      case Do:
        CombineLEDState = Do;
        break;
    }
    switch(CombineLEDState) {
      case Do:
        PORTB = threeLED | blinkingLED;
        break;
    }
} 

int main(void) {
    DDRB = 0xFF; PORTB = 0x00;
    unsigned long count1 = 0;
    unsigned long count2 = 0;
    TimerSet(1);
    TimerOn();
    while (1) {
      if (count1 < 1000) {
        count1++;
      } else {
        ThreeLEDTick();
        count1 = 0;
      }
      if (count2 < 1000) {
        count2++;
      } else {
        BlinkingLEDTick();
        count2 = 0;
      }
      CombineLEDTick();
    }
    return 1;
}
