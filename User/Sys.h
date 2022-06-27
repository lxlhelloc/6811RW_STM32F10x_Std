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

//工作状态结构体
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

//系统工作模式   0x00:初始模式 		0x01：充电模式 		0x02：放电模式 		0x03：充电保护模式  0x04：放电保护模式	0xff：等待关机
#define Sys_Mode_Init								0x00
#define Sys_Mode_Charge							0x01
#define Sys_Mode_Discharge					0x02
#define Sys_Mode_ChargeProtect			0x03
#define Sys_Mode_DischargeProtect		0x04
#define Sys_Mode_Close							0xff

#define Ladder_Current			0.5					//以0.5A为阶梯，降低充电电流
#define Ladder_Current_Min	0.5					//最小充电电流
#define ReCharge_Vol				0.95				//0.95倍		重新开启充电最大电压条件
#define ReCharge_Temp				0.65				//0.65倍		重新开启充电温度条件
#define OverCurrent_Num			10					//过放电流采集次数  每次采集间隔为数据更新周期 DataAcquisitionCycle  时间为DataAcquisitionCycle*10


#endif
