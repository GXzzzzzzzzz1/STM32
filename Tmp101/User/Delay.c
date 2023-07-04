#include "delay.h"


/**
 * @bieaf 微秒延时函数
 * @detail 注意:由于指令执行需要时间, 实际的延时精度约为1微秒
 *
 * @param unsigned int Delay 延时的微秒
 */
void delay_us(unsigned int Delay)
{
    uint32_t tickstart = SysTick->VAL;                            ///<获取当前tick
    uint32_t tickNum = 0;
    uint32_t tickMax = SysTick->LOAD + 1;
    uint32_t delay_usvalue = (tickMax / 1000) * Delay;            ///<计算一共需要延时的tick
    while(1)
    {
        uint32_t cur_tick = SysTick->VAL;
        if (cur_tick > tickstart)                                 ///<进行了一次重载
        {
            tickNum = tickstart + (tickMax - cur_tick);
        }
        else                                                      ///<未进行过重载
        {
            tickNum = tickstart - cur_tick;
        }

        if (tickNum > delay_usvalue)                              ///<达到延时的tick数
        {
            return;
        }
    }
}
