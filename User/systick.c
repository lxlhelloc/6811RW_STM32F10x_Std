/*
*****************************************************************************
*  �ļ����ƣ�systick.h, systick.c
*  ����    ��
*  Ӳ��    ��
*  ����    ��
*  ��汾  ��ST3.5.0
*  ����    ��2016-01-18
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
 SysTick->LOAD=((Nms*9000) & 0xffffff); //����װ�ؼĴ�����ֵ��9000ʱ��������1ms��ʱ��
 SysTick->CTRL |= 0x00000001;               //SysTick_CLKSource����Ϊ72/8M�� ��ʼ����   
// while(!(SysTick->CTRL&(1<<16)));   //�ȴ�ʱ�䵽��
	do 
	{ 
	temp=SysTick->CTRL; 
	} 
	while(temp&0x01 && !(temp&(1<<16)));//�ȴ�ʱ�䵽��  
	 
 SysTick->CTRL  = 0X00000000;         //�رռ�����
  SysTick->VAL=0;
} 


 
//��ʱus          				   //static __INLINE uint32_t SysTick_Config(uint32_t ticks)
void delay_us(unsigned int Nus)	  //SysTick_CLKSourceConfig(uint32_t Sy sTick_CLKSource) 
{
  unsigned int temp;
 SysTick->VAL=0;
 SysTick->LOAD=((Nus*9) & 0xffffff);       //ʱ�����     
 SysTick->CTRL |= 0x00000001;            //SysTick_CLKSource����Ϊ72/8M�� ��ʼ����   
// while(!(SysTick->CTRL&(1<<16)));//�ȴ�ʱ�䵽��
	do 
	{ 
	temp=SysTick->CTRL; 
	} 
	while(temp&0x01 && !(temp&(1<<16)));//�ȴ�ʱ�䵽��	   
 SysTick->CTRL  = 0X00000000;       //�رռ����� 
  SysTick->VAL=0;
}

//
//static __IO u32 TimingDelay;
// 
///**
//  * @brief  ����ϵͳ�δ�ʱ�� SysTick
//  * @param  ��
//  * @retval ��
//  */
//void SysTick_Init(void)
//{
//	/* SystemFrequency / 1000    1ms�ж�һ��
//	 * SystemFrequency / 100000	 10us�ж�һ��
//	 * SystemFrequency / 1000000 1us�ж�һ��
//	 */
////	if (SysTick_Config(SystemFrequency / 100000))	// ST3.0.0��汾
//	if (SysTick_Config(SystemCoreClock / 1000000))	// ST3.5.0��汾
//	{ 
//		/* Capture error */ 
//		while (1);
//	}
//		// �رյδ�ʱ��  
////	SysTick->CTRL &= ~ SysTick_CTRL_ENABLE_Msk;
//}
//
///**
//  * @brief   us��ʱ����,1usΪһ����λ
//  * @param  
//  *		@arg nTime: Delay_us( 1 ) ��ʵ�ֵ���ʱΪ 1 * 1us = 1us
//  * @retval  ��
//  */
//void delay_us(__IO u32 nTime)
//{ 
//	TimingDelay = nTime;	
//
//	// ʹ�ܵδ�ʱ��  
////	SysTick->CTRL |=  SysTick_CTRL_ENABLE_Msk;
//
//	while(TimingDelay != 0);
//}
//
//
//
///**
//  * @brief  ��ȡ���ĳ���
//  * @param  ��
//  * @retval ��
//  * @attention  �� SysTick �жϺ��� SysTick_Handler()����
//  */
//void TimingDelay_Decrement(void)
//{
//	if (TimingDelay != 0x00)
//	{ 
//		TimingDelay--;
//	}
//}



/*********************************************END OF FILE**********************/
