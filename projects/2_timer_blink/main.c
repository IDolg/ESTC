#include "main.h"

const uint16_t LEDs = GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
const uint32_t LED[4] = { GPIO_Pin_12, GPIO_Pin_13, GPIO_Pin_14, GPIO_Pin_15 };

int LED_number = 0;
int direction = 1;

uint8_t blink(int n, int direction);
void InitLEDs(void);
void InitTimer(void);
void InitButton(void);
int changeDirection(int);

uint8_t blink(int n, int dir){
      if (dir==1){
      GPIO_SetBits(GPIOD, LED[n]);
      for(int i=0; i<500000; i++);
      GPIO_ResetBits(GPIOD, LED[n]);
      n++;
      if (n>3) n=0;
      }else{
      GPIO_SetBits(GPIOD, LED[n]);
      for(int i=0; i<500000; i++);
      GPIO_ResetBits(GPIOD, LED[n]);
      n--;
      if (n<0) n=3;
    };
      return n;
};

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

void InitButton(void){
 
  GPIO_InitTypeDef GPIO_InitStructure;
    
/* Enable peripheral clock for button port */
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
  /* Init button */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_DOWN;
  GPIO_Init(GPIOA, &GPIO_InitStructure);    
};

int changeDirection(int dir){
    if (dir==1) return -1;
    else return 1;
};

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
  InitButton();
  direction = 1;
  
  while (1)
  {
    if (TIM_GetCounter(TIM2)==0) LED_number = blink(LED_number, direction);
    if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0) != RESET){
    direction = changeDirection(direction); };
  }
}
