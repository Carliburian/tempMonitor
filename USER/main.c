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
#include "cd4052.h"
extern char TimeDisplay;
char flag=0;
#define DMA_Size  256
uint8_t DMA_buf[DMA_Size];
void Data_Analysis(void);
int main(void)
{

		
    delay_init();
    USART_Config();
    Max485_init();
    RTC_Init();
		CD4052_Init();
		SPI2_Init();
		DMA_SPI_Init((uint32_t)DMA_buf,DMA_Size);
		
	  TM7705_Init(1);
		
		RTDOUT(RTD3);
		DRDYINT_Init();	
		DMA_Transfer();
		//Data_Analysis();
		memset(DMA_buf,0,DMA_Size);
		RTDOUT(RTD1);
		DMA_Transfer();
		Data_Analysis();
		
    while(1)
    {
				
			
    }


}


void Data_Analysis(void)
{
	int i=1,j=2;
	uint16_t data;
	while(i<DMA_Size&&j<DMA_Size)
	{
		data=DMA_buf[i]<<8|DMA_buf[j];
		i+=3;
		j+=3;
		printk("%d,",data);
	}
	
	
	
	
}
