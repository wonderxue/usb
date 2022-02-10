#include "usb_core.h"

const USB_DeviceRequest *devReq;
static void usbReadPMA(unsigned short addr, void *buffer, int length);
void usbWritePMA(unsigned short addr, const void *buffer, int length);
#pragma region
#pragma region
static void usbStandardGetStatus();
static void usbStandardGetDescriptor();
static void usbStandardGetConfiguration();
static void usbStandardGetInterface();
static void usbStandardSynchFrame();

static void usbStandardClearFeature();
static void usbStandardSetFeature();
static void usbStandardSetAddress();
static void usbStandardSetDescriptor();
static void usbStandardSetConfiguration();
static void usbStandardSetInterface();
#pragma endregion
#pragma region
static void usbClassSetIdle();
#pragma endregion

#pragma endregion

#pragma region
static void usbTransferHandler();
static void usbResetHandler();
#pragma endregion

#pragma region
static void usbReadPMA(unsigned short addr, void *buffer, int length)
{
    const unsigned short *pPMA = USB_PMA + addr * 2;
    unsigned short *pbuf = buffer;
    while (length >= 2)
    {
        *pbuf = *pPMA;
        // usb 16位访问
        pPMA += 2;
        // APB 32位访问
        pbuf++;
        length -= 2;
    }
    if (length == 1)
    {
        *(unsigned char *)pbuf = *(unsigned char *)pPMA;
    }
}
void usbWritePMA(unsigned short addr, const void *buffer, int length)
{
    unsigned short *pPMA = USB_PMA + addr * 2;
    const unsigned short *pbuf = buffer;
    while (length >= 2)
    {
        *pPMA = *pbuf;
        // usb 16位访问
        pPMA += 2;
        // APB 32位访问
        pbuf++;
        length -= 2;
    }
    if (length == 1)
    {
        *(unsigned char *)pPMA = *(unsigned char *)pbuf;
    }
}
#pragma endregion

void usbInit()
{
    devReq = app.pread;
    //配置分频1.5，开启时钟
    RCC->CFGR &= ~(1 << 22);
    RCC->APB1ENR |= 1 << 23;

    //中断NVIC
    //抢断优先级[7:6] 子优先级[5:4]
    NVIC->IPR[20] = 0b0001 << 4;
    //使能
    NVIC->ISER[20 >> 5] = 1 << (20 & 0x1f);

    //复位USB,退出断电，退出低功耗，取消强制挂起，取消唤醒请求
    USB->CNTR = 1;
    //取消复位
    USB->CNTR = 0;
    //等待复位完成
    while ((USB->ISTR & (1 << 10)) == 0)
        ;
    //清除所有中断
    USB->ISTR = 0;

    //初始化端点0，1
    //对于程序而言buffer起始地址为USB_USB_BufferDescriptor[2];
    //对于usb模块而言buffer起始地址为偏移地址的一半;
    //真实程序偏移4*2*2*2=32
    int offset = 32;
    USB_BufferDescriptor[0].ADDR_TX = offset;
    USB_BufferDescriptor[0].COUNT_TX = 60;
    USB_BufferDescriptor[0].ADDR_RX = offset + 60;
    USB_BufferDescriptor[0].COUNT_RX = 25 << 10;

    USB_BufferDescriptor[1].ADDR_TX = offset + 110;
    USB_BufferDescriptor[1].COUNT_TX = 10;
    USB_BufferDescriptor[1].ADDR_RX = offset + 120;
    USB_BufferDescriptor[1].COUNT_RX = 5 << 10;

    USB->CNTR = 0b11111111 << 8;
}
static void usbEPxSend(unsigned char EPx, unsigned char maxPacketSize)
{
    if (app.dataToSend == -1)
    {
        return;
    }
    if (app.dataToSend == 0)
    {
        USB_BufferDescriptor[EPx].COUNT_TX = 0;
        app.dataToSend = -1;
    }
    else
    {
        //发送字节数不超过端点maxpacksize
        USB_BufferDescriptor[EPx].COUNT_TX = app.dataToSend > maxPacketSize ? maxPacketSize : app.dataToSend;
        // 发送的字节数不能超过主机要求的最大长度
        USB_BufferDescriptor[EPx].COUNT_TX = USB_BufferDescriptor[EPx].COUNT_TX > devReq->wLength ? devReq->wLength : USB_BufferDescriptor[EPx].COUNT_TX; // 只修改发送长度, 内容原封不动, 切记!!!!
        // 比如在请求字符串语言列表时, 待发送的数据量是str->bLength=4
        // 如果主机要求最大只能发送devreq->wLength=2字节, 则数据内容str->bLength应该仍为4, 不能改成2
        usbWritePMA(USB_BufferDescriptor[EPx].ADDR_TX, app.pwrite, USB_BufferDescriptor[EPx].COUNT_TX);
        if (app.dataToSend >= maxPacketSize)
        {
            app.dataToSend -= USB_BufferDescriptor[EPx].COUNT_TX;
            app.pwrite += USB_BufferDescriptor[EPx].COUNT_TX;
        }
        else
        {
            app.dataToSend = -1;
        }
    }
    SET_STAT_TX(GET_EPxR(EPx), VALID); // STAT_TX设为VALID, 允许发送; STAT_RX保持NAK
}

#pragma region
static void usbStandardGetStatus()
{
    DEBUG("usbStandardGetStatus\n");
    USB_BufferDescriptor[0].COUNT_TX = 0;
    SET_STAT_TX(USB->EP0R, VALID);
}
static void usbStandardGetDescriptor()
{
    DEBUG("usbStandardGetDescriptor\n");
    switch ((devReq->bmRequestType & RECIPIENT_MASK) >> RECIPIENT_BIT)
    {
    case DEVICE_RECIPIENT:
        switch ((devReq->wValue & DESCRIPTOR_TYPE_MASK) >> DESCRIPTOR_TYPE_BIT)
        {
        case DEVICE_DESCRIPTOR:
            app.endpoint0->getDeviceDescriptor();
            app.dataToSend = app.endpoint0->deviceDescriptor->bLength;
            break;
        case CONFIG_DESCRIPTOR:
            app.endpoint0->getConfigurationDescriptor();
            app.dataToSend = app.endpoint0->configDescriptor->wTotalLength;
            break;
        case STRING_DESCRIPTOR:
            app.endpoint0->getStringDescriptor(devReq->wValue, devReq->wIndex);
            app.dataToSend = app.endpoint0->stringDescriptor->bLength;
            break;
        default:
            DEBUG("ERR1\n");
            return;
        }
        break;
    case INTERFACE_RECIPIENT:
        app.dataToSend = app.endpoint0->getReportDescriptor();
        break;
    case ENDPOINT_RECIPIENT:
        break;
    case OTHER_RECIPIENT:
        break;
    default:
        DEBUG("ERR2\n");
        break;
    }
    usbEPxSend(EP0R, app.endpoint0->maxPacketSize);
}

static void usbStandardGetConfiguration()
{
    DEBUG("usbStandardGetConfiguration\n");
    USB_BufferDescriptor[0].COUNT_TX = 0;
    SET_STAT_TX(USB->EP0R, VALID);
}
static void usbStandardGetInterface()
{
    DEBUG("usbStandardGetInterface\n");
    USB_BufferDescriptor[0].COUNT_TX = 0;
    SET_STAT_TX(USB->EP0R, VALID);
}
static void usbStandardSynchFrame()
{
    DEBUG("usbStandardSynchFrame\n");
    USB_BufferDescriptor[0].COUNT_TX = 0;
    SET_STAT_TX(USB->EP0R, VALID);
}
static void usbStandardClearFeature()
{
    DEBUG("usbStandardClearFeature\n");
    USB_BufferDescriptor[0].COUNT_TX = 0;
    SET_STAT_TX(USB->EP0R, VALID);
    SET_STAT_TX(USB->EP1R, NAK);
}
static void usbStandardSetFeature()
{
    DEBUG("usbStandardSetFeature\n");
    USB_BufferDescriptor[0].COUNT_TX = 0;
    SET_STAT_TX(USB->EP0R, VALID);
}
static void usbStandardSetAddress()
{
    DEBUG("usbStandardSetAddress\n");
    app.endpoint0->setAddress(devReq->wValue);
    USB_BufferDescriptor[0].COUNT_TX = 0;
    SET_STAT_TX(USB->EP0R, VALID);
    app.status = 1;
}
static void usbStandardSetDescriptor()
{
    DEBUG("usbStandardSetDescriptor\n");
    USB_BufferDescriptor[0].COUNT_TX = 0;
    SET_STAT_TX(USB->EP0R, VALID);
}
static void usbStandardSetConfiguration()
{
    DEBUG("usbStandardSetConfiguration\n");
    USB_BufferDescriptor[0].COUNT_TX = 0;
    SET_STAT_TX(USB->EP0R, VALID);
    app.status = 3;
}
static void usbStandardSetInterface()
{
    DEBUG("usbStandardSetInterface\n");
    USB_BufferDescriptor[0].COUNT_TX = 0;
    SET_STAT_TX(USB->EP0R, VALID);
}
#pragma endregion
#pragma region
static void usbClassSetIdle()
{
    DEBUG("usbClassSetIdle\n");
    USB_BufferDescriptor[0].COUNT_TX = 0;
    SET_STAT_TX(USB->EP0R, VALID);
}
#pragma endregion
#pragma region
static void usbSetup()
{
    DEBUG("\n--SETUP--\n");
    int length = 0;
    length = USB_BufferDescriptor[0].COUNT_RX & (0x03ff);
    if (length > 0)
    {
        usbReadPMA(USB_BufferDescriptor[0].ADDR_RX, app.pread, length);
        DUMP_BUFFER(app.pread, length);
        if ((devReq->bmRequestType & TRANSFER_DIR_MASK) >> TRANSFER_DIR_BIT == IN_DIR)
        {
            switch ((devReq->bmRequestType & REQUEST_TYPE_MASK) >> REQUEST_TYPE_BIT)
            {
            case STANDARD_REQ:
                switch (devReq->bRequest)
                {
                case GET_STATUS:
                    usbStandardGetStatus();
                    break;
                case GET_DESCRIPTOR:
                    usbStandardGetDescriptor();
                    break;
                case GET_CONFIGURATION:
                    usbStandardGetConfiguration();
                    break;
                case GET_INTERFACE:
                    usbStandardGetInterface();
                    break;
                case SYNCH_FRAME:
                    usbStandardSynchFrame();
                    break;
                default:
                    DEBUG("ERR3\n");
                    break;
                }
                break;
            case CLASS_REQ:
                break;
            case VENDOR_REQ:
                break;
            default:
                DEBUG("ERR4\n");
                break;
            }
        }
        else
        {
            switch ((devReq->bmRequestType & REQUEST_TYPE_MASK) >> REQUEST_TYPE_BIT)
            {
            case STANDARD_REQ:
                switch (devReq->bRequest)
                {
                case CLEAR_FEATURE:
                    usbStandardClearFeature();
                    break;
                case SET_FEATURE:
                    usbStandardSetFeature();
                    break;
                case SET_ADDRESS:
                    usbStandardSetAddress();
                    break;
                case SET_DESCRIPTOR:
                    usbStandardSetDescriptor();
                    break;
                case SET_CONFIGURATION:
                    usbStandardSetConfiguration();
                    break;
                case SET_INTERFACE:
                    usbStandardSetInterface();
                    break;
                default:
                    DEBUG("ERR5\n");
                    break;
                }
                break;
            case CLASS_REQ:
                switch (devReq->bRequest)
                {
                case SET_IDLE:
                    usbClassSetIdle();
                    break;
                default:
                    DEBUG("ERR6\n");
                    break;
                }
                break;
            case VENDOR_REQ:
                break;
            default:
                DEBUG("ERR7\n");
                break;
            }
        }
    }
    else
    {
        //空包
        DEBUG("no\n");
    }
}

static void usbEPxOut(unsigned char epId)
{
    DEBUG("\n--EP%dOut--\n", epId);
    int length = 0;
    if (epId == 0)
    {
        length = USB_BufferDescriptor[0].COUNT_RX & (0x03ff);
        if (length > 0)
        {
            usbReadPMA(USB_BufferDescriptor[0].ADDR_RX, app.pread, length);
            DUMP_BUFFER(app.pread, length);
        }
        else
        {
            //空包
            DEBUG("no\n");
        }
    }
    if (epId == 1)
    {
        length = USB_BufferDescriptor[1].COUNT_RX & (0x03ff);
        if (length > 0)
        {
            usbReadPMA(USB_BufferDescriptor[1].ADDR_RX, app.pread, length);
            DUMP_BUFFER(app.pread, length);
            if (app.pread[0] == 0x01)
            {
                if (app.pread[1] == 0x03)
                {
                    ledOn();
                }
                else if (app.pread[1] == 0x01)
                {
                    ledOff();
                }
            }
        }
        else
        {
            //空包
            DEBUG("no\n");
        }
        SET_STAT_RX(GET_EPxR(epId), VALID);
    }
}
static void usbEPxIn(unsigned char epId)
{
    DEBUG("\n--EP%dIn--\n", epId);
    int length = 0;
    if (epId == 0)
    {
        //继续发送未发完数据
        usbEPxSend(EP0R, app.endpoint0->maxPacketSize);
        if (app.status == 1)
        {
            USB->DADDR |= app.addr;
            app.status = 2;
            DEBUG("ADDR SET\n");
        }
        SET_STAT_RX(USB->EP0R, VALID);
    }
    if (epId == 1)
    {
        //继续发送未发完数据
        usbEPxSend(EP1R, app.endpoint1->maxPacketSize);
        app.endpoint1->isAvailable = 1;
    }
}
static void usbTransferHandler()
{
    unsigned char epId;
    epId = USB->ISTR & (0x0f);
    if (epId == 0)
    {
        if (USB->ISTR & (1 << 4))
        {
            // setup
            if (USB->EP0R & (1 << 11))
            {
                //清除正确接收中断
                CLR_CTR_RX(USB->EP0R);
                usbSetup();
            }
            else
            {
                //清除正确接收中断
                CLR_CTR_RX(USB->EP0R);
                usbEPxOut(0);
            }
        }
        else
        {
            //清除正确发送中断
            CLR_CTR_TX(USB->EP0R);
            usbEPxIn(0);
        }
    }

    if (epId != 0)
    {
        if (USB->ISTR & (1 << 4))
        {
            //清除正确接收中断
            CLR_CTR_RX(GET_EPxR(epId));
            usbEPxOut(epId);
        }
        else
        {
            //清除正确发送中断
            CLR_CTR_TX(GET_EPxR(epId));
            usbEPxIn(epId);
        }
    }
}
static void usbResetHandler()
{
    DEBUG("\n--\n");
    //模块使能,地址为0
    app.addr = 0;
    app.status = 0;

    //端点0使能
    USB->EP0R = 0b0011001000110000;
    //端点1失能
    USB->EP1R = 0b0011011000110001;
    USB->DADDR = 1 << 7 | app.addr;
}
#pragma endregion
void USB_LP_CAN1_RX0_IRQHandler()
{
    if (USB->ISTR & (1 << 15))
    {
        DEBUG("correct\n");
        usbTransferHandler();
    }
    if (USB->ISTR & (1 << 14))
    {
        DEBUG("overflow\n");
        USB->ISTR &= ~(1 << 14);
    }
    if (USB->ISTR & (1 << 13))
    {
        //DEBUG("error\n");
        USB->ISTR &= ~(1 << 13);
    }
    if (USB->ISTR & (1 << 12))
    {
        DEBUG("wakeup\n");
        USB->ISTR &= ~(1 << 12);
    }
    if (USB->ISTR & (1 << 11))
    {
        //DEBUG("susp\n");
        USB->ISTR &= ~(1 << 11);
    }
    if (USB->ISTR & (1 << 10))
    {
        DEBUG("reset\n");
        USB->ISTR &= ~(1 << 10);
        usbResetHandler();
    }
    if (USB->ISTR & (1 << 9))
    {
        // DEBUG("sof;");
        USB->ISTR &= ~(1 << 9);
    }
    if (USB->ISTR & (1 << 8))
    {
        //DEBUG("esof;");
        USB->ISTR &= ~(1 << 8);
        USB->CNTR &= ~(1 << 2);
        USB->CNTR &= ~(1 << 3);
    }
}