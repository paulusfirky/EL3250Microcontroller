#include "stm32l4xx.h"
#include "stm32l4xx_ll_bus.h"
#include "stm32l4xx_ll_gpio.h"
#include "stm32l4xx_ll_usart.h"
#include "BSP476.h"

#define USARTx USART2						//Use USART2
#define USART_Tx LL_GPIO_PIN_2  				//Tx on PA2 (for USB output) 

// Function to setup and initialise USART (required to send data over USB to Terminal
void USART_Init()
{	
  LL_USART_InitTypeDef USART_InitStruct;			// Declare a struct for the USART initialisation
  LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOA);	// Enable GPIOA clock
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_USART2); 	// Enable USART2 clock
	
	LL_GPIO_InitTypeDef  GPIO_InitStruct;	// Declare a Struct for configuring the GPIO
  GPIO_InitStruct.Pin = USART_Tx;		// Set the pin to PA2 (from #define)
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;		// Set to Push Pull output
  GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;		// Use the Alternate Function of the GPIO pin
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_VERY_HIGH;  // Set the speed to very high
  GPIO_InitStruct.Alternate = LL_GPIO_AF_7;		// Select Alternate Function 7
	
  LL_GPIO_Init(GPIOA, &GPIO_InitStruct);			//Initialise the GPIO using the struct
	
  USART_InitStruct.BaudRate = 4800;	// Set the baud rate of USART to 4800
  USART_InitStruct.DataWidth = LL_USART_DATAWIDTH_8B;	// Set the data width of the message to 8 bits
  USART_InitStruct.StopBits = LL_USART_STOPBITS_1;	// Use one stop bit for the data frame
  USART_InitStruct.Parity = LL_USART_PARITY_NONE;		// No parity bit for the data frame
  USART_InitStruct.TransferDirection = LL_USART_DIRECTION_TX;		// Set the pin to TX (transmit)
  USART_InitStruct.HardwareFlowControl = LL_USART_HWCONTROL_NONE;		// Disable Hardware Control for the USART
  USART_InitStruct.OverSampling = LL_USART_OVERSAMPLING_16;		// Set Oversampling to 16
	
  LL_USART_Init(USARTx, &USART_InitStruct);		// Initialise USART2 using the struct
  LL_USART_Enable(USARTx);					//Enable USART
}

void send_char(uint8_t ch)
{  
  while (!LL_USART_IsActiveFlag_TXE(USARTx)); 	// Wait for TXE flag to be raised
  LL_USART_TransmitData8(USARTx, ch); 					// Transmit the data frame via USART2
}
