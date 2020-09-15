#include "stm32l476xx.h"
#include "stdio.h"
#include "math.h"
#include "USART.h"
#include "ADC.h"
#include "BSP476.h"
#include "DAC.h"
#include "stm32l4xx_ll_adc.h"
#include "stm32l4xx_ll_dac.h"

/* *** Declaring Variables  ***

		int logic - Used to obtain values to display in the logic analyser
		int length - Stores the size of the array required for printing to Terminal
		int freqCount - Increments each time a value goes above or below the average value
		int freq - Stores the calculated frequency from the frequency function
		*/
int logic, length, freqCount, freq;

/*  float value - Used to store the actual voltage level calculated from ADC readings
		float avgVoltage, minVoltage, maxVoltage, rmsVoltage - Store the values calculated from their respective functions
*/

float value, avgVoltage, minVoltage, maxVoltage, rmsVoltage;


float adcValues[1000]; // Array to store 1000 samples of the waveform from the Frequency Generator

// Function signatures
void SysTick_Init(void);  // Initialises the SysTick Timer, which is set to 1ms for the Delay_ms function
void Delay_ms(uint32_t time);  // Used to create a delay in ms, specified by the time variable

void takeReadings(void);	// Function that takes 1000 samples over 1 second of the signal from the Frequency Generator, calculated from the ADC
void checkVoltage(void);	// Function to continually check the voltage to see if it rises above 3.1V even if readings are not being taken
float averageVoltage(void);	// Calculates the average voltage and returns it as a float
float minimumVoltage(void);	// Calculates minimum voltage of the signal and returns it as a float
float maximumVoltage(void);	// Calculates maximum voltage of the signal and returns it as a float
float rmsValue(void);	// Calculates RMS value of the signal and returns it as a float
int frequency(float average);	// Calculates the frequency and returns it as an integer

int main(void)
{
	SysTick_Init();	// Initialise the SysTick Timer
	BSP476_Init();	// Initialise the peripherals required on the shield
	ADC_Init();	// Initialise the ADC for signal conversion
	DAC_Init();	// Initialise the DAC for outputting the converted signal
	USART_Init();	// Initialise the USART port for sending data to Terminal
	
	int buttonPushed = 0;  //Flag to see if the button has been pressed to avoid multiple presses (1 if pressed, 0 otherwise)
	char message[400]; //Arbitrarily large array to avoid overflow when setting using sprintf()
	
	while(1)
	{
		checkVoltage();  // Checks to see if the voltage is above 3.1V and handle accordingly
		Delay_ms(1);  // Delay of 1ms after voltage is checked
		
		// Check to see if the button has been pressed and that the buttonPushed flag is 0 (not pressed)
		if (Switch_Read(0) && !buttonPushed)
			{
				buttonPushed = 1;  // Turn on the buttonPushed flag to avoid multiple pushes
				takeReadings();	// Take readings from the ADC
				
			  avgVoltage = averageVoltage();  // Calculate the average voltage and store in the avgVoltage variable
        minVoltage = minimumVoltage();	// Calculate the minimum voltage and store in the minVotlage variable
        maxVoltage = maximumVoltage();	// Calculate the maximum voltage and store in the maxVoltage variable
        rmsVoltage = rmsValue();				// Calculate the RMS voltage and store in the rmsVoltage variable
				freq = frequency(avgVoltage);		// Calculate the frequency and store in the freq variable
				
				// sprintf() sends formatted output into the char message array so that it can be sent to Terminal using the send_char function
				sprintf(message, "Average Voltage: %3.2f\r\nMinimum Voltage: %3.2f\r\nMaximum Voltage: %3.2f\r\nRMS Voltage: %3.2f\r\nFrequency: %d\r\n\r\n", avgVoltage, minVoltage, maxVoltage, rmsVoltage, freq);
				
				length = strlen(message);// strlen returns the size of the message array, which is used for the loop to send data to Terminal
				
				// For loop which will send each character in the message buffer via send_char
				for(int i=0; i<length; i++)
				{
					send_char(message[i]);	// Send the current character to Terminal
					Delay_ms(5); 						// Delay of 5ms between sending data
				}				
				
				buttonPushed = 0;  //Reset buttonPushed flag for next reading
			}
	 }
}

// Function to take 1000 samples of the signal received from the Function Generator over 1 second via ADC
void takeReadings(void)
{
	// Loop 1000 times to take readings
	for(int i=0; i<1000; i++)
	{		  
		LL_ADC_REG_StartConversion(ADC1);				// Start converting the reading taken from ADC
		
		while(!LL_ADC_IsActiveFlag_EOC(ADC1));	// Checks to see if the Active Flag for ADC1 is on. Waits until the flag is lowered before continuing
		
		LL_DAC_ConvertData12RightAligned(DAC1, LL_DAC_CHANNEL_1, LL_ADC_REG_ReadConversionData12(ADC1));  // Convert data read from ADC1 for DAC output
		
		value = LL_ADC_REG_ReadConversionData12(ADC1);  // Stores the converted value from ADC1 into value variable (between 0x000 and 0xFFF)
		logic = value; 																	// Variable for displaying readings via Logic Analyzer
		value = value / (4095/3.333333f);								// Converts the value read into a voltage level
		
		adcValues[i] = value;														// Store the converted value into the adcValues array at the 'i'th position
		
		LL_ADC_REG_StopConversion(ADC1);								// Stop converting data on ADC1
		
		// If statement to check whether the voltage was above 3.1V and if so, light up all LEDs
		if(value >= 3.10f)
			{	
				// Light all LEDs as this makes it easier to see
				LED_On(0);
				LED_On(1);
				LED_On(2);
				LED_On(3);
			}
			// If the voltage drops below 3.1V, turn off all LEDs
			else if(value < 3.10f)
			{
				LED_Off(0);
				LED_Off(1);
				LED_Off(2);
				LED_Off(3);
			}
		
		Delay_ms(1);  // Delay of 1ms between readings to get 1000 readings over 1 second
	}	
}

// Function that adds up all values in the adcValues array and divides by the total number of readings to obtain the average
float averageVoltage(void)
{
	float average;	// Float in which to store the average value for return
	
	// Loop 1000 times to add all values
	for(int i=0; i<1000; i++)
	{
		average = average + adcValues[i];	// Add the current value to the running total
	}
	
	average = average/1000;							// Divide total by 1000 to obtain average
	
	return average;											// Return the calculated average
}

// Function that checks each value in the adcValues array against the current lowest, and stores it in minVal if it is lower
float minimumVoltage(void)
{
	float minVal = 1000; // Arbitrarily large value above the highest possible voltage in adcValues
	
	// Loop 1000 times to check all values in the adcValues array
	for(int i=0; i<1000; i++)
	{
		// Check to see if the 'i'th values in adcValues is lower than the current minimum value
		if (adcValues[i] < minVal) 
			minVal = adcValues[i]; // If current value in adcValues is lower, store it in minVal
	}
	
	return minVal;  // Return the minimum value of adcValues
}

// Function that checks each value in the adcValues array aginst the current highest, and stores it in maxVal if it is higher
float maximumVoltage(void)
{
	float maxVal = 0; //Lowest possible value for voltage read from ADC
	
	// Loop 1000 times to check all values in the adcValues array
	for(int i=0; i<1000; i++)
	{
		// Check to see if the 'i'th value in adcValues is higher than the current maximum value
		if (adcValues[i] > maxVal) 
		  maxVal = adcValues[i];	// If current value in adcValues is lower, store it in maxVal
	}
	
	return maxVal;	// Return the maximum value of adcValues
}

// Function that calculates the RMS voltage of the acquired signal
float rmsValue(void)
{
	double total;	// Stores the total of all squared values
	double mean;	// Stores the mean of all squared values
	float rms;		// Stores the root of the mean of squared values
	
	// Loop 1000 times to get the total of all squared values
	for(int i=0; i<1000; i++)
	{
		total = total + (adcValues[i] * adcValues[i]);  // Add the square of the current value in adcValues to the running total
	}
	
	mean = total / 1000;	// Divides the calculated total by 1000 to obtain the mean
	rms = sqrt(mean);			// Obtains the root of the mean
	
	return rms;						// Returns the calculated RMS value of the signal
}
	// Function that checks whether the current value is higher or lower than the average value and if so, increase the freqCount variable
int frequency(float average)
{
	freqCount = 0;	// Reset the freqCount variable to 0 for a new count
	int flag = 1;		// Set the flag to 1 (ON) to begin checking if the value is higher than the average voltage
	
	// Loop 1000 times to check all values in adcValues
	for (int i=0; i<1000; i++)
	{
		// If the value in adcValues is higher than the average and the flag is ON, add 1 to freqCount and turn the flag OFF
		if((adcValues[i] > average)&& flag==1)
			{ 
				freqCount++;	// Increase freqCount by 1
				flag = 0;			// Set the flag to 0 for checking when the voltage drops below the average voltage	
			}
		// Else if the voltage has dropped below the average and the flag is OFF, increase the frequency count
		else if((adcValues[i] < average) && flag==0)
			{
				freqCount++;	// Increase freqCount by 1
				flag = 1;			// Turn the flag on
			}
	}	
	
	freqCount = freqCount/2;  // Once count has completed, divide the frequency count by 2 to obtain the frequency of the signal
	
	return freqCount;					// Return the calculated frequency
}

// Function to check if the voltage read by the ADC is over 3.1V whilst readings are not being taken
void checkVoltage(void)
{	
	LL_ADC_REG_StartConversion(ADC1);				// Start converting the reading taken from ADC
			
	while(!LL_ADC_IsActiveFlag_EOC(ADC1));	// Checks to see if the Active Flag for ADC1 is on. Waits until the flag is lowered before continuing
	
	LL_DAC_ConvertData12RightAligned(DAC1, LL_DAC_CHANNEL_1, LL_ADC_REG_ReadConversionData12(ADC1));	// Convert data read from ADC1 for DAC output
	
	value = LL_ADC_REG_ReadConversionData12(ADC1);	// Stores the converted value from ADC1 into value variable (between 0x000 and 0xFFF)
	value = value / (4095/3.333333f);								// Converts the value read into a voltage level
	
	LL_ADC_REG_StopConversion(ADC1);								// Stop converting data on ADC1
	
	// If statement to check whether the voltage was above 3.1V and if so, light up all LEDs
	if(value >= 3.10f)
	{	
		// Light all LEDs as this makes it easier to see
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

// Function to initialise the SysTick at 1ms for the Delay_ms function
void SysTick_Init()
{
	SysTick->LOAD = (SystemCoreClock / 1000) - 1; // Divide system core clock by 1000 to get 1 millisecond
	
	// Setup for the SysTick
	SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk			// Enable the SysTick
								| SysTick_CTRL_TICKINT_Msk			// Enable interrupt for SysTick
								| SysTick_CTRL_CLKSOURCE_Msk;		// Set SysTick clock source to hardware clock
}
