#pragma once
#define GPIOC_BASE 0x40011000
#define GPIOA_BASE 0x40010800

typedef struct
{
    /* data */
    volatile unsigned int CRL;
    volatile unsigned int CRH;
    volatile unsigned int IDR;
    volatile unsigned int ODR;
    volatile unsigned int BSRR;
    volatile unsigned int BRR;
    volatile unsigned int LCKR;
}GPIO_def;

#define GPIOC ((GPIO_def *)GPIOC_BASE)
#define GPIOA ((GPIO_def *)GPIOA_BASE)
