/* MAIN.C file
 * 
 * Copyright (c) 2002-2005 STMicroelectronics
 */

#include "cm300.h"

Power_State gPowerState = POWER_ON_WAIT;
Function_State gFuncState = FUNC_IDLE;
Function_State gLastFuncState = FUNC_IDLE;
u32 gTimeTick;

main()
{
	u32 workTime = 0;
	u32 massageTime = 0;
	u32 buttonTimer = 0;
	u32 blinkTime = 0;

	bool buttonActive = FALSE;
	bool longPressActive = FALSE;
	bool isBlink = FALSE;
	
	u8 blinkCnt = 0;
	u8 i = 0;
	u8 idx1 = 0;	//massage1_duty index
	u8 idx2 = 0;	//massage2_duty index
	uint16_t massage1_duty[9] = {50, 55, 60, 65, 70, 65, 60, 55, 50};
	uint16_t massage2_duty[9] = {50, 55, 60, 65, 70, 65, 60, 55, 50};
	
	board_init();
	gTimeTick = 0;
	
	while (1)
	{

		switch(gPowerState)
		{
			case POWER_ON_WAIT:
				if(gTimeTick - workTime > POWER_ON_TIME) {
					ps_hold_cmd(OUTPUT_ON);	// ps_hold on
					beep_ceg();	// power on beep
					delay(3000);
					//beep_bdg();
					if(get_batt_level() < LOW_BATT_LEVEL) {
						gPowerState = POWER_LOW_BATT;
					}else {
						led_ind_cmd(OUTPUT_ON);
						gLastFuncState = FLASH_ReadByte(FLASH_DATA_START_PHYSICAL_ADDRESS);
						if( gLastFuncState >= FUNC_01 && gLastFuncState <= FUNC_10 ) {
							gFuncState = gLastFuncState;
						}else {
							gFuncState = gLastFuncState = FUNC_01;
						}
						gPowerState = POWER_FUNC;
						switch(gFuncState)
						{
							case FUNC_IDLE:
								
							break;
							
							case FUNC_01:
								motor_enable_duty(42);
								beep_one(0);
								//beep_bd();
							break;
							
							case FUNC_02:
								motor_enable_duty(46);
								beep_one(1);
								//beep_bd();
							break;
							
							case FUNC_03:
								motor_enable_duty(50);
								beep_one(2);
								//beep_bd();
							break;
							
							case FUNC_04:
								motor_enable_duty(54);
								beep_one(3);
								//beep_bd();
							break;
							
							case FUNC_05:
								motor_enable_duty(58);
								beep_one(4);
								//beep_bd();
							break;
							
							case FUNC_06:
								motor_enable_duty(62);
								beep_one(5);
								//beep_bd();
							break;
							
							case FUNC_07:
								motor_enable_duty(66);
								beep_one(6);
								//beep_bd();
							break;
							
							case FUNC_08:
								motor_enable_duty(70);
								beep_one(7);
								//beep_bd();
							break;
							
							case FUNC_09:
								beep_ceg();
								//beep_bcd();
								idx1 = 0;
								motor_enable_duty(massage1_duty[idx1]);
								massageTime = gTimeTick;
							break;
							
							case FUNC_10:
								beep_gec();
								//beep_bcd();
								idx2 = 0;
								motor_enable_duty(massage2_duty[idx2]);
								massageTime = gTimeTick;
							break;
						}
						
						workTime = gTimeTick;
						blinkTime = gTimeTick;
						isBlink = TRUE;
						blinkCnt = 0;
						delay(500);
					}
				}
			break;

			case POWER_FUNC:
				if (!get_key_status()) {
					if (buttonActive == FALSE) {
						buttonActive = TRUE;
						buttonTimer = gTimeTick;
					}
					if ((gTimeTick - buttonTimer > LONG_PRESS_TIME) && (longPressActive == FALSE)) {
						longPressActive = TRUE;
						gPowerState = POWER_OFF_WAIT;
						gFuncState = FUNC_IDLE;
						led_ind_cmd(OUTPUT_OFF);
						TIM1_CtrlPWMOutputs(DISABLE);
					}
				} else {
					if (buttonActive == TRUE) {
						if (longPressActive == TRUE) {
							longPressActive = FALSE;
						} else {
							gFuncState++;
							if(gFuncState >= FUNC_MAX) {
									gFuncState = FUNC_01;
							}
							gLastFuncState = gFuncState;
							
							switch(gFuncState)
							{
								case FUNC_IDLE:
									
								break;
								
								case FUNC_01:
									motor_enable_duty(42);
									beep_one(0);
									//beep_bd();
								break;
								
								case FUNC_02:
									motor_enable_duty(46);
									beep_one(1);
									//beep_bd();
								break;
								
								case FUNC_03:
									motor_enable_duty(50);
									beep_one(2);
									//beep_bd();
								break;
								
								case FUNC_04:
									motor_enable_duty(54);
									beep_one(3);
									//beep_bd();
								break;
								
								case FUNC_05:
									motor_enable_duty(58);
									beep_one(4);
									//beep_bd();
								break;
								
								case FUNC_06:
									motor_enable_duty(62);
									beep_one(5);
									//beep_bd();
								break;
								
								case FUNC_07:
									motor_enable_duty(66);
									beep_one(6);
									//beep_bd();
								break;
								
								case FUNC_08:
									motor_enable_duty(70);
									beep_one(7);
									//beep_bd();
								break;
								
								case FUNC_09:
									beep_ceg();
									//beep_bcd();
									idx1 = 0;
									motor_enable_duty(massage1_duty[idx1]);
									massageTime = gTimeTick;
								break;
								
								case FUNC_10:
									beep_gec();
									//beep_bcd();
									idx2 = 0;
									motor_enable_duty(massage2_duty[idx2]);
									massageTime = gTimeTick;
								break;
							}
							
							workTime = gTimeTick;
							blinkTime = gTimeTick;
							buttonActive = FALSE;
							isBlink = TRUE;
							blinkCnt = 0;
							delay(500);
						}
					}
				}
				
				if(gFuncState == FUNC_09) {
					if(gTimeTick - massageTime > MASSAGE_TIME1) {
						if( idx1 > 8 ) {
							idx1 = 0;
						}
						motor_enable_duty(massage1_duty[idx1]);
						idx1++;
						massageTime = gTimeTick;
					}
				}
				
				if(gFuncState == FUNC_10) {
					if(gTimeTick - massageTime > MASSAGE_TIME2 ) {
						if( idx2 > 8 ) {
							idx2 = 0;
						}
						motor_enable_duty(massage2_duty[idx2]);
						idx2++;
						massageTime = gTimeTick;
					}
				}

				if( isBlink && gTimeTick - blinkTime > LED_BLINK_TIME ) {
					if( blinkCnt == 0 || blinkCnt == 2 ) {
						led_ind_cmd(OUTPUT_OFF);
					}else {
						led_ind_cmd(OUTPUT_ON);
					}
					blinkCnt++;
					blinkTime = gTimeTick;
					if(blinkCnt > 3) {
						isBlink = FALSE;
						blinkCnt = 0;
						FLASH_ProgramByte(FLASH_DATA_START_PHYSICAL_ADDRESS, gLastFuncState);
					}
				}
				
				if(gTimeTick - workTime > WORK_END_TIME) {
					TIM1_CtrlPWMOutputs(DISABLE);
					gPowerState = POWER_OFF_WAIT;
				}
			break;
			
			case POWER_LOW_BATT:
				for(i=0; i < 3; i++) {
					beep_one(1);
					led_ind_cmd(OUTPUT_REV);
					delay(300);
				}
				delay(500);
				gPowerState = POWER_OFF;
			break;
			
			case POWER_OFF_WAIT:
				beep_gfedc();
				//beep_gdb();
				gPowerState = POWER_OFF;
			break;
			
			case POWER_OFF:
				motor_cmd(OUTPUT_OFF);
				led_ind_cmd(OUTPUT_OFF);
				ps_hold_cmd(OUTPUT_OFF);
			break;
		}
	}
}