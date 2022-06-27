#include "Sys.h"
//#include "Config.h"
//#include "Signal_GPIO_Config.h"
//#include "RWInformation.h"
//#include "Data_Transmission.h"
#define MAX_BUFFER_SIZE	512


const u32 CellTotalCapacity=12500;					//电池容量(mAh)
/*************************均衡参数****************************/
//均衡开启电压差
const u32 EqualizingVoltageDifference=500;	//50mV/100uV
//LTC6811电池组电池节数与对应位置
//const u8 ActiveCellIndex[12]={1,2,3,4,7,8,9,10};
const u8 ActiveCellIndex[12]={1,2,3,4,5,6,7,8,9,10,11,12};

//LTC6811电池组电池节数
const u8 ActiveCellQuantity=12;
//电池容量
float CellCapacity;

//电池已充电容量
float Charged_CellCapacity;

/*************************充电参数****************************/
//单节电池充电保护电压（任意电池达到该电压值时，停止充电）（可设置）
u32 ChargeCellPortectionVoltage=42500;		//4.25V/100uV

//单节最高充电电压(任意电池达到该电压值时，开始以0.5A为阶梯降低充电电流)
const u32 ChargeCellMaxVoltage=42000;			//4.2V/100uV

//已充电时间
u32 ChargedTime;
//充电剩余时间
u32 RemainingChargeTime;

/*************************放电参数****************************/
//单节电池放电截止电压(任意电池小于该电压值时,停止放电)	（可设置）
u32 DisChargeCellCutOffVoltage=30000;			//3V/100uV

//放电过流提示电流值40A
const float DisChargeCurrent=40;				//40A

//充电过流值8A （需关闭充电）
const float OverChargeCurrent=8;						//8A

//已放电时间
u32 DischargedTime;												
/************************温度参数*****************************/

//过温温度
float MAX_Temp=75;
//欠温温度
float Min_Temp=-30;

/************************时间参数****************************/

//1s
//电池组放电时间
const u32 DisChargeTime=500;						//500s 				放电时间
const u32 TurnDownELETime=5;						//5s					阶梯降电流时间
const u32 ChargeELETIME=6;						  //6s				  缓充容量累加时间

u32 TurnDownELE_Count;									//降电流计数
u8 TurnDownELE_Over;										//须降电流


//10ms

//数据发送周期
const u32 DataTransmissionCycle=20;			// 200ms

//数据采集周期
const u32 DataAcquisitionCycle=10;			  //100ms

//均衡开启时间点
const u32 BalanceOpenCycle=2;						

//均衡关闭时间点
const u32 BalanceCloseCycle=7;					

u32 Discharge_Conunt;										//充电计数
u8 Discharge_Over;											//充电完成标志

u32 SendData_Count;											//数据发送计数
u8 SendData_Over;												//需要发送数据

u32 RefreshData_Count;									//更新数据计数
u8 RefreshData_Over=0;									//需要更新数据

u32 OverDischargeCurrent_Count;					//放电过流计数
u8 OverDischargeCurrent_Over;						//放电过流标志

u32 BalanceCount_Open;									//均衡打开计数
u8 BalanceOpen_Over;										//均衡打开计数完毕

u32 BalanceCount_Close;								  //均衡关闭计数
u8 BalanceClose_Over;										//均衡关闭计数完毕

u32 ChargedTime_Count;									//充电时间计数

u32 ChargeCAP_Count;											//充电容量时间计数
u8 	ChargeCAP_Over=1;											//充电容量计数完毕					

u8 Balance_Flag;													//已开启均衡

u8 CloseCharge_Count;											//关闭充电计数
u8 ChargeOver_Count;											//充电完成计数
u8 CloseDischarge_Count;									//打开充电计数
const u8 Close_Num=5;											//关闭计数总次数


/*************************数据缓冲区*************************/

u8 data1[MAX_BUFFER_SIZE];				//USART1 接收缓冲区	
u32 data1_len;										//USART1 接收数据长度

u8 data2[MAX_BUFFER_SIZE];				//USART2 接收缓冲区	
u32 data2_len;										//USART2 接收数据长度

u8 data3[MAX_BUFFER_SIZE];				//USART3 接收缓冲区	
u32 data3_len;										//USART3 接收数据长度

u16 CellVoltage[12]={0};					//电池电压缓冲区			//实际电压值（V）*10000
u16 CellTotalVol;									//电池总压						//实际电压值（V）*500
u16 CellMaxVol;										//最大电压值
u16 CellMinVol;										//最小电压值

u8  CellBalanceIndex=0xff;				//需均衡的电池位置

ADC_ValueStructure adc_data;			//ADC采集缓冲区

float Temp_Max;										//温度（多个采集点最高温度）
/*
Signal_Structure signal_data;			//输入信号缓冲区

u8 Sys_WorkMode;									//系统工作模式   0x00:初始模式 		0x01：充电模式 		0x02：放电模式 		0x03：充电保护模式  0x04：放电保护模式	0xff：等待关机
State_Structure Sys_WorkState;		//系统工作状态	 bit0:温度 0：正常 1：过温    bit1：充电 0：正常 1：过充    bit2：放电 0：正常 1：过放  
																	//bit3: 0：均衡 1：未均衡 bit4: 0：未短路 1：短路 bit5：0：二级未保护 1：二级保护
																	//bit6：0：放电未过流 1：放电过流  bit7: 0：充电未过流 1：充电过流

u8 BMS_SelfTest;									//自检状态

Information_Structure	read_Information;						//读出来的存储的设置信息
Information_Structure	write_Information;					//需要写进去的存储的设置信息

Data_Transmission_Structure_Table1 Data_Table1;		//需要传输的数据

CanTxMsg	TxMessage;							//can发送缓冲区
CanRxMsg	RxMessage;							//can接收缓冲区
*/

