#include "stm32l4xx.h"
#include "stm32l4xx_ll_rcc.h"
#include "stm32l4xx_ll_adc.h"
#include "stm32l4xx_ll_bus.h"
#include "stm32l4xx_ll_gpio.h"

void ADC_Init()
{	
	LL_RCC_SetADCClockSource(LL_RCC_ADC_CLKSOURCE_SYSCLK);	// Set the clock source for ADC to  
	LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_ADC);			// Enable the clock for the AHB bus
	
	// This block sets up PA0 for ADC
	LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_0, LL_GPIO_MODE_ANALOG);	// Set GPIOA Pin 0 to Analog mode
	LL_GPIO_SetPinOutputType(GPIOA, LL_GPIO_PIN_0, LL_GPIO_OUTPUT_PUSHPULL);	// Set the output of PA0 to Push Pull
	LL_GPIO_SetPinSpeed(GPIOA, LL_GPIO_PIN_0, LL_GPIO_SPEED_FREQ_LOW);	// Set the frequency of PA0 to low
	LL_GPIO_EnablePinAnalogControl(GPIOA, LL_GPIO_PIN_0);		// Enable Analog Control for PA0
	
	LL_ADC_SetChannelSamplingTime(ADC1, LL_ADC_CHANNEL_5, LL_ADC_SAMPLINGTIME_12CYCLES_5);	// Set the sampling time of ADC1 to 12.5 cycles
	LL_ADC_DisableDeepPowerDown(ADC1);	// Disable Deep Power Down for ADC1
	LL_ADC_EnableInternalRegulator(ADC1);	// Enable the internal voltage regulator on ADC1
	LL_ADC_REG_SetSequencerRanks(ADC1, LL_ADC_REG_RANK_1, LL_ADC_CHANNEL_5);	// Set ADC1 to Rank 1 on ADC channel 5
	LL_ADC_REG_SetContinuousMode(ADC1, LL_ADC_REG_CONV_SINGLE);		// Set ADC1 to single conversion mode
	LL_ADC_REG_SetSequencerLength(ADC1, LL_ADC_REG_SEQ_SCAN_DISABLE);	// Disable Scan mode for ADC1
	
	LL_ADC_StartCalibration(ADC1, LL_ADC_SINGLE_ENDED);	// Start calibration of ADC1
	while( LL_ADC_IsCalibrationOnGoing(ADC1) ){}			  // Wait until calibration has completed
	
	LL_ADC_Enable(ADC1);																// Enable ADC1
	while( LL_ADC_IsActiveFlag_ADRDY(ADC1) ){} 					// Wait until ADC1 is ready for conversion
}
