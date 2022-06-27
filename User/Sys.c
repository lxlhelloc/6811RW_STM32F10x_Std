#include "Sys.h"
//#include "Config.h"
//#include "Signal_GPIO_Config.h"
//#include "RWInformation.h"
//#include "Data_Transmission.h"
#define MAX_BUFFER_SIZE	512


const u32 CellTotalCapacity=12500;					//�������(mAh)
/*************************�������****************************/
//���⿪����ѹ��
const u32 EqualizingVoltageDifference=500;	//50mV/100uV
//LTC6811������ؽ������Ӧλ��
//const u8 ActiveCellIndex[12]={1,2,3,4,7,8,9,10};
const u8 ActiveCellIndex[12]={1,2,3,4,5,6,7,8,9,10,11,12};

//LTC6811������ؽ���
const u8 ActiveCellQuantity=12;
//�������
float CellCapacity;

//����ѳ������
float Charged_CellCapacity;

/*************************������****************************/
//���ڵ�س�籣����ѹ�������شﵽ�õ�ѹֵʱ��ֹͣ��磩�������ã�
u32 ChargeCellPortectionVoltage=42500;		//4.25V/100uV

//������߳���ѹ(�����شﵽ�õ�ѹֵʱ����ʼ��0.5AΪ���ݽ��ͳ�����)
const u32 ChargeCellMaxVoltage=42000;			//4.2V/100uV

//�ѳ��ʱ��
u32 ChargedTime;
//���ʣ��ʱ��
u32 RemainingChargeTime;

/*************************�ŵ����****************************/
//���ڵ�طŵ��ֹ��ѹ(������С�ڸõ�ѹֵʱ,ֹͣ�ŵ�)	�������ã�
u32 DisChargeCellCutOffVoltage=30000;			//3V/100uV

//�ŵ������ʾ����ֵ40A
const float DisChargeCurrent=40;				//40A

//������ֵ8A ����رճ�磩
const float OverChargeCurrent=8;						//8A

//�ѷŵ�ʱ��
u32 DischargedTime;												
/************************�¶Ȳ���*****************************/

//�����¶�
float MAX_Temp=75;
//Ƿ���¶�
float Min_Temp=-30;

/************************ʱ�����****************************/

//1s
//�����ŵ�ʱ��
const u32 DisChargeTime=500;						//500s 				�ŵ�ʱ��
const u32 TurnDownELETime=5;						//5s					���ݽ�����ʱ��
const u32 ChargeELETIME=6;						  //6s				  ���������ۼ�ʱ��

u32 TurnDownELE_Count;									//����������
u8 TurnDownELE_Over;										//�뽵����


//10ms

//���ݷ�������
const u32 DataTransmissionCycle=20;			// 200ms

//���ݲɼ�����
const u32 DataAcquisitionCycle=10;			  //100ms

//���⿪��ʱ���
const u32 BalanceOpenCycle=2;						

//����ر�ʱ���
const u32 BalanceCloseCycle=7;					

u32 Discharge_Conunt;										//������
u8 Discharge_Over;											//�����ɱ�־

u32 SendData_Count;											//���ݷ��ͼ���
u8 SendData_Over;												//��Ҫ��������

u32 RefreshData_Count;									//�������ݼ���
u8 RefreshData_Over=0;									//��Ҫ��������

u32 OverDischargeCurrent_Count;					//�ŵ��������
u8 OverDischargeCurrent_Over;						//�ŵ������־

u32 BalanceCount_Open;									//����򿪼���
u8 BalanceOpen_Over;										//����򿪼������

u32 BalanceCount_Close;								  //����رռ���
u8 BalanceClose_Over;										//����رռ������

u32 ChargedTime_Count;									//���ʱ�����

u32 ChargeCAP_Count;											//�������ʱ�����
u8 	ChargeCAP_Over=1;											//��������������					

u8 Balance_Flag;													//�ѿ�������

u8 CloseCharge_Count;											//�رճ�����
u8 ChargeOver_Count;											//�����ɼ���
u8 CloseDischarge_Count;									//�򿪳�����
const u8 Close_Num=5;											//�رռ����ܴ���


/*************************���ݻ�����*************************/

u8 data1[MAX_BUFFER_SIZE];				//USART1 ���ջ�����	
u32 data1_len;										//USART1 �������ݳ���

u8 data2[MAX_BUFFER_SIZE];				//USART2 ���ջ�����	
u32 data2_len;										//USART2 �������ݳ���

u8 data3[MAX_BUFFER_SIZE];				//USART3 ���ջ�����	
u32 data3_len;										//USART3 �������ݳ���

u16 CellVoltage[12]={0};					//��ص�ѹ������			//ʵ�ʵ�ѹֵ��V��*10000
u16 CellTotalVol;									//�����ѹ						//ʵ�ʵ�ѹֵ��V��*500
u16 CellMaxVol;										//����ѹֵ
u16 CellMinVol;										//��С��ѹֵ

u8  CellBalanceIndex=0xff;				//�����ĵ��λ��

ADC_ValueStructure adc_data;			//ADC�ɼ�������

float Temp_Max;										//�¶ȣ�����ɼ�������¶ȣ�
/*
Signal_Structure signal_data;			//�����źŻ�����

u8 Sys_WorkMode;									//ϵͳ����ģʽ   0x00:��ʼģʽ 		0x01�����ģʽ 		0x02���ŵ�ģʽ 		0x03����籣��ģʽ  0x04���ŵ籣��ģʽ	0xff���ȴ��ػ�
State_Structure Sys_WorkState;		//ϵͳ����״̬	 bit0:�¶� 0������ 1������    bit1����� 0������ 1������    bit2���ŵ� 0������ 1������  
																	//bit3: 0������ 1��δ���� bit4: 0��δ��· 1����· bit5��0������δ���� 1����������
																	//bit6��0���ŵ�δ���� 1���ŵ����  bit7: 0�����δ���� 1��������

u8 BMS_SelfTest;									//�Լ�״̬

Information_Structure	read_Information;						//�������Ĵ洢��������Ϣ
Information_Structure	write_Information;					//��Ҫд��ȥ�Ĵ洢��������Ϣ

Data_Transmission_Structure_Table1 Data_Table1;		//��Ҫ���������

CanTxMsg	TxMessage;							//can���ͻ�����
CanRxMsg	RxMessage;							//can���ջ�����
*/

