#include "stm32l476xx.h"
#include "stdio.h"
#include "math.h"
#include "USART.h"
#include "ADC.h"
#include "BSP476.h"
#include "DAC.h"
#include "stm32l4xx_ll_adc.h"
#include "stm32l4xx_ll_dac.h"

int logic, length, freqCount, freq;
float value, avgVoltage, minVoltage, maxVoltage, rmsVoltage;
float adcValues[1000];

void SysTick_Init(void);
void Delay_ms(uint32_t time);

void takeReadings(void);
void checkVoltage(void);
float averageVoltage(void);
float minimumVoltage(void);
float maximumVoltage(void);
float rmsValue(void);
int frequency(float average);

int main(void)
{
	SysTick_Init();
	BSP476_Init();
	ADC_Init();
	DAC_Init();
	USART_Init();
	
	int buttonPushed = 0;
	char message[400];
	
	while(1)
	{
		checkVoltage();
		Delay_ms(1);
		
		if (Switch_Read(0) && !buttonPushed)
			{
				buttonPushed = 1;
				takeReadings();
				
			  avgVoltage = averageVoltage();
        minVoltage = minimumVoltage();
        maxVoltage = maximumVoltage();
        rmsVoltage = rmsValue();
				freq = frequency(avgVoltage);
				
				
				sprintf(message, "Average Voltage: %3.2f\r\nMinimum Voltage: %3.2f\r\nMaximum Voltage: %3.2f\r\nRMS Voltage: %3.2f\r\nFrequency: %d\r\n\r\n", avgVoltage, minVoltage, maxVoltage, rmsVoltage, freq);
				
				length = strlen(message);
				
				for(int i=0; i<length; i++)
				{
					send_char(message[i]);
					Delay_ms(5);
				}				
				
				buttonPushed = 0;
			}
	 }
}

void takeReadings(void)
{
	for(int i=0; i<1000; i++)
	{		  
		LL_ADC_REG_StartConversion(ADC1);
		
		while(!LL_ADC_IsActiveFlag_EOC(ADC1));
		
		LL_DAC_ConvertData12RightAligned(DAC1, LL_DAC_CHANNEL_1, LL_ADC_REG_ReadConversionData12(ADC1));
		
		value = LL_ADC_REG_ReadConversionData12(ADC1);
		logic = value;
		value = value / (4095/3.333333f);
		
		adcValues[i] = value;
		
		LL_ADC_REG_StopConversion(ADC1);
		
		if(value >= 3.10f)
			{	
				LED_On(0);
				LED_On(1);
				LED_On(2);
				LED_On(3);
			}
			else if(value < 3.10f)
			{
				LED_Off(0);
				LED_Off(1);
				LED_Off(2);
				LED_Off(3);
			}
		
		Delay_ms(1);
	}	
}

float averageVoltage(void)
{
	float average;
	
	for(int i=0; i<1000; i++)
	{
		average = average + adcValues[i];
	}
	
	average = average/1000;
	
	return average;
}

float minimumVoltage(void)
{
	float minVal = 1000;
	
	for(int i=0; i<1000; i++)
	{
		if (adcValues[i] < minVal) 
			minVal = adcValues[i];
	}
	
	return minVal;
}

float maximumVoltage(void)
{
	float maxVal = 0;
	
	for(int i=0; i<1000; i++)
	{
		if (adcValues[i] > maxVal) 
		  maxVal = adcValues[i];
	}
	
	return maxVal;
}

float rmsValue(void)
{
	double total;
	double mean;
	float rms;
	
	for(int i=0; i<1000; i++)
	{
		total = total + (adcValues[i] * adcValues[i]);
	}
	
	mean = total / 1000;
	rms = sqrt(mean);
	
	return rms;
}

int frequency(float average)
{
	freqCount = 0;
	int flag = 1;
	
	for (int i=0; i<1000; i++)
	{
		if((adcValues[i] > average)&& flag==1)
			{ 
				freqCount++;
				flag = 0;
			}
		else if((adcValues[i] < average) && flag==0)
			{
				freqCount++;
				flag = 1;
			}
	}	
	
	freqCount = freqCount/2;
	
	return freqCount;
}

void checkVoltage(void)
{	
	LL_ADC_REG_StartConversion(ADC1);
			
	while(!LL_ADC_IsActiveFlag_EOC(ADC1));
	
	LL_DAC_ConvertData12RightAligned(DAC1, LL_DAC_CHANNEL_1, LL_ADC_REG_ReadConversionData12(ADC1));
	
	value = LL_ADC_REG_ReadConversionData12(ADC1);
	value = value / (4095/3.333333f);
	
	LL_ADC_REG_StopConversion(ADC1);
	
	if(value >= 3.10f)
	{	
		LED_On(0);
		LED_On(1);
		LED_On(2);
		LED_On(3);
	}
	else if(value < 3.10f)
	{
		LED_Off(0);
		LED_Off(1);
		LED_Off(2);
		LED_Off(3);
	}
}

void SysTick_Init()
{
	SysTick->LOAD = (SystemCoreClock / 1000) - 1;
	
	// Setup for the SysTick
	SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk
								| SysTick_CTRL_TICKINT_Msk
								| SysTick_CTRL_CLKSOURCE_Msk;
}
