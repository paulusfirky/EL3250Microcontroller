#include "stm32l476xx.h"
#include "BSP476.h"
#include "stm32l4xx_ll_bus.h"
#include "stm32l4xx_ll_gpio.h"

// *** Function Prototypes ***
void BSP476_Init(void);
void LED_Toggle(uint8_t led);
void LED_On(uint8_t led);
void LED_Off(uint8_t led);
uint8_t Switch_Read(uint8_t sw);
uint32_t ms_time;

// Pre-defined function that is called at each 'Tick' of SysTick timer
void SysTick_Handler(void)
{
	if (ms_time != 0x00) ms_time--;	// Decrease the value of ms_time by one at each Tick (1ms per tick as defined in SysTick_Init)
}

// Set up the LEDs as outputs and the Switches as inputs
void BSP476_Init()
{
	//Enable the required peripheral clocks
	LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOA);		// Enable the clock for GPIOA to use LEDs and Switches (all peripherals are on GPIOA)
	LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_SYSCFG);		// Enable the SysCfg clock to manage the required GPIO and allow use
	
	
	LL_GPIO_InitTypeDef GPIO_InitStructLED;		// Define a name for the initialisation struct for LEDS
	LL_GPIO_StructInit(&GPIO_InitStructLED); // Set to Default Values before configuration
	GPIO_InitStructLED.Pin = (LL_GPIO_PIN_5 | LL_GPIO_PIN_6 | LL_GPIO_PIN_7 | LL_GPIO_PIN_8);		// Use pins 5-8 of GPIOA
	GPIO_InitStructLED.OutputType = LL_GPIO_OUTPUT_PUSHPULL;		// Set each pin to Push Pull output
	GPIO_InitStructLED.Mode = LL_GPIO_MODE_OUTPUT;		// Set each pin to be output
	GPIO_InitStructLED.Speed = LL_GPIO_SPEED_FREQ_VERY_HIGH;		// Set the output speed to very high
	LL_GPIO_Init(GPIOA, &GPIO_InitStructLED);		// Initialise the specified GPIO using the struct
	
	LL_GPIO_InitTypeDef GPIO_InitStructSW;		// Define a name for the initialisation struct for Switches
	LL_GPIO_StructInit(&GPIO_InitStructSW); // Set to Default Values before configuration	
	GPIO_InitStructSW.Pin = (LL_GPIO_PIN_1 | LL_GPIO_PIN_9 | LL_GPIO_PIN_10);		// Use pins 1, 9 and 10
	GPIO_InitStructSW.Pull = LL_GPIO_PULL_UP;		// Set pins to pull up (required for switches)
	GPIO_InitStructSW.Mode = LL_GPIO_MODE_INPUT;	// Set the pins to input
	LL_GPIO_Init(GPIOA, &GPIO_InitStructSW);	// Initialise the specified GPIO using the struct
}

// Function to toggle a specified LED
void LED_Toggle(uint8_t led)
{
	// For each LED, check the output data register of GPIOA against the current value of that LED in the data register, and use XOR on them (causing a toggle)
	if(led==0) GPIOA->ODR ^= LED0;
	if(led==1) GPIOA->ODR ^= LED1;
	if(led==2) GPIOA->ODR ^= LED2;
	if(led==3) GPIOA->ODR ^= LED3;
}

// Function to turn on a specified LED
void LED_On(uint8_t led)
{
	// For each LED, check the output data register of GPIOA and OR the value of the LED pin with the current state of the specified LED
	if(led==0) GPIOA->ODR |= LED0;
	if(led==1) GPIOA->ODR |= LED1;
	if(led==2) GPIOA->ODR |= LED2;
	if(led==3) GPIOA->ODR |= LED3;
}

// Function to turn off a specified LED
void LED_Off(uint8_t led)
{	
	// For each LED, check the output data register of GPIOA and use AND EQUALS NOT of the current state of the LED pin
	if(led==0) GPIOA->ODR &= ~(LED0);
	if(led==1) GPIOA->ODR &= ~(LED1);
	if(led==2) GPIOA->ODR &= ~(LED2);
	if(led==3) GPIOA->ODR &= ~(LED3);
}

// Function to read the state of each switch and return its state (1 or 0)
uint8_t Switch_Read(uint8_t sw)
{
	uint8_t switchRead = 0;	// Default switchRead to 0
	
	// Check if the value of the specified switch in the Input Data Register is the same as current value in the IDR, if not, switch is pressed
	if((sw==0) && !(GPIOA->IDR & SW0)) switchRead = 1;
	if((sw==1) && !(GPIOA->IDR & SW1)) switchRead = 1;
	if((sw==2) && !(GPIOA->IDR & SW2)) switchRead = 1;
	
	return switchRead;
}

// Function that will create a delay for the specified number of milliseconds
void Delay_ms(uint32_t time)
{
	ms_time = time;	// Set the value of ms_time to the specified number of milliseconds
	while(ms_time != 0);	// Empty while loop that waits until the value of ms_time reaches 0
}
