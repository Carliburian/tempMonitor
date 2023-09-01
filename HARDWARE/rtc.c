/**
  ************************************* Copyright ******************************
  *
  *                 (C) Copyright 2023,DrMa,China, GCU.
  *                            All Rights Reserved
  *
  *                     By(ElonMa)
  *
  *
  * FileName   : rtc.c
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
#include "rtc.h"
#include <time.h>
//到时候更改这个数组就行
uint16_t MyRTC_Time[] = {2023, 8, 18, 23, 59, 55};
//设定时间函数
char* time_str;
static char tm_str[80];
//static char time_str[80];
/* -------------------------------- begin  -------------------------------- */
/**
  * @Name    RTC_Init
  * @brief
  * @param   : [输入/出]
  * @retval
  * @author  DrMa
  * @Data    2023-08-18
  * 1. ...
  * <modify staff>:
  * <data>        :
  * <description> :
  * 2. ...
 **/
/* -------------------------------- end -------------------------------- */

void RTC_Init()
{
    RTC_NVIC_Configuration();
    if (BKP_ReadBackupRegister(BKP_DR1) != 0xA5A5)
    {
        /* Backup data register value is not correct or not yet programmed (when
           the first time the program is executed) */

        printk("\r\n\n RTC未配置....");

        /* RTC Configuration */
        RTC_Configuration();

        printk("\r\n RTC时钟已配置...");

        /* Adjust time by values entered by the user on the hyperterminal */
        Time_Adjust();
        printk("\r\n时间已经调整...");
        BKP_WriteBackupRegister(BKP_DR1, 0xA5A5);
    }
    else
    {
        /*检查是否由系统掉电引起复位 */
        if (RCC_GetFlagStatus(RCC_FLAG_PORRST) != RESET)
        {
            printk("\r\n\n电源上电....");
        }
        else if (RCC_GetFlagStatus(RCC_FLAG_PINRST) != RESET)
        {
            printk("\r\n\n 外部复位触发....");
        }

        printk("\r\n 无需配置RTC....");

        RTC_WaitForSynchro();
        RTC_ITConfig(RTC_IT_SEC, ENABLE);
        RTC_WaitForLastTask();
    }

}



/* -------------------------------- begin  -------------------------------- */
/**
  * @Name    RTC_NVIC_Configuration
  * @brief
  * @param   : [输入/出]
  * @retval
  * @author  DrMa
  * @Data    2023-08-18
  * 1. ...
  * <modify staff>:
  * <data>        :
  * <description> :
  * 2. ...
 **/
/* -------------------------------- end -------------------------------- */

void RTC_NVIC_Configuration()
{
    NVIC_InitTypeDef NVIC_InitStructure;

    /* Configure one bit for preemption priority */
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

    /* Enable the RTC Interrupt */
    NVIC_InitStructure.NVIC_IRQChannel = RTC_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}







/* -------------------------------- begin  -------------------------------- */
/**
  * @Name    RTC_Configuration
  * @brief
  * @param   : [输入/出]
  * @retval
  * @author  DrMa
  * @Data    2023-08-18
  * 1. ...
  * <modify staff>:
  * <data>        :
  * <description> :
  * 2. ...
 **/
/* -------------------------------- end -------------------------------- */

void  RTC_Configuration()
{
//		uint16_t t=0;
    /* Enable PWR and BKP clocks */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);

    /* Allow access to BKP Domain */
    PWR_BackupAccessCmd(ENABLE);

    /* Reset Backup Domain */
    BKP_DeInit();

    /* Enable LSE */
    RCC_LSEConfig(RCC_LSE_ON);
    /* Wait till LSE is ready */
    while ((RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET));
    if(RCC_GetFlagStatus(RCC_FLAG_LSERDY)!=RESET)
        printk("\r\nLSE低速时钟初始化完成");
    else
        printk("\r\nLSE低速时钟初始化失败");
    /* Select LSE as RTC Clock Source */
    RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);

    /* Enable RTC Clock */
    RCC_RTCCLKCmd(ENABLE);

    /* 等待寄存器时钟同步 */
    RTC_WaitForSynchro();

    /* Wait until last write operation on RTC registers has finished */
    RTC_WaitForLastTask();

    /* 使能秒中断 */
    RTC_ITConfig(RTC_IT_SEC, ENABLE);

    /* Wait until last write operation on RTC registers has finished */
    RTC_WaitForLastTask();

    /* Set RTC prescaler: set RTC period to 1sec */
    RTC_SetPrescaler(32767); /* RTC period = RTCCLK/RTC_PR = (32.768 KHz)/(32767+1) */

    /* Wait until last write operation on RTC registers has finished */
    RTC_WaitForLastTask();

}


/* -------------------------------- begin  -------------------------------- */
/**
  * @Name    Time_Adjust
  * @brief  	需要更改系统时间时可以直接更改上面的myrtc_time数组
  * @param   : [输入/出]
  * @retval
  * @author  DrMa
  * @Data    2023-08-18
  * 1. ...
  * <modify staff>:
  * <data>        :
  * <description> :
  * 2. ...
 **/
/* -------------------------------- end -------------------------------- */

void Time_Adjust()
{
    time_t time_cnt; //是一个64位无符号整型，作为秒钟计时能表示到2106年
    struct tm time_date;

    time_date.tm_year = MyRTC_Time[0] - 1900;
    time_date.tm_mon = MyRTC_Time[1] - 1;
    time_date.tm_mday = MyRTC_Time[2];
    time_date.tm_hour = MyRTC_Time[3];
    time_date.tm_min = MyRTC_Time[4];
    time_date.tm_sec = MyRTC_Time[5];
    time_cnt = mktime(&time_date) - 8 * 60 * 60;
    /* Wait until last write operation on RTC registers has finished */
    RTC_WaitForLastTask();
    /* 设定当前时间 */
    RTC_SetCounter(time_cnt);//3600*h+ 60*min+ sec;
    /* Wait until last write operation on RTC registers has finished */
    RTC_WaitForLastTask();
}


/* -------------------------------- begin  -------------------------------- */
/**
  * @Name    RTC_ReadTime
  * @brief
  * @param   : [输入/出]
  * @retval
  * @author  DrMa
  * @Data    2023-08-18
  * 1. ...
  * <modify staff>:
  * <data>        :
  * <description> :
  * 2. ...
 **/
/* -------------------------------- end -------------------------------- */

char* RTC_ReadTime()
{
    time_t time_cnt;
    struct tm time_date;

    time_cnt = RTC_GetCounter() + 8 * 60 * 60;//获取计数器值，+8表示东8区时区，伦敦时间转换为
    //北京时间
    time_date = *localtime(&time_cnt);//将秒钟计数值转换成年月日的表示形式，并将其存储在
    //struct tm 结构体里面,localtime使用的是伦敦时间表示方式

    /************************转换方式一，换成tm结构体*********************/


//		MyRTC_Time[0] = time_date.tm_year + 1900;//从1900年开始计数，所以要加上1900偏移，以下同理
//		MyRTC_Time[1] = time_date.tm_mon + 1;
//		MyRTC_Time[2] = time_date.tm_mday;
//		MyRTC_Time[3] = time_date.tm_hour;
//		MyRTC_Time[4] = time_date.tm_min;
//		MyRTC_Time[5] = time_date.tm_sec;

    /************************转换方式一，换成tm结构体*********************/




    /************************转换方式二，使用Ctime函数，换成字符串*********************/

//	time_str=ctime(&time_cnt);

    /*注：asctime函数作用与ctime一样，只不过传入的参数不是int指针，而是tm结构体指针*/

    /************************转换方式二，使用Ctime函数，换成字符串*********************/






    /************************转换方式三，使用strftime函数，换成自定义字符串*********************/

//size_t strftime(char *str, size_t maxsize, const char *format, const struct tm *timeptr)

    /***
    @param: 字符串名，长度，显示格式，tm结构体指针
    ***/
//strftime(tm_str,80,"%A  %B - %d  %H : %M : %S",&time_date);
    strftime(tm_str,80,"%H:%M:%S",&time_date);
    return tm_str;
    /************************转换方式三，使用strftime函数，换成自定义字符串*********************/

}




