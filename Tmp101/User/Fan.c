#include "Fan.h"

void Fan_Init(){
	__HAL_TIM_SetCompare(&htim4, TIM_CHANNEL_3, 0);
	HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_3);
}

void SetFanSpeed(int16_t rawTemp){
		if(rawTemp<480){
			__HAL_TIM_SetCompare(&htim4, TIM_CHANNEL_3, 0);//0%
		}else if(rawTemp>=480 && rawTemp<640){
			__HAL_TIM_SetCompare(&htim4, TIM_CHANNEL_3, 4);//10%
		}else if(rawTemp>=640 && rawTemp<800){
			__HAL_TIM_SetCompare(&htim4, TIM_CHANNEL_3, 12);//30%
		}else if(rawTemp>=800 && rawTemp<960){
			__HAL_TIM_SetCompare(&htim4, TIM_CHANNEL_3, 20);//50%
		}else if(rawTemp>=960){
			__HAL_TIM_SetCompare(&htim4, TIM_CHANNEL_3, 28);//70%
		}
}
