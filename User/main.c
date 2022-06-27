  

#include "main.h"

#ifdef DEBUG

    unsigned char i = 0;
    u16 test1 = 0;

#endif



int main(void)
{
	Sys_Init();
    

	while(1){

        LTC6811_Refresh_Data();
				_6811update();
        delay_ms(1000);
       
        #ifdef DEBUG
        printf("run ok!\n");
         i++;
        #endif

	}
}




void Sys_Init(void)
{
    USART1_DMA_Config(115200);  //USART1初始化 		用于422外部通信(内含配置数据 升级通信接口)
    USART2_DMA_Config(115200);  //USART2初始化 		用于422外部通信(内含配置数据 升级通信接口)
//	USART3_DMA_Config(115200);  //USART3初始化         用于外部TTL串口调试接口
    SPI1_Init();
    #ifdef DEBUG
        printf("System initialization succeeded\n");
    #endif
}

