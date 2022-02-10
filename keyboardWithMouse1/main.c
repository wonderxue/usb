#include "gpio.h"
#include "usart.h"
#include "usb_app.h"
#include "dma.h"
#include "line.h"
void usbInit(void);
void delay(int i);
int flag = 0;
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
	dmaInit();
	printf("dma Init\n");
	printf("\n\n\n\n--------------------------start-------------------\n");
	static unsigned char data[9] = {0, 0, 0, 0, 0, 0, 0, 0};
	unsigned char temp;
	while (1)
	{
		// ledOn();
		//发送串口数据
		if (isDmaAvailable == 1 && dataCount != 0)
		{
			DMA_STOP;
			if (pstart + dataCount >= 1000)
			{
				temp = 100 - pstart;
				//更新地址
				dmaSetAddr(usartWrite + pstart);
				//更新数据大小
				dmaSetNum(temp);
				pstart = 0;
				dataCount -= temp;
			}
			else
			{
				//更新地址
				dmaSetAddr(usartWrite + pstart);
				//更新数据大小
				dmaSetNum(dataCount);
				pstart += dataCount;
				dataCount = 0;
			}
			DMA_START;
			isDmaAvailable = 0;
		}
		delay(2000);
		delay(2000);
		delay(2000);
		if (app.status == 3 && app.endpoint1->isAvailable == 1)
		{
			data[0]=0x02;//鼠标
			data[1]=0x00;
			data[2]=0x02;
			data[3]=0x02;
			data[4]=0x00;
			app.endpoint1->isAvailable = 0;
			app.endpoint1->sendReport(data, 5);
		}
		delay(2000);
		delay(2000);
		if (app.status == 3 && app.endpoint1->isAvailable == 1)
		{
			data[0]=0x01;//键盘
			data[1]=0x59;
			data[2]=0x5d;
			data[3]=0x5d;
			data[4]=0x5d;
			app.endpoint1->isAvailable = 0;
			app.endpoint1->sendReport(data, 9);
		}
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
