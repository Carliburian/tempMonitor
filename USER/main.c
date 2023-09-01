#include "stm32f10x.h"
#include "bsp_usart.h"
#include "delay.h"
#include <stdio.h>
#include "MAX485.h"
#include "sys.h"
#include "flash.h"
#include "rtc.h"
#include "spi_dma.h"
extern char TimeDisplay;
char flag=0;
int main(void)
{



    delay_init();
    USART_Config();
    Max485_init();
    RTC_Init();
		SPI2_Init();
    while(1)
    {
				
    }


}
