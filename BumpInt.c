// BumpInt.c
// Runs on MSP432, interrupt version
// Provide low-level functions that interface bump switches on the robot.
// Daniel Valvano and Jonathan Valvano
// July 11, 2019
/* This example accompanies the book
"Embedded Systems: Introduction to Robotics,
Jonathan W. Valvano, ISBN: 9781074544300, copyright (c) 2019
For more information about my classes, my research, and my books, see
http://users.ece.utexas.edu/~valvano/
Simplified BSD License (FreeBSD License)
Copyright (c) 2019, Jonathan Valvano, All rights reserved.
Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:
1. Redistributions of source code must retain the above copyright notice,
this list of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright notice,
this list of conditions and the following disclaimer in the documentation
and/or other materials provided with the distribution.
THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE
USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
The views and conclusions contained in the software and documentation are
those of the authors and should not be interpreted as representing official
policies, either expressed or implied, of the FreeBSD Project.
*/
// Negative logic bump sensors
// P4.7 Bump5, left side of robot
// P4.6 Bump4
// P4.5 Bump3
// P4.3 Bump2
// P4.2 Bump1
// P4.0 Bump0, right side of robot
#include <stdint.h>
#include "msp.h"
void (*BumpTask)(void);
// Initialize Bump sensors
// Make six Port 4 pins inputs
// Activate interface pullup
// pins 7,6,5,3,2,0
// Interrupt on falling edge (on touch)
void BumpInt_Init(void(*task)(uint8_t)){
// write this as part of Lab 14
BumpTask=task;
//?Bump5
P4->SEL0 &= ~0x80;
P4->SEL1 &= ~0x80; // P4.7 as bump5
P4->DIR &= ~0x80; // make P4.7 in
P4->REN |= 0x80; // enable pull resistors
P4->OUT |= 0x80; // P4.7 pull-up
P4->IES |= 0x80; // P4.7 falling edge event
P4->IFG &= ~0x80; // clear flag4
P4->IE |= 0x80; // arm interrupt on P4.7
//?Bump4
P4->SEL0 &= ~0x40;
P4->SEL1 &= ~0x40; // P4.6 as bump4
P4->DIR &= ~0x40; // make P4.6 in
P4->REN |= 0x40; // enable pull resistors
P4->OUT |= 0x40; // P4.6 pull-up
P4->IES |= 0x40; // P4.6 falling edge event
P4->IFG &= ~0x40; // clear flag4
P4->IE |= 0x40; // arm interrupt on P4.6
//?Bump3
P4->SEL0 &= ~0x20;
P4->SEL1 &= ~0x20; // P4.5 as bump3
P4->DIR &= ~0x20; // make P4.5 in
P4->REN |= 0x20; // enable pull resistors
P4->OUT |= 0x20; // P4.5 pull-up
P4->IES |= 0x20; // P4.5 falling edge event
P4->IFG &= ~0x20; // clear flag4
P4->IE |= 0x20; // arm interrupt on P4.5
//?Bump2
P4->SEL0 &= ~0x08;
P4->SEL1 &= ~0x08; // P4.3 as bump2
P4->DIR &= ~0x08; // make P4.3 in
P4->REN |= 0x08; // enable pull resistors
P4->OUT |= 0x08; // P4.3 pull-up
P4->IES |= 0x08; // P4.3 falling edge event
P4->IFG &= ~0x08; // clear flag4
P4->IE |= 0x08; // arm interrupt on P4.3
//?Bump1
P4->SEL0 &= ~0x04;
P4->SEL1 &= ~0x04; // P4.2 as bump1
P4->DIR &= ~0x04; // make P4.2 in
P4->REN |= 0x04; // enable pull resistors
P4->OUT |= 0x04; // P4.2 pull-up
P4->IES |= 0x04; // P4.2 falling edge event
P4->IFG &= ~0x04; // clear flag4
P4->IE |= 0x04; // arm interrupt on P4.2
//?Bump0
P4->SEL0 &= ~0x01;
P4->SEL1 &= ~0x01; // P4.0 as bump0
P4->DIR &= ~0x01; // make P4.0 in
P4->REN |= 0x01; // enable pull resistors
P4->OUT |= 0x01; // P4.0 pull-up
P4->IES |= 0x01; // P4.0 falling edge event
P4->IFG &= ~0x01; // clear flag4
P4->IE |= 0x01; // arm interrupt on P4.0
//interrupts
NVIC->IP[9]=(NVIC->IP[9]&0x00FFFFFF)|0x40000000;
NVIC->ISER[1] = 0x00000040; // enable
EnableInterrupts();
}
// Read current state of 6 switches
// Returns a 6-bit positive logic result (0 to 63)
// bit 5 Bump5
// bit 4 Bump4
// bit 3 Bump3
// bit 2 Bump2
// bit 1 Bump1
// bit 0 Bump0
uint8_t Bump_Read(void){
// write this as part of Lab 14
uint8_t result;
return result; // replace this line
}
// we do not care about critical section/race conditions
// triggered on touch, falling edge
void PORT4_IRQHandler(void){
BumpTask();
// write this as part of Lab 14
}