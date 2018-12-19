#include "main.h"

const uint32_t LED[] = { GPIO_Pin_12, GPIO_Pin_14, GPIO_Pin_15 };

const uint32_t LEDs = GPIO_Pin_12 | GPIO_Pin_14 | GPIO_Pin_15 ;

uint8_t led_number = 0;

void InitLEDs(void);
void InitTimer(void);
void InitPMW(void);
void InitButtonInterruption(void);
void InitButton2Interruption(void);
void EXTI0_IRQHandler(void);
void EXTI1_IRQHandler(void);
void EXTI2_IRQHandler(void);
void init_button1(void);
void init_button2(void);
void AddRed(void);
void AddGreen(void);
void AddBlue(void);

void InitLEDs(void){
    
    GPIO_InitTypeDef GPIO_InitStructure;
    // init external LEDs
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource8, GPIO_AF_TIM1);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_TIM1);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_TIM1);
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    // init LEDs on the board
     RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
  GPIO_InitTypeDef A;
  A.GPIO_Pin = LEDs ;
  A.GPIO_Mode  = GPIO_Mode_OUT;
  A.GPIO_OType = GPIO_OType_PP;
  A.GPIO_Speed = GPIO_Speed_100MHz;
  A.GPIO_PuPd  = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOD, &A);
  };



void InitTimer(void){
    TIM_TimeBaseInitTypeDef TIM_BaseStruct;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
    TIM_BaseStruct.TIM_Prescaler = 168;
    TIM_BaseStruct.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_BaseStruct.TIM_Period = 1000;
    TIM_BaseStruct.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_BaseStruct.TIM_RepetitionCounter = 0;
    TIM_TimeBaseInit(TIM1, &TIM_BaseStruct);
}

void InitPMW(void){
    
    TIM_OCInitTypeDef TIM_OCStruct;
    TIM_OCStruct.TIM_OCMode = TIM_OCMode_PWM2;
    TIM_OCStruct.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCStruct.TIM_OCPolarity = TIM_OCPolarity_Low;
    
    TIM_OCStruct.TIM_Pulse = 0;
    
    TIM_OC1Init(TIM1, &TIM_OCStruct);
    TIM_OC1PreloadConfig(TIM1,TIM_OCPreload_Enable);
    
    TIM_OC2Init(TIM1, &TIM_OCStruct);
    TIM_OC2PreloadConfig(TIM1,TIM_OCPreload_Enable);
    
    TIM_OC3Init(TIM1, &TIM_OCStruct);
    TIM_OC3PreloadConfig(TIM1,TIM_OCPreload_Enable);
    
    TIM_CtrlPWMOutputs(TIM1, ENABLE);
}

void InitButtonInterruption(void){
  
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
  SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource0);  
  EXTI_InitTypeDef EXTI_InitStruct;
  EXTI_InitStruct.EXTI_Line = EXTI_Line0;
  EXTI_InitStruct.EXTI_LineCmd = ENABLE;
  EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Rising;
  EXTI_Init(&EXTI_InitStruct);
  
  NVIC_EnableIRQ(EXTI0_IRQn);
  
  NVIC_InitTypeDef NVIC_InitStruct;
  NVIC_InitStruct.NVIC_IRQChannel = EXTI0_IRQn;
  NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0x00;
  NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0x00;
  NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStruct);
};

void InitButton2Interruption(void){
  
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
  SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE, EXTI_PinSource1);  
  EXTI_InitTypeDef EXTI_InitStruct;
  EXTI_InitStruct.EXTI_Line = EXTI_Line1;
  EXTI_InitStruct.EXTI_LineCmd = ENABLE;
  EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Rising;
  EXTI_Init(&EXTI_InitStruct);
  
  NVIC_EnableIRQ(EXTI1_IRQn);
  
  NVIC_InitTypeDef NVIC_InitStruct;
  NVIC_InitStruct.NVIC_IRQChannel = EXTI1_IRQn;
  NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0x00;
  NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0x00;
  NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStruct);
};

void init_button2(void){
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
  GPIO_InitTypeDef B;
  B.GPIO_Pin   = GPIO_Pin_1;
  B.GPIO_Mode  = GPIO_Mode_IN;
  B.GPIO_OType = GPIO_OType_PP;
  B.GPIO_Speed = GPIO_Speed_2MHz;
  B.GPIO_PuPd  = GPIO_PuPd_UP;
  GPIO_Init(GPIOE, &B);  
};

void init_button1(void){
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
  GPIO_InitTypeDef B;
  B.GPIO_Pin   = GPIO_Pin_0;
  B.GPIO_Mode  = GPIO_Mode_IN;
  B.GPIO_OType = GPIO_OType_PP;
  B.GPIO_Speed = GPIO_Speed_2MHz;
  B.GPIO_PuPd  = GPIO_PuPd_DOWN;
  GPIO_Init(GPIOE, &B);  
};

void AddRed(void){
    int R = TIM_GetCapture1(TIM1);
      R += 5;
      if (R>1000) R=0;
      TIM_SetCompare1(TIM1, R);
};

void AddGreen(void){
    int G = TIM_GetCapture2(TIM1);
      G += 5;
      if (G>1000) G=0;
      TIM_SetCompare2(TIM1, G);
};

void AddBlue(void){
    int B = TIM_GetCapture3(TIM1);
      B += 5;
      if (B>1000) B=0;
      TIM_SetCompare3(TIM1, B);
};

void EXTI0_IRQHandler(void)
{
  if (EXTI_GetITStatus(EXTI_Line0) != RESET)
  {
      GPIO_ResetBits(GPIOD, LEDs);
      led_number++; if (led_number>2) led_number = 0;
      GPIO_SetBits(GPIOD, LED[led_number]);
      EXTI_ClearITPendingBit(EXTI_Line0);
  }
};

void EXTI1_IRQHandler(void)
{
  if (EXTI_GetITStatus(EXTI_Line1) != RESET)
    { EXTI_ClearITPendingBit(EXTI_Line1);
          switch(led_number){
           case 1: AddRed(); break;
           case 0: AddGreen(); break;
           case 2: AddBlue(); break;
            }
      
    }
};


int main(void)
{
  InitLEDs();
  InitTimer();
  InitPMW();
  init_button2();
  init_button1();
  InitButtonInterruption();
  InitButton2Interruption();
  TIM_Cmd(TIM1, ENABLE);
  led_number = 0;
  while (1)
  {
   
  }
}
