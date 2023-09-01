#ifndef _TM7705_H
#define _TM7705_H
#include <string.h>
#include "sys.h"
#include "spi_dma.h"

#define  DataRDY GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_12)

/*================下面为芯片的驱动数据定义============================*/



/* 通信寄存器bit定义 */
enum
{
    /* 寄存器选择  默认与通讯寄存器连接，写操作，正常工作模式，通道已经选择完毕 */

    REG_COMM	= 0x00,	/* 通信寄存器 */
    REG_SETUP	= 0x10,	/* 设置寄存器 */
    REG_CLOCK	= 0x20,	/* 时钟寄存器 */
    REG_DATA	= 0x30,	/* 数据寄存器 */
    REG_SETUP_CH2=0x11,/*配置寄存器通道二*/ //0001 0001
    REG_CLOCK_CH2=0x21,/*时钟寄存器通道二*/
    REG_DATA_CH2=0x31,/*数据寄存器通道二*/
    REG_ZERO_CH1	= 0x60,	/* CH1 偏移寄存器 */
    REG_FULL_CH1	= 0x70,	/* CH1 满量程寄存器 */
    REG_ZERO_CH2	= 0x61,	/* CH2 偏移寄存器 */
    REG_FULL_CH2	= 0x71,	/* CH2 满量程寄存器 */

    /* 读写操作 */
    WRITE 		= 0x00,	/* 写操作 */
    READ 		= 0x08,	/* 读操作 */

    /* 通道 */
    CH_1		= 0,	/* AIN1+  AIN1- */
    CH_2		= 1,	/* AIN2+  AIN2- */

};



/* 设置寄存器bit定义 */
enum
{
    MD_NORMAL		= (0 << 6),	/* 正常模式 */
    MD_CAL_SELF		= (1 << 6),	/* 自校准模式 */
    MD_CAL_ZERO		= (2 << 6),	/* 校准0刻度模式 */
    MD_CAL_FULL		= (3 << 6),	/* 校准满刻度模式 */

    GAIN_1			= (0 << 3),	/* 增益 */
    GAIN_2			= (1 << 3),	/* 增益 */
    GAIN_4			= (2 << 3),	/* 增益 */
    GAIN_8			= (3 << 3),	/* 增益 */
    GAIN_16			= (4 << 3),	/* 增益 */
    GAIN_32			= (5 << 3),	/* 增益 */
    GAIN_64			= (6 << 3),	/* 增益 */
    GAIN_128		= (7 << 3),	/* 增益 */

    /* 无论双极性还是单极性都不改变任何输入信号的状态，它只改变输出数据的代码和转换函数上的校准点 */
    BIPOLAR			= (0 << 2),	/* 双极性输入 */
    UNIPOLAR		= (1 << 2),	/* 单极性输入 */

    BUF_NO			= (0 << 1),	/* 输入无缓冲（内部缓冲器不启用) */
    BUF_EN			= (1 << 1),	/* 输入有缓冲 (启用内部缓冲器) 可处理高阻抗源 */

    FSYNC_0			= 0,
    FSYNC_1			= 1		/* 不启用 */
};

/* 时钟寄存器bit定义 */
enum
{
    CLKDIS_0	= 0x00,		/* 时钟输出使能 （当外接晶振时，必须使能才能振荡） */
    CLKDIS_1	= 0x10,		/* 时钟禁止 （当外部提供时钟时，设置该位可以禁止MCK_OUT引脚输出时钟以省电 */

    /*
    	2.4576MHz（CLKDIV=0 ）或为 4.9152MHz （CLKDIV=1 ），CLK 应置 “0”。
    	1MHz （CLKDIV=0 ）或 2MHz   （CLKDIV=1 ），CLK 该位应置  “1”
    */
    CLK_4_9152M = 0x08,
    CLK_2_4576M = 0x00,
    CLK_1M 		= 0x04,
    CLK_2M 		= 0x0C,

    /*输出更新速率设置*/
    FS_20HZ     = 0X00,
    FS_25HZ     = 0x01,
    FS_100HZ    = 0x20,
    FS_200HZ    = 0x03,
    FS_50HZ		= 0x04,
    FS_60HZ		= 0x05,
    FS_250HZ	= 0x06,
    FS_500HZ	= 0x07,

    /*
    	四十九、电子秤应用中提高TM7705 精度的方法
    		当使用主时钟为 2.4576MHz 时，强烈建议将时钟寄存器设为 84H,此时数据输出更新率为10Hz,即每0.1S 输出一个新数据。
    		当使用主时钟为 1MHz 时，强烈建议将时钟寄存器设为80H, 此时数据输出更新率为4Hz, 即每0.25S 输出一个新数据
    */
    ZERO_0		= 0x00,
    ZERO_1		= 0x80
};









void TM7705_Init(void);                               			/*初始化*/
void TM7705_Calibself(void); /*TM7705自校准函数*/
void TM7705_WriteReg(u8 reg_name,u8 data);       /*向TM7705指定寄存器写入数据*/
int TM7705_RDY(void);
u16 TM7705_ReadData_CH1(void);       					           /*读TM7705的采样数据*/
u16 TM7705_ReadData_CH2(void);
void DRDY_EXTI_Open(void);         /*打开中断触发*/
void DRDY_EXTI_Close(void);        /*关闭中断触发*/
void DRDYINT_Init(void);   /*DRDY中断配置初始化*/













#endif
