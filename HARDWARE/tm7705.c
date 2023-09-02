/**
  ************************************* Copyright ******************************
  *
  *                 (C) Copyright 2023,DrMa,China, GCU.
  *                            All Rights Reserved
  *
  *                     By(ElonMa)
  *
  *
  * FileName   : tm7705.c
  * Version    : v1.0
  * Author     : DrMa
  * Date       : 2023-07-06
  * Description:
  * Function List:
  	1. ....
  	   <version>:
  <modify staff>:
  		  <data>:
   <description>:
  	2. ...
  ******************************************************************************
 */
#include "tm7705.h"
#include "delay.h"


static unsigned char sw=0;
u32 adVal1=0;//����ͨ��һ��ֵ
u32 adVal2=0;//����ͨ������ֵ
u16 Discard;
char i=0;
u16 await=0;//�趨ͨ�ŵȴ�ֵ

char value[5];//����lcd��ʾ
char voltage1[5];//������ʾͨ��һ��ʵ�ʺ���ֵ
char voltage2[5];//������ʾͨ������ʵ�ʺ���ֵ



/* -------------------------------- begin  -------------------------------- */
/**
  * @Name    TM7705_Init
  * @brief  
  * @param   type: [����/��]  �����ȵ������ͣ���ͬ���ȵ������Ͷ�Ӧ��ͬ�Ĳ���
  * @retval
  * @author  DrMa
  * @Data    2023-09-02
  * 1. ...
  * <modify staff>:
  * <data>        :
  * <description> :
  * 2. ...
 **/
/* -------------------------------- end -------------------------------- */

void TM7705_Init(uint8_t type)
{
    u8 general_clock_config=0x04;//ʱ�ӼĴ��������� �����ܣ�����Ƶ��50hz��� ��20msһ������
		u8 TR_config;
		switch(type)
		{
			case 0:
				TR_config=Pt100_config;
				break;
			case 1:
				TR_config=Pt10_config;
				break;
			case 2:
				TR_config=Cu100_config;
				break;
			case 3:
				TR_config=Cu50_config;
				break;
		}
    
	
    TM7705_WriteReg(REG_SETUP,TR_config); //0x10 0x14   0001 0000
    delay_ms(1);
    TM7705_WriteReg(REG_CLOCK,general_clock_config); //0000 0100
    delay_ms(1);

	
    TM7705_WriteReg(REG_SETUP_CH2,TR_config);//ͨ����Ҳ����������ģʽ 8������ ������ ��ʹ�ܻ���
    delay_ms(1);
    TM7705_WriteReg(REG_CLOCK_CH2,general_clock_config);//�����ܣ�����Ƶ��50hz��� ��20msһ������
    delay_ms(1);
		
		TM7705_Calibself(type);
		
		
		
}



/* -------------------------------- begin  -------------------------------- */
/**
  * @Name    TM7705_Calibself
  * @brief    ��У׼����
  * @param   : [����/��]
  * @retval
  * @author  DrMa
  * @Data    2023-07-06
  * 1. ...
  * <modify staff>:
  * <data>        :
  * <description> :
  * 2. ...
 **/
/* -------------------------------- end -------------------------------- */

void TM7705_Calibself(uint8_t type)
{
    u8 data=0;
    u8 Reg_Ins;  //���üĴ�����  ��У��ģʽ ��8������ �������� ���岻ʹ��  0101 1100
	
	switch(type)
		{
			case 0:
				Reg_Ins=Pt100_config;
				break;
			case 1:
				Reg_Ins=Pt10_config;
				break;
			case 2:
				Reg_Ins=Cu100_config;
				break;
			case 3:
				Reg_Ins=Cu50_config;
				break;
		}
		Reg_Ins+=0x40;//ת��Ϊ��У��ģʽ
    //��У��ͨ��һ
    TM7705_WriteReg(REG_SETUP,Reg_Ins);
    delay_ms(190);
    while(DataRDY!=0);//����0˵������׼����ɣ�������0˵����ûУ�����
    SPI2_ReadWriteByte(0x68);//0110 1000   ���ƫ�ƼĴ��� ������
    //��ȡ���ƫ�ƼĴ�����ֵ ����3�ֽ�����
    data=SPI2_ReadWriteByte(0xFF);
    data=SPI2_ReadWriteByte(0XFF);
    data=SPI2_ReadWriteByte(0XFF);
    //��ȡ����ϵ���Ĵ�����ֵ������3�ֽ�����
    SPI2_ReadWriteByte(0X78);  //  0111 1000  ����ϵ���Ĵ��� ������
    data=SPI2_ReadWriteByte(0xFF);
    data=SPI2_ReadWriteByte(0XFF);
    data=SPI2_ReadWriteByte(0XFF);


    //��У��ͨ����
    TM7705_WriteReg(REG_SETUP_CH2,Reg_Ins);
    delay_ms(190);
    while(DataRDY!=0);//����0˵������׼����ɣ�������0˵����ûУ�����
    SPI2_ReadWriteByte(0x69);//0110 1001   ���ƫ�ƼĴ��� ������
    //��ȡ���ƫ�ƼĴ�����ֵ ����3�ֽ�����
    data=SPI2_ReadWriteByte(0xFF);
    data=SPI2_ReadWriteByte(0XFF);
    data=SPI2_ReadWriteByte(0XFF);
    //��ȡ����ϵ���Ĵ�����ֵ������3�ֽ�����
    SPI2_ReadWriteByte(0X79);  //  0111 1001  ����ϵ���Ĵ��� ������
    data=SPI2_ReadWriteByte(0xFF);
    data=SPI2_ReadWriteByte(0XFF);
    data=SPI2_ReadWriteByte(0XFF);
}




/* -------------------------------- begin  -------------------------------- */
/**
  * @Name    TM7705_SyncSPI
  * @brief   ͬ��SPI�ӿ�ʱ��,��ֹ�������ݴ�λ
  * @param   None
  * @retval
  * @author  DrMa
  * @Data    2023-07-06
  * 1. ...
  * <modify staff>:
  * <data>        :
  * <description> :
  * 2. ...
 **/
/* -------------------------------- end -------------------------------- */

static void TM7705_SyncSPI(void)
{

    SPI2_ReadWriteByte(0xFF);//32��1
    SPI2_ReadWriteByte(0xFF);
    SPI2_ReadWriteByte(0xFF);
    SPI2_ReadWriteByte(0xFF);
}



/* -------------------------------- begin  -------------------------------- */
/**
  * @Name    TM7705_WriteReg
  * @brief
  * @param   reg_name: [����/��]
**			 data: [����/��]
  * @retval
  * @author  DrMa
  * @Data    2023-07-06
  * 1. ...
  * <modify staff>:
  * <data>        :
  * <description> :
  * 2. ...
 **/
/* -------------------------------- end -------------------------------- */

void TM7705_WriteReg(u8 reg_name,u8 data)
{   // ʹ��ͨ��1 д���� ��������ģʽ
    SPI2_ReadWriteByte(reg_name);
    // ǰ4λΪ�Ĵ���ֵ����4λΪ��/д����������ģʽ ͨ��ѡ��
    SPI2_ReadWriteByte(data);//дָ��

}


/* -------------------------------- begin  -------------------------------- */
/**
  * @Name    TM7705_ReadData
  * @brief		7705 ͨ��һ�����ݼĴ���
  * @param   : [����/��]
  * @retval
  * @author  DrMa
  * @Data    2023-07-06
  * 1. ...
  * <modify staff>:
  * <data>        :
  * <description> :
  * 2. ...
 **/
/* -------------------------------- end -------------------------------- */

u16 TM7705_ReadData_CH1()
{
    u8 Reg_Ins=0;
    u8 Data_H=0,Data_L=0;
    u16 Data=0;
    Reg_Ins=(REG_DATA|0x08); //0x30 0x08   0 011 1000  adc����Ĵ��� ������
    SPI2_ReadWriteByte(Reg_Ins);
    Data_H=SPI2_ReadWriteByte(0xEE);
    Data_L=SPI2_ReadWriteByte(0xEE);

    Data=Data_H;
    Data=(Data<<8)|Data_L;
    return Data;
}

/* -------------------------------- begin  -------------------------------- */
/**
  * @Name    TM7705_ReadData_CH2
  * @brief  	7705ͨ���������ݼĴ���
  * @param   : [����/��]
  * @retval
  * @author  DrMa
  * @Data    2023-07-10
  * 1. ...
  * <modify staff>:
  * <data>        :
  * <description> :
  * 2. ...
 **/
/* -------------------------------- end -------------------------------- */

u16 TM7705_ReadData_CH2()
{
    u8 Reg_Ins=0;
    u8 Data_H=0,Data_L=0;
    u16 Data=0;
    Reg_Ins=(REG_DATA_CH2|0x08); //0x31| 0x08=0x39   0 011 1000  adc����Ĵ��� ������
    SPI2_ReadWriteByte(Reg_Ins);
    Data_H=SPI2_ReadWriteByte(0xEE);
    Data_L=SPI2_ReadWriteByte(0xEE);

    Data=Data_H;
    Data=(Data<<8)|Data_L;
    return Data;
}


/* -------------------------------- begin  -------------------------------- */
/**
  * @Name    DRDYINT_Init
  * @brief  	������ɱ�־λ�жϳ�ʼ��
  * @param   : [����/��]
  * @retval
  * @author  DrMa
  * @Data    2023-07-10
  * 1. ...
  * <modify staff>:
  * <data>        :
  * <description> :
  * 2. ...
 **/
/* -------------------------------- end -------------------------------- */

void DRDYINT_Init()
{
    GPIO_InitTypeDef   GPIO_InitStructure;
    EXTI_InitTypeDef   EXTI_InitStructure;
    NVIC_InitTypeDef   NVIC_InitStructure;
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
    //RCC_Enable
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO,ENABLE);
    //GPIO Config
    GPIO_InitStructure.GPIO_Pin=GPIO_Pin_12;
    GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU;
    GPIO_Init(GPIOB,&GPIO_InitStructure);

    GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource12);
    //EXTI Config
    EXTI_InitStructure.EXTI_Line=EXTI_Line12;
    EXTI_InitStructure.EXTI_Mode=EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger=EXTI_Trigger_Falling;
    EXTI_InitStructure.EXTI_LineCmd=ENABLE;
    EXTI_Init(&EXTI_InitStructure);
    //NVIC Config
    NVIC_InitStructure.NVIC_IRQChannel=EXTI15_10_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority=0;
    NVIC_Init(&NVIC_InitStructure);

}


/* -------------------------------- begin  -------------------------------- */
/**
  * @Name    EXTI15_10_IRQHandler
  * @brief  	7705����жϴ������
  * @param   : [����/��]
  * @retval
  * @author  DrMa
  * @Data    2023-07-10
  * 1. ...
  * <modify staff>:
  * <data>        :
  * <description> :
  * 2. ...
 **/
/* -------------------------------- end -------------------------------- */

void EXTI15_10_IRQHandler()
{

    switch(sw)
    {   //case 0����һ��ͨ����case 1���ڶ���ͨ��
    case 0:
        DRDY_EXTI_Close();
        Discard=TM7705_ReadData_CH1();//��һ�ζ�����ֵΪ��һ��ͨ����ֵ������
        DRDY_EXTI_Open();
//			for(i=0;i<10;i++)
//			{
        delay_ms(20);
        while(DataRDY!=0);//�ȴ��յ�����
        adVal1=TM7705_ReadData_CH1();
        //strcpy(value,ConvertToASC(adVal1));//adֵת�����ַ���
      //  strcpy(voltage1,ConvertToASC(adVal1*2500/8/65536));
        

        //}

        sw=(sw+1)%2;
        break;

    case 1:
        DRDY_EXTI_Close();
        Discard=TM7705_ReadData_CH2();//��һ�ζ�����ֵΪ��һ��ͨ����ֵ������
        DRDY_EXTI_Open();
//			for(i=0;i<10;i++)
//			{
        delay_ms(20);
        while(DataRDY!=0);//�ȴ��յ�����
        adVal2=TM7705_ReadData_CH2();
        //strcpy(value,ConvertToASC(adVal2));//adֵת�����ַ���
     //   strcpy(voltage2,ConvertToASC(adVal2*2500/8/65536));
        
        sw=(sw+1)%2;
        break;
    }

    EXTI_ClearITPendingBit(EXTI_Line12);


}



void DRDY_EXTI_Open()
{   //���жϴ���
    EXTI_InitTypeDef EXTI_InitStructure;
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource12);

    EXTI_InitStructure.EXTI_Line=EXTI_Line12;
    EXTI_InitStructure.EXTI_LineCmd=ENABLE;
    EXTI_InitStructure.EXTI_Mode=EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger=EXTI_Trigger_Falling;
    EXTI_Init(&EXTI_InitStructure);

}

void DRDY_EXTI_Close()
{   //�ر��жϴ���
    EXTI_InitTypeDef EXTI_InitStructure;
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource12);

    EXTI_InitStructure.EXTI_Line=EXTI_Line12;
    EXTI_InitStructure.EXTI_LineCmd=DISABLE;
    EXTI_InitStructure.EXTI_Mode=EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger=EXTI_Trigger_Falling;
    EXTI_Init(&EXTI_InitStructure);

}
