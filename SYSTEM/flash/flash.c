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



uint16_t Flash_Buf[FLASH_PAGE_SIZE/2];//最多2K字节



/* -------------------------------- begin  -------------------------------- */
/**
  * @Name    Flash_WriteNoCheck
  * @brief  
  * @param   addr: [输入/出] 
**			 pBuffer: [输入/出] 
**			 len: [输入/出] 
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
  * @brief  	支持数据按页写入
  * @param   StartAddr: [输入/出] 
**			 pBuffer: [输入/出] 
**			 len: [输入/出] 
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
	u32 secpos;	   //扇区地址
	u16 secoff;	   //扇区内偏移地址(16位字计算)
	u16 secremain; //扇区内剩余地址(16位字计算)	   
 	u16 i;    
	u32 offaddr;   //去掉0X08000000后的地址
	if(WriteAddr<STM32_FLASH_BASE||(WriteAddr>=(STM32_FLASH_BASE+1024*256)))return;//非法地址
	FLASH_Unlock();						//解锁
	offaddr=WriteAddr-STM32_FLASH_BASE;		//实际偏移地址.
	secpos=offaddr/FLASH_PAGE_SIZE;			//扇区地址  0~127 for STM32F103RBT6
	secoff=(offaddr%FLASH_PAGE_SIZE)/2;		//在扇区内的偏移(2个字节为基本单位.)
	secremain=FLASH_PAGE_SIZE/2-secoff;		//扇区剩余空间大小   
	if(NumToWrite<=secremain)secremain=NumToWrite;//不大于该扇区范围
	while(1) 
	{	
		FLASH_ReadData(secpos*FLASH_PAGE_SIZE+STM32_FLASH_BASE,Flash_Buf,FLASH_PAGE_SIZE/2);//读出整个扇区的内容
		for(i=0;i<secremain;i++)//校验数据
		{
			if(Flash_Buf[secoff+i]!=0XFFFF)break;//需要擦除  	  
		}
		if(i<secremain)//需要擦除
		{
			FLASH_ErasePage(secpos*FLASH_PAGE_SIZE+STM32_FLASH_BASE);//擦除这个扇区
			for(i=0;i<secremain;i++)//复制
			{
				Flash_Buf[i+secoff]=pBuffer[i];	  
			}
			Flash_WriteNoCheck(secpos*FLASH_PAGE_SIZE+STM32_FLASH_BASE,Flash_Buf,FLASH_PAGE_SIZE/2);//写入整个扇区  
		}else Flash_WriteNoCheck(WriteAddr,pBuffer,secremain);//写已经擦除了的,直接写入扇区剩余区间. 				   
		if(NumToWrite==secremain)break;//写入结束了
		else//写入未结束
		{
			secpos++;				//扇区地址增1
			secoff=0;				//偏移位置为0 	 
		   	pBuffer+=secremain;  	//指针偏移
			WriteAddr+=(secremain*2);	//写地址偏移	   
		   	NumToWrite-=secremain;	//字节(16位)数递减
			if(NumToWrite>(FLASH_PAGE_SIZE/2))secremain=FLASH_PAGE_SIZE/2;//下一个扇区还是写不完
			else secremain=NumToWrite;//下一个扇区可以写完了
		}	 
	};	
	FLASH_Lock();//上锁
}



/* -------------------------------- begin  -------------------------------- */
/**
  * @Name    //读取指定地址的半字
  * @brief  
  * @param   16位数据: [输入/出] 
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
  * @Name    //读取指定地址的全字数据
  * @brief  
  * @param   32位: [输入/出] 
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
  * @param   StartAddr: [输入/出] 
**			 EntryPrama: [输入/出]  超标准的写法，将形参中的指针作为入口参数，
传入的EntryPrama可直接作为输出使用
**			 len: [输入/出] 
  * @retval
  * @author  DrMa
  * @Data    2023-08-17
  * 1. ...
  * <modify staff>:
  * <data>        :
  * <description> :这么写的好处？
不会像上一个函数一样全局变量漫天飞，变量的生存周期与函数周期相同
  * 2. ...
 **/
/* -------------------------------- end -------------------------------- */

void FLASH_ReadData(uint32_t StartAddr,uint16_t* EntryPrama,uint16_t len)
{
		uint16_t i;
		for(i=0;i<len;i++)
		{
				EntryPrama[i]=FLASH_ReadHalfWord(StartAddr);
				StartAddr+=2;//地址自增
		}
}


