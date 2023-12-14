#include "stm32f10x.h"
#include "core_cm3.h"
#include "misc.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_usart.h"
#include "stm32f10x_adc.h"
#include "lcd.h"
#include "touch.h"

int color[12] = {WHITE,CYAN,BLUE,RED,MAGENTA,LGRAY,GREEN,YELLOW,BROWN,BRRED,GRAY};
uint16_t value, x, y;

void RCC_Configure(){
   RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
   RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);  
}

void GPIO_Configure(){
   GPIO_InitTypeDef GPIO_InitStructure;
   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN; 
   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
   GPIO_Init(GPIOA, &GPIO_InitStructure);
}

void ADC_Configure(){
  ADC_InitTypeDef ADC_InitStructure;
  
  ADC_DeInit(ADC1);
  ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
  ADC_InitStructure.ADC_ScanConvMode = DISABLE;
  ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
  ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
  ADC_InitStructure.ADC_NbrOfChannel = 1;
  ADC_Init(ADC1, &ADC_InitStructure);
  
  ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 1, ADC_SampleTime_239Cycles5);
  ADC_ITConfig(ADC1, ADC_IT_EOC, ENABLE);
  ADC_Cmd(ADC1, ENABLE);

  ADC_ResetCalibration(ADC1);
  while(ADC_GetResetCalibrationStatus(ADC1)); //->while ����Ͽ� üũ
  ADC_StartCalibration(ADC1);
  while(ADC_GetCalibrationStatus(ADC1)); //->while ����Ͽ� üũ
  ADC_SoftwareStartConvCmd(ADC1, ENABLE);
}

void NVIC_Configure(){
  NVIC_InitTypeDef NVIC_InitStructure;
  
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
  NVIC_EnableIRQ(ADC1_2_IRQn);
  
  NVIC_InitStructure.NVIC_IRQChannel = ADC1_2_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0xf;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0;
  NVIC_Init(&NVIC_InitStructure);
}

void ADC1_2_IRQHandler(){
  if (ADC_GetITStatus(ADC1, ADC_IT_EOC) != RESET) {
    value = ADC_GetConversionValue(ADC1);
  }
  ADC_ClearITPendingBit(ADC1, ADC_IT_EOC);
}

int main() {
  // LCD ���� ������ LCD_Init�� �����Ǿ� �����Ƿ� ���⼭ �� �ʿ� ����
  SystemInit();
  RCC_Configure();
  GPIO_Configure();
  ADC_Configure();
  NVIC_Configure();
  // ------------------------------------
  LCD_Init();
  Touch_Configuration();
  Touch_Adjust();
  LCD_Clear(WHITE);
  
  LCD_ShowString(50, 20, "WED_TEAM08", BLUE, WHITE);
  
  while(1){
    // TODO : LCD �� ��� �� ��ġ ��ǥ �б�
    Touch_GetXY(&x, &y, 1);
    Convert_Pos(x, y, &x, &y);
    if (x!=0 || y!=0) {
      LCD_DrawCircle(x,y,3);
      LCD_ShowNum(100, 120, x, 3, BLACK, WHITE);
      LCD_ShowNum(100, 150, y, 3, BLACK, WHITE);
      LCD_ShowNum(50, 180, value, 4, BLACK, WHITE);
    }
  }


}