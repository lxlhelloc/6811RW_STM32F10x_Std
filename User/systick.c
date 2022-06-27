/*
*****************************************************************************
*  文件名称：systick.h, systick.c
*  描述    ：
*  硬件    ：
*  作者    ：
*  库版本  ：ST3.5.0
*  日期    ：2016-01-18
*****************************************************************************
*/


#include "systick.h"
/*****72MHz******delay_xs***********/
void my_delay_us(unsigned int Nus)
{
	unsigned short int Time;
	while(Nus--)
	{
	 	Time=3;
		while(Time--);
	}	
}

void my_delay_ms(unsigned short int Nms)
{
	unsigned short int Time;
	while(Nms--)
	{
	 	Time=8000;
		while(Time--);	
	}
}



void delay_ms(unsigned short int Nms)// Nms	<1864
{   
  unsigned int temp;
  SysTick->VAL=0;
 SysTick->LOAD=((Nms*9000) & 0xffffff); //给重装载寄存器赋值，9000时，将产生1ms的时基
 SysTick->CTRL |= 0x00000001;               //SysTick_CLKSource设置为72/8M并 开始计数   
// while(!(SysTick->CTRL&(1<<16)));   //等待时间到达
	do 
	{ 
	temp=SysTick->CTRL; 
	} 
	while(temp&0x01 && !(temp&(1<<16)));//等待时间到达  
	 
 SysTick->CTRL  = 0X00000000;         //关闭计数器
  SysTick->VAL=0;
} 


 
//延时us          				   //static __INLINE uint32_t SysTick_Config(uint32_t ticks)
void delay_us(unsigned int Nus)	  //SysTick_CLKSourceConfig(uint32_t Sy sTick_CLKSource) 
{
  unsigned int temp;
 SysTick->VAL=0;
 SysTick->LOAD=((Nus*9) & 0xffffff);       //时间加载     
 SysTick->CTRL |= 0x00000001;            //SysTick_CLKSource设置为72/8M并 开始计数   
// while(!(SysTick->CTRL&(1<<16)));//等待时间到达
	do 
	{ 
	temp=SysTick->CTRL; 
	} 
	while(temp&0x01 && !(temp&(1<<16)));//等待时间到达	   
 SysTick->CTRL  = 0X00000000;       //关闭计数器 
  SysTick->VAL=0;
}

//
//static __IO u32 TimingDelay;
// 
///**
//  * @brief  启动系统滴答定时器 SysTick
//  * @param  无
//  * @retval 无
//  */
//void SysTick_Init(void)
//{
//	/* SystemFrequency / 1000    1ms中断一次
//	 * SystemFrequency / 100000	 10us中断一次
//	 * SystemFrequency / 1000000 1us中断一次
//	 */
////	if (SysTick_Config(SystemFrequency / 100000))	// ST3.0.0库版本
//	if (SysTick_Config(SystemCoreClock / 1000000))	// ST3.5.0库版本
//	{ 
//		/* Capture error */ 
//		while (1);
//	}
//		// 关闭滴答定时器  
////	SysTick->CTRL &= ~ SysTick_CTRL_ENABLE_Msk;
//}
//
///**
//  * @brief   us延时程序,1us为一个单位
//  * @param  
//  *		@arg nTime: Delay_us( 1 ) 则实现的延时为 1 * 1us = 1us
//  * @retval  无
//  */
//void delay_us(__IO u32 nTime)
//{ 
//	TimingDelay = nTime;	
//
//	// 使能滴答定时器  
////	SysTick->CTRL |=  SysTick_CTRL_ENABLE_Msk;
//
//	while(TimingDelay != 0);
//}
//
//
//
///**
//  * @brief  获取节拍程序
//  * @param  无
//  * @retval 无
//  * @attention  在 SysTick 中断函数 SysTick_Handler()调用
//  */
//void TimingDelay_Decrement(void)
//{
//	if (TimingDelay != 0x00)
//	{ 
//		TimingDelay--;
//	}
//}



/*********************************************END OF FILE**********************/
