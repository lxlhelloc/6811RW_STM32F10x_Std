#ifndef _USART_DMA_H
#define _USART_DMA_H

#include "string.h"
#include "stdio.h"
#include "stm32f10x.h"
#include "basic_date_str.h"



#define MAX_BUFFER_SIZE	512
#define DEBUG_USARTx USART2  //自定义调试串口



void USART1_DMA_Config(u32 baud);
bool USART1_Read_DATA(u8 *data,u32 *len);
void USART1_Write_DATA(u8 *data,u32 len);

void USART2_DMA_Config(u32 baud);
bool USART2_Read_DATA(u8 *data,u32 *len);
void USART2_Write_DATA(u8 *data,u32 len);

void USART3_DMA_Config(u32 baud);
bool USART3_Read_DATA(u8 *data,u32 *len);
void USART3_Write_DATA(u8 *data,u32 len);



void Usart_SendByte( USART_TypeDef * pUSARTx, uint8_t ch);
void Usart_SendString( USART_TypeDef * pUSARTx, char *str);





#endif
