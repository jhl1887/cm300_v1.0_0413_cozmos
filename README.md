<!--
***********************************
*** 1. Company : cozmos         ***
*** 2. Edit_date : 2022-04-18   ***
*** 3. Editer : JHLee           ***
***********************************
-->

# cm300_v1.0_0413 Release Note
## 1. 전동클렌저 P/G 개요
### 1.1 릴리즈 목적
* cozmos 내부적으로 기능 점검 및 변경 위함.
* source 분석 통해 -> 차기 PJT에 적용점 찾기 위함.

### 1.2 개발환경
* OS : window10
* language : c
* TOOL : 1. IAR Embedded Workbench IDE - STM8  
　　　&nbsp;2. VScode

### 1.3 제품 동작 방식
* Button Press 방식
```
Power_On - 버튼을 1초 동안 누르면 LED/Motor 기능 On  
Func_01~10 - 버튼을 1번씩 누를때 마다 다음기능으로 변경  
             (Func_09~10 : Massage 기능)  
Power_Off - 버튼을 1초 동안 누르면 LED/Motor 기능 Off
```

***

## 2. 제품 동작 시나리오 

### 2.1 POWER_ON
> #### 버튼 1초 long_Press -> Func_01 동작
>  
> <동작 순서>
> - ps_hold_on (상시온)
> - beep음 - 도미솔  
> - 1초 delay  
> - motor_On(duty:42)  
> - LED 두차례 blink(beep음 동작 후 LED동작)
> - 제품사용 기록이 FLASH에 저장되어 있다면 1. 해당 FUNC값에 따른 동작 진행 / 2. 없으면 FUNC_01 동작 
> - (Batt_level_550이하일 경우 BATT_LOW로 이동) ※570(3.8V), 555(3.75V)   
``` C
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
```

### 2.2 POWER_FUNC
> #### 버튼 short_Press -> Func_02 동작 (1번 누를때마다 FuncState+1)  
>  
> <동작 순서>
> - beep음 - 도레미파솔라시도  
> - motor_On (duty:도-42, 레-46, 미-50, 파-54, 솔-58, 라-62, 시-66, 도-70)  
> - LED 두차례 blink(beep음 동작 후 LED동작)  
> - (long_press일 경우 POWER_OFF_WAIT로 이동)  
``` C
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
                        
                        (case 03~10 생략)
```


### 2.3 POWER_LOW_BATT
> #### POWER_ON_WAIT상태에서 BATT_LOW일 경우 해당 case로 넘어옴   
>  
> <동작 순서>
> - beep음 - 도(3차례 반복)  
> - motor_On (duty:FLASH에 저장되어있는 값)  
> - LED 3차례 blink  
> - 0.5초 delay 이후 POWER_OFF로 넘어감

``` C
case POWER_LOW_BATT:
    for(i=0; i < 3; i++) {
        beep_one(1);
        led_ind_cmd(OUTPUT_REV);
        delay(300);
    }
    delay(500);
    gPowerState = POWER_OFF;
break;
```

### 2.4 POWER_OFF_WAIT
> #### POWER_FUNC 상태에서 LONG_PRESS(1초)일 경우 해당 case로 넘어옴   
>  
> <동작 순서>
> - beep음 - 솔파미레도  
> - POWER_OFF로 넘어감

``` C
case POWER_OFF_WAIT:
    beep_gfedc();
    //beep_gdb();
    gPowerState = POWER_OFF;
break;
```

### 2.5 POWER_OFF
> #### 1. POWER_FUNC 상태에서 LONG_PRESS(1초)일 경우 / 2. BATT_LOW일 경우 해당 case로 넘어옴   
>
> <동작 순서>
>
> - motor_Off
> - led_Off
> - ps_hold_Off
``` C
case POWER_OFF:
    motor_cmd(OUTPUT_OFF);
    led_ind_cmd(OUTPUT_OFF);
    ps_hold_cmd(OUTPUT_OFF);
break;
```

***

## 3. 수정사항(v2.0)

### 3.1 Added

* Power_On_sound 이후 delay_time 추가
``` C
if(gTimeTick - workTime > POWER_ON_TIME) {  
    ps_hold_cmd(OUTPUT_ON);	// ps_hold on  
    beep_ceg();	// power on beep  
    delay(3000);  
```
### 3.2 Changed
(없음)

### 3.3 Removed
(없음)

***

## 4. 신뢰성테스트
```
* 제품 사용 중 상태 확인을 위한 신뢰성테스트
* Test 종류 - Button Press Test / 충전 중 발열 상태 확인
* Button Press Test 소요시간 - 매일 20~30분 / 7일간
* Test 종료 후 이상유무 확인 -> 공유
```

### 4.1 Button Press Test
#### 4.1.1 Press 강도에 따른 Test (성인 남자 기준)
* Button Press 강도 약하게 × 5회 × 2set
* Button Press 강도 강하게 × 5회 × 2set

#### 4.1.2 Press 반복에 따른 Test
* Button Press 5번 반복 × 2set
* Button Press 10번 반복 × 2set
* Button Press 20번 반복 × 2set

#### 4.1.3 침수상태 동작 Test (수심 100mm)
* Button Press 5번 반복 × 2set
* Button Press 10번 반복 × 2set
* Button Press 20번 반복 × 2set

### 4.2 충전 중 발열 Test (시간별 발열 상태 확인)
* 30분 charging
* 1시간 charging
* 5시간 charging
* 10시간 charging
* 24시간 charging

