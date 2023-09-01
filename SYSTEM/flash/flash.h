#ifndef __INTERNAL_FLASH_H
#define	__INTERNAL_FLASH_H

#include "stm32f10x.h"
#include "sys.h"
/* STM32大容量产品每页大小2KByte，中、小容量产品每页大小1KByte */
#if defined (STM32F10X_HD) || defined (STM32F10X_HD_VL) || defined (STM32F10X_CL) || defined (STM32F10X_XL)
#define FLASH_PAGE_SIZE    ((uint16_t)0x800)	//2048
#else
#define FLASH_PAGE_SIZE    ((uint16_t)0x400)	//1024
#endif

//FLASH起始地址
#define STM32_FLASH_BASE 0x08000000 	//STM32 FLASH的起始地址


//测试 起始地址与结束地址
#define WRITE_START_ADDR  ((uint32_t)0x08008000)
#define WRITE_END_ADDR    ((uint32_t)0x0800C000)
//内部flash页起始地址

#define Page000_Address_Start  ((uint32_t))0x08000000
#define Page001_Address_Start  ((uint32_t))0x08000800
#define Page002_Address_Start  ((uint32_t))0x08001000
#define Page003_Address_Start  ((uint32_t))0x08001800
#define Page004_Address_Start  ((uint32_t))0x08002000
#define Page005_Address_Start  ((uint32_t))0x08002800
#define Page006_Address_Start  ((uint32_t))0x08003000
#define Page007_Address_Start  ((uint32_t))0x08003800
#define Page008_Address_Start  ((uint32_t))0x08004000
#define Page009_Address_Start  ((uint32_t))0x08004800
#define Page010_Address_Start  ((uint32_t))0x08005000
#define Page011_Address_Start  ((uint32_t))0x08005800
#define Page012_Address_Start  ((uint32_t))0x08006000
#define Page013_Address_Start  ((uint32_t))0x08006800
#define Page014_Address_Start  ((uint32_t))0x08007000
#define Page015_Address_Start  ((uint32_t))0x08007800
#define Page016_Address_Start  ((uint32_t))0x08008000
#define Page017_Address_Start  ((uint32_t))0x08008800
#define Page018_Address_Start  ((uint32_t))0x08009000
#define Page019_Address_Start  ((uint32_t))0x08009800
#define Page020_Address_Start  ((uint32_t))0x0800A000
#define Page021_Address_Start  ((uint32_t))0x0800A800
#define Page022_Address_Start  ((uint32_t))0x0800B000
#define Page023_Address_Start  ((uint32_t))0x0800B800
#define Page024_Address_Start  ((uint32_t))0x0800C000
#define Page025_Address_Start  ((uint32_t))0x0800C800
#define Page026_Address_Start  ((uint32_t))0x0800D000
#define Page027_Address_Start  ((uint32_t))0x0800D800
#define Page028_Address_Start  ((uint32_t))0x0800E000
#define Page029_Address_Start  ((uint32_t))0x0800E800
#define Page030_Address_Start  ((uint32_t))0x0800F000
#define Page031_Address_Start  ((uint32_t))0x0800F800
#define Page032_Address_Start  ((uint32_t))0x08010000
#define Page033_Address_Start  ((uint32_t))0x08010800
#define Page034_Address_Start  ((uint32_t))0x08011000
#define Page035_Address_Start  ((uint32_t))0x08011800
#define Page036_Address_Start  ((uint32_t))0x08012000
#define Page037_Address_Start  ((uint32_t))0x08012800
#define Page038_Address_Start  ((uint32_t))0x08013000
#define Page039_Address_Start  ((uint32_t))0x08013800
#define Page040_Address_Start  ((uint32_t))0x08014000
#define Page041_Address_Start  ((uint32_t))0x08014800
#define Page042_Address_Start  ((uint32_t))0x08015000
#define Page043_Address_Start  ((uint32_t))0x08015800
#define Page044_Address_Start  ((uint32_t))0x08016000
#define Page045_Address_Start  ((uint32_t))0x08016800
#define Page046_Address_Start  ((uint32_t))0x08017000
#define Page047_Address_Start  ((uint32_t))0x08017800
#define Page048_Address_Start  ((uint32_t))0x08018000
#define Page049_Address_Start  ((uint32_t))0x08018800
#define Page050_Address_Start  ((uint32_t))0x08019000
#define Page051_Address_Start  ((uint32_t))0x08019800
#define Page052_Address_Start  ((uint32_t))0x0801A000
#define Page053_Address_Start  ((uint32_t))0x0801A800
#define Page054_Address_Start  ((uint32_t))0x0801B000
#define Page055_Address_Start  ((uint32_t))0x0801B800
#define Page056_Address_Start  ((uint32_t))0x0801C000
#define Page057_Address_Start  ((uint32_t))0x0801C800
#define Page058_Address_Start  ((uint32_t))0x0801D000
#define Page059_Address_Start  ((uint32_t))0x0801D800
#define Page060_Address_Start  ((uint32_t))0x0801E000
#define Page061_Address_Start  ((uint32_t))0x0801E800
#define Page062_Address_Start  ((uint32_t))0x0801F000
#define Page063_Address_Start  ((uint32_t))0x0801F800
#define Page064_Address_Start  ((uint32_t))0x08020000
#define Page065_Address_Start  ((uint32_t))0x08020800
#define Page066_Address_Start  ((uint32_t))0x08021000
#define Page067_Address_Start  ((uint32_t))0x08021800
#define Page068_Address_Start  ((uint32_t))0x08022000
#define Page069_Address_Start  ((uint32_t))0x08022800
#define Page070_Address_Start  ((uint32_t))0x08023000
#define Page071_Address_Start  ((uint32_t))0x08023800
#define Page072_Address_Start  ((uint32_t))0x08024000
#define Page073_Address_Start  ((uint32_t))0x08024800
#define Page074_Address_Start  ((uint32_t))0x08025000
#define Page075_Address_Start  ((uint32_t))0x08025800
#define Page076_Address_Start  ((uint32_t))0x08026000
#define Page077_Address_Start  ((uint32_t))0x08026800
#define Page078_Address_Start  ((uint32_t))0x08027000
#define Page079_Address_Start  ((uint32_t))0x08027800
#define Page080_Address_Start  ((uint32_t))0x08028000
#define Page081_Address_Start  ((uint32_t))0x08028800
#define Page082_Address_Start  ((uint32_t))0x08029000
#define Page083_Address_Start  ((uint32_t))0x08029800
#define Page084_Address_Start  ((uint32_t))0x0802A000
#define Page085_Address_Start  ((uint32_t))0x0802A800
#define Page086_Address_Start  ((uint32_t))0x0802B000
#define Page087_Address_Start  ((uint32_t))0x0802B800
#define Page088_Address_Start  ((uint32_t))0x0802C000
#define Page089_Address_Start  ((uint32_t))0x0802C800
#define Page090_Address_Start  ((uint32_t))0x0802D000
#define Page091_Address_Start  ((uint32_t))0x0802D800
#define Page092_Address_Start  ((uint32_t))0x0802E000
#define Page093_Address_Start  ((uint32_t))0x0802E800
#define Page094_Address_Start  ((uint32_t))0x0802F000
#define Page095_Address_Start  ((uint32_t))0x0802F800
#define Page096_Address_Start  ((uint32_t))0x08030000
#define Page097_Address_Start  ((uint32_t))0x08030800
#define Page098_Address_Start  ((uint32_t))0x08031000
#define Page099_Address_Start  ((uint32_t))0x08031800
#define Page100_Address_Start  ((uint32_t))0x08032000
#define Page101_Address_Start  ((uint32_t))0x08032800
#define Page102_Address_Start  ((uint32_t))0x08033000
#define Page103_Address_Start  ((uint32_t))0x08033800
#define Page104_Address_Start  ((uint32_t))0x08034000
#define Page105_Address_Start  ((uint32_t))0x08034800
#define Page106_Address_Start  ((uint32_t))0x08035000
#define Page107_Address_Start  ((uint32_t))0x08035800
#define Page108_Address_Start  ((uint32_t))0x08036000
#define Page109_Address_Start  ((uint32_t))0x08036800
#define Page110_Address_Start  ((uint32_t))0x08037000
#define Page111_Address_Start  ((uint32_t))0x08037800
#define Page112_Address_Start  ((uint32_t))0x08038000
#define Page113_Address_Start  ((uint32_t))0x08038800
#define Page114_Address_Start  ((uint32_t))0x08039000
#define Page115_Address_Start  ((uint32_t))0x08039800
#define Page116_Address_Start  ((uint32_t))0x0803A000
#define Page117_Address_Start  ((uint32_t))0x0803A800
#define Page118_Address_Start  ((uint32_t))0x0803B000
#define Page119_Address_Start  ((uint32_t))0x0803B800
#define Page120_Address_Start  ((uint32_t))0x0803C000
#define Page121_Address_Start  ((uint32_t))0x0803C800
#define Page122_Address_Start  ((uint32_t))0x0803D000
#define Page123_Address_Start  ((uint32_t))0x0803D800
#define Page124_Address_Start  ((uint32_t))0x0803E000
#define Page125_Address_Start  ((uint32_t))0x0803E800
#define Page126_Address_Start  ((uint32_t))0x0803F000
#define Page127_Address_Start  ((uint32_t))0x0803F800



typedef enum
{
    FAILED = 0,
    PASSED = !FAILED
} FlashStatus;


void Flash_WriteNoCheck(uint32_t addr,uint16_t* pBuffer,uint16_t len);

void Flash_Write(uint32_t StartAddr,uint16_t* pBuffer,uint16_t len);

void FLASH_ReadData(uint32_t StartAddr,uint16_t* EntryPrama,uint16_t len);

FlashStatus InternalFlash_Test(void);

uint16_t FLASH_ReadHalfWord(uint32_t address);

uint32_t FLASH_ReadWord(uint32_t address);

#endif /* __INTERNAL_FLASH_H */

