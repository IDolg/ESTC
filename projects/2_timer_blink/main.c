#include "main.h"

const uint16_t LEDs = GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
const uint32_t LED[4] = { GPIO_Pin_12, GPIO_Pin_13, GPIO_Pin_14, GPIO_Pin_15 };

uint8_t LED_number = 0;

uint8_t blink(uint8_t);
void InitLEDs(void);
void InitTimer(void);

uint8_t blink(uint8_t n){
      GPIO_SetBits(GPIOD, LED[n]);
      for(int i=0; i<500000; i++);
      GPIO_ResetBits(GPIOD, LED[n]);
      n++;
      if (n>3) n=0;
      return n;
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
}

void InitTimer(void){
    
  TIM_TimeBaseInitTypeDef tim_struct;
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
  tim_struct.TIM_Period = 2000 - 1;
  tim_struct.TIM_Prescaler = 42000 - 1;
  tim_struct.TIM_ClockDivision = 0;
  tim_struct.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseInit(TIM2, &tim_struct);
  TIM_Cmd(TIM2, ENABLE);
};

int main(void)
{

  InitLEDs();
  InitTimer();
  
  while (1)
  {
    if (TIM_GetCounter(TIM2)==0) LED_number = blink(LED_number);
  }
}
