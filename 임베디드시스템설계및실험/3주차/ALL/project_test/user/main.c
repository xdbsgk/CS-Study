#include "stm32f10x.h"

#define OUR_RCC (*(volatile unsigned int *)0x40021018)
#define LED (*(volatile unsigned int *)0x40011400)
#define KEY1 (*(volatile unsigned int *)0x40011000)
#define KEY2 (*(volatile unsigned int *)0x40010c04)
#define KEY3 (*(volatile unsigned int *)0x40011004)
#define KEY4 (*(volatile unsigned int *)0x40010800)

#define INPUT_KEY1 (*(volatile unsigned int *)0x40011008)
#define INPUT_KEY2 (*(volatile unsigned int *)0x40010c08)
#define INPUT_KEY3 (*(volatile unsigned int *)0x40011008)
#define INPUT_KEY4 (*(volatile unsigned int *)0x40010808)

#define OUTPUT_LED (*(volatile unsigned int *)0x40011410)

int main(void){
  
  // clock enable
  OUR_RCC |= 0x3C;
  
  //Initializing
  LED &= 0x00000000;
  KEY1 &= 0x00000000;
  KEY2 &= 0x00000000;
  KEY3 &= 0x00000000;
  KEY4 &= 0x00000000;
  OUTPUT_LED &= 0x00000000;  
  
  // mode setting
  LED |= 0x30033300;
  KEY1 |= 0x00080000;
  KEY2 |= 0x00000800;
  KEY3 |= 0x00800000;
  KEY4 |= 0x00000008;
  
  while(1){
    if (!(INPUT_KEY1 & 0x0010)) {
      OUTPUT_LED |= 0x000C0000;
    } 
    else if (!(INPUT_KEY2 & 0x0400)) {
      OUTPUT_LED |= 0x0000000C;
    } 
    if (!(INPUT_KEY3 & 0x2000)) {
      OUTPUT_LED |= 0x00900000;
    } 
    else if (!(INPUT_KEY4 & 0x0001)) {
      OUTPUT_LED |= 0x00000090;
    }
  }
  
  
  return 0;
}