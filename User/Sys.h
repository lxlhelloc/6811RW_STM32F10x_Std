#ifndef _SYS_H
#define _SYS_H

#include "stm32f10x.h"

typedef struct
{
	float temp_1;
	float temp_2;
	float temp_3;
	float charge_electricity;
	float discharge_eletricity;
	float nc;
} ADC_ValueStructure;

//����״̬�ṹ��
typedef union
{
	struct
	{
		u8 Temp_State	   								:1;
		u8 Charge_State  								:1;
		u8 Discharge_State							:1;
		u8 Balance_State								:1;
		u8 SC_State											:1;
		u8 Protect_State								:1;
		u8 OverDisChargeCurrent_State		:1;
		u8 OverChargeCurrent_State			:1;
	}State_bitdata;
	u8 State;
}State_Structure;

//ϵͳ����ģʽ   0x00:��ʼģʽ 		0x01�����ģʽ 		0x02���ŵ�ģʽ 		0x03����籣��ģʽ  0x04���ŵ籣��ģʽ	0xff���ȴ��ػ�
#define Sys_Mode_Init								0x00
#define Sys_Mode_Charge							0x01
#define Sys_Mode_Discharge					0x02
#define Sys_Mode_ChargeProtect			0x03
#define Sys_Mode_DischargeProtect		0x04
#define Sys_Mode_Close							0xff

#define Ladder_Current			0.5					//��0.5AΪ���ݣ����ͳ�����
#define Ladder_Current_Min	0.5					//��С������
#define ReCharge_Vol				0.95				//0.95��		���¿����������ѹ����
#define ReCharge_Temp				0.65				//0.65��		���¿�������¶�����
#define OverCurrent_Num			10					//���ŵ����ɼ�����  ÿ�βɼ����Ϊ���ݸ������� DataAcquisitionCycle  ʱ��ΪDataAcquisitionCycle*10


#endif
