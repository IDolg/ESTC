#include "main.h"

const uint16_t LEDs = GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;

const uint16_t LED[] = {GPIO_Pin_12, GPIO_Pin_13, GPIO_Pin_14, GPIO_Pin_15};

int direction = 1;

uint8_t stage = 0;

void InitButton(void);
void InitButtonInterruption(void);
void InitTimerInterruption(void);
void EXTI0_IRQHandler(void);
void TIM2_IRQHandler(void);
void InitLEDs(void);

void EXTI0_IRQHandler(void)
{
  if (EXTI_GetITStatus(EXTI_Line0) != RESET)
  {
      if(direction == 1) direction =-1;
      else if(direction == -1) direction =1;
      GPIO_ResetBits(GPIOD, LEDs);
      stage = 0;
      EXTI_ClearITPendingBit(EXTI_Line0);
  }
}

void TIM2_IRQHandler(void)
{
  if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET){ 
       if (direction == 1) {
           switch (stage){
           case 0: GPIO_SetBits(GPIOD, LED[0]); break;
           case 1: GPIO_ResetBits(GPIOD, LED[0]); break;
           case 2: GPIO_SetBits(GPIOD, LED[1]); break;
           case 3: GPIO_ResetBits(GPIOD, LED[1]); break;
           case 4: GPIO_SetBits(GPIOD, LED[2]); break;
           case 5: GPIO_ResetBits(GPIOD, LED[2]); break;
           case 6: GPIO_SetBits(GPIOD, LED[3]); break;
           case 7: GPIO_ResetBits(GPIOD, LED[3]); break; }
       }
      else if (direction == -1){
          switch (stage){
           case 0: GPIO_SetBits(GPIOD, LED[3]); break;
           case 1: GPIO_ResetBits(GPIOD, LED[3]); break;
           case 2: GPIO_SetBits(GPIOD, LED[2]); break;
           case 3: GPIO_ResetBits(GPIOD, LED[2]); break;
           case 4: GPIO_SetBits(GPIOD, LED[1]); break;
           case 5: GPIO_ResetBits(GPIOD, LED[1]); break;
           case 6: GPIO_SetBits(GPIOD, LED[0]); break;
           case 7: GPIO_ResetBits(GPIOD, LED[0]); break;}
           
  }
  stage += 1; if (stage>7) stage=0;
  TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
}
}

void InitLEDs(void){
 
  GPIO_InitTypeDef GPIO_InitStructure;
    
/* Enable peripheral clock for LEDs port */
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);

  /* Init LEDs */
  GPIO_InitStructure.GPIO_Pin = LEDs;
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOD, &GPIO_InitStructure);    
};

void InitButton(void){
 
  GPIO_InitTypeDef GPIO_InitStructure;
    
/* Enable peripheral clock for button port */
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
  /* Init button */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
  GPIO_Init(GPIOA, &GPIO_InitStructure);    
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

void InitTimerInterruption(void){
  
    TIM_TimeBaseInitTypeDef tim_struct;
  
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
  tim_struct.TIM_Period = 1000 - 1;
  tim_struct.TIM_Prescaler = 42000 - 1;
  tim_struct.TIM_ClockDivision = 0;
  tim_struct.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseInit(TIM2, &tim_struct);
  TIM_Cmd(TIM2, ENABLE);

  TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
  
  NVIC_InitTypeDef nvic_struct;
  nvic_struct.NVIC_IRQChannel = TIM2_IRQn;
  nvic_struct.NVIC_IRQChannelPreemptionPriority = 0;
  nvic_struct.NVIC_IRQChannelSubPriority = 1;
  nvic_struct.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&nvic_struct);

}

int main(void)
{
  direction = 1;  
  stage = 0;
  
  InitLEDs();
  InitButton();
  InitButtonInterruption();
  InitTimerInterruption();
  
  while (1)
  {
      
  }
}
