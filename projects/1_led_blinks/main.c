#include "main.h"
#include <stm32f4xx.h>
#include <stm32f4xx_rcc.h>
#include <stm32f4xx_gpio.h>
#include <misc.h>

void init_button(void);
void init_LED(void);
void pause(void);
int changeDirection(int dir);

int direction = 1;
void blink(int dir);

const uint32_t LED[4] = { GPIO_Pin_12, GPIO_Pin_13, GPIO_Pin_14, GPIO_Pin_15 };

const uint32_t LEDs = GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15 ;

uint8_t led_number =0;
uint32_t delay = 5000000;

void pause(void){
    for (uint32_t i=0;i < delay ;i++);
};

void init_button(void){
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
  GPIO_InitTypeDef B;
  B.GPIO_Pin   = GPIO_Pin_0;
  B.GPIO_Mode  = GPIO_Mode_IN;
  B.GPIO_OType = GPIO_OType_PP;
  B.GPIO_Speed = GPIO_Speed_2MHz;
  B.GPIO_PuPd  = GPIO_PuPd_UP;
  GPIO_Init(GPIOD, &B);  
};

void init_LED(void){
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
  GPIO_InitTypeDef A;
  A.GPIO_Pin = LEDs ;
  A.GPIO_Mode  = GPIO_Mode_OUT;
  A.GPIO_OType = GPIO_OType_PP;
  A.GPIO_Speed = GPIO_Speed_100MHz;
  A.GPIO_PuPd  = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOD, &A);
};

void blink(int dir){
    if (dir == 1) {
    for(int i=0; i<4; i++)
           {
            GPIO_SetBits(GPIOD, LED[i]);
            for(int j=0; j<1000000; j++);
            GPIO_ResetBits(GPIOD, LED[i]);
           }
      }else if (dir == -1){
        for(int i=3; i>-1; i--)
           {
            GPIO_SetBits(GPIOD, LED[i]);
            for(int j=0; j<1000000; j++);
            GPIO_ResetBits(GPIOD, LED[i]);
  
    }
  }
        
    };
    
int changeDirection(int dir){
    if (dir==1) return -1;
    if (dir==-1) return 1;
    else return 0;
};    

int main(void)
{
  
  init_button();
  init_LED();
  
  while (1)
  {
    blink(direction);
    if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0) != RESET){
    direction = changeDirection(direction); };    
  }
  
}
