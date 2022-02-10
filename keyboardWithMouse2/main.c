#include "gpio.h"
#include "usart.h"
#include "usb_app.h"
#include "dma.h"
#include "line.h"

void delay(int i);
void HardFault_Handler()
{
	printf("\nHardFault_Handler\n");
	while(1);
}
int main()
{
	//下拉usb引脚,usb初始化后设置失效
	//目的在pc不要过早检查到usb接入
	RCC->APB2ENR |= 1 << 2;
	GPIOA->CRH |= 0b0011 << 12;
	GPIOA->CRH |= 0b0011 << 16;
	GPIOA->ODR = 0b11 << 11;
	usartInit();
	printf("usart Init\n");
	ledInit();
	ledOff();
	printf("led Init\n");
	app.init();
	printf("usb Init\n");
	delay(5000);
	//dmaInit();
	printf("dma Init\n");
	printf("\n\n\n\n--------------------------start-------------------\n");
	static unsigned char data[9] = {0, 0, 0, 0, 0, 0, 0, 0};
	unsigned char temp;
	while (1)
	{
		// ledOn();
		//发送串口数据
		// if (isDmaAvailable == 1 && dataCount != 0)
		// {
		// 	DMA_STOP;
		// 	if (pstart + dataCount >= 1000)
		// 	{
		// 		temp = 100 - pstart;
		// 		//更新地址
		// 		dmaSetAddr(usartWrite + pstart);
		// 		//更新数据大小
		// 		dmaSetNum(temp);
		// 		pstart = 0;
		// 		dataCount -= temp;
		// 	}
		// 	else
		// 	{
		// 		//更新地址
		// 		dmaSetAddr(usartWrite + pstart);
		// 		//更新数据大小
		// 		dmaSetNum(dataCount);
		// 		pstart += dataCount;
		// 		dataCount = 0;
		// 	}
		// 	DMA_START;
		// 	isDmaAvailable = 0;
		// }
		delay(200);
		//delay(2000);
		//delay(2000);
		ledOn();
		delay(200);
		//delay(2000);
		//delay(2000);
		ledOff();
		// if (app.status == 3 && app.endpoint2->isAvailable == 1)
		// {
		// 	data[0]=0x01;//鼠标
		// 	data[1]=0x00;
		// 	data[2]=0x02;
		// 	data[3]=0x02;
		// 	data[4]=0x00;
		// 	app.endpoint1->isAvailable = 0;
		// 	app.sendReport(EP2,data, 5);
		// }
		// if (app.status == 3 && app.endpoint1->isAvailable == 1)
		// {
		// 	data[0]=0x01;//键盘
		// 	data[1]=0x5a;
		// 	data[2]=0x5b;
		// 	data[3]=0x0;
		// 	data[4]=0x0;
		// 	app.endpoint1->isAvailable = 0;
		// 	app.sendReport(EP1,data, 9);
		// }
	}
}

void delay(int i)
{
	for (int j = 0; j < i; j++)
	{
		for (int k = 0; k < 500; k++)
			;
	}
}
