#ifndef __TMP_H
#define __TMP_H

#include "i2c.h"


#define HARDWAREI2C 0   

//配置寄存器（上电默认0x80）
//SD 	1-休眠	 				0-连续转换
//TM 	1-中断模式     	1-比较模式
//POL 1-高电平有效  		0-低电平有效      （影响ALERT极性）
//F1	记录报警次数
//F0 	记录报警次数
//R0
//R1  数据转换时间和分辨率
//OS/ALERT 1-将ALERT设置成中断引脚，超过阈值则会自动触发中断


//温度的精度是0.0625
// 1 摄氏度 16
//10 摄氏度 160
//30 摄氏度 480
#define TMP101_ADDRESS 			  0x49    //add 接地 1001000 悬空 1001001 高电平 1001010 //6.30 0x4a
#define TMP101_ADDRESS_WRITE 	0x92    
#define TMP101_ADDRESS_READ 	0x93    

#define POINTER_REGISTER			0x00
#define TMP101_REG_TEMP 			0x00
#define TMP101_REG_CONF 			0x01
#define TMP101_REG_HIGHLIMIT 	0x02
#define TMP101_REG_LOWLIMIT 	0x03
#define TMP101_REG_ALERT		 	0x04

void TMP101_Init(void);

/*	硬件I2C	*/
#if HARDWAREI2C

float TMP101_ReadTemperature(void);
int16_t TMP101_ReadRawData(void);
#else

/*	软件I2C	*/
uint8_t ReadTMP101Temperature(int16_t *temperature);
#endif



#endif /* __TMP_H */
