#include "spi_dma.h"
#include "stm32f10x.h"
#include "tm7705.h"
uint32_t DMA_size;

/* -------------------------------- begin  -------------------------------- */
/**
  * @Name    SPI2_Init
  * @brief  	spi初始化函数
  * @param   : [输入/出]
  * @retval
  * @author  DrMa
  * @Data    2023-07-07
  * 1. ...
  * <modify staff>:
  * <data>        :
  * <description> :
  * 2. ...
 **/
/* -------------------------------- end -------------------------------- */

void SPI2_Init()
{
    GPIO_InitTypeDef GPIO_InitStructure1;
    SPI_InitTypeDef SPI_InitStucrture;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    //mosi，sck推挽输出 miso浮空或上拉
    //sck-13 miso--14 mosi--15
    GPIO_InitStructure1.GPIO_Pin=GPIO_Pin_13|GPIO_Pin_15;
    GPIO_InitStructure1.GPIO_Mode=GPIO_Mode_AF_PP;//推挽
    GPIO_InitStructure1.GPIO_Speed=GPIO_Speed_50MHz;
    GPIO_Init(GPIOB,&GPIO_InitStructure1);

    GPIO_InitStructure1.GPIO_Pin   = GPIO_Pin_14|GPIO_Pin_12;
    GPIO_InitStructure1.GPIO_Mode  = GPIO_Mode_IPU; //上拉
    GPIO_Init(GPIOB,&GPIO_InitStructure1);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);	//spi时钟使能
    //spi配置

    SPI_InitStucrture.SPI_Direction=SPI_Direction_2Lines_FullDuplex;//单向通信还是双向通信
    SPI_InitStucrture.SPI_Mode=SPI_Mode_Master;//主机模式
    SPI_InitStucrture.SPI_DataSize=SPI_DataSize_8b;//数据大小为8b
    SPI_InitStucrture.SPI_CPOL=SPI_CPOL_Low;//空闲状态是高电平
    SPI_InitStucrture.SPI_CPHA=SPI_CPHA_1Edge;//偶数沿采样  先输出，后采样
    SPI_InitStucrture.SPI_FirstBit=SPI_FirstBit_MSB;//高位传输还是低位传输
    SPI_InitStucrture.SPI_BaudRatePrescaler=SPI_BaudRatePrescaler_32;//波特率预分频的值
    SPI_InitStucrture.SPI_NSS=SPI_NSS_Soft;//片选 软件管理还是硬件管理
		

    SPI_Init(SPI2,&SPI_InitStucrture);
    SPI_Cmd(SPI2,ENABLE);
    //	SPI2_ReadWriteByte(0xFF);//启动传输
    //SPI2_SetSpeed(SPI_BaudRatePrescaler_2);//设置为18mhz时钟，高速模式
		SPI_I2S_DMACmd(SPI2,SPI_I2S_DMAReq_Rx,ENABLE);//使能spi2的dma接收接口

}




/* -------------------------------- begin  -------------------------------- */
/**
  * @Name    SPI2_ReadWriteByte
  * @brief  	spi读写函数，Txdata为待发送数据  return 的数据为u8格式
  * @param   TxData: [输入/出]
  * @retval
  * @author  DrMa
  * @Data    2023-07-07
  * 1. ...
  * <modify staff>:
  * <data>        :
  * <description> :
  * 2. ...
 **/
/* -------------------------------- end -------------------------------- */

u8 SPI2_ReadWriteByte(u8 TxData)
{
    u8 retry=0;
    while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET) //检查指定的SPI标志位设置与否:发送缓存空标志位
    {
        retry++;
        if(retry>200)return 0;
    }
    SPI_I2S_SendData(SPI2, TxData); //通过外设SPIx发送一个数据
    retry=0;

    while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET) //检查指定的SPI标志位设置与否:接受缓存非空标志位
    {
        retry++;
        if(retry>200)return 0;
    }
    return SPI_I2S_ReceiveData(SPI2); //返回通过SPIx最近接收的数据
}



/* -------------------------------- begin  -------------------------------- */
/**
  * @Name    DMA_SPI_Init
  * @brief  	
  * @param   DMA_Addr: [输入/出] 外设地址 
**			 Buffer_Size: [输入/出]  数据元的大小
  * @retval
  * @author  DrMa
  * @Data    2023-08-21
  * 1. ...
  * <modify staff>:
  * <data>        :
  * <description> :
  * 2. ...
 **/
/* -------------------------------- end -------------------------------- */

void DMA_SPI_Init(uint32_t DMA_Addr,uint32_t Buffer_Size)
{
		DMA_InitTypeDef    DMA_InitStructure;
		RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);
	  DMA_size=Buffer_Size;
		//使能DMA接收
		DMA_DeInit(DMA1_Channel4);//spi2接收对应通道4
		DMA_InitStructure.DMA_PeripheralBaseAddr=(uint32_t)&(SPI2->DR);//外设地址 SPI2的数据寄存器 强转成32位
		DMA_InitStructure.DMA_MemoryBaseAddr=(uint32_t)DMA_Addr;   //内存地址 传入的变量的指针
		DMA_InitStructure.DMA_DIR=DMA_DIR_PeripheralSRC;//定义传输方向 外设为源数据地址,要传入内存
		DMA_InitStructure.DMA_BufferSize=DMA_size;//传输数据量 ，暂定为10份数据   20B
		DMA_InitStructure.DMA_PeripheralInc=DMA_PeripheralInc_Disable;//传输完后外设地址是否自增  否
		DMA_InitStructure.DMA_MemoryInc=DMA_MemoryInc_Enable;//传输完后内存地址是否自增 是
		DMA_InitStructure.DMA_PeripheralDataSize=DMA_PeripheralDataSize_Byte;//每次传输的数据大小  spi为8位数据，所以传输半字
		DMA_InitStructure.DMA_MemoryDataSize=DMA_PeripheralDataSize_Byte;
		DMA_InitStructure.DMA_Mode=DMA_Mode_Normal;//单次传输  另一种为循环传输
		DMA_InitStructure.DMA_Priority=DMA_Priority_VeryHigh;//优先级为非常高
		DMA_InitStructure.DMA_M2M=DMA_M2M_Disable;//禁止内存到内存的传输
		DMA_Init(DMA1_Channel4,&DMA_InitStructure);//配置SPI2接收完成
		DMA_Cmd(DMA1_Channel4,DISABLE);
		
//		//使能DMA发送
//		DMA_DeInit(DMA1_Channel5);//spi2发送对应通道4
//		DMA_InitStructure.DMA_PeripheralBaseAddr=(uint32_t)&(SPI2->DR);//外设地址 SPI2的数据寄存器 强转成32位
//		DMA_InitStructure.DMA_MemoryBaseAddr=;   //内存地址 传入的变量的指针
//		DMA_InitStructure.DMA_DIR=DMA_DIR_PeripheralDST;//定义传输方向 外设为源数据地址,要传入内存
//		DMA_InitStructure.DMA_BufferSize=DMA_size;//传输数据量 
//		DMA_InitStructure.DMA_PeripheralInc=DMA_PeripheralInc_Disable;//传输完后外设地址是否自增  否
//		DMA_InitStructure.DMA_MemoryInc=DMA_MemoryInc_Enable;//传输完后内存地址是否自增 是
//		DMA_InitStructure.DMA_PeripheralDataSize=DMA_PeripheralDataSize_Byte;//每次传输的数据大小  spi为8位数据，所以传输半字
//		DMA_InitStructure.DMA_MemoryDataSize=DMA_PeripheralDataSize_Byte;
//		DMA_InitStructure.DMA_Mode=DMA_Mode_Normal;//单次传输  另一种为循环传输
//		DMA_InitStructure.DMA_Priority=DMA_Priority_VeryHigh;//优先级为非常高
//		DMA_InitStructure.DMA_M2M=DMA_M2M_Disable;//禁止内存到内存的传输
//		DMA_Init(DMA1_Channel4,&DMA_InitStructure);//配置SPI2接收完成
//		DMA_Cmd(DMA1_Channel4,DISABLE);
}

/* -------------------------------- begin  -------------------------------- */
/**
  * @Name    DMA_Transfer
  * @brief   启动一次传输，需要传输数据的时候调用该函数接收 ，每次接收10份数据
  * @param   : [输入/出] 
  * @retval
  * @author  DrMa
  * @Data    2023-08-21
  * 1. ...
  * <modify staff>:
  * <data>        :
  * <description> :
  * 2. ...
 **/
/* -------------------------------- end -------------------------------- */

void DMA_Transfer()
{
	 DMA_Cmd(DMA1_Channel4,DISABLE);
	 DMA_SetCurrDataCounter(DMA1_Channel4,DMA_size);//这个函数只有当DMA失能时才能使用
	 
	 DMA_Cmd(DMA1_Channel4,ENABLE);
	 DMA_ReadData();
	 while(DMA_GetFlagStatus(DMA1_FLAG_TC4)==RESET);//通道4传输完成标志
		DMA_ClearFlag(DMA1_FLAG_TC4);
}//当传输完DMA_size个数据后停止传输

 



