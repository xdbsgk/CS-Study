#include "stm32f10x.h"
#include "core_cm3.h"
#include "misc.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_usart.h"
#include "stm32f10x_adc.h"
#include "stm32f10x_tim.h"
#include "lcd.h"
#include "touch.h"

int color[12] = {WHITE,CYAN,BLUE,RED,MAGENTA,LGRAY,GREEN,YELLOW,BROWN,BRRED,GRAY};
uint16_t x, y;

int toggle = 0;
int pulse = 1500;
int ledCount = 0;
 
void RCC_Configure(){
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); //서보모터 
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE); //LED
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); //TIM2
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); //TIM3
}

void GPIO_Configure(){
  GPIO_InitTypeDef GPIO_InitStructure;
  
  GPIO_InitStructure.GPIO_Pin = (GPIO_Pin_2 | GPIO_Pin_3);
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOD, &GPIO_InitStructure);
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
}

void NVIC_Configure(){
  NVIC_InitTypeDef NVIC_InitStructure;

  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
  NVIC_EnableIRQ(TIM2_IRQn);

  NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0xf;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0;
  NVIC_Init(&NVIC_InitStructure);
}

void TIM_Configure() {
  //TIM3
  int prescale = (uint16_t) (SystemCoreClock / 1000000);
  TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
  TIM_OCInitTypeDef TIM_OCInitStructure;

  TIM_TimeBaseStructure.TIM_Period = 20000;         
  TIM_TimeBaseStructure.TIM_Prescaler = prescale;
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Down;

  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_Pulse = 1000; // us
  TIM_OC3Init(TIM3, &TIM_OCInitStructure);

  TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
  TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Disable);
  TIM_ARRPreloadConfig(TIM3, ENABLE);
  TIM_Cmd(TIM3, ENABLE);
  
  //TIM2
  prescale = (uint16_t) (SystemCoreClock / 10000);

  TIM_TimeBaseStructure.TIM_Period = 10000;         
  TIM_TimeBaseStructure.TIM_Prescaler = prescale;
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  
  TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
  TIM_ARRPreloadConfig(TIM2, ENABLE);
  TIM_Cmd(TIM2, ENABLE);
  TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
}

void changePulse() {
  TIM_OCInitTypeDef TIM_OCInitStructure;
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_Pulse = pulse;
  TIM_OC3Init(TIM3, &TIM_OCInitStructure);
}

void TIM2_IRQHandler() {
  if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET) {
    changePulse();
    
    if (toggle == 1) {
      if (ledCount%2 == 0)
        GPIO_SetBits(GPIOD, GPIO_Pin_2);
      else
        GPIO_ResetBits(GPIOD, GPIO_Pin_2);
      if (ledCount == 0)
        GPIO_SetBits(GPIOD, GPIO_Pin_3);
      else if (ledCount == 5)
        GPIO_ResetBits(GPIOD, GPIO_Pin_3);
      
      ledCount++;
      ledCount %= 10;
      
      pulse += 100;
      if (pulse > 2000)
        pulse = 1000;
    }
    
    else {
      GPIO_SetBits(GPIOD, GPIO_Pin_2);
      GPIO_SetBits(GPIOD, GPIO_Pin_3);
      
      pulse -= 100;
      if (pulse < 1000)
        pulse = 2000;
    }
    
    TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
  }
}

int main() {
  SystemInit();
  RCC_Configure();
  GPIO_Configure();
  NVIC_Configure();
  TIM_Configure();
  
  LCD_Init();
  Touch_Configuration();
  Touch_Adjust();
  LCD_Clear(WHITE);
  
  LCD_ShowString(20, 50, "WED_TEAM08", BLUE, WHITE);
  LCD_ShowString(20, 70, "OFF", RED, WHITE);
  LCD_DrawRectangle(40, 100, 90, 150);
  LCD_ShowString(40, 115, "BUTTON", RED, WHITE);
  
  while(1) {
    Touch_GetXY(&x, &y, 1);
    Convert_Pos(x, y, &x, &y);
    
    if ((x>40)&&(x<90)&&(y>100)&&(y<150)) {
      toggle = 1 - toggle;
      if (toggle == 1) {
        LCD_ShowString(20, 70, "OFF", WHITE, WHITE);
        LCD_ShowString(20, 70, "ON", RED, WHITE);
      } 
      else {
        LCD_ShowString(20, 70, "ON", WHITE, WHITE);
        LCD_ShowString(20, 70, "OFF", RED, WHITE);
      }
    }
     
  }
  
  return 0;
}
