#include "BMS.h"


/***********************BMS����*********************************/
//���ڵ�س�籣����ѹ�������شﵽ�õ�ѹֵʱ��ֹͣ��磩�������ã�

extern const u32 ChargeCellPortectionVoltage;
extern const u8 ActiveCellIndex[12];
extern const u8 ActiveCellQuantity;	
//���⿪����ѹ��
extern const u32 EqualizingVoltageDifference;

extern u16 CellVoltage[12];					//��ص�ѹ������
extern u16 CellTotalVol;						//�����ѹ

extern u16 test1;
extern u8 LTC6811_ADD;


u16 CellVols;
u16 Cell[13];

/************************����*********************************/
//��ȡ�����ѹ
u8 LTC6811_get_total_voltage(u16 *ptv)
{
	 u8 crc;
	 u8 dasiy_rxdata[8];//����2Byte PEC���8Byte
	 ltc6811_wakeup_standy(); 
	 LTC681x_adstat(MD_27KHZ_14KHZ,STAT_CH_SOC);
	 ltc6811_delay_ms(1);
	 LTC681x_rdstat_reg(1,1,dasiy_rxdata);
	 if(check_crc(dasiy_rxdata,1*8,&crc)==0)
	 {
	 	*ptv=*(u16*)(&dasiy_rxdata[0]);
        #ifdef DEBUG
            printf("get total voltage successful!\n"); 
            test1 = 1;
        #endif
		return 1;
	 }
	 return 0;
}
//��ȡÿ�ڵ�ص�ѹ  12�ڵ�ص�ѹ���� p����Ϊһ��u16���͵�12���ȵ�����
u8 LTC6811_Get_PerCell_Voltage(u16* p)
{
	u8 i = 0,j = 0;
	u8 crc = 0;
	u8 dasiy_rxdata[8];
	u8 ReturnValue = 1;
	ltc6811_wakeup_standy();
	LTC681x_clrcell(); 
	ltc6811_delay_ms(5);
	LTC681x_adcvax(MD_27KHZ_14KHZ/*MD_7KHZ_3KHZ*/,DCP_DISABLED);
	ltc6811_delay_ms(10);
	for(i=1;i<5;i++)
	{
		LTC681x_rdcv_reg(i,1,dasiy_rxdata);
		if(check_crc(dasiy_rxdata,8,&crc)==0)
		{
			for(j=0;j<3;j++)
				*(p+j+3*(i-1)) = dasiy_rxdata[j*2] + (dasiy_rxdata[j*2+1] << 8);
			ReturnValue = 1;
			continue;
		}
		else
		{
			ReturnValue = 0;
			break;
		}
	}
	return ReturnValue;
}


void LTC6811_Get_GPIO1_GPIO3(u16 *temp)
{
	u8 crc;
    u8 dasiy_rxdata[8];
	CELLV *Auxv;
	ltc6811_wakeup_standy(); 
	LTC681x_clraux();
	ltc6811_delay_ms(6);
	wakeup_idle(2);		//ֻ��AUX 
	ltc6811_wakeup_standy(); 

	LTC681x_adax(MD_7KHZ_3KHZ,AUX_CH_ALL); 
//	LTC681x_adax(MD_7KHZ_3KHZ,AUX_CH_GPIO1);
	delay_us(250);
	
//	LTC681x_adcvax(MD_26HZ_2KHZ,DCP_DISABLED);
//	LTC681x_adcvax(MD_7KHZ_3KHZ,DCP_ENABLED);
	ltc6811_delay_ms(6);
	LTC681x_rdaux_reg(1,1,dasiy_rxdata);
	if(check_crc(dasiy_rxdata,1*8,&crc)==0)
	{
		Auxv=(CELLV*)(dasiy_rxdata);
		temp[0]=Auxv->cell1;
		temp[1]=Auxv->cell2;
		temp[2]=Auxv->cell3;
	}
}


void LTC6811_Get_GPIO4_VREF2(u16 *temp)
{
	u8 crc;
    u8 dasiy_rxdata[8];
	CELLV *Auxv;
	ltc6811_wakeup_standy(); 
	LTC681x_clraux();
	ltc6811_delay_ms(6);
	wakeup_idle(2);		//ֻ��AUX 
	ltc6811_wakeup_standy(); 

	LTC681x_adax(MD_7KHZ_3KHZ,AUX_CH_ALL); 
//	LTC681x_adax(MD_7KHZ_3KHZ,AUX_CH_GPIO1);
	delay_us(250);
	
//	LTC681x_adcvax(MD_26HZ_2KHZ,DCP_DISABLED);
//	LTC681x_adcvax(MD_7KHZ_3KHZ,DCP_ENABLED);
	ltc6811_delay_ms(6);
	LTC681x_rdaux_reg(2,1,dasiy_rxdata);
	if(check_crc(dasiy_rxdata,1*8,&crc)==0)
	{
		Auxv=(CELLV*)(dasiy_rxdata);
		temp[0]=Auxv->cell1;
		temp[1]=Auxv->cell2;
		temp[2]=Auxv->cell3;
	}
}








//������
u8 LTC6811_ActiveBallancing(u8 CellNumber)									//
{
	u8 i = 0;
	u16 PEC = 0;
	u8 RxData[8],TxData[8],WriteBuffer[12];//����2Byte PEC���8Byte
	ltc6811_wakeup_standy();
	ltc6811_delay_ms(5);
//	if((LTC6804_rdcfg(RxData) == 1) && (CellNumber >= 1))//12�ڵ�ض����ŵ�Ļ�CellNumber = 0
	if((LTC6804_rdcfg(RxData) == 1))//12�ڵ�ض����ŵ�Ļ�CellNumber = 0
	{
		for(i=0;i<4;i++)
			*(TxData + i) = *(RxData + i);//�Ѷ�������ǰ4Byte���Ƹ����ͻ���
		if((CellNumber >=1 ) && (CellNumber <= 8))
		{
			TxData[4] = 0x01 << (CellNumber - 1);
			TxData[5] = 0x00;
		}
		else if((CellNumber >8 ) && (CellNumber <= 12))
		{
			TxData[4] = 0x00;
			TxData[5] = 0x01 << (CellNumber - 9);
		}
		else if(CellNumber==0||CellNumber==0xff)		//�رվ���
		{	
			TxData[4] = 0x00;
			TxData[5] = 0x00;
		}

		WriteBuffer[0] = 0x80;
		WriteBuffer[1] = 0x01;
		PEC = pec15_calc(2, WriteBuffer);
		WriteBuffer[2] = (uint8_t)(PEC >> 8);
		WriteBuffer[3] = (uint8_t)(PEC);
		PEC = pec15_calc(6, TxData);
		for(i=0;i<6;i++)
			WriteBuffer[i+4] = TxData[i];
		WriteBuffer[10] = (uint8_t)(PEC >> 8);
		WriteBuffer[11] = (uint8_t)(PEC);
		DASILY_RESET();
 		delay_us(10);
  		spi_write_array(12,WriteBuffer);
  		delay_us(10);
  		DASILY_SET();
		return 1;
	}
	else
		return 0;
}

//����ֵ0������Ҫ���� 1�����⣨BalanceCeLLIndexΪ��Ҫ����ĵ��λ�ã� 2������
u8 BMS_CheckCellStackStatus(u16 *CellStackVoltage,u8 *BalanceCellIndex)
{
	unsigned char i;
	unsigned int MaxVoltageCellIndex=0,MinVoltageCellIndex=0;
//	if(LTC6811_Get_PerCell_Voltage(CellStackVoltage)==1)
	{
		for(i=0;i<ActiveCellQuantity;i++)
		{
//			if(ChargeCellPortectionVoltage <= *(CellStackVoltage + ActiveCellIndex[i]-1))
//			{
//				*BalanceCellIndex = 0xFF;
//				return 2;
//			}
			if(*(CellStackVoltage + MaxVoltageCellIndex) < *(CellStackVoltage + ActiveCellIndex[i]-1))
			{
				MaxVoltageCellIndex = ActiveCellIndex[i]-1;
			}
			if(*(CellStackVoltage + MinVoltageCellIndex) > *(CellStackVoltage + ActiveCellIndex[i]-1))
			{
				MinVoltageCellIndex = ActiveCellIndex[i]-1;
			}
		}
		
		if(*(CellStackVoltage + MaxVoltageCellIndex) - *(CellStackVoltage + MinVoltageCellIndex) >= EqualizingVoltageDifference)
		{
			*BalanceCellIndex = MaxVoltageCellIndex + 1;
			return 1;
		}
		else
		{
			*BalanceCellIndex = 0xFF;
			return 0;
		}
	}
//	return 0;
}

/********************ˢ�»���������*************************/
  u8 LTC6811_Refresh_Data(void)
{
	u8 re=1,a1;
	u16 temp1[3] = {1,2,3};
	if(LTC6811_get_total_voltage(&CellTotalVol)==0)					//��ѹ��ȡʧ��
	{
		printf("get total voltage failed!\n");
		re=0;
        test1 = 2;
	}
	if(LTC6811_Get_PerCell_Voltage(CellVoltage)==0)					//���ڵ�ѹ��ȡʧ��
	{
		printf("get percell voltage failed!\n");
		re=0;
        test1 = 3;
	}
    else{
        printf("get percell voltage successful!\n");
        test1 = 4;
    }
		for(a1=0;a1<3;a1++)
		{
			printf("%d\n",temp1[a1]);
		}
		LTC6811_Get_GPIO1_GPIO3(temp1);
		
		for(a1=0;a1<3;a1++)
		{
			printf("%d\n",temp1[a1]);
		}
		LTC6811_Get_GPIO4_VREF2(temp1);
		for(a1=0;a1<3;a1++)
		{
			printf("%d\n",temp1[a1]);
		}
		

	return re;
}
void BMS_GetMaxMinVoltage(u16 *max,u16 *min)
{
	u8 i;
	u8 MaxVoltageCellIndex=0,MinVoltageCellIndex=0;
	for(i=0;i<ActiveCellQuantity;i++)
	{
		if(*(CellVoltage + MaxVoltageCellIndex) < *(CellVoltage + ActiveCellIndex[i]-1))
		{
			MaxVoltageCellIndex = ActiveCellIndex[i]-1;
		}
		if(*(CellVoltage + MinVoltageCellIndex) > *(CellVoltage + ActiveCellIndex[i]-1))
		{
			MinVoltageCellIndex = ActiveCellIndex[i]-1;
		}
	}
	*max=CellVoltage[MaxVoltageCellIndex];
	*min=CellVoltage[MinVoltageCellIndex];
}






void GetBatteryTotalVol(u16* vol)
{
    u16 _6811TotalVol1,_6811TotalVol2;
		

    LTC6811_ADD = 0x80;//���õ�һƬ6811��ַ
    LTC6811_get_total_voltage(&_6811TotalVol1);

    LTC6811_ADD = 0x80;//���õڶ�Ƭ6811��ַ
//		LTC6811_ADD = 0x88;//���õڶ�Ƭ6811��ַ
    LTC6811_get_total_voltage(&_6811TotalVol2);

    *vol =_6811TotalVol1+_6811TotalVol2;
}


void GetBatteryCellVol(u16* p)
{
    u16 vol1[12],vol2[12];
    
    LTC6811_ADD = 0x80;//���õ�һƬ6811��ַ
    LTC6811_Get_PerCell_Voltage(vol1);

    LTC6811_ADD = 0x80;//���õڶ�Ƭ6811��ַ
		//LTC6811_ADD = 0x88;//���õڶ�Ƭ6811��ַ
    LTC6811_Get_PerCell_Voltage(vol2);

    //�ܹ�13�ڵ��
    p[0] = vol1[0];
    p[1] = vol1[1];
    p[2] = vol1[2];
    p[3] = vol1[3];
    p[4] = vol1[4];
    p[5] = vol1[6];
    p[6] = vol1[7];
    p[7] = vol2[0];
    p[8] = vol2[1];
    p[9] = vol2[2];
    p[10] = vol2[3];
    p[11] = vol2[4];
    p[12] = vol2[6];

    
}


void _6811update(void)
{
	GetBatteryTotalVol(&CellVols);
	GetBatteryCellVol(Cell);

}
