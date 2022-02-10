#pragma once

#define USART1_BASE 0x40013800

typedef struct 
{
    volatile unsigned int SR;
    volatile unsigned int DR;
    volatile unsigned int BRR;
    volatile unsigned int CR1;
    volatile unsigned int CR2;
    volatile unsigned int CR3;
    volatile unsigned int GTPR;
}USART_def;

#define USART1 ((USART_def *)USART1_BASE)
