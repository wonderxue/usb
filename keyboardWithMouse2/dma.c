#include "dma.h"

unsigned char usartWrite[1000];
unsigned char pstart = 0;
unsigned char dataCount=0;
unsigned char isDmaAvailable = 1;
void dmaInit()
{
    //开启时钟
    RCC->AHBENR |= (1 << 0);

    DMA->CCR4 |= 1 << 7 | 1 << 4 | 1 << 1; //不循环
    DMA->CPAR4 = &(USART1->DR);

    //抢断优先级[7:6] 子优先级[5:4]
    NVIC->IPR[14] = 0b0000 << 4;
    //使能
    NVIC->ISER[14 >> 5] = 1 << (14 & 0x1f);
}

void dmaSetNum(unsigned short num)
{
    DMA->CNDTR4 = num;
}

void dmaSetAddr(unsigned int addr)
{
    DMA->CMAR4 = addr;
}

void DMA1_Channel4_IRQHandler()
{
    if(DMA->ISR&(1<<13))
    {
        DMA->IFCR|=1<<13;
    }
    unsigned char temp;
    
    if(dataCount==0)
    {
        isDmaAvailable = 1;
        return;
    }
    DMA_STOP;
    if (pstart + dataCount >= 1000)
    {
        temp = 1000 - pstart;
        //更新地址
        dmaSetAddr(usartWrite+pstart);
        //更新数据大小
        dmaSetNum(temp);
        pstart = 0;
        dataCount -= temp;
    }
    else
    {
        //更新地址
        dmaSetAddr(usartWrite+pstart);
        //更新数据大小
        dmaSetNum(dataCount);
        pstart += dataCount;
        dataCount = 0;
    }
    DMA_START;
    isDmaAvailable = 0;
}
