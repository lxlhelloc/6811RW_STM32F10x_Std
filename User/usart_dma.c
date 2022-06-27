#include "usart_dma.h"


/************************USART1****************************/

u8 usart1_buffer[MAX_BUFFER_SIZE];
u32 usart1_receive_len;



void USART1_DMA_Config(u32 baud)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	DMA_InitTypeDef	DMA_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	memset(usart1_buffer,'\0',MAX_BUFFER_SIZE);
	
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	
	//USART IO	CLOCK
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	//DMA IO CLOCK
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);
	
	//usart io config
	//usart	TX
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	//usart RX
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	//usart config
	USART_InitStructure.USART_BaudRate=baud;
	USART_InitStructure.USART_WordLength=USART_WordLength_8b;
	USART_InitStructure.USART_StopBits=USART_StopBits_1;
	USART_InitStructure.USART_Parity=USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl=USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode=USART_Mode_Rx|USART_Mode_Tx;
	USART_Init(USART1,&USART_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel=USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0;
	NVIC_Init(&NVIC_InitStructure);
	
	//dma config
	//参考手册选择DMA_Channel
	DMA_DeInit(DMA1_Channel5);
	
	DMA_InitStructure.DMA_PeripheralBaseAddr=(u32)&(USART1->DR);
	DMA_InitStructure.DMA_PeripheralInc=DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_PeripheralDataSize=DMA_PeripheralDataSize_Byte;
	DMA_InitStructure.DMA_MemoryBaseAddr=(u32)usart1_buffer;
	DMA_InitStructure.DMA_MemoryDataSize=DMA_MemoryDataSize_Byte;
	DMA_InitStructure.DMA_MemoryInc=DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_M2M=DMA_M2M_Disable;	
	DMA_InitStructure.DMA_DIR=DMA_DIR_PeripheralSRC;
	DMA_InitStructure.DMA_Priority=DMA_Priority_High;
	DMA_InitStructure.DMA_BufferSize=MAX_BUFFER_SIZE;
	DMA_InitStructure.DMA_Mode=DMA_Mode_Circular;
	
	DMA_Init(DMA1_Channel5,&DMA_InitStructure);
	DMA_Cmd(DMA1_Channel5,ENABLE);
	
	//usart cmd
	USART_ITConfig(USART1,USART_IT_IDLE,ENABLE);
	USART_DMACmd(USART1,USART_DMAReq_Rx,ENABLE);
	USART_Cmd(USART1,ENABLE);
	
}

bool USART1_Read_DATA(u8 *data,u32 *len)
{
	if(usart1_receive_len!=0)
	{
		memcpy(data,usart1_buffer,usart1_receive_len);
		*len=usart1_receive_len;
		usart1_receive_len=0;
		return TRUE;
	}
	data=NULL;
	len=0;
	return FALSE;
}

void USART1_Write_DATA(u8 *data,u32 len)
{
	while(len--)
	{
		USART_SendData(USART1,*data);
		data++;
		while(USART_GetFlagStatus(USART1,USART_FLAG_TXE)==RESET);
	}
}




/************************USART2****************************/
u8 usart2_buffer[MAX_BUFFER_SIZE];
u32 usart2_receive_len;




void USART2_DMA_Config(u32 baud)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	DMA_InitTypeDef	DMA_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	memset(usart2_buffer,'\0',MAX_BUFFER_SIZE);
	
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	
	//USART IO	CLOCK
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	//DMA IO CLOCK
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);
	
	//usart io config
	//usart	TX
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	//usart RX
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	//usart config
	USART_InitStructure.USART_BaudRate=baud;
	USART_InitStructure.USART_WordLength=USART_WordLength_8b;
	USART_InitStructure.USART_StopBits=USART_StopBits_1;
	USART_InitStructure.USART_Parity=USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl=USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode=USART_Mode_Rx|USART_Mode_Tx;
	USART_Init(USART2,&USART_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel=USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=1;
	NVIC_Init(&NVIC_InitStructure);
	
	//dma config
	//参考手册选择DMA_Channel
	DMA_DeInit(DMA1_Channel6);
	
	DMA_InitStructure.DMA_PeripheralBaseAddr=(u32)&(USART2->DR);
	DMA_InitStructure.DMA_PeripheralInc=DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_PeripheralDataSize=DMA_PeripheralDataSize_Byte;
	DMA_InitStructure.DMA_MemoryBaseAddr=(u32)usart2_buffer;
	DMA_InitStructure.DMA_MemoryDataSize=DMA_MemoryDataSize_Byte;
	DMA_InitStructure.DMA_MemoryInc=DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_M2M=DMA_M2M_Disable;	
	DMA_InitStructure.DMA_DIR=DMA_DIR_PeripheralSRC;
	DMA_InitStructure.DMA_Priority=DMA_Priority_High;
	DMA_InitStructure.DMA_BufferSize=MAX_BUFFER_SIZE;
	DMA_InitStructure.DMA_Mode=DMA_Mode_Circular;
	
	DMA_Init(DMA1_Channel6,&DMA_InitStructure);
	DMA_Cmd(DMA1_Channel6,ENABLE);
	
	//usart cmd
	USART_ITConfig(USART2,USART_IT_IDLE,ENABLE);
	USART_DMACmd(USART2,USART_DMAReq_Rx,ENABLE);
	USART_Cmd(USART2,ENABLE);
}


bool USART2_Read_DATA(u8 *data,u32 *len)
{
	if(usart2_receive_len!=0)
	{
		memcpy(data,usart2_buffer,usart2_receive_len);
		*len=usart2_receive_len;
		usart2_receive_len=0;
		return TRUE;
	}
	data=NULL;
	len=0;
	return FALSE;
}

void USART2_Write_DATA(u8 *data,u32 len)
{
	while(len--)
	{
		USART_SendData(USART2,*data);
		data++;
		while(USART_GetFlagStatus(USART2,USART_FLAG_TXE)==RESET);
	}
}

/************************USART3***********************/

//int fputc(int ch,FILE *p)  //函数默认的，在使用printf函数时自动调用
//{
//	USART_SendData(USART3,(u8)ch);	
//	while(USART_GetFlagStatus(USART3,USART_FLAG_TXE)==RESET);
//	return ch;
//}

u8 usart3_buffer[MAX_BUFFER_SIZE];
u32 usart3_receive_len;


void USART3_DMA_Config(u32 baud)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	DMA_InitTypeDef	DMA_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	memset(usart3_buffer,'\0',MAX_BUFFER_SIZE);
	
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	
	//USART IO	CLOCK
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	
	//DMA IO CLOCK
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);
	
	//usart io config
	//usart	TX
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	//usart RX
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	
	//usart config
	USART_InitStructure.USART_BaudRate=baud;
	USART_InitStructure.USART_WordLength=USART_WordLength_8b;
	USART_InitStructure.USART_StopBits=USART_StopBits_1;
	USART_InitStructure.USART_Parity=USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl=USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode=USART_Mode_Rx|USART_Mode_Tx;
	USART_Init(USART3,&USART_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel=USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=1;
	NVIC_Init(&NVIC_InitStructure);
	
	//dma config
	//参考手册选择DMA_Channel
	DMA_DeInit(DMA1_Channel2);
	
	DMA_InitStructure.DMA_PeripheralBaseAddr=(u32)&(USART3->DR);
	DMA_InitStructure.DMA_PeripheralInc=DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_PeripheralDataSize=DMA_PeripheralDataSize_Byte;
	DMA_InitStructure.DMA_MemoryBaseAddr=(u32)usart3_buffer;
	DMA_InitStructure.DMA_MemoryDataSize=DMA_MemoryDataSize_Byte;
	DMA_InitStructure.DMA_MemoryInc=DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_M2M=DMA_M2M_Disable;	
	DMA_InitStructure.DMA_DIR=DMA_DIR_PeripheralSRC;
	DMA_InitStructure.DMA_Priority=DMA_Priority_High;
	DMA_InitStructure.DMA_BufferSize=MAX_BUFFER_SIZE;
	DMA_InitStructure.DMA_Mode=DMA_Mode_Circular;
	
	DMA_Init(DMA1_Channel2,&DMA_InitStructure);
	DMA_Cmd(DMA1_Channel2,ENABLE);
	
	//usart cmd
	USART_ITConfig(USART3,USART_IT_IDLE,ENABLE);
	USART_DMACmd(USART3,USART_DMAReq_Rx,ENABLE);
	USART_Cmd(USART3,ENABLE);
}


bool USART3_Read_DATA(u8 *data,u32 *len)
{
	if(usart3_receive_len!=0)
	{
		memcpy(data,usart3_buffer,usart3_receive_len);
		*len=usart3_receive_len;
		usart3_receive_len=0;
		return TRUE;
	}
	data=NULL;
	len=0;
	return FALSE;
}

void USART3_Write_DATA(u8 *data,u32 len)
{
	while(len--)
	{
		USART_SendData(USART3,*data);
		data++;
		while(USART_GetFlagStatus(USART3,USART_FLAG_TXE)==RESET);
	}
}



/***************** 发送一个字符 **********************/
 void Usart_SendByte( USART_TypeDef * pUSARTx, uint8_t ch)
 {
 /* 发送一个字节数据到 USART */
    USART_SendData(pUSARTx,ch);

 /* 等待发送数据寄存器为空 */
 while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);
 }

/***************** 发送字符串 **********************/
 void Usart_SendString( USART_TypeDef * pUSARTx, char *str)
 {
    unsigned int k=0;
    do {
        Usart_SendByte( pUSARTx, *(str + k) );
        k++;
      } while (*(str + k)!='\0');

     /* 等待发送完成 */
    while (USART_GetFlagStatus(pUSARTx,USART_FLAG_TC)==RESET);
 }




///重定向 c 库函数 printf 到串口，重定向后可使用 printf 函数
 int fputc(int ch, FILE *f)
 {
     /* 发送一个字节数据到串口 */
    USART_SendData(DEBUG_USARTx, (uint8_t) ch);

    /* 等待发送完毕 */
    while (USART_GetFlagStatus(DEBUG_USARTx, USART_FLAG_TXE) == RESET);

    return (ch);
 }

 ///重定向 c 库函数 scanf 到串口，重写向后可使用 scanf、 getchar 等函数
 int fgetc(FILE *f)
 {
    /* 等待串口输入数据 */
    while (USART_GetFlagStatus(DEBUG_USARTx, USART_FLAG_RXNE) == RESET);

    return (int)USART_ReceiveData(DEBUG_USARTx);
 }






