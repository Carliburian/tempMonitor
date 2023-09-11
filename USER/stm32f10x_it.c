/**
  ******************************************************************************
  * @file    GPIO/IOToggle/stm32f10x_it.c
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and peripherals
  *          interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_it.h"
#include "bsp_usart.h"
#include "rtc.h"
uint8_t ucTemp;
extern  char* time_str;
char TimeDisplay=0;

void NMI_Handler(void)
{
}

void HardFault_Handler(void)
{
    /* Go to infinite loop when Hard Fault exception occurs */
    while (1)
    {
    }
}

void MemManage_Handler(void)
{
    /* Go to infinite loop when Memory Manage exception occurs */
    while (1)
    {
    }
}


void BusFault_Handler(void)
{
    /* Go to infinite loop when Bus Fault exception occurs */
    while (1)
    {
    }
}

void UsageFault_Handler(void)
{
    /* Go to infinite loop when Usage Fault exception occurs */
    while (1)
    {
    }
}

void SVC_Handler(void)
{
}

void DebugMon_Handler(void)
{
}

void PendSV_Handler(void)
{
}

void SysTick_Handler(void)
{
}

// 串口中断服务函数
void DEBUG_USART_IRQHandler(void)
{

    if(USART_GetITStatus(DEBUG_USARTx,USART_IT_RXNE)!=RESET)
    {
        USART_ClearFlag(DEBUG_USARTx, USART_IT_RXNE); //清除标志位；
        ucTemp = USART_ReceiveData(DEBUG_USARTx);

    }
}

void RTC_IRQHandler(void)
{
    if (RTC_GetITStatus(RTC_IT_SEC) != RESET)
    {
        /* Clear the RTC Second interrupt */
        RTC_ClearITPendingBit(RTC_IT_SEC);

        /* Enable time update */
//    TimeDisplay = 1;//使用了 一个标志位来指示是否需要显示时间
        time_str= RTC_ReadTime();
				//printk("%s",time_str);
				//Usart_SendStr(DEBUG_USARTx,time_str);
				//printk("\r\n");
        /* Wait until last write operation on RTC registers has finished */
        RTC_WaitForLastTask();

    }

}

void DMA1_Channel4_IRQHandler(void)
{
    if (DMA_GetITStatus(DMA1_IT_TC4) != RESET)
    {
        DMA_ClearFlag(DMA1_FLAG_TC4);
        DMA_Cmd(DMA1_Channel4, DISABLE);
    }
}



/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/
