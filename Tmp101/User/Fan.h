#ifndef __FAN_H
#define __FAN_H

#include "tim.h"

//ռ�ձ�0 - 100 ��Ӧֵ 0 - 40
void Fan_Init(void);
void SetFanSpeed(int16_t rawTemp);


#endif /* __FAN_H */
