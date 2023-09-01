/**
  ************************************* Copyright ******************************
  *
  *                 (C) Copyright 2023,DrMa,China, GCU.
  *                            All Rights Reserved
  *
  *                     By(ElonMa)
  *
  *
  * FileName   : bsp_internal_flash.c
  * Version    : v1.0
  * Author     : DrMa
  * Date       : 2023-08-12
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


#include "flash.h"



uint16_t Flash_Buf[FLASH_PAGE_SIZE/2];//���2K�ֽ�



/* -------------------------------- begin  -------------------------------- */
/**
  * @Name    Flash_WriteNoCheck
  * @brief  
  * @param   addr: [����/��] 
**			 pBuffer: [����/��] 
**			 len: [����/��] 
  * @retval
  * @author  DrMa
  * @Data    2023-08-17
  * 1. ...
  * <modify staff>:
  * <data>        :
  * <description> :
  * 2. ...
 **/
/* -------------------------------- end -------------------------------- */

void Flash_WriteNoCheck(uint32_t addr,uint16_t* pBuffer,uint16_t len)
{
			uint16_t num=0;
			while(num<len)
			{
				FLASH_ProgramHalfWord(addr,pBuffer[num]);
				addr+=2;
				num++;
			}
			
}

/* -------------------------------- begin  -------------------------------- */
/**
  * @Name    Flash_Write
  * @brief  	֧�����ݰ�ҳд��
  * @param   StartAddr: [����/��] 
**			 pBuffer: [����/��] 
**			 len: [����/��] 
  * @retval
  * @author  DrMa
  * @Data    2023-08-17
  * 1. ...
  * <modify staff>:
  * <data>        :
  * <description> :
  * 2. ...
 **/
/* -------------------------------- end -------------------------------- */

void Flash_Write(u32 WriteAddr,u16 *pBuffer,u16 NumToWrite)	
{
	u32 secpos;	   //������ַ
	u16 secoff;	   //������ƫ�Ƶ�ַ(16λ�ּ���)
	u16 secremain; //������ʣ���ַ(16λ�ּ���)	   
 	u16 i;    
	u32 offaddr;   //ȥ��0X08000000��ĵ�ַ
	if(WriteAddr<STM32_FLASH_BASE||(WriteAddr>=(STM32_FLASH_BASE+1024*256)))return;//�Ƿ���ַ
	FLASH_Unlock();						//����
	offaddr=WriteAddr-STM32_FLASH_BASE;		//ʵ��ƫ�Ƶ�ַ.
	secpos=offaddr/FLASH_PAGE_SIZE;			//������ַ  0~127 for STM32F103RBT6
	secoff=(offaddr%FLASH_PAGE_SIZE)/2;		//�������ڵ�ƫ��(2���ֽ�Ϊ������λ.)
	secremain=FLASH_PAGE_SIZE/2-secoff;		//����ʣ��ռ��С   
	if(NumToWrite<=secremain)secremain=NumToWrite;//�����ڸ�������Χ
	while(1) 
	{	
		FLASH_ReadData(secpos*FLASH_PAGE_SIZE+STM32_FLASH_BASE,Flash_Buf,FLASH_PAGE_SIZE/2);//������������������
		for(i=0;i<secremain;i++)//У������
		{
			if(Flash_Buf[secoff+i]!=0XFFFF)break;//��Ҫ����  	  
		}
		if(i<secremain)//��Ҫ����
		{
			FLASH_ErasePage(secpos*FLASH_PAGE_SIZE+STM32_FLASH_BASE);//�����������
			for(i=0;i<secremain;i++)//����
			{
				Flash_Buf[i+secoff]=pBuffer[i];	  
			}
			Flash_WriteNoCheck(secpos*FLASH_PAGE_SIZE+STM32_FLASH_BASE,Flash_Buf,FLASH_PAGE_SIZE/2);//д����������  
		}else Flash_WriteNoCheck(WriteAddr,pBuffer,secremain);//д�Ѿ������˵�,ֱ��д������ʣ������. 				   
		if(NumToWrite==secremain)break;//д�������
		else//д��δ����
		{
			secpos++;				//������ַ��1
			secoff=0;				//ƫ��λ��Ϊ0 	 
		   	pBuffer+=secremain;  	//ָ��ƫ��
			WriteAddr+=(secremain*2);	//д��ַƫ��	   
		   	NumToWrite-=secremain;	//�ֽ�(16λ)���ݼ�
			if(NumToWrite>(FLASH_PAGE_SIZE/2))secremain=FLASH_PAGE_SIZE/2;//��һ����������д����
			else secremain=NumToWrite;//��һ����������д����
		}	 
	};	
	FLASH_Lock();//����
}



/* -------------------------------- begin  -------------------------------- */
/**
  * @Name    //��ȡָ����ַ�İ���
  * @brief  
  * @param   16λ����: [����/��] 
  * @retval
  * @author  DrMa
  * @Data    2023-08-17
  * 1. ...
  * <modify staff>:
  * <data>        :
  * <description> :
  * 2. ...
 **/
/* -------------------------------- end -------------------------------- */


uint16_t FLASH_ReadHalfWord(uint32_t address)
{
  return *(__IO uint16_t*)address; 
}

/* -------------------------------- begin  -------------------------------- */
/**
  * @Name    //��ȡָ����ַ��ȫ������
  * @brief  
  * @param   32λ: [����/��] 
  * @retval
  * @author  DrMa
  * @Data    2023-08-17
  * 1. ...
  * <modify staff>:
  * <data>        :
  * <description> :
  * 2. ...
 **/
/* -------------------------------- end -------------------------------- */


uint32_t FLASH_ReadWord(uint32_t address)
{
	return *(__IO uint32_t*)address; 
}


/* -------------------------------- begin  -------------------------------- */
/**
  * @Name    FLASH_ReadData
  * @brief  
  * @param   StartAddr: [����/��] 
**			 EntryPrama: [����/��]  ����׼��д�������β��е�ָ����Ϊ��ڲ�����
�����EntryPrama��ֱ����Ϊ���ʹ��
**			 len: [����/��] 
  * @retval
  * @author  DrMa
  * @Data    2023-08-17
  * 1. ...
  * <modify staff>:
  * <data>        :
  * <description> :��ôд�ĺô���
��������һ������һ��ȫ�ֱ�������ɣ����������������뺯��������ͬ
  * 2. ...
 **/
/* -------------------------------- end -------------------------------- */

void FLASH_ReadData(uint32_t StartAddr,uint16_t* EntryPrama,uint16_t len)
{
		uint16_t i;
		for(i=0;i<len;i++)
		{
				EntryPrama[i]=FLASH_ReadHalfWord(StartAddr);
				StartAddr+=2;//��ַ����
		}
}


