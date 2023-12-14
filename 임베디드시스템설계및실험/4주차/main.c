#include "stm32f10x.h"

#define RCC (*(volatile unsigned int *)0x40021018)

#define RELAY_1 (*(volatile unsigned int *)0x40010800)    // PA1
#define RELAY_2 (*(volatile unsigned int *)0x40010800)    // PA2

#define KEY1 (*(volatile unsigned int *)0x40011000) 
#define KEY2 (*(volatile unsigned int *)0x40010c04)
#define KEY3 (*(volatile unsigned int *)0x40011004)
#define KEY4 (*(volatile unsigned int *)0x40010800)

#define INPUT_KEY1 (*(volatile unsigned int *)0x40011008)
#define INPUT_KEY2 (*(volatile unsigned int *)0x40010c08)
#define INPUT_KEY3 (*(volatile unsigned int *)0x40011008)
#define INPUT_KEY4 (*(volatile unsigned int *)0x40010808)

#define OUTPUT_RELAY_1 (*(volatile unsigned int *)0x40010810)
#define OUTPUT_RELAY_2 (*(volatile unsigned int *)0x40010810)

void delay() {
  int i;
  for (i=0; i<10000000; i++);
}

void motor_forward() {
  OUTPUT_RELAY_1 |= 0x00020000;
}

void motor_reverse() {
  OUTPUT_RELAY_2 |= 0x00040000;
}

void motor_stop() {
  OUTPUT_RELAY_1 |= 0x00000002;
  OUTPUT_RELAY_2 |= 0x00000004;
}

void motor_fr() {
  motor_forward();
  delay();
  motor_stop();
  motor_reverse();
  delay();  
  motor_stop();
}

int main() {
  // 1. Clock Enable
  RCC |= 0x1C; // 0001 1100 (Port A, B, C)
  
  // 2. Initializing
  KEY1 &= 0x00000000;
  KEY2 &= 0x00000000;
  KEY3 &= 0x00000000;
  KEY4 &= 0x00000000;
  RELAY_1 &= 0x00000000;
  RELAY_2 &= 0x00000000;
  OUTPUT_RELAY_1 &= 0x00000000;
  OUTPUT_RELAY_2 &= 0x00000000;
  
  // 3. Mode Setting
    // 3-1. INPUT MODE
  KEY1 |= 0x00080000;
  KEY2 |= 0x00000800;
  KEY3 |= 0x00800000;
  KEY4 |= 0x00000008;
    // 3-2. OUTPUT MODE
  RELAY_1 |= 0x00000030;
  RELAY_2 |= 0x00000300;
   
  motor_stop();
  // 4. while문 구현
  while(1) {
    if(!(INPUT_KEY1 & 0x0010)) {
      motor_stop();
      motor_forward();
    } 
    if (!(INPUT_KEY2 & 0x0400)) {
      motor_stop();
      motor_reverse();
    }
    if (!(INPUT_KEY3 & 0x2000)) {
      motor_stop();
    }
    if (!(INPUT_KEY4 & 0x0001)) {
      motor_stop();
      motor_fr();
    }
  }
  
  return 0;
}