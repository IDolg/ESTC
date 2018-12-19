#include <stm32f4xx.h>
#include <SetColor.h>

uint8_t led_number = 0;


void InitButtonInterruption(void);
void InitButton2Interruption(void);
void EXTI0_IRQHandler(void);
void EXTI1_IRQHandler(void);
void init_button1(void);
void init_button2(void);

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
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
  GPIO_InitTypeDef B;
  B.GPIO_Pin   = GPIO_Pin_0;
  B.GPIO_Mode  = GPIO_Mode_IN;
  B.GPIO_OType = GPIO_OType_PP;
  B.GPIO_Speed = GPIO_Speed_2MHz;
  B.GPIO_PuPd  = GPIO_PuPd_DOWN;
  GPIO_Init(GPIOA, &B);  
};

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


void EXTI0_IRQHandler(void)
{
  if (EXTI_GetITStatus(EXTI_Line0) != RESET)
  {
      led_number = ChooseColor(led_number);
      EXTI_ClearITPendingBit(EXTI_Line0);
  }
};

void EXTI1_IRQHandler(void)
{
  if (EXTI_GetITStatus(EXTI_Line1) != RESET)
  { 
      AddThisColor(led_number);
      EXTI_ClearITPendingBit(EXTI_Line1);
    // for(int i =0;i<5000000;i++);
  }
};

/**
 * brief/ the program starts from this function
 * 
 */

int main(void)
{
  InitLEDs();
  InitTimer();
  InitPWM();
  init_button1();
  init_button2();
  InitButtonInterruption();
  InitButton2Interruption();
  TIM_Cmd(TIM1, ENABLE);
  SetColor(0, 0, 0);
  while (1)
  {
   
  }
}
