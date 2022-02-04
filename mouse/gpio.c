#include "gpio.h"

void ledInit()
{
    RCC->APB2ENR|=(1<<4);
	GPIOC->CRH|=0x00300000;
}

void ledOn()
{
    GPIOC->ODR&=~(unsigned int)0x2000;
}

void ledOff()
{
    GPIOC->ODR|=(unsigned int)0x2000;
}