#include "stm32f10x.h"
#include "core_cm3.h"
#include "misc.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_usart.h"
#include "stm32f10x_adc.h"
#include "stm32f10x_dma.h"
#include "lcd.h"
#include "touch.h"

int color[12] = {WHITE,CYAN,BLUE,RED,MAGENTA,LGRAY,GREEN,YELLOW,BROWN,BRRED,GRAY};
volatile uint32_t ADC_value[1];

void RCC_Configure(){
RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
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
ADC_DMACmd(ADC1, ENABLE);
ADC_Cmd(ADC1, ENABLE);

ADC_ResetCalibration(ADC1);
while(ADC_GetResetCalibrationStatus(ADC1));
ADC_StartCalibration(ADC1);
while(ADC_GetCalibrationStatus(ADC1));
ADC_SoftwareStartConvCmd(ADC1, ENABLE);
}

void DMA_Configure() {
DMA_InitTypeDef DMA_InitStructure;

DMA_DeInit(DMA1_Channel1);
DMA_InitStructure.DMA_BufferSize = 1;
DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)&ADC_value[0];
DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Word;
DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Disable;
DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&(ADC1->DR);
DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Word;
DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;

DMA_Init(DMA1_Channel1, &DMA_InitStructure);
DMA_Cmd(DMA1_Channel1, ENABLE);
}

void delay() {
for (int i=0; i<1000000; i++);
}

int main() {
SystemInit();
RCC_Configure();
GPIO_Configure();
ADC_Configure();
DMA_Configure();
// ------------------------------------
LCD_Init();
Touch_Configuration();
Touch_Adjust();
LCD_Clear(WHITE);

LCD_ShowNum(50, 20, (uint32_t)ADC_value[0], 5, BLACK, WHITE);

while(1){
if ((uint32_t)ADC_value[0] >= 3600) {
LCD_Clear(WHITE);
LCD_ShowNum(50, 20, (uint32_t)ADC_value[0], 5, BLACK, WHITE);
}
else {
LCD_Clear(GRAY);
LCD_ShowNum(50, 20, (uint32_t)ADC_value[0], 5, WHITE, GRAY);
}
delay();
}


}
