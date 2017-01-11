
#include "stm32f30x.h"
#include "main.h"
//#include “core_cm3.h” //needed for data types 




/*
system setup:
HSE = 13.56MHz
SYSCLK = 67.8MHz
APB2 = 67.8MHz
APB1 = 33.9MHz

Pins:
LCD_D/C: PB0
Button_B: PB3
Button_A: PB4
LED_1: PB6
LED_2: PB7

CURRENT_ADC: PA0 ADC1_IN1
OUTPUT_ADC: PA1 ADC1_IN2
SUPPLY_ADC: PA2 ADC1_IN3
AUDIO_ADC: PA3 ADC1_IN4

LCD_CS!: PA4
SPI_SCK: PA5
SPI_MOSI: PA7
MOD_A: PA8  HRTIM1_CHA1
MOD_B: PA9  HRTIM1_CHA2
RF_A: PA10  HRTIM1_CHB1
RF_B: PA11  HRTIM1_CHB2
*/


//global variables
uint32_t output_current, output_voltage, supply_voltage, audio_voltage;

//function prototypes: 
void led_on();
void led_off();
void GPIO_config();
void ADC_config();

void RF_config();
void RF_enable();
void RF_disable(); 
void SMPS_config();

void Delay_init();
void msDelay();

int main(){

	
	
	Delay_init(8928); //1ms period
	GPIO_config();
	led_on();
	RF_config();
	ADC_config();
	SMPS_config();
	RF_enable();
	
	int i = 0;
	while(1){
		//code goes here
		i++;
		//led_on();
		//led_off();

	}


	return 0;
}


//CURRENT_ADC: PA0 ADC1_IN1
//OUTPUT_ADC: PA1 ADC1_IN2
//SUPPLY_ADC: PA2 ADC1_IN3
//AUDIO_ADC: PA3 ADC1_IN4
void ADC_config(){

	ADC_InitTypeDef       ADC_InitStructure;
  	ADC_CommonInitTypeDef ADC_CommonInitStructure;
  	ADC_InjectedInitTypeDef ADC_InjectedInitStruct;

	/* Configure the ADC clock */
	RCC_ADCCLKConfig(RCC_ADC12PLLCLK_Div1);

	/* Enable ADC1 clock */
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_ADC12, ENABLE);

	//enable power to ADC
	ADC_VoltageRegulatorCmd(ADC1, ENABLE);

	msDelay(1); //allow power to stabilize 

	//calibrate the ADC
	ADC_SelectCalibrationMode(ADC1, ADC_CalibrationMode_Single);
 	ADC_StartCalibration(ADC1);
  	while(ADC_GetCalibrationStatus(ADC1) != RESET ); //wait for finish

	//structure to init ADC1
	ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;                                                                    
	ADC_CommonInitStructure.ADC_Clock = ADC_Clock_AsynClkMode;                    
	ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;             
	ADC_CommonInitStructure.ADC_DMAMode = ADC_DMAMode_OneShot;                  
	ADC_CommonInitStructure.ADC_TwoSamplingDelay = 0;          
	ADC_CommonInit(ADC1, &ADC_CommonInitStructure); //init the ADC
	
	
	//more detailed init structure
	//do not set up in regular sampling mode, want injected sampling mode
	ADC_InitStructure.ADC_ContinuousConvMode = ADC_ContinuousConvMode_Disable;
	ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b; 
	ADC_InitStructure.ADC_ExternalTrigConvEvent = ADC_ExternalTrigConvEvent_0;         
	ADC_InitStructure.ADC_ExternalTrigEventEdge = ADC_ExternalTrigEventEdge_None;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStructure.ADC_OverrunMode = ADC_OverrunMode_Disable;   
	ADC_InitStructure.ADC_AutoInjMode = ADC_AutoInjec_Disable;  
	ADC_InitStructure.ADC_NbrOfRegChannel = 1;
	ADC_Init(ADC1, &ADC_InitStructure);

	//set up ADC injection sampling
	//injected event comes from page 230 of the refrence manual
	//HRTIM_ADCTRG2 event
	ADC_InjectedInitStruct.ADC_ExternalTrigInjecConvEvent = ADC_ExternalTrigInjecConvEvent_9;
  	ADC_InjectedInitStruct.ADC_ExternalTrigInjecEventEdge = ADC_ExternalTrigInjecEventEdge_RisingEdge;
  	ADC_InjectedInitStruct.ADC_InjecSequence1 = ADC_InjectedChannel_1; /* corresponds to PA0 (CURRENT_ADC) */
  	ADC_InjectedInitStruct.ADC_InjecSequence2 = ADC_InjectedChannel_2; /* corresponds to PA1 (OUTPUT_ADC) */
	ADC_InjectedInitStruct.ADC_InjecSequence3 = ADC_InjectedChannel_3; /* corresponds to PA2 (SUPPLY_ADC) */
  	ADC_InjectedInitStruct.ADC_InjecSequence4 = ADC_InjectedChannel_4; /* corresponds to PA3 (AUDIO_ADC) */
  	ADC_InjectedInitStruct.ADC_NbrOfInjecChannel = 4;
  	ADC_InjectedInit(ADC1, &ADC_InjectedInitStruct);
	
	ADC_InjectedChannelSampleTimeConfig(ADC1, ADC_Channel_1, ADC_SampleTime_7Cycles5); //~15ns time constant
	ADC_InjectedChannelSampleTimeConfig(ADC1, ADC_Channel_2, ADC_SampleTime_7Cycles5); //clk is 14ns
	ADC_InjectedChannelSampleTimeConfig(ADC1, ADC_Channel_3, ADC_SampleTime_7Cycles5); //~7 time constants
	ADC_InjectedChannelSampleTimeConfig(ADC1, ADC_Channel_4, ADC_SampleTime_7Cycles5);
	
	ADC_Cmd(ADC1, ENABLE); //enable ADC
	
	// wait for adc to be ready
	while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_RDY));   

	//Start ADC1 Injected Conversions 
	//this actually just enables it
	ADC_StartInjectedConversion(ADC1);


}


//MOD_A: PA8  HRTIM1_CHA1
//MOD_B: PA9  HRTIM1_CHA2
void SMPS_config(){

	HRTIM_OutputCfgTypeDef HRTIM_TIM_OutputStructure;
	HRTIM_BaseInitTypeDef HRTIM_BaseInitStructure;
	HRTIM_TimerInitTypeDef HRTIM_TimerInitStructure;  
	HRTIM_TimerCfgTypeDef HRTIM_TimerWaveStructure;
	HRTIM_CompareCfgTypeDef HRTIM_CompareStructure;
	HRTIM_DeadTimeCfgTypeDef HRTIM_TIM_DeadTimeStructure;
	HRTIM_ADCTriggerCfgTypeDef HRTIM_ADCTrigStructure;   
	NVIC_InitTypeDef    NVIC_InitStructure;


	/* --------------------------------------------------- */
	/* TIMERA initialization: timer mode and PWM frequency */
	/* --------------------------------------------------- */
	//intilize timer
	HRTIM_TimerInitStructure.HalfModeEnable = HRTIM_HALFMODE_DISABLED;
	HRTIM_TimerInitStructure.StartOnSync = HRTIM_SYNCSTART_DISABLED;
	HRTIM_TimerInitStructure.ResetOnSync = HRTIM_SYNCRESET_DISABLED;
	HRTIM_TimerInitStructure.DACSynchro = HRTIM_DACSYNC_NONE;
	HRTIM_TimerInitStructure.PreloadEnable = HRTIM_PRELOAD_ENABLED;
	HRTIM_TimerInitStructure.UpdateGating = HRTIM_UPDATEGATING_INDEPENDENT;
	HRTIM_TimerInitStructure.BurstMode = HRTIM_TIMERBURSTMODE_MAINTAINCLOCK;
	HRTIM_TimerInitStructure.RepetitionUpdate = HRTIM_UPDATEONREPETITION_ENABLED;

	//sets counter for timer
	HRTIM_BaseInitStructure.Period = BUCK_PWM_PERIOD; /* 400kHz switching frequency */
	HRTIM_BaseInitStructure.RepetitionCounter = MOD_UPDATE_RATE;   /* ISR rate */
	HRTIM_BaseInitStructure.PrescalerRatio = HRTIM_PRESCALERRATIO_MUL32;
	HRTIM_BaseInitStructure.Mode = HRTIM_MODE_CONTINOUS;          

	HRTIM_Waveform_Init(HRTIM1, HRTIM_TIMERINDEX_TIMER_A, &HRTIM_BaseInitStructure, &HRTIM_TimerInitStructure);


	/* ------------------------------------------------ */
	/* TIMERA output and registers update configuration */
	/* ------------------------------------------------ */
	//sets more timer things
	HRTIM_TimerWaveStructure.DeadTimeInsertion = HRTIM_TIMDEADTIMEINSERTION_ENABLED;
	HRTIM_TimerWaveStructure.DelayedProtectionMode = HRTIM_TIMDELAYEDPROTECTION_DISABLED;
	HRTIM_TimerWaveStructure.FaultEnable = HRTIM_TIMFAULTENABLE_NONE;//no stopping this
	HRTIM_TimerWaveStructure.FaultLock = HRTIM_TIMFAULTLOCK_READWRITE;
	HRTIM_TimerWaveStructure.PushPull = HRTIM_TIMPUSHPULLMODE_DISABLED;
	HRTIM_TimerWaveStructure.ResetTrigger = HRTIM_TIMRESETTRIGGER_NONE;
	HRTIM_TimerWaveStructure.ResetUpdate = HRTIM_TIMUPDATEONRESET_DISABLED;
	HRTIM_TimerWaveStructure.UpdateTrigger = HRTIM_TIMUPDATETRIGGER_NONE;
	HRTIM_WaveformTimerConfig(HRTIM1, HRTIM_TIMERINDEX_TIMER_A, &HRTIM_TimerWaveStructure);


	/* -------------------------------- */
	/* TA1 and TA2 waveform description */
	/* -------------------------------- */
	/* PWM on TA1 */
	//set up outputs TA1 and TA2
	
	
	HRTIM_TIM_OutputStructure.Polarity = HRTIM_OUTPUTPOLARITY_HIGH; 
	HRTIM_TIM_OutputStructure.SetSource = HRTIM_OUTPUTSET_TIMPER;  
	HRTIM_TIM_OutputStructure.ResetSource = HRTIM_OUTPUTRESET_TIMCMP1; 
	HRTIM_TIM_OutputStructure.IdleMode = HRTIM_OUTPUTIDLEMODE_NONE;  
	HRTIM_TIM_OutputStructure.IdleState = HRTIM_OUTPUTIDLESTATE_INACTIVE;          
	HRTIM_TIM_OutputStructure.FaultState = HRTIM_OUTPUTFAULTSTATE_INACTIVE;          
	HRTIM_TIM_OutputStructure.ChopperModeEnable = HRTIM_OUTPUTCHOPPERMODE_DISABLED;        
	HRTIM_TIM_OutputStructure.BurstModeEntryDelayed = HRTIM_OUTPUTBURSTMODEENTRY_REGULAR;
	HRTIM_WaveformOutputConfig(HRTIM1, HRTIM_TIMERINDEX_TIMER_A, HRTIM_OUTPUT_TA1, &HRTIM_TIM_OutputStructure);
	HRTIM_WaveformOutputConfig(HRTIM1, HRTIM_TIMERINDEX_TIMER_A, HRTIM_OUTPUT_TA2, &HRTIM_TIM_OutputStructure);
	

	/* Set compare registers for duty cycle on TA1 */
	//this sets the compare for the switching of TA1/TA2
	HRTIM_CompareStructure.AutoDelayedMode = HRTIM_AUTODELAYEDMODE_REGULAR;
	HRTIM_CompareStructure.AutoDelayedTimeout = 0;
	HRTIM_CompareStructure.CompareValue = BUCK_PWM_PERIOD/5;     //starting value
	HRTIM_WaveformCompareConfig(HRTIM1, HRTIM_TIMERINDEX_TIMER_A, HRTIM_COMPAREUNIT_1, &HRTIM_CompareStructure);

	//this sets up the deadtime value 
	HRTIM_TIM_DeadTimeStructure.FallingLock = HRTIM_TIMDEADTIME_FALLINGLOCK_WRITE;
	HRTIM_TIM_DeadTimeStructure.FallingSign = HRTIM_TIMDEADTIME_FALLINGSIGN_POSITIVE;
	HRTIM_TIM_DeadTimeStructure.FallingSignLock = HRTIM_TIMDEADTIME_FALLINGSIGNLOCK_WRITE;
	HRTIM_TIM_DeadTimeStructure.FallingValue = DT_FALLING;
	HRTIM_TIM_DeadTimeStructure.Prescaler = 0x0;
	HRTIM_TIM_DeadTimeStructure.RisingLock = HRTIM_TIMDEADTIME_RISINGLOCK_WRITE;
	HRTIM_TIM_DeadTimeStructure.RisingSign = HRTIM_TIMDEADTIME_RISINGSIGN_POSITIVE;
	HRTIM_TIM_DeadTimeStructure.RisingSignLock = HRTIM_TIMDEADTIME_RISINGSIGNLOCK_WRITE;
	HRTIM_TIM_DeadTimeStructure.RisingValue = DT_RISING;
	HRTIM_DeadTimeConfig(HRTIM1, HRTIM_TIMERINDEX_TIMER_A, &HRTIM_TIM_DeadTimeStructure);

	
	/* --------------------------*/
	/* ADC trigger initialization */
	/* --------------------------*/
	/* Set compare 2 registers for ADC trigger */
	
	HRTIM_CompareStructure.AutoDelayedMode = HRTIM_AUTODELAYEDMODE_REGULAR;
	HRTIM_CompareStructure.AutoDelayedTimeout = 0;
	HRTIM_CompareStructure.CompareValue = BUCK_PWM_PERIOD/10;  //samples in middle of duty cycle 
	HRTIM_WaveformCompareConfig(HRTIM1, HRTIM_TIMERINDEX_TIMER_A, HRTIM_COMPAREUNIT_2, &HRTIM_CompareStructure);

	HRTIM_ADCTrigStructure.Trigger = HRTIM_ADCTRIGGEREVENT24_TIMERA_CMP2; 
	HRTIM_ADCTrigStructure.UpdateSource = HRTIM_ADCTRIGGERUPDATE_TIMER_A; //this was D in example, switched to A?
	HRTIM_ADCTriggerConfig(HRTIM1, HRTIM_ADCTRIGGER_2, &HRTIM_ADCTrigStructure);
	

	
	/*   */
	/* -------------------------*/
	/* Interrupt initialization */
	/* -------------------------*/
	/* Configure and enable HRTIM TIMERA interrupt channel in NVIC */
	NVIC_InitStructure.NVIC_IRQChannel = HRTIM1_TIMA_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	/* TIMER A issues an interrupt on each repetition event */
	 HRTIM_ITConfig(HRTIM1, HRTIM_TIMERINDEX_TIMER_A, HRTIM_TIM_IT_REP, ENABLE);
	

	/* ---------------*/
	/* HRTIM start-up */
	/* ---------------*/
	/* Enable HRTIM's outputs TA1 and TA2 */
	/* Note: it is necessary to enable also GPIOs to have outputs functional */
	HRTIM_WaveformOutputStart(HRTIM1, HRTIM_OUTPUT_TA1 | HRTIM_OUTPUT_TA2); 

	/* Start HRTIM's TIMER A */
	HRTIM_WaveformCounterStart(HRTIM1, HRTIM_TIMERID_TIMER_A); 
	HRTIM_SlaveSetCompare(HRTIM1, HRTIM_TIMERINDEX_TIMER_A, HRTIM_COMPAREUNIT_1,  BUCK_PWM_PERIOD/3); //initial PWM period

	HRTIM_SlaveSetCompare(HRTIM1, HRTIM_TIMERINDEX_TIMER_A, HRTIM_COMPAREUNIT_2,  (uint32_t)ADC_SAMPLE_TIME); /* ADC trigger update */	

}

void GPIO_config(){
	

	//enable clock to pin ports
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA | RCC_AHBPeriph_GPIOB, ENABLE);
	//init GPIO pin 
	GPIO_InitTypeDef GPIO_InitStruct;

	//LED_1: PB6
	GPIO_ResetBits(GPIOB,GPIO_Pin_6);
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOB,&GPIO_InitStruct);

	//LED_2: PB7
	GPIO_ResetBits(GPIOB,GPIO_Pin_7);
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_7;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOB,&GPIO_InitStruct);
	
	//MOD_A: PA8, uses HRTIMER
	GPIO_ResetBits(GPIOA,GPIO_Pin_8);
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_8;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOA,&GPIO_InitStruct);
	
	//MOD_A alternative function setup
	// HRTIMER is on AF_13
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource8, GPIO_AF_13);

	//MOD_B: PA9, uses HRTIMER
	GPIO_ResetBits(GPIOA,GPIO_Pin_9);
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOA,&GPIO_InitStruct);

	//MOD_B alternative function setup
	// HRTIMER is on AF_13
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_13);
	
	//RF_A: PA10, uses HRTIMER
	GPIO_ResetBits(GPIOA,GPIO_Pin_10);
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOA,&GPIO_InitStruct);

	//RF_A alternative function setup
	// HRTIMER is on AF_13
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_13);

	//RF_B: PA11, uses HRTIMER
	GPIO_ResetBits(GPIOA,GPIO_Pin_11);
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_11;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOA,&GPIO_InitStruct);

	//RF_B alternative function setup
	// HRTIMER is on AF_13
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource11, GPIO_AF_13);

	//analog inputs

	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AN;
  	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL ;
  	GPIO_Init(GPIOA, &GPIO_InitStruct);
	

}

void led_on(){

	GPIO_SetBits(GPIOB,GPIO_Pin_6);
}

void led_off(){
	GPIO_ResetBits(GPIOB,GPIO_Pin_6);
}

//configure high resolution timer for buck converter and RF out
//push pull mode for HRTIM1_B
void RF_config(){

	HRTIM_OutputCfgTypeDef HRTIM_TIM_OutputStructure;
	HRTIM_BaseInitTypeDef HRTIM_BaseInitStruct;
	HRTIM_TimerInitTypeDef HRTIM_TimerInitStructure;  
	HRTIM_TimerCfgTypeDef HRTIM_TimerWaveStructure;
	HRTIM_CompareCfgTypeDef HRTIM_CompareStructure;

	/* ----------------------------*/
	/* HRTIM Global initialization */
	/* ----------------------------*/
	/* Use the PLLx2 clock for HRTIM */
	RCC_HRTIM1CLKConfig(RCC_HRTIM1CLK_PLLCLK);
	/* Enable HRTIM clock*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_HRTIM1, ENABLE);

	/* HRTIM DLL calibration: periodic calibration, set period to 14µs */
	HRTIM_DLLCalibrationStart(HRTIM1, HRTIM_CALIBRATIONRATE_14);
	HRTIM1_COMMON->DLLCR |= HRTIM_DLLCR_CALEN; 

	/* Wait calibration completion*/
	while(HRTIM_GetCommonFlagStatus(HRTIM1, HRTIM_ISR_DLLRDY) == RESET);


	/* --------------------------------------------------- */
	/* TIMERB initialization: timer mode and PWM frequency */
	/* --------------------------------------------------- */
	


	HRTIM_TimerInitStructure.HalfModeEnable = HRTIM_HALFMODE_DISABLED;
	HRTIM_TimerInitStructure.StartOnSync = HRTIM_SYNCSTART_DISABLED;
	HRTIM_TimerInitStructure.ResetOnSync = HRTIM_SYNCRESET_DISABLED;
	HRTIM_TimerInitStructure.DACSynchro = HRTIM_DACSYNC_NONE;
	HRTIM_TimerInitStructure.PreloadEnable = HRTIM_PRELOAD_ENABLED;
	HRTIM_TimerInitStructure.UpdateGating = HRTIM_UPDATEGATING_INDEPENDENT;
	HRTIM_TimerInitStructure.BurstMode = HRTIM_TIMERBURSTMODE_MAINTAINCLOCK;
	HRTIM_TimerInitStructure.RepetitionUpdate = HRTIM_UPDATEONREPETITION_ENABLED;


	//setup hrtimer for max clock speed in continous mode.

	HRTIM_BaseInitStruct.Period = PERIOD_6_78MHZ;
	HRTIM_BaseInitStruct.RepetitionCounter = 255; //this is for interupts?
	HRTIM_BaseInitStruct.PrescalerRatio = HRTIM_PRESCALERRATIO_MUL32; 
	HRTIM_BaseInitStruct.Mode= HRTIM_MODE_CONTINOUS; 

	
	//setup timer B
	HRTIM_Waveform_Init(HRTIM1, HRTIM_TIMERINDEX_TIMER_B, &HRTIM_BaseInitStruct, &HRTIM_TimerInitStructure);
	
	HRTIM_TimerWaveStructure.DeadTimeInsertion = HRTIM_TIMDEADTIMEINSERTION_DISABLED;
	HRTIM_TimerWaveStructure.DelayedProtectionMode = HRTIM_TIMDELAYEDPROTECTION_DISABLED;
	HRTIM_TimerWaveStructure.FaultEnable = HRTIM_TIMFAULTENABLE_NONE;
	HRTIM_TimerWaveStructure.FaultLock = HRTIM_TIMFAULTLOCK_READWRITE;
	HRTIM_TimerWaveStructure.PushPull = HRTIM_TIMPUSHPULLMODE_DISABLED;
	HRTIM_TimerWaveStructure.ResetTrigger = HRTIM_TIMRESETTRIGGER_NONE;
	HRTIM_TimerWaveStructure.ResetUpdate = HRTIM_TIMUPDATEONRESET_DISABLED;
	HRTIM_TimerWaveStructure.UpdateTrigger = HRTIM_TIMUPDATETRIGGER_NONE;

	HRTIM_WaveformTimerConfig(HRTIM1, HRTIM_TIMERINDEX_TIMER_B, &HRTIM_TimerWaveStructure);

	//more configure things
	HRTIM_TIM_OutputStructure.Polarity = HRTIM_OUTPUTPOLARITY_HIGH; 
	HRTIM_TIM_OutputStructure.SetSource = HRTIM_OUTPUTSET_TIMPER;  
	HRTIM_TIM_OutputStructure.ResetSource = HRTIM_OUTPUTRESET_TIMCMP1; 
	HRTIM_TIM_OutputStructure.IdleMode = HRTIM_OUTPUTIDLEMODE_NONE;  
	HRTIM_TIM_OutputStructure.IdleState = HRTIM_OUTPUTIDLESTATE_INACTIVE;          
	HRTIM_TIM_OutputStructure.FaultState = HRTIM_OUTPUTFAULTSTATE_INACTIVE;          
	HRTIM_TIM_OutputStructure.ChopperModeEnable = HRTIM_OUTPUTCHOPPERMODE_DISABLED;        
	HRTIM_TIM_OutputStructure.BurstModeEntryDelayed = HRTIM_OUTPUTBURSTMODEENTRY_REGULAR;
	HRTIM_WaveformOutputConfig(HRTIM1, HRTIM_TIMERINDEX_TIMER_B, HRTIM_OUTPUT_TB1, &HRTIM_TIM_OutputStructure);

	HRTIM_TIM_OutputStructure.SetSource = HRTIM_OUTPUTSET_TIMCMP2;  
	HRTIM_TIM_OutputStructure.ResetSource = HRTIM_OUTPUTRESET_TIMCMP3; 

	HRTIM_WaveformOutputConfig(HRTIM1, HRTIM_TIMERINDEX_TIMER_B, HRTIM_OUTPUT_TB2, &HRTIM_TIM_OutputStructure);

	//set up the 3 compares
	HRTIM_CompareStructure.AutoDelayedMode = HRTIM_AUTODELAYEDMODE_REGULAR;
	HRTIM_CompareStructure.AutoDelayedTimeout = 0;
	HRTIM_CompareStructure.CompareValue = (PERIOD_6_78MHZ>>1);     //starting value
	HRTIM_WaveformCompareConfig(HRTIM1, HRTIM_TIMERINDEX_TIMER_B, HRTIM_COMPAREUNIT_1, &HRTIM_CompareStructure);

	HRTIM_CompareStructure.AutoDelayedMode = HRTIM_AUTODELAYEDMODE_REGULAR;
	HRTIM_CompareStructure.AutoDelayedTimeout = 0;
	HRTIM_CompareStructure.CompareValue = (PERIOD_6_78MHZ)>>2;     //starting value
	HRTIM_WaveformCompareConfig(HRTIM1, HRTIM_TIMERINDEX_TIMER_B, HRTIM_COMPAREUNIT_2, &HRTIM_CompareStructure);

	HRTIM_CompareStructure.AutoDelayedMode = HRTIM_AUTODELAYEDMODE_REGULAR;
	HRTIM_CompareStructure.AutoDelayedTimeout = 0;
	HRTIM_CompareStructure.CompareValue = (PERIOD_6_78MHZ*3)>>2;     //starting value
	HRTIM_WaveformCompareConfig(HRTIM1, HRTIM_TIMERINDEX_TIMER_A, HRTIM_COMPAREUNIT_3, &HRTIM_CompareStructure);

	//enable timer
	HRTIM_WaveformCounterStart(HRTIM1, HRTIM_TIMERID_TIMER_B); 
}

void RF_enable(){
	//enable outputs for class E drive stage
	HRTIM_WaveformOutputStart(HRTIM1, HRTIM_OUTPUT_TB1 | HRTIM_OUTPUT_TB2);
}

void RF_disable(){
	HRTIM_WaveformOutputStop(HRTIM1, HRTIM_OUTPUT_TB1 | HRTIM_OUTPUT_TB2);
}


//delays by count of system tick counter 
void msDelay(uint32_t nTime)
{
 	SysTick->CTRL = (~SysTick_CTRL_COUNTFLAG_Msk)&(SysTick->CTRL); //clear count flag
	SysTick->VAL = 0; //set counter value to 0

	while(nTime != 0){
		while(((SysTick->CTRL) & SysTick_CTRL_COUNTFLAG_Msk) == 0); //wait for count flag to be set
		nTime = nTime - 1;
		SysTick->CTRL = (~SysTick_CTRL_COUNTFLAG_Msk)&(SysTick->CTRL); //clear count flag
	}
	
	
  
  
}

void Delay_init(uint32_t interval){
	
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8); //set clock source

	SysTick->LOAD = interval; //set reload value
	SysTick->VAL = 0; //set counter value to 0
	SysTick->CTRL =  SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_ENABLE_Msk; //set clock and enable
}



