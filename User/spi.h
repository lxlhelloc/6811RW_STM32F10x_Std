#ifndef __SPI_H
#define	__SPI_H

#include "stm32f10x.h"


#define SPI_CS_0 GPIO_ResetBits(GPIOA, GPIO_Pin_4)
#define SPI_CS_1 GPIO_SetBits(GPIOA, GPIO_Pin_4)

void SPI1_Init(void);
u8 SPI1_SendByte(u8 TxData);


void SPI2_Init(void);
u8 SPI2_SendByte(u8 byte);



#endif

