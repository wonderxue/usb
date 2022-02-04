#include "usb_app.h"
#include "usb_reg.h"
//stm32 小端模式，因此正好与usb的lsb在前负负得正
unsigned char readBuffer[READ_MAX_BUFFER_SIZE];
unsigned char writeBuffer[WRITE_MAX_BUFFER_SIZE];
extern void usbWritePMA(unsigned short addr, const void *buffer, int length);
extern void usbInit();
extern unsigned char ReportDescriptor[REPORTDESCRIPTOR_SIZE];
extern USB_App app;

wchar_t *wcscpy(wchar_t *s1, const wchar_t *s2) // 复制UTF-16字符串
{
    wchar_t *r = s1;
    while ((*r++ = *s2++) != 0)
        ;
    return s1;
}

size_t wcslen(const wchar_t *s) // 求UTF-16字符串的长度
{
    size_t n = 0;
    while (*s++)
        n++;
    return n;
}

void init()
{
    //endpoint初始化
    usbInit();
}
void setAddress(unsigned short address)
{
    app.addr = address;
}
void getDeviceDescriptor()
{
    USB_DeviceDescriptor *pdevice = app.endpoint0->deviceDescriptor;
    pdevice->bLength = sizeof(USB_DeviceDescriptor);
    pdevice->bDescriptorType = DEVICE_DESCRIPTOR;
    pdevice->bcdUSB = 0x0110; // USB 2.0
    pdevice->bDeviceClass = 0;
    pdevice->bDeviceSubClass = 0;
    pdevice->bDeviceProtocol = 0;
    pdevice->bMaxPacketSize0 = 64;
    pdevice->idVendor = 0x1022;  // STMicroelectronics (http://www.linux-usb.org/usb.ids)
    pdevice->idProduct = 0x0007; // STM microSD Flash pdevice
    pdevice->bcdDevice = 0x0081;
    pdevice->iManufacturer = 1;      // 制造商名称字符串序号
    pdevice->iProduct = 2;           // 产品名字符串序号
    pdevice->iSerialNumber = 3;      // 产品序列号字符串序号
    pdevice->bNumConfigurations = 1; // 配置数
}

void getConfigurationDescriptor()
{
    USB_ConfigurationDescriptor *pconfig = app.endpoint0->configDescriptor;
    USB_InterfaceDescriptor *pinterface = pconfig + 1;
    USB_HIDDescriptor *phid = pinterface + 1;
    USB_EndpointDescriptor *pendpoint = phid + 1;
    pconfig->bLength = sizeof(USB_ConfigurationDescriptor);
    pconfig->bDescriptorType = CONFIG_DESCRIPTOR;
    pconfig->wTotalLength = sizeof(USB_ConfigurationDescriptor) + sizeof(USB_InterfaceDescriptor) + sizeof(USB_HIDDescriptor) + sizeof(USB_EndpointDescriptor);
    pconfig->bNumInterfaces = 1;      // 接口数
    pconfig->bConfigurationValue = 1; // 此配置的编号
    pconfig->iConfiguration = 5;      // 配置名字符串序号(0表示没有)
    pconfig->bmAttributes = 0x80;     // self-powered
    pconfig->bMaxPower = 32;          // 最大电流: 100mA

    pinterface->bLength = sizeof(USB_InterfaceDescriptor);
    pinterface->bDescriptorType = INTERFACE_DESCRIPTOR;
    pinterface->bInterfaceNumber = 1;      // 此接口的编号
    pinterface->bAlternateSetting = 1;     // 可用的备用接口编号
    pinterface->bNumEndpoints = 1;         // 除了端点0外, 此接口还需要的端点数
    pinterface->bInterfaceClass = 0x03;    // HID
    pinterface->bInterfaceSubClass = 0x01; // BIOS
    pinterface->bInterfaceProtocol = 0x02; // 鼠标
    pinterface->iInterface = 4;            // 接口名称字符串序号

    phid->bLength = sizeof(USB_HIDDescriptor);
    phid->bDescriptorType = HID_DESCRIPTOR;
    phid->bcdHID = 0x0110;
    phid->bCountryCode = 0x21;
    phid->bNumDescriptors = 1;
    phid->bDescriptorType0 = REPORT_DESCRIPTOR;
    phid->wDescriptorLength0 = REPORTDESCRIPTOR_SIZE; //下级描述符长度

    pendpoint[0].bLength = sizeof(USB_EndpointDescriptor);
    pendpoint[0].bDescriptorType = ENDPOINT_DESCRIPTOR;
    pendpoint[0].bEndpointAddress = 0x81; // IN, address 1
    pendpoint[0].bmAttributes = 0x03;     // interrupt
    pendpoint[0].wMaxPacketSize = 0x0004;
    pendpoint[0].bInterval = 0x0a; // 查询间隔
}

void getStringDescriptor(const unsigned short wValue, const unsigned short wIndex)
{
    USB_StringDescriptor *pstring = app.endpoint0->stringDescriptor;
    pstring->bDescriptorType = STRING_DESCRIPTOR;
    if (wIndex == 0x409) // 字符串英文内容
    {
        // 字符串的编码为UTF-16
        switch (wValue & 0xff) // 低8位为字符串序号
        {
        case 1:
            DEBUG("string 1\n");
            wcscpy((wchar_t *)pstring->wData, L"Hello Manufacturer!");
            break;
        case 2:
            DEBUG("string 2\n");
            wcscpy((wchar_t *)pstring->wData, L"Hello Product!");
            break;
        case 3:
            DEBUG("string 3\n");
            wcscpy((wchar_t *)pstring->wData, L"Hello SerialNumber!");
            break;
        case 4:
            DEBUG("string 4\n");
            wcscpy((wchar_t *)pstring->wData, L"Hello Interface!");
            break;
        case 5:
            DEBUG("string 5\n");
            wcscpy((wchar_t *)pstring->wData, L"Hello Config!");
            break;
        default:
            DEBUG("STR_%d\n", wValue & 0xff);
            wcscpy((wchar_t *)pstring->wData, L"???");
            break;
        }
        pstring->bLength = 2 + wcslen((wchar_t *)pstring->wData) * 2;
    }
    else if (wIndex == 0) // 字符串语言列表
    {
        DEBUG("123123\n");
        pstring->bLength = 4;
        pstring->wData[0] = 0x0409; // English (United States)
    }
    else
    {
        DEBUG("IND%hd\n", wIndex);
    }
}
int getReportDescriptor()
{
    HID_ReportDescriptor *preport = app.endpoint0->reportDescriptor;
    memcpy(preport, ReportDescriptor, REPORTDESCRIPTOR_SIZE);
    return REPORTDESCRIPTOR_SIZE;
}

void sendReport(unsigned char *buff,int length)
{
    //printf("asdasd\n");
    app.endpoint1->isUpdated = 1;
    USB_BufferDescriptor[1].COUNT_TX = length;
    usbWritePMA(USB_BufferDescriptor[1].ADDR_TX, buff, USB_BufferDescriptor[1].COUNT_TX);
    SET_STAT_TX(USB->EP1R, VALID);
    DEBUG("\n\n----1111-----\n\n");
}
static USB_EP0R endPoint0 =
    {
        .deviceDescriptor = writeBuffer,
        .configDescriptor = writeBuffer,
        .stringDescriptor = writeBuffer,
        .reportDescriptor = writeBuffer,

        .getDeviceDescriptor = getDeviceDescriptor,
        .getConfigurationDescriptor = getConfigurationDescriptor,
        .getStringDescriptor = getStringDescriptor,
        .getReportDescriptor = getReportDescriptor,
        .setAddress = setAddress};
static USB_EP1R endPoint1 =
    {
        .mouseReport = writeBuffer,
        .isUpdated = 0,
        .sendReport = sendReport};
USB_App app =
    {
        .addr = 0,
        .pread = readBuffer,
        .pwrite = writeBuffer,
        .endpoint0 = &endPoint0,
        .endpoint1 = &endPoint1,
        .init = init,
};