#ifndef __CM300_H__
#define __CM300_H__

#include "stm8s.h"

#define POWER_ON_TIME						500			// 500=0.5sec
#define LONG_PRESS_TIME					1000		// 1000=1sec

#define LOW_BATT_LEVEL					550			// 570(3.8V), 555(3.75V)
#define MASSAGE_TIME1						100			// 100=0.1sec
#define MASSAGE_TIME2						30			// 30=0.03sec
#define WORK_END_TIME						300000	// 300000=5min 30000=30sec
#define LED_BLINK_TIME					500			// 500=0.5sec


typedef enum
{
	POWER_ON_WAIT = 0
	,POWER_FUNC_IDLE = 1
	,POWER_FUNC = 2
	,POWER_LOW_BATT = 3
	,POWER_OFF_WAIT = 4
	,POWER_OFF = 5
}Power_State;

typedef enum
{
	FUNC_IDLE = 0
	,FUNC_01 = 1
	,FUNC_02 = 2
	,FUNC_03 = 3
	,FUNC_04 = 4
	,FUNC_05 = 5
	,FUNC_06 = 6
	,FUNC_07 = 7
	,FUNC_08 = 8
	,FUNC_09 = 9
	,FUNC_10 = 10
	,FUNC_MAX = 11
}Function_State;

typedef enum
{
	OUTPUT_ON
	,OUTPUT_OFF
	,OUTPUT_REV
}OutputType;

void delay(u16 delay);

void board_init(void);
void clk_init(void);
void gpio_init(void);
void timer_init(void);
void adc_init(void);
void flash_init(void);

void ps_hold_cmd(OutputType type);
BitStatus get_key_status(void);
u16 get_batt_level(void);
void beep_play_freq(u16 freq);
void beep_stop(void);
void beep_cdefg(void);
void beep_gfedc(void);
void beep_one(u8 idx);
void beep_low_batt(void);
void beep_ceg(void);
void beep_gec(void);
void beep_edc(void);

void beep_bdg(void);
void beep_bd(void);
void beep_bcd(void);
void beep_gdb(void);

void motor_cmd(OutputType type);
void motor_enable_duty(uint16_t duty);
void led_ind_cmd(OutputType type);

#endif /*__CM300_H__*/