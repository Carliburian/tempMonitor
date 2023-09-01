/**
  ************************************* Copyright ******************************
  *
  *                 (C) Copyright 2023,DrMa,China, GCU.
  *                            All Rights Reserved
  *
  *                     By(ElonMa)
  *
  *
  * FileName   : MAX485.c
  * Version    : v1.0
  * Author     : DrMa
  * Date       : 2023-08-15
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


#include "MAX485.h"
#include "sys.h"
#include "stm32f10x_gpio.h"
#include "delay.h"



void  MAX_485_Init()
{
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin=GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
    GPIO_Init(GPIOB,&GPIO_InitStructure);
//	En485_RW=0;//拉低 默认接收模式

}

void rs485_delay()
{
    __IO uint32_t count = 30;
    for(; count!=0; count--);
}

/* 控制485模块收发模式 0:recv, 1:send*/
void rs485_mode_set(uint8_t mode)
{
    rs485_delay();
    if (mode)
        En485_RW=1;
//		gpio_bit_reset(GPIOA, GPIO_PIN_5);  /* PA5是rs485收发模式控制引脚 */
    else
        En485_RW=0;
//		gpio_bit_set(GPIOA, GPIO_PIN_5);
    rs485_delay();
}
