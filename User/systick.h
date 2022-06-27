#ifndef __SYSTICK_H__
#define __SYSTICK_H__

#include "stm32f10x.h"

void my_delay_ms(unsigned  short int Nms);
void my_delay_us(unsigned int Nus);

void delay_ms(unsigned  short int Nms);
void delay_us(unsigned int Nus);
//void SysTick_Init(void);
//void delay_us(__IO u32 nTime);
//void TimingDelay_Decrement(void);
//#define delay_ms(x) delay_us(1000*x)	 //单位ms
//#define delay_s(x) delay_ms(100*x)	 //单位ms

#endif
