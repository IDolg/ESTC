#include "SetColor.h"

/**
 *  @brief this function initializes LEDs
 */

void InitLEDs(void)
{
    GPIO_InitTypeDef gpio_init;
    GPIO_StructInit(&gpio_init);

    /* Enable clock */
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

    GPIO_PinAFConfig(GPIOA, GPIO_PinSource8, GPIO_AF_TIM1);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_TIM1);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_TIM1);

    /* Initialize GPIOs for an alternative function (TIM1 PWM output) */
    gpio_init.GPIO_Pin   = GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10;
    gpio_init.GPIO_Mode  = GPIO_Mode_AF;
    gpio_init.GPIO_OType = GPIO_OType_PP;
    gpio_init.GPIO_Speed = GPIO_Speed_100MHz;
    gpio_init.GPIO_PuPd  = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOA, &gpio_init);
    
    // init LEDs on the board
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
    GPIO_InitTypeDef A;
    A.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_14 | GPIO_Pin_15 ;
    A.GPIO_Mode  = GPIO_Mode_OUT;
    A.GPIO_OType = GPIO_OType_PP;
    A.GPIO_Speed = GPIO_Speed_100MHz;
    A.GPIO_PuPd  = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOD, &A);
};


/**
 *  @brief this function initializes a timer
 */

void InitTimer(void)
{
    TIM_TimeBaseInitTypeDef tim_init;

    TIM_TimeBaseStructInit(&tim_init);

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);

    tim_init.TIM_Prescaler         = 168 - 1; /* One microsecond resolution */
    tim_init.TIM_CounterMode       = TIM_CounterMode_Up;
    tim_init.TIM_Period            = 1000 / 25 - 1;
    tim_init.TIM_ClockDivision     = TIM_CKD_DIV1;
    tim_init.TIM_RepetitionCounter = 0;
    TIM_TimeBaseInit(TIM1, &tim_init);
}


/**
 *  @brief this function initializes pulse width modulation
 */

void InitPWM(void){
    
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
};


/**
 *  @brief this function sets a (r,g,b) color
 *  @param [in] R sets Red brightness from0 to 1000
 *  @param [in] G sets Green brightness from0 to 1000
 *  @param [in] B sets Blue brightness from0 to 1000
 */

void SetColor(int R, int G, int B)
{
    TIM_SetCompare1(TIM1, R );
    TIM_SetCompare2(TIM1, G );
    TIM_SetCompare3(TIM1, B );
};


/**
 *  @brief this function changes the color's number
 *  @param [in] led_number this number from 0 to 2 shows which color should be added to a lamp
 *  @param [out] led_number this number from 0 to 2 shows which color should be added to a lamp  
 * 
 * This function lights a LED with needed number on a board and sets the variable led_number to a corresponding value 
 */

uint8_t ChooseColor(uint8_t led_number){
    GPIO_ResetBits(GPIOD, GPIO_Pin_12 | GPIO_Pin_14 | GPIO_Pin_15 );
    led_number++; if (led_number>2) led_number = 0;
    switch(led_number){
        case 0: GPIO_SetBits(GPIOD, GPIO_Pin_12); break;
        case 1: GPIO_SetBits(GPIOD, GPIO_Pin_14); break;
        case 2: GPIO_SetBits(GPIOD, GPIO_Pin_15); break;
    };
    return led_number;
};


/**
 *  @brief this function adds alittle of color №n to the lamp
 *  @param [in] led_number this number from 0 to 2 shows which color should be added to a lamp
 * 
 * This function adds a little of color №N to the lamp. N can be from 0 to 2 which means Red, Green or Blue. 
 */

void AddThisColor(uint8_t led_number){
    switch(led_number){
           case 1: AddRed(); break;
           case 0: AddGreen(); break;
           case 2: AddBlue(); break;
            }
};


/**
 *  @brief this function Adds a little red
 */

void AddRed(void){
      uint16_t R = TIM_GetCapture2(TIM1);
      R += 1;
      if (R>1000) R=0;
      TIM_SetCompare1(TIM1, R);
};


/**
 *  @brief this function Adds a little green
 */


void AddGreen(void){
      uint16_t G = TIM_GetCapture2(TIM1);
      G += 1;
      if (G>1000) G=0;
      TIM_SetCompare2(TIM1, G);
};


/**
 *  @brief this function Adds a little blue
 */


void AddBlue(void){
      uint16_t B = TIM_GetCapture3(TIM1);
      B += 1;
      if (B>1000) B=0;
      TIM_SetCompare3(TIM1, B);
};
 
