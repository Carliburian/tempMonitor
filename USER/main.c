#include "stm32f10x.h"
#include "bsp_usart.h"
#include "delay.h"
#include <stdio.h>
#include "MAX485.h"
#include "sys.h"
#include "flash.h"
#include "rtc.h"
#include "spi_dma.h"
#include "tm7705.h"
extern char TimeDisplay;
char flag=0;

uint16_t DMA_buf[64];

int main(void)
{



    delay_init();
    USART_Config();
    Max485_init();
    RTC_Init();
		SPI2_Init();
		DMA_SPI_Init((uint32_t)&DMA_buf,10);
	  TM7705_Init(0);
    while(1)
    {
				
    }


}
