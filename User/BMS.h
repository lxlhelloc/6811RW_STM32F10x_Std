#ifndef _BMS_H
#define _BMS_H

#include "stm32f10x.h"
#include "LTC6811_W.h"
#include "Spi.h"
#include "systick.h"
//#include "Sys.h"
#include "stdio.h"
#include "main.h"

typedef struct {
	u16 cell1;
	u16 cell2;
	u16 cell3;
}CELLV;
void LTC6811_Get_GPIO1_GPIO3(u16 *temp);
void LTC6811_Get_GPIO4_VREF2(u16 *temp);

u8 LTC6811_get_total_voltage(u16 *ptv);
u8 LTC6811_Get_PerCell_Voltage(u16* p);
u8 LTC6811_ActiveBallancing(u8 CellNumber);
u8 BMS_CheckCellStackStatus(u16 *CellStatckVoltage,u8 *BalanceCeLLIndex);
u8 LTC6811_Refresh_Data(void);
void BMS_GetMaxMinVoltage(u16 *max,u16 *min);

void _6811update(void);

#endif
