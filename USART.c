#include "stm32l4xx.h"
#include "stm32l4xx_ll_bus.h"
#include "stm32l4xx_ll_gpio.h"
#include "stm32l4xx_ll_usart.h"
#include "BSP476.h"

#define USARTx USART2
#define USART_Tx LL_GPIO_PIN_2

void USART_Init()
{	
  LL_USART_InitTypeDef USART_InitStruct;
  LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOA);
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_USART2);
	
	LL_GPIO_InitTypeDef  GPIO_InitStruct;
  GPIO_InitStruct.Pin = USART_Tx;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = LL_GPIO_AF_7;
	
  LL_GPIO_Init(GPIOA, &GPIO_InitStruct);
	
  USART_InitStruct.BaudRate = 4800;
  USART_InitStruct.DataWidth = LL_USART_DATAWIDTH_8B;
  USART_InitStruct.StopBits = LL_USART_STOPBITS_1;
  USART_InitStruct.Parity = LL_USART_PARITY_NONE;
  USART_InitStruct.TransferDirection = LL_USART_DIRECTION_TX;
  USART_InitStruct.HardwareFlowControl = LL_USART_HWCONTROL_NONE;
  USART_InitStruct.OverSampling = LL_USART_OVERSAMPLING_16;
	
  LL_USART_Init(USARTx, &USART_InitStruct);
  LL_USART_Enable(USARTx);
}

void send_char(uint8_t ch)
{  
  while (!LL_USART_IsActiveFlag_TXE(USARTx)); 
  LL_USART_TransmitData8(USARTx, ch);
}
