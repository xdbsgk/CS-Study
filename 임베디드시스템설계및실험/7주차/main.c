#include "stm32f10x.h"
#include "stm32f10x_exti.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_usart.h"
#include "stm32f10x_rcc.h"

#include "misc.h"

/* function prototype */
void RCC_Configure(void);
void GPIO_Configure(void);
void EXTI_Configure(void);
void USART1_Init(void);
void NVIC_Configure(void);

void EXTI15_10_IRQHandler(void);

void Delay(void);

void sendDataUART1(uint16_t data);

void LED_On();

int dir = 0;
int ledNumber = 3;

//---------------------------------------------------------------------------------------------------

void RCC_Configure(void)
{
	// TODO: Enable the APB2 peripheral clock using the function 'RCC_APB2PeriphClockCmd'
	
	/* UART TX/RX port clock enable */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
        
	/* Button KEY1 KEY2 KEY3 port clock enable */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
        
	/* LED port clock enable */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);
	
	/* USART1 clock enable */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
		
	/* Alternate Function IO clock enable */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
        
}

void GPIO_Configure(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

	// TODO: Initialize the GPIO pins using the structure 'GPIO_InitTypeDef' and the function 'GPIO_Init'
	
    /* Button KEY1 KEY2 KEY3 pin setting */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_13; //KEY1(PC4), KEY3(PC13)
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; //KEY2(PB10)
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    
    /* LED pin setting*/
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOD, &GPIO_InitStructure);
	
    /* UART pin setting */
    //TX
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    //RX
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
}

void EXTI_Configure(void)
{
    EXTI_InitTypeDef EXTI_InitStructure;

	// TODO: Select the GPIO pin (Joystick, button) used as EXTI Line using function 'GPIO_EXTILineConfig'
	// TODO: Initialize the EXTI using the structure 'EXTI_InitTypeDef' and the function 'EXTI_Init'
	
    /* Button KEY1 */
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource4);
    EXTI_InitStructure.EXTI_Line = EXTI_Line4;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);

    /* Button KEY2 */
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource10);
    EXTI_InitStructure.EXTI_Line = EXTI_Line10;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);
	
    /* Button KEY3 */
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource13);
    EXTI_InitStructure.EXTI_Line = EXTI_Line13;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);

	// NOTE: do not select the UART GPIO pin used as EXTI Line here
}

void USART1_Init(void)
{
	USART_InitTypeDef USART1_InitStructure;

	// Enable the USART1 peripheral
	USART_Cmd(USART1, ENABLE);
	
	// TODO: Initialize the USART using the structure 'USART_InitTypeDef' and the function 'USART_Init'
        USART1_InitStructure.USART_BaudRate = 9600;
        USART1_InitStructure.USART_WordLength = USART_WordLength_8b;
        USART1_InitStructure.USART_StopBits = USART_StopBits_1;
        USART1_InitStructure.USART_Parity = USART_Parity_No;
        USART1_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
        USART1_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
        USART_Init(USART1, &USART1_InitStructure);	
	
	// TODO: Enable the USART1 RX interrupts using the function 'USART_ITConfig' and the argument value 'Receive Data register not empty interrupt'
        USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);

}

void NVIC_Configure(void) {

    NVIC_InitTypeDef NVIC_InitStructure;
    
    // TODO: fill the arg you want
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);

    // TODO: Initialize the NVIC using the structure 'NVIC_InitTypeDef' and the function 'NVIC_Init'
	
    // Button KEY1
    NVIC_InitStructure.NVIC_IRQChannel = EXTI4_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0xF;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    // Button KEY2
    NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0xF;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    // Button KEY3
    NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0xF;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    // UART1
	// 'NVIC_EnableIRQ' is only required for USART setting
    NVIC_EnableIRQ(USART1_IRQn);
    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0xF;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

void USART1_IRQHandler() {
	uint16_t word;
    if(USART_GetITStatus(USART1,USART_IT_RXNE)!=RESET){
    	// the most recent received data by the USART1 peripheral
        word = USART_ReceiveData(USART1);

        // TODO implement
        if (word == 'a') {
          dir = 0;
        }
        else if (word == 'b') {
          dir = 1;
        }

        // clear 'Read data register not empty' flag
    	USART_ClearITPendingBit(USART1,USART_IT_RXNE);
    }
}

void EXTI15_10_IRQHandler(void) { // when the button is pressed

    if (EXTI_GetITStatus(EXTI_Line10) != RESET) {
		if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_10) == Bit_RESET) { //KEY 2
                  dir = 1;
		}
		EXTI_ClearITPendingBit(EXTI_Line10);
	}
    if (EXTI_GetITStatus(EXTI_Line13) != RESET) {
		if (GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_13) == Bit_RESET) { //KEY 3
                  char teamName[] = "Team08\r\n";
                  for (int i=0; i<8; i++) {
                    sendDataUART1(teamName[i]);
                  }
		}
        EXTI_ClearITPendingBit(EXTI_Line13);
	}
}

// TODO: Create Joystick interrupt handler functions
// TODO: KEY 1 interrupt handler functions
void EXTI4_IRQHandler(void) {
      if (EXTI_GetITStatus(EXTI_Line4) != RESET) {
		if (GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_4) == Bit_RESET) { //KEY 1
                  dir = 0;
		}
		EXTI_ClearITPendingBit(EXTI_Line4);
	}
}


void Delay(void) {
	int i;

	for (i = 0; i < 2000000; i++) {}
}

void sendDataUART1(uint16_t data) {
	/* Wait till TC is set */
	while ((USART1->SR & USART_SR_TC) == 0);
	USART_SendData(USART1, data);
}

void LED_On() {
  GPIO_SetBits(GPIOD, GPIO_Pin_2);
  GPIO_SetBits(GPIOD, GPIO_Pin_3);
  GPIO_SetBits(GPIOD, GPIO_Pin_4);
  GPIO_SetBits(GPIOD, GPIO_Pin_7);
  
  switch(ledNumber) {
  case 0:
    GPIO_ResetBits(GPIOD, GPIO_Pin_2);
    break;
  case 1:
    GPIO_ResetBits(GPIOD, GPIO_Pin_3);
    break;
  case 2:
    GPIO_ResetBits(GPIOD, GPIO_Pin_4);
    break;
  case 3:
    GPIO_ResetBits(GPIOD, GPIO_Pin_7);
    break;
  }
}

int main(void)
{

    SystemInit();

    RCC_Configure();

    GPIO_Configure();

    EXTI_Configure();

    USART1_Init();

    NVIC_Configure();

    while (1) {
    	// TODO: implement 
      if (dir == 0) {
        ledNumber = (ledNumber + 1) % 4;
        LED_On();
      }
      else {
        ledNumber = (ledNumber + 3) % 4;
        LED_On();
      }
      // Delay
      Delay();
    }
    return 0;
}
