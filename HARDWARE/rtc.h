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
  * FileName   : rtc.h
  * Version    : v1.0
  * Author     : DrMa
  * Date       : 2023-08-17
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


#ifndef __RTC_H_
#define __RTC_H_

#include "sys.h"
#include "bsp_usart.h"
#include "stm32f10x_rtc.h"

extern char* time_str;  //方案二使用
void RTC_Init(void);
void  RTC_Configuration(void);
void RTC_NVIC_Configuration(void);
void Time_Adjust(void);
char* RTC_ReadTime(void);
#endif




