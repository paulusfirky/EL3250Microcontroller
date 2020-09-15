#define LED0 1UL<<5
#define LED1 1UL<<6
#define LED2 1UL<<7
#define LED3 1UL<<8

#define SW0 1UL<<1
#define SW1 1UL<<9
#define SW2 1UL<<10

void BSP476_Init(void);
void LED_Toggle(uint8_t number);
void LED_On(uint8_t number);
void LED_Off(uint8_t number);
uint8_t Switch_Read(uint8_t number);
