// Lab14_EdgeInterruptsmain.c
// Runs on MSP432, interrupt version
// Main test program for interrupt driven bump switches the robot.
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
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
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
#include "../inc/Clock.h"
#include "../inc/CortexM.h"
#include "../inc/LaunchPad.h"
#include "../inc/Motor.h"
#include "../inc/BumpInt.h"
#include "../inc/TExaS.h"
#include "../inc/TimerA1.h"
#include "../inc/FlashProgram.h"
#include "..\inc\Reflectance.h"

//FSM Code-------------------------------------------------
struct State {
  uint32_t out;                // 2-bit output
  uint32_t delay;              // time to delay in 1ms
  const struct State *next[4]; // Next if 2-bit input is 0-3
};
typedef const struct State State_t;


#define Center  &fsm[0]
#define Left1   &fsm[1]
#define Left2   &fsm[2]
#define Right1  &fsm[3]
#define Right2  &fsm[4]
#define Stop    &fsm[5]


State_t fsm[6]={

     {0x03, 100, { Stop, Left1,   Right1,  Center }},  // Center
     {0x02, 75, { Stop,  Left2, Right2,  Center }},  // Left1
     {0x03, 75, { Stop, Left1,   Right1, Center }},  // Left2
     {0x01, 75, { Stop, Left2,   Right2,  Center }},  // Right1
     {0x03, 75, { Stop,  Left1, Right1,  Center }},  // Right2
     {0, 500, { Stop, Left1, Right1, Center}}  // Stop

};

//Collision Handler------------------------------------
uint8_t CollisionData, CollisionFlag;  // mailbox
void HandleCollision(uint8_t bumpSensor){
   Motor_Stop();
   CollisionData = bumpSensor;
   CollisionFlag = 1;
}


//Timed InterruptFSM--------------------------------------
State_t *Spt;     // pointer to the current state
uint32_t Input;   // 00=off, 01=right, 10=left, 11=on
uint32_t Output;  // 3=straight, 2=turn right, 1=turn left

void Task(void){
  int speed=4500;

  Spt = Spt->next[Input];           // next depends on input and state
  Output = Spt->out;



      if(Output==0x03){
          Motor_Forward(speed,speed);
         }
      if(Output==0x02){
          Motor_Right(speed,speed);
         }
      if(Output==0x01){
          Motor_Left(speed,speed);
         }
      if(Output==0){
          Motor_Stop();
          }
}

//Main----------------------------------
int main(void){

  Clock_Init48MHz();
  CollisionFlag = 0;
  BumpInt_Init(&HandleCollision);
  Motor_Init();  // initialize DC motors
  LaunchPad_Init();
  TimerA1_Init(&Task,50); //100Hz
  Spt = Center;
  EnableInterrupts();

    while(1){

        Input = Reflectance_Center(1000); // read sensors

    }
}


