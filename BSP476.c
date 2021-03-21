#include "stm32l476xx.h"
#include "BSP476.h"
#include "stm32l4xx_ll_bus.h"
#include "stm32l4xx_ll_gpio.h"

void BSP476_Init(void);
void LED_Toggle(uint8_t led);
void LED_On(uint8_t led);
void LED_Off(uint8_t led);
uint8_t Switch_Read(uint8_t sw);
uint32_t ms_time;

void SysTick_Handler(void)
{
	if (ms_time != 0x00) ms_time--;
}

void BSP476_Init()
{
	LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOA);
	LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_SYSCFG);
	
	
	LL_GPIO_InitTypeDef GPIO_InitStructLED;
	LL_GPIO_StructInit(&GPIO_InitStructLED);
	GPIO_InitStructLED.Pin = (LL_GPIO_PIN_5 | LL_GPIO_PIN_6 | LL_GPIO_PIN_7 | LL_GPIO_PIN_8);
	GPIO_InitStructLED.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
	GPIO_InitStructLED.Mode = LL_GPIO_MODE_OUTPUT;
	GPIO_InitStructLED.Speed = LL_GPIO_SPEED_FREQ_VERY_HIGH;
	LL_GPIO_Init(GPIOA, &GPIO_InitStructLED);
	
	LL_GPIO_InitTypeDef GPIO_InitStructSW;
	LL_GPIO_StructInit(&GPIO_InitStructSW);
	GPIO_InitStructSW.Pin = (LL_GPIO_PIN_1 | LL_GPIO_PIN_9 | LL_GPIO_PIN_10);
	GPIO_InitStructSW.Pull = LL_GPIO_PULL_UP;
	GPIO_InitStructSW.Mode = LL_GPIO_MODE_INPUT;
	LL_GPIO_Init(GPIOA, &GPIO_InitStructSW);
}

void LED_Toggle(uint8_t led)
{
	if(led==0) GPIOA->ODR ^= LED0;
	if(led==1) GPIOA->ODR ^= LED1;
	if(led==2) GPIOA->ODR ^= LED2;
	if(led==3) GPIOA->ODR ^= LED3;
}

void LED_On(uint8_t led)
{
	if(led==0) GPIOA->ODR |= LED0;
	if(led==1) GPIOA->ODR |= LED1;
	if(led==2) GPIOA->ODR |= LED2;
	if(led==3) GPIOA->ODR |= LED3;
}

void LED_Off(uint8_t led)
{	
	if(led==0) GPIOA->ODR &= ~(LED0);
	if(led==1) GPIOA->ODR &= ~(LED1);
	if(led==2) GPIOA->ODR &= ~(LED2);
	if(led==3) GPIOA->ODR &= ~(LED3);
}

uint8_t Switch_Read(uint8_t sw)
{
	uint8_t switchRead = 0;
	
	if((sw==0) && !(GPIOA->IDR & SW0)) switchRead = 1;
	if((sw==1) && !(GPIOA->IDR & SW1)) switchRead = 1;
	if((sw==2) && !(GPIOA->IDR & SW2)) switchRead = 1;
	
	return switchRead;
}


void Delay_ms(uint32_t time)
{
	ms_time = time;
	while(ms_time != 0);
}
