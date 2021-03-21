#include "stm32l4xx.h"
#include "stm32l4xx_ll_rcc.h"
#include "stm32l4xx_ll_adc.h"
#include "stm32l4xx_ll_bus.h"
#include "stm32l4xx_ll_gpio.h"

void ADC_Init()
{	
	LL_RCC_SetADCClockSource(LL_RCC_ADC_CLKSOURCE_SYSCLK);
	LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_ADC);
	
	// This block sets up PA0 for ADC
	LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_0, LL_GPIO_MODE_ANALOG);
	LL_GPIO_SetPinOutputType(GPIOA, LL_GPIO_PIN_0, LL_GPIO_OUTPUT_PUSHPULL);
	LL_GPIO_SetPinSpeed(GPIOA, LL_GPIO_PIN_0, LL_GPIO_SPEED_FREQ_LOW);
	LL_GPIO_EnablePinAnalogControl(GPIOA, LL_GPIO_PIN_0);
	
	LL_ADC_SetChannelSamplingTime(ADC1, LL_ADC_CHANNEL_5, LL_ADC_SAMPLINGTIME_12CYCLES_5);
	LL_ADC_DisableDeepPowerDown(ADC1);
	LL_ADC_EnableInternalRegulator(ADC1);
	LL_ADC_REG_SetSequencerRanks(ADC1, LL_ADC_REG_RANK_1, LL_ADC_CHANNEL_5);
	LL_ADC_REG_SetContinuousMode(ADC1, LL_ADC_REG_CONV_SINGLE);
	LL_ADC_REG_SetSequencerLength(ADC1, LL_ADC_REG_SEQ_SCAN_DISABLE);
	
	LL_ADC_StartCalibration(ADC1, LL_ADC_SINGLE_ENDED);
	while( LL_ADC_IsCalibrationOnGoing(ADC1) ){}
	
	LL_ADC_Enable(ADC1);
	while( LL_ADC_IsActiveFlag_ADRDY(ADC1) ){}
}
