#ifndef _TM7705_H
#define _TM7705_H
#include <string.h>
#include "sys.h"
#include "spi_dma.h"

#define  DataRDY GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_12)

/*================����ΪоƬ���������ݶ���============================*/



/* ͨ�żĴ���bit���� */
enum
{
    /* �Ĵ���ѡ��  Ĭ����ͨѶ�Ĵ������ӣ�д��������������ģʽ��ͨ���Ѿ�ѡ����� */

    REG_COMM	= 0x00,	/* ͨ�żĴ��� */
    REG_SETUP	= 0x10,	/* ���üĴ��� */
    REG_CLOCK	= 0x20,	/* ʱ�ӼĴ��� */
    REG_DATA	= 0x30,	/* ���ݼĴ��� */
    REG_SETUP_CH2=0x11,/*���üĴ���ͨ����*/ //0001 0001
    REG_CLOCK_CH2=0x21,/*ʱ�ӼĴ���ͨ����*/
    REG_DATA_CH2=0x31,/*���ݼĴ���ͨ����*/
    REG_ZERO_CH1	= 0x60,	/* CH1 ƫ�ƼĴ��� */
    REG_FULL_CH1	= 0x70,	/* CH1 �����̼Ĵ��� */
    REG_ZERO_CH2	= 0x61,	/* CH2 ƫ�ƼĴ��� */
    REG_FULL_CH2	= 0x71,	/* CH2 �����̼Ĵ��� */

    /* ��д���� */
    WRITE 		= 0x00,	/* д���� */
    READ 		= 0x08,	/* ������ */

    /* ͨ�� */
    CH_1		= 0,	/* AIN1+  AIN1- */
    CH_2		= 1,	/* AIN2+  AIN2- */

};



/* ���üĴ���bit���� */
enum
{
    MD_NORMAL		= (0 << 6),	/* ����ģʽ */
    MD_CAL_SELF		= (1 << 6),	/* ��У׼ģʽ */
    MD_CAL_ZERO		= (2 << 6),	/* У׼0�̶�ģʽ */
    MD_CAL_FULL		= (3 << 6),	/* У׼���̶�ģʽ */

    GAIN_1			= (0 << 3),	/* ���� */
    GAIN_2			= (1 << 3),	/* ���� */
    GAIN_4			= (2 << 3),	/* ���� */
    GAIN_8			= (3 << 3),	/* ���� */
    GAIN_16			= (4 << 3),	/* ���� */
    GAIN_32			= (5 << 3),	/* ���� */
    GAIN_64			= (6 << 3),	/* ���� */
    GAIN_128		= (7 << 3),	/* ���� */

    /* ����˫���Ի��ǵ����Զ����ı��κ������źŵ�״̬����ֻ�ı�������ݵĴ����ת�������ϵ�У׼�� */
    BIPOLAR			= (0 << 2),	/* ˫�������� */
    UNIPOLAR		= (1 << 2),	/* ���������� */

    BUF_NO			= (0 << 1),	/* �����޻��壨�ڲ�������������) */
    BUF_EN			= (1 << 1),	/* �����л��� (�����ڲ�������) �ɴ������迹Դ */

    FSYNC_0			= 0,
    FSYNC_1			= 1		/* ������ */
};

/* ʱ�ӼĴ���bit���� */
enum
{
    CLKDIS_0	= 0x00,		/* ʱ�����ʹ�� ������Ӿ���ʱ������ʹ�ܲ����񵴣� */
    CLKDIS_1	= 0x10,		/* ʱ�ӽ�ֹ �����ⲿ�ṩʱ��ʱ�����ø�λ���Խ�ֹMCK_OUT�������ʱ����ʡ�� */

    /*
    	2.4576MHz��CLKDIV=0 ����Ϊ 4.9152MHz ��CLKDIV=1 ����CLK Ӧ�� ��0����
    	1MHz ��CLKDIV=0 ���� 2MHz   ��CLKDIV=1 ����CLK ��λӦ��  ��1��
    */
    CLK_4_9152M = 0x08,
    CLK_2_4576M = 0x00,
    CLK_1M 		= 0x04,
    CLK_2M 		= 0x0C,

    /*���������������*/
    FS_20HZ     = 0X00,
    FS_25HZ     = 0x01,
    FS_100HZ    = 0x20,
    FS_200HZ    = 0x03,
    FS_50HZ		= 0x04,
    FS_60HZ		= 0x05,
    FS_250HZ	= 0x06,
    FS_500HZ	= 0x07,

    /*
    	��ʮ�š����ӳ�Ӧ�������TM7705 ���ȵķ���
    		��ʹ����ʱ��Ϊ 2.4576MHz ʱ��ǿ�ҽ��齫ʱ�ӼĴ�����Ϊ 84H,��ʱ�������������Ϊ10Hz,��ÿ0.1S ���һ�������ݡ�
    		��ʹ����ʱ��Ϊ 1MHz ʱ��ǿ�ҽ��齫ʱ�ӼĴ�����Ϊ80H, ��ʱ�������������Ϊ4Hz, ��ÿ0.25S ���һ��������
    */
    ZERO_0		= 0x00,
    ZERO_1		= 0x80
};









void TM7705_Init(void);                               			/*��ʼ��*/
void TM7705_Calibself(void); /*TM7705��У׼����*/
void TM7705_WriteReg(u8 reg_name,u8 data);       /*��TM7705ָ���Ĵ���д������*/
int TM7705_RDY(void);
u16 TM7705_ReadData_CH1(void);       					           /*��TM7705�Ĳ�������*/
u16 TM7705_ReadData_CH2(void);
void DRDY_EXTI_Open(void);         /*���жϴ���*/
void DRDY_EXTI_Close(void);        /*�ر��жϴ���*/
void DRDYINT_Init(void);   /*DRDY�ж����ó�ʼ��*/













#endif