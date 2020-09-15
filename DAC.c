#include "stm32l4xx.h"
#include "stm32l4xx_ll_dac.h"
#include "stm32l4xx_ll_bus.h"
#include "stm32l4xx_ll_gpio.h"

// Function to initialise DAC conversion to output values read from ADC
void DAC_Init()
{	
	LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_DAC1);	// Enable the Clock for the APB Peripheral bus
	
	LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_4, LL_GPIO_MODE_OUTPUT);	// Set GPIOA Pin 4 to output
	LL_GPIO_SetPinOutputType(GPIOA, LL_GPIO_PIN_4, LL_GPIO_OUTPUT_PUSHPULL);	// Set PA4 to Push Pull
	LL_GPIO_SetPinSpeed(GPIOA, LL_GPIO_PIN_4, LL_GPIO_SPEED_FREQ_VERY_HIGH);	// Set PA4 to Very High speed
	
	LL_DAC_ConvertData12RightAligned(DAC1, LL_DAC_CHANNEL_1, 0x000);	// Reset the DAC Data Register before enabling
	LL_DAC_Enable(DAC1, LL_DAC_CHANNEL_1);	// Enable DAC1
}
