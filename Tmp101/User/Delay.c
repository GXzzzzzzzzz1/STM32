#include "delay.h"


/**
 * @bieaf ΢����ʱ����
 * @detail ע��:����ָ��ִ����Ҫʱ��, ʵ�ʵ���ʱ����ԼΪ1΢��
 *
 * @param unsigned int Delay ��ʱ��΢��
 */
void delay_us(unsigned int Delay)
{
    uint32_t tickstart = SysTick->VAL;                            ///<��ȡ��ǰtick
    uint32_t tickNum = 0;
    uint32_t tickMax = SysTick->LOAD + 1;
    uint32_t delay_usvalue = (tickMax / 1000) * Delay;            ///<����һ����Ҫ��ʱ��tick
    while(1)
    {
        uint32_t cur_tick = SysTick->VAL;
        if (cur_tick > tickstart)                                 ///<������һ������
        {
            tickNum = tickstart + (tickMax - cur_tick);
        }
        else                                                      ///<δ���й�����
        {
            tickNum = tickstart - cur_tick;
        }

        if (tickNum > delay_usvalue)                              ///<�ﵽ��ʱ��tick��
        {
            return;
        }
    }
}
