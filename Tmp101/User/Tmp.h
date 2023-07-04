#ifndef __TMP_H
#define __TMP_H

#include "i2c.h"


#define HARDWAREI2C 0   

//���üĴ������ϵ�Ĭ��0x80��
//SD 	1-����	 				0-����ת��
//TM 	1-�ж�ģʽ     	1-�Ƚ�ģʽ
//POL 1-�ߵ�ƽ��Ч  		0-�͵�ƽ��Ч      ��Ӱ��ALERT���ԣ�
//F1	��¼��������
//F0 	��¼��������
//R0
//R1  ����ת��ʱ��ͷֱ���
//OS/ALERT 1-��ALERT���ó��ж����ţ�������ֵ����Զ������ж�


//�¶ȵľ�����0.0625
// 1 ���϶� 16
//10 ���϶� 160
//30 ���϶� 480
#define TMP101_ADDRESS 			  0x49    //add �ӵ� 1001000 ���� 1001001 �ߵ�ƽ 1001010 //6.30 0x4a
#define TMP101_ADDRESS_WRITE 	0x92    
#define TMP101_ADDRESS_READ 	0x93    

#define POINTER_REGISTER			0x00
#define TMP101_REG_TEMP 			0x00
#define TMP101_REG_CONF 			0x01
#define TMP101_REG_HIGHLIMIT 	0x02
#define TMP101_REG_LOWLIMIT 	0x03
#define TMP101_REG_ALERT		 	0x04

void TMP101_Init(void);

/*	Ӳ��I2C	*/
#if HARDWAREI2C

float TMP101_ReadTemperature(void);
int16_t TMP101_ReadRawData(void);
#else

/*	���I2C	*/
uint8_t ReadTMP101Temperature(int16_t *temperature);
#endif



#endif /* __TMP_H */
