#include "spi_dma.h"
#include "stm32f10x.h"
#include "tm7705.h"
uint32_t DMA_size;

/* -------------------------------- begin  -------------------------------- */
/**
  * @Name    SPI2_Init
  * @brief  	spi��ʼ������
  * @param   : [����/��]
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
    //mosi��sck������� miso���ջ�����
    //sck-13 miso--14 mosi--15
    GPIO_InitStructure1.GPIO_Pin=GPIO_Pin_13|GPIO_Pin_15;
    GPIO_InitStructure1.GPIO_Mode=GPIO_Mode_AF_PP;//����
    GPIO_InitStructure1.GPIO_Speed=GPIO_Speed_50MHz;
    GPIO_Init(GPIOB,&GPIO_InitStructure1);

    GPIO_InitStructure1.GPIO_Pin   = GPIO_Pin_14|GPIO_Pin_12;
    GPIO_InitStructure1.GPIO_Mode  = GPIO_Mode_IPU; //����
    GPIO_Init(GPIOB,&GPIO_InitStructure1);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);	//spiʱ��ʹ��
    //spi����

    SPI_InitStucrture.SPI_Direction=SPI_Direction_2Lines_FullDuplex;//����ͨ�Ż���˫��ͨ��
    SPI_InitStucrture.SPI_Mode=SPI_Mode_Master;//����ģʽ
    SPI_InitStucrture.SPI_DataSize=SPI_DataSize_8b;//���ݴ�СΪ8b
    SPI_InitStucrture.SPI_CPOL=SPI_CPOL_Low;//����״̬�Ǹߵ�ƽ
    SPI_InitStucrture.SPI_CPHA=SPI_CPHA_1Edge;//ż���ز���  ������������
    SPI_InitStucrture.SPI_FirstBit=SPI_FirstBit_MSB;//��λ���仹�ǵ�λ����
    SPI_InitStucrture.SPI_BaudRatePrescaler=SPI_BaudRatePrescaler_32;//������Ԥ��Ƶ��ֵ
    SPI_InitStucrture.SPI_NSS=SPI_NSS_Soft;//Ƭѡ ���������Ӳ������
		

    SPI_Init(SPI2,&SPI_InitStucrture);
    SPI_Cmd(SPI2,ENABLE);
    //	SPI2_ReadWriteByte(0xFF);//��������
    //SPI2_SetSpeed(SPI_BaudRatePrescaler_2);//����Ϊ18mhzʱ�ӣ�����ģʽ
		SPI_I2S_DMACmd(SPI2,SPI_I2S_DMAReq_Rx,ENABLE);//ʹ��spi2��dma���սӿ�

}




/* -------------------------------- begin  -------------------------------- */
/**
  * @Name    SPI2_ReadWriteByte
  * @brief  	spi��д������TxdataΪ����������  return ������Ϊu8��ʽ
  * @param   TxData: [����/��]
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
    while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET) //���ָ����SPI��־λ�������:���ͻ���ձ�־λ
    {
        retry++;
        if(retry>200)return 0;
    }
    SPI_I2S_SendData(SPI2, TxData); //ͨ������SPIx����һ������
    retry=0;

    while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET) //���ָ����SPI��־λ�������:���ܻ���ǿձ�־λ
    {
        retry++;
        if(retry>200)return 0;
    }
    return SPI_I2S_ReceiveData(SPI2); //����ͨ��SPIx������յ�����
}



/* -------------------------------- begin  -------------------------------- */
/**
  * @Name    DMA_SPI_Init
  * @brief  	
  * @param   DMA_Addr: [����/��] �����ַ 
**			 Buffer_Size: [����/��]  ����Ԫ�Ĵ�С
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
		//ʹ��DMA����
		DMA_DeInit(DMA1_Channel4);//spi2���ն�Ӧͨ��4
		DMA_InitStructure.DMA_PeripheralBaseAddr=(uint32_t)&(SPI2->DR);//�����ַ SPI2�����ݼĴ��� ǿת��32λ
		DMA_InitStructure.DMA_MemoryBaseAddr=(uint32_t)DMA_Addr;   //�ڴ��ַ ����ı�����ָ��
		DMA_InitStructure.DMA_DIR=DMA_DIR_PeripheralSRC;//���崫�䷽�� ����ΪԴ���ݵ�ַ,Ҫ�����ڴ�
		DMA_InitStructure.DMA_BufferSize=DMA_size;//���������� ���ݶ�Ϊ10������   20B
		DMA_InitStructure.DMA_PeripheralInc=DMA_PeripheralInc_Disable;//������������ַ�Ƿ�����  ��
		DMA_InitStructure.DMA_MemoryInc=DMA_MemoryInc_Enable;//��������ڴ��ַ�Ƿ����� ��
		DMA_InitStructure.DMA_PeripheralDataSize=DMA_PeripheralDataSize_Byte;//ÿ�δ�������ݴ�С  spiΪ8λ���ݣ����Դ������
		DMA_InitStructure.DMA_MemoryDataSize=DMA_PeripheralDataSize_Byte;
		DMA_InitStructure.DMA_Mode=DMA_Mode_Normal;//���δ���  ��һ��Ϊѭ������
		DMA_InitStructure.DMA_Priority=DMA_Priority_VeryHigh;//���ȼ�Ϊ�ǳ���
		DMA_InitStructure.DMA_M2M=DMA_M2M_Disable;//��ֹ�ڴ浽�ڴ�Ĵ���
		DMA_Init(DMA1_Channel4,&DMA_InitStructure);//����SPI2�������
		DMA_Cmd(DMA1_Channel4,DISABLE);
		
//		//ʹ��DMA����
//		DMA_DeInit(DMA1_Channel5);//spi2���Ͷ�Ӧͨ��4
//		DMA_InitStructure.DMA_PeripheralBaseAddr=(uint32_t)&(SPI2->DR);//�����ַ SPI2�����ݼĴ��� ǿת��32λ
//		DMA_InitStructure.DMA_MemoryBaseAddr=;   //�ڴ��ַ ����ı�����ָ��
//		DMA_InitStructure.DMA_DIR=DMA_DIR_PeripheralDST;//���崫�䷽�� ����ΪԴ���ݵ�ַ,Ҫ�����ڴ�
//		DMA_InitStructure.DMA_BufferSize=DMA_size;//���������� 
//		DMA_InitStructure.DMA_PeripheralInc=DMA_PeripheralInc_Disable;//������������ַ�Ƿ�����  ��
//		DMA_InitStructure.DMA_MemoryInc=DMA_MemoryInc_Enable;//��������ڴ��ַ�Ƿ����� ��
//		DMA_InitStructure.DMA_PeripheralDataSize=DMA_PeripheralDataSize_Byte;//ÿ�δ�������ݴ�С  spiΪ8λ���ݣ����Դ������
//		DMA_InitStructure.DMA_MemoryDataSize=DMA_PeripheralDataSize_Byte;
//		DMA_InitStructure.DMA_Mode=DMA_Mode_Normal;//���δ���  ��һ��Ϊѭ������
//		DMA_InitStructure.DMA_Priority=DMA_Priority_VeryHigh;//���ȼ�Ϊ�ǳ���
//		DMA_InitStructure.DMA_M2M=DMA_M2M_Disable;//��ֹ�ڴ浽�ڴ�Ĵ���
//		DMA_Init(DMA1_Channel4,&DMA_InitStructure);//����SPI2�������
//		DMA_Cmd(DMA1_Channel4,DISABLE);
}

/* -------------------------------- begin  -------------------------------- */
/**
  * @Name    DMA_Transfer
  * @brief   ����һ�δ��䣬��Ҫ�������ݵ�ʱ����øú������� ��ÿ�ν���10������
  * @param   : [����/��] 
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
	 DMA_SetCurrDataCounter(DMA1_Channel4,DMA_size);//�������ֻ�е�DMAʧ��ʱ����ʹ��
	 
	 DMA_Cmd(DMA1_Channel4,ENABLE);
	 DMA_ReadData();
	 while(DMA_GetFlagStatus(DMA1_FLAG_TC4)==RESET);//ͨ��4������ɱ�־
		DMA_ClearFlag(DMA1_FLAG_TC4);
}//��������DMA_size�����ݺ�ֹͣ����

 



