#include "gpio.h"
#include "usart.h"
#include "usb_app.h"
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
	printf("\n\n\n\n--------------------------start-------------------\n");
	static unsigned char temp[8] = {0, 0, 0x59, 0x5a,0,0,0,0};
	while (1)
	{
		// ledOn();
		delay(2000);
		delay(2000);
		delay(2000);
		delay(2000);
		delay(2000);
		if(app.status==3&&app.endpoint1->isAvailable==1)
		{
			app.endpoint1->isAvailable=0;
			app.endpoint1->sendReport(temp,8);
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
