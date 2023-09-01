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
//��ʱ���������������
uint16_t MyRTC_Time[] = {2023, 8, 18, 23, 59, 55};
//�趨ʱ�亯��
char* time_str;
static char tm_str[80];
//static char time_str[80];
/* -------------------------------- begin  -------------------------------- */
/**
  * @Name    RTC_Init
  * @brief
  * @param   : [����/��]
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

        printk("\r\n\n RTCδ����....");

        /* RTC Configuration */
        RTC_Configuration();

        printk("\r\n RTCʱ��������...");

        /* Adjust time by values entered by the user on the hyperterminal */
        Time_Adjust();
        printk("\r\nʱ���Ѿ�����...");
        BKP_WriteBackupRegister(BKP_DR1, 0xA5A5);
    }
    else
    {
        /*����Ƿ���ϵͳ��������λ */
        if (RCC_GetFlagStatus(RCC_FLAG_PORRST) != RESET)
        {
            printk("\r\n\n��Դ�ϵ�....");
        }
        else if (RCC_GetFlagStatus(RCC_FLAG_PINRST) != RESET)
        {
            printk("\r\n\n �ⲿ��λ����....");
        }

        printk("\r\n ��������RTC....");

        RTC_WaitForSynchro();
        RTC_ITConfig(RTC_IT_SEC, ENABLE);
        RTC_WaitForLastTask();
    }

}



/* -------------------------------- begin  -------------------------------- */
/**
  * @Name    RTC_NVIC_Configuration
  * @brief
  * @param   : [����/��]
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
  * @param   : [����/��]
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
        printk("\r\nLSE����ʱ�ӳ�ʼ�����");
    else
        printk("\r\nLSE����ʱ�ӳ�ʼ��ʧ��");
    /* Select LSE as RTC Clock Source */
    RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);

    /* Enable RTC Clock */
    RCC_RTCCLKCmd(ENABLE);

    /* �ȴ��Ĵ���ʱ��ͬ�� */
    RTC_WaitForSynchro();

    /* Wait until last write operation on RTC registers has finished */
    RTC_WaitForLastTask();

    /* ʹ�����ж� */
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
  * @brief  	��Ҫ����ϵͳʱ��ʱ����ֱ�Ӹ��������myrtc_time����
  * @param   : [����/��]
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
    time_t time_cnt; //��һ��64λ�޷������ͣ���Ϊ���Ӽ�ʱ�ܱ�ʾ��2106��
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
    /* �趨��ǰʱ�� */
    RTC_SetCounter(time_cnt);//3600*h+ 60*min+ sec;
    /* Wait until last write operation on RTC registers has finished */
    RTC_WaitForLastTask();
}


/* -------------------------------- begin  -------------------------------- */
/**
  * @Name    RTC_ReadTime
  * @brief
  * @param   : [����/��]
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

    time_cnt = RTC_GetCounter() + 8 * 60 * 60;//��ȡ������ֵ��+8��ʾ��8��ʱ�����׶�ʱ��ת��Ϊ
    //����ʱ��
    time_date = *localtime(&time_cnt);//�����Ӽ���ֵת���������յı�ʾ��ʽ��������洢��
    //struct tm �ṹ������,localtimeʹ�õ����׶�ʱ���ʾ��ʽ

    /************************ת����ʽһ������tm�ṹ��*********************/


//		MyRTC_Time[0] = time_date.tm_year + 1900;//��1900�꿪ʼ����������Ҫ����1900ƫ�ƣ�����ͬ��
//		MyRTC_Time[1] = time_date.tm_mon + 1;
//		MyRTC_Time[2] = time_date.tm_mday;
//		MyRTC_Time[3] = time_date.tm_hour;
//		MyRTC_Time[4] = time_date.tm_min;
//		MyRTC_Time[5] = time_date.tm_sec;

    /************************ת����ʽһ������tm�ṹ��*********************/




    /************************ת����ʽ����ʹ��Ctime�����������ַ���*********************/

//	time_str=ctime(&time_cnt);

    /*ע��asctime����������ctimeһ����ֻ��������Ĳ�������intָ�룬����tm�ṹ��ָ��*/

    /************************ת����ʽ����ʹ��Ctime�����������ַ���*********************/






    /************************ת����ʽ����ʹ��strftime�����������Զ����ַ���*********************/

//size_t strftime(char *str, size_t maxsize, const char *format, const struct tm *timeptr)

    /***
    @param: �ַ����������ȣ���ʾ��ʽ��tm�ṹ��ָ��
    ***/
//strftime(tm_str,80,"%A  %B - %d  %H : %M : %S",&time_date);
    strftime(tm_str,80,"%H:%M:%S",&time_date);
    return tm_str;
    /************************ת����ʽ����ʹ��strftime�����������Զ����ַ���*********************/

}




