  

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
    USART1_DMA_Config(115200);  //USART1��ʼ�� 		����422�ⲿͨ��(�ں��������� ����ͨ�Žӿ�)
    USART2_DMA_Config(115200);  //USART2��ʼ�� 		����422�ⲿͨ��(�ں��������� ����ͨ�Žӿ�)
//	USART3_DMA_Config(115200);  //USART3��ʼ��         �����ⲿTTL���ڵ��Խӿ�
    SPI1_Init();
    #ifdef DEBUG
        printf("System initialization succeeded\n");
    #endif
}

