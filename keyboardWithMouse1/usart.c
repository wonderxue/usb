#include "usart.h"
#include "nvic_reg.h"
#include "scb_reg.h"
#include "dma.h"
#include "gpio.h"

void usartInit()
{
    unsigned int tempReg; 
    //使能时钟
    RCC->APB2ENR|=1<<14;
    RCC->APB2ENR|=1<<2;
    //gpio配置
    //TX
    tempReg=GPIOA->CRH;
    tempReg&=~(15<<4);
    tempReg|=11<<4;
    GPIOA->CRH=tempReg;
    //RX
    tempReg=GPIOA->CRH;
    tempReg&=~(15<<8);
    tempReg|=4<<8;
    GPIOA->CRH=tempReg;

    //usart配置
    //PCLK2 72MHz buad 2250k 
    USART1->BRR=(4<<4)+14;
    //8 0
    tempReg=USART1->CR1;
    tempReg&=~(1<<12);
    tempReg&=~(1<<10);
    //En Tx Rx
    tempReg|=3<<2;
    //stop_1
    USART1->CR1=tempReg;
    USART1->CR3&=~(1<<3);
    USART1->CR3|=(1<<7);

    //中断配置NVIC
    //优先组2
    SCB_AIRCR=(0x05FA<<16)|0b0000010100000000;
    //抢断优先级[7:6] 子优先级[5:4]
    NVIC->IPR[37]=0b1110<<4;
    //使能
    NVIC->ISER[37>>5]=1<<(37&0x1f);

    //配置串口中断 仅Rx
    USART1->CR1|=1<<5;

    //使能USART
    USART1->CR1|=1<<13;
}

void sendByte(unsigned char byte)
{
    USART1->DR=(unsigned int)byte;
    while(!(USART1->SR&(1<<7)));
}
unsigned char receiveByte()
{
    while (!(USART1->SR&(1<<5)));
    return (unsigned char)USART1->DR;
}

int fputc(int ch,FILE *f)
{
    // if(dataCount==100)
    // {
    //     ledOn();
    // }
    // else
    // {
    // usartWrite[(pstart+dataCount)%100]=(unsigned char)ch;
    // dataCount++;
    // }
    USART1->DR=(unsigned int)ch;
    while(!(USART1->SR&(1<<7)));
	return ch;
}

int fgetc(FILE *f)
{
    while (!(USART1->SR&(1<<5)));
    return (unsigned char)USART1->DR;
}

void USART1_IRQHandler(void)
{
    char cmd[20];
    if(USART1->SR&(1<<5))
    { 
        scanf("%s",cmd);
				printf("USART1_IRQHandler\n");
        printf("what u input is%s\n",cmd);
    }
    //清除标志
    USART1->SR&=~(1<<5);
}