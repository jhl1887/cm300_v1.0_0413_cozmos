#include "cm300.h"

u16 freq[] = {1047, 1175, 1319, 1397, 1568, 1760, 1976, 2093 };
u16 duration[] = {2000, 2000, 2000, 2000, 2000, 2000, 2000, 2000 };

void delay(u16 delay) {
  volatile unsigned int i;
  while(delay--)
  {
    for(i=0; i<70; i++)
    {
			nop();
    }
  }
}

void board_init(void) {
	disableInterrupts();
	clk_init();
	gpio_init();
	timer_init();
	adc_init();
	flash_init();
	enableInterrupts();
}

void clk_init(void) {
	CLK_DeInit();
	
	CLK_HSECmd(DISABLE);
	CLK_LSICmd(DISABLE);
	CLK_HSICmd(ENABLE);
	while(CLK_GetFlagStatus(CLK_FLAG_HSIRDY) == FALSE);
	
	CLK_ClockSwitchCmd(ENABLE);
	CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV1);
	CLK_SYSCLKConfig(CLK_PRESCALER_CPUDIV1);
	
	CLK_ClockSwitchConfig(CLK_SWITCHMODE_AUTO, CLK_SOURCE_HSI,DISABLE, CLK_CURRENTCLOCKSTATE_ENABLE);
	
	CLK_PeripheralClockConfig(CLK_PERIPHERAL_I2C, DISABLE);
	CLK_PeripheralClockConfig(CLK_PERIPHERAL_SPI, DISABLE);
	CLK_PeripheralClockConfig(CLK_PERIPHERAL_UART1, DISABLE);
	CLK_PeripheralClockConfig(CLK_PERIPHERAL_AWU, DISABLE);
	CLK_PeripheralClockConfig(CLK_PERIPHERAL_ADC, ENABLE);
	CLK_PeripheralClockConfig(CLK_PERIPHERAL_TIMER1, ENABLE);
	CLK_PeripheralClockConfig(CLK_PERIPHERAL_TIMER2, ENABLE);
	CLK_PeripheralClockConfig(CLK_PERIPHERAL_TIMER4, ENABLE);
}

void gpio_init(void) {
	// PS_HOLD (PA1) GPIO OUTPUT
	GPIO_Init(GPIOA, GPIO_PIN_1, GPIO_MODE_OUT_PP_LOW_SLOW);
	// KEY_DET (PB5) GPIO INPUT
	GPIO_Init(GPIOB, GPIO_PIN_5, GPIO_MODE_IN_FL_NO_IT);
	// BATT_LVL (PC4) ADC INPUT
	GPIO_Init(GPIOC, GPIO_PIN_4, GPIO_MODE_IN_FL_NO_IT);
	// BEEP (PC5) PWM OUTPUT
	GPIO_Init(GPIOC, GPIO_PIN_5, GPIO_MODE_OUT_PP_LOW_SLOW);
	// MOTOR (PC6) PWM OUTPUT
	GPIO_Init(GPIOC, GPIO_PIN_6, GPIO_MODE_OUT_PP_LOW_SLOW);
	// LED_IND (PD5) GPIO OUTPUT
	GPIO_Init(GPIOD, GPIO_PIN_5, GPIO_MODE_OUT_PP_LOW_SLOW);
}

void timer_init(void) {
	// Timer1
	// 16MHz / (15 + 1) = 1000KHz (period : 0.001ms)
	// 30KHz = 1 / 0.033ms -> count : 33
	TIM1_DeInit();
	TIM1_TimeBaseInit(15, TIM1_COUNTERMODE_UP, 33, 1);
	TIM1_OC1Init(TIM1_OCMODE_PWM1, 
							TIM1_OUTPUTSTATE_ENABLE, 
							TIM1_OUTPUTNSTATE_ENABLE, 
							17, 
							TIM1_OCPOLARITY_HIGH, 
							TIM1_OCNPOLARITY_LOW, 
							TIM1_OCIDLESTATE_RESET, 
							TIM1_OCNIDLESTATE_RESET);
	TIM1_CtrlPWMOutputs(ENABLE);
	TIM1_Cmd(ENABLE);
	TIM1_CtrlPWMOutputs(DISABLE);
	
	// Timer4 8bit Basic Timer init
	// 16MHz/128 = 125KHz -> 8us
	TIM4_DeInit();
	TIM4_TimeBaseInit(TIM4_PRESCALER_128, 125);
	TIM4_ClearFlag(TIM4_FLAG_UPDATE);
	TIM4_ITConfig(TIM4_IT_UPDATE, ENABLE);
	TIM4_Cmd(ENABLE);
}

void adc_init(void) {
	ADC1_DeInit();
  ADC1_Init(ADC1_CONVERSIONMODE_CONTINUOUS, ADC1_CHANNEL_2, ADC1_PRESSEL_FCPU_D2, \
            ADC1_EXTTRIG_TIM, DISABLE, ADC1_ALIGN_RIGHT, ADC1_SCHMITTTRIG_CHANNEL2,\
            DISABLE);	// V_BATT Level
  ADC1_Cmd(ENABLE);
  ADC1_StartConversion();
}

void flash_init(void) {
	// Set the programming time, just specify the standard programming time
	FLASH_SetProgrammingTime(FLASH_PROGRAMTIME_STANDARD);
	
	// Unlock the EEPROM area (note that the type is Data)
	FLASH_Unlock(FLASH_MEMTYPE_DATA);

	// Wait for unlock success
	// The essence is to determine whether the DUL flag in the FLASH-> IAPSR register becomes 1. 1 means write protection is eliminated, 0 means protection
	// At any time, you can restore the write protection status by changing this flag to 0
	while (FLASH_GetFlagStatus(FLASH_FLAG_DUL) == RESET);
}

// PS_HOLD (PA1) GPIO OUTPUT
void ps_hold_cmd(OutputType type)
{
	if(type == OUTPUT_ON) {
		GPIOA->ODR |= (uint8_t)GPIO_PIN_1;
	}else if(type == OUTPUT_OFF) {
		GPIOA->ODR &= (uint8_t)(~GPIO_PIN_1);
	}
}

// KEY_DET (PB5) GPIO INPUT
BitStatus get_key_status(void)
{
	return ((BitStatus)(GPIOB->IDR & (uint8_t)GPIO_PIN_5));
}

// BATT_LVL (PC4) ADC INPUT
u16 get_batt_level(void)
{
	ADC1->CSR &= (u8)(~ADC1_CSR_CH);
	ADC1->CSR |= ADC1_CHANNEL_2;

	ADC1_StartConversion();
	while(RESET == ADC1_GetFlagStatus(ADC1_FLAG_EOC));
	ADC1_ClearFlag(ADC1_FLAG_EOC);

	return ADC1_GetConversionValue();
}

// BEEP (PC5) PWM OUTPUT
void beep_play_freq(u16 freq)
{
	int sub_cnt=0;
	int count = 16000000 / freq;
	while(count > 0xffff) {
		count /= 2;
		sub_cnt++;
	}
	
	TIM2_DeInit();
	TIM2_TimeBaseInit(sub_cnt, count);
	TIM2_ITConfig(TIM2_IT_UPDATE, ENABLE);
	
	TIM2_OC1Init(TIM2_OCMODE_PWM2, TIM2_OUTPUTSTATE_ENABLE,count/2,TIM2_OCPOLARITY_LOW);
	TIM2_OC1PreloadConfig(ENABLE);
	TIM2_ARRPreloadConfig(ENABLE);
	
	TIM2_Cmd(ENABLE);
}

void beep_stop(void)
{
	TIM2_ITConfig(TIM2_IT_UPDATE, DISABLE);
	TIM2_Cmd(DISABLE);
	TIM2_DeInit();
	GPIO_WriteLow(GPIOC, GPIO_PIN_5);
}

void beep_cdefg(void)
{
	u8 i = 0;

	for(i=0; i < 5; i++) {
		beep_play_freq(freq[i]);
		delay(duration[i]);
	}
	beep_stop();
}

void beep_gfedc(void)
{
	s8 i = 0;

	for(i=4; i >= 0; i--) {
		beep_play_freq(freq[i]);
		delay(duration[i]);
	}
	beep_stop();
}

void beep_one(u8 idx)
{
	beep_play_freq(freq[idx]);
	delay(duration[idx]);
	beep_stop();
}

void beep_low_batt(void)
{
	beep_one(0);
	beep_one(0);
}

void beep_ceg(void)
{
	u8 i = 0;

	for(i=0; i < 5; i=i+2) {
		beep_play_freq(freq[i]);
		delay(duration[i]);
	}
	beep_stop();
}

void beep_gec(void)
{
	s8 i = 0;

	for(i=4; i >= 0; i=i-2) {
		beep_play_freq(freq[i]);
		delay(duration[i]);
	}
	beep_stop();
}

void beep_edc(void)
{
	s8 i = 0;

	for(i=2; i >= 0; i--) {
		beep_play_freq(freq[i]);
		delay(duration[i]);
	}
	beep_stop();
}

#if 0
void beep_bdg(void)
{
	beep_play_freq(1976);
	delay(2000);
	beep_play_freq(2349);
	delay(2000);
	beep_play_freq(3136);
	delay(2000);
	beep_stop();
}

void beep_bd(void)
{
	beep_play_freq(3951);
	delay(2000);
	beep_play_freq(4699);
	delay(4000);
	beep_stop();
}

void beep_bcd(void)
{
	beep_play_freq(1976);
	delay(2000);
	beep_play_freq(2093);
	delay(2000);
	beep_play_freq(2349);
	delay(2000);
	beep_stop();
}

void beep_gdb(void)
{
	beep_play_freq(3136);
	delay(2000);
	beep_play_freq(2349);
	delay(2000);
	beep_play_freq(1976);
	delay(2000);
	beep_stop();
}
#else
void beep_bdg(void)
{
	beep_play_freq(988);
	delay(2000);
	beep_play_freq(1175);
	delay(2000);
	beep_play_freq(1568);
	delay(2000);
	beep_stop();
}

void beep_bd(void)
{
	beep_play_freq(1976);
	delay(2000);
	beep_play_freq(2349);
	delay(4000);
	beep_stop();
}

void beep_bcd(void)
{
	beep_play_freq(988);
	delay(2000);
	beep_play_freq(1047);
	delay(2000);
	beep_play_freq(1175);
	delay(2000);
	beep_stop();
}

void beep_gdb(void)
{
	beep_play_freq(1568);
	delay(2000);
	beep_play_freq(1175);
	delay(2000);
	beep_play_freq(988);
	delay(2000);
	beep_stop();
}
#endif

// MOTOR (PC6) GPIO OUTPUT
void motor_cmd(OutputType type)
{
	if(type == OUTPUT_ON) {
		GPIOC->ODR |= (uint8_t)(GPIO_PIN_6);		
	}else if(type == OUTPUT_OFF) {
		GPIOC->ODR &= (uint8_t)(~GPIO_PIN_6);
	}else if(type == OUTPUT_REV) {
		GPIOC->ODR ^= (uint8_t)GPIO_PIN_6;
	}
}

void motor_enable_duty(uint16_t duty)
{
	uint16_t ccr = 33 * duty / 100;
	TIM1_CtrlPWMOutputs(DISABLE);
	TIM1->CCR1H = (uint8_t)(ccr >> 8);
	TIM1->CCR1L = (uint8_t)(ccr);
	TIM1_CtrlPWMOutputs(ENABLE);
}

// LED_IND (PD3) GPIO OUTPUT
void led_ind_cmd(OutputType type)
{
	if(type == OUTPUT_ON) {
		GPIOD->ODR |= (uint8_t)(GPIO_PIN_5);		
	}else if(type == OUTPUT_OFF) {
		GPIOD->ODR &= (uint8_t)(~GPIO_PIN_5);
	}else if(type == OUTPUT_REV) {
		GPIOD->ODR ^= (uint8_t)GPIO_PIN_5;
	}
}

