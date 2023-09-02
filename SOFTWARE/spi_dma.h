/*
 * @Author: Drma
 * @Date: 2023-06-28 13:59:52
 * @LastEditors: Drma
 * @LastEditTime: 2023-07-23 15:42:31
 * @Description: 
 */
/**
  ************************************* Copyright ******************************   
  *                 (C) Copyright 2023,DrMa,China, GCU.
  *                            All Rights Reserved
  *                              
  *                     By(ElonMa)
  *                     https://github.com/GCUWildwolfteam
  *      
  * FileName   : spi_dma.h   
  * Version    : v1.0		
  * Author     : DrMa
  * Date       : 2023-08-21
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


#ifndef __SPI_DMA_H_
#define __SPI_DMA_H_

#include "sys.h"
#include "stm32f10x_spi.h"
#include "stm32f10x_dma.h"
void SPI2_Init(void);  //初始化SPI端口
void DMA_SPI_Init(uint32_t DMA_Addr,uint32_t Buffer_Size);
u8 SPI2_ReadWriteByte(u8 TxData);
void DMA_Transfer(void);
#endif


 

