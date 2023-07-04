#ifndef __FAN_H
#define __FAN_H

#include "tim.h"

//占空比0 - 100 对应值 0 - 40
void Fan_Init(void);
void SetFanSpeed(int16_t rawTemp);


#endif /* __FAN_H */
