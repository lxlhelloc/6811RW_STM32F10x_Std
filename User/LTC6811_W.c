#include "LTC6811_W.h"


// A3 A2 A1 A0   adr
// 0  0  0  0    80
// 0  0  0  1    88
// 0  0  1  0    90
// 0  0  1  1    98
// 0  1  0  0    A0
// 0  1  0  1    A8
// 0  1  1  0    B0
// 0  1  1  1    B8


u8 LTC6811_ADD=0x80;//ic地址片选LTC6811_ADD

static const uint16_t crc15Table[256]= {0x0,0xc599, 0xceab, 0xb32, 0xd8cf, 0x1d56, 0x1664, 0xd3fd, 0xf407, 0x319e, 0x3aac,  //!<precomputed CRC15 Table
                                0xff35, 0x2cc8, 0xe951, 0xe263, 0x27fa, 0xad97, 0x680e, 0x633c, 0xa6a5, 0x7558, 0xb0c1,
                                0xbbf3, 0x7e6a, 0x5990, 0x9c09, 0x973b, 0x52a2, 0x815f, 0x44c6, 0x4ff4, 0x8a6d, 0x5b2e,
                                0x9eb7, 0x9585, 0x501c, 0x83e1, 0x4678, 0x4d4a, 0x88d3, 0xaf29, 0x6ab0, 0x6182, 0xa41b,
                                0x77e6, 0xb27f, 0xb94d, 0x7cd4, 0xf6b9, 0x3320, 0x3812, 0xfd8b, 0x2e76, 0xebef, 0xe0dd,
                                0x2544, 0x2be, 0xc727, 0xcc15, 0x98c, 0xda71, 0x1fe8, 0x14da, 0xd143, 0xf3c5, 0x365c,
                                0x3d6e, 0xf8f7,0x2b0a, 0xee93, 0xe5a1, 0x2038, 0x7c2, 0xc25b, 0xc969, 0xcf0, 0xdf0d,
                                0x1a94, 0x11a6, 0xd43f, 0x5e52, 0x9bcb, 0x90f9, 0x5560, 0x869d, 0x4304, 0x4836, 0x8daf,
                                0xaa55, 0x6fcc, 0x64fe, 0xa167, 0x729a, 0xb703, 0xbc31, 0x79a8, 0xa8eb, 0x6d72, 0x6640,
                                0xa3d9, 0x7024, 0xb5bd, 0xbe8f, 0x7b16, 0x5cec, 0x9975, 0x9247, 0x57de, 0x8423, 0x41ba,
                                0x4a88, 0x8f11, 0x57c, 0xc0e5, 0xcbd7, 0xe4e, 0xddb3, 0x182a, 0x1318, 0xd681, 0xf17b,
                                0x34e2, 0x3fd0, 0xfa49, 0x29b4, 0xec2d, 0xe71f, 0x2286, 0xa213, 0x678a, 0x6cb8, 0xa921,
                                0x7adc, 0xbf45, 0xb477, 0x71ee, 0x5614, 0x938d, 0x98bf, 0x5d26, 0x8edb, 0x4b42, 0x4070,
                                0x85e9, 0xf84, 0xca1d, 0xc12f, 0x4b6, 0xd74b, 0x12d2, 0x19e0, 0xdc79, 0xfb83, 0x3e1a, 0x3528,
                                0xf0b1, 0x234c, 0xe6d5, 0xede7, 0x287e, 0xf93d, 0x3ca4, 0x3796, 0xf20f, 0x21f2, 0xe46b, 0xef59,
                                0x2ac0, 0xd3a, 0xc8a3, 0xc391, 0x608, 0xd5f5, 0x106c, 0x1b5e, 0xdec7, 0x54aa, 0x9133, 0x9a01,
                                0x5f98, 0x8c65, 0x49fc, 0x42ce, 0x8757, 0xa0ad, 0x6534, 0x6e06, 0xab9f, 0x7862, 0xbdfb, 0xb6c9,
                                0x7350, 0x51d6, 0x944f, 0x9f7d, 0x5ae4, 0x8919, 0x4c80, 0x47b2, 0x822b, 0xa5d1, 0x6048, 0x6b7a,
                                0xaee3, 0x7d1e, 0xb887, 0xb3b5, 0x762c, 0xfc41, 0x39d8, 0x32ea, 0xf773, 0x248e, 0xe117, 0xea25,
                                0x2fbc, 0x846, 0xcddf, 0xc6ed, 0x374, 0xd089, 0x1510, 0x1e22, 0xdbbb, 0xaf8, 0xcf61, 0xc453,
                                0x1ca, 0xd237, 0x17ae, 0x1c9c, 0xd905, 0xfeff, 0x3b66, 0x3054, 0xf5cd, 0x2630, 0xe3a9, 0xe89b,
                                0x2d02, 0xa76f, 0x62f6, 0x69c4, 0xac5d, 0x7fa0, 0xba39, 0xb10b, 0x7492, 0x5368, 0x96f1, 0x9dc3,
                                0x585a, 0x8ba7, 0x4e3e, 0x450c, 0x8095
                               };
uint16_t pec15_calc(uint8_t len, //Number of bytes that will be used to calculate a PEC
                    uint8_t *data //Array of data that will be used to calculate  a PEC
                   )
{
  uint16_t remainder,addr;
	uint16_t i;
  remainder = 16;//initialize the PEC
  for ( i = 0; i<len; i++) // loops for each byte in data array
  {
    addr = ((remainder>>7)^data[i])&0xff;//calculate PEC table address
//#ifdef MBED
    remainder = (remainder<<8)^crc15Table[addr];
//#else
    //remainder = (remainder<<8)^pgm_read_word_near(crc15Table+addr);
//#endif
  }
  return(remainder*2);//The CRC15 has a 0 in the LSB so the remainder must be multiplied by 2
}

void wakeup_idle(uint8_t total_ic)
{
	char i;
  for (  i =0; i<total_ic; i++)
  {
    DASILY_RESET();
    //delayMicroseconds(2); //Guarantees the isoSPI will be in ready mode
    SPI1_SendByte(0xff);
    DASILY_SET();
  }
}
void ltc6811_delay_ms(u16 ms)//保证isospi不sleep,同时保证core不发生5.5ms  sleep
{
	u16 i=0;
//	u16 counter,triger_counter;
	//TIM2_Start();
	//counter=ms*10;
  //triger_counter=10;
	/*
	while(TIM2->CNT <=counter)
	{ 
		if(TIM2->CNT>=triger_counter){
			triger_counter+=10;
			//wakeup_idle(IC_COUNT);
			wakeup_idle(1);;;
		}
	}
	*/
	//wakeup_idle(IC_COUNT);
	for(i=0;i<ms;i++)
		{
			wakeup_idle(1);
			delay_us(1000);
		}
	wakeup_idle(1);
	 
}

//Generic wakeup commannd to wake the LTC6813 from sleep
void wakeup_sleep(uint8_t total_ic)
{
	char i;
  for ( i =0; i<total_ic; i++)
  {
    DASILY_RESET();
    delay_us(600); // Guarantees the LTC6813 will be in standby
    DASILY_SET();
    delay_us(50);
  }
}

void spi_write_array(uint8_t len, 
                     uint8_t data[])
{
    uint8_t i; 
	//DASILY_RESET();
    for ( i = 0; i < len; i++ )
    {
        SPI1_SendByte(data[i]); 
    }
	//DASILY_SET();
}

void cmd_68(uint8_t tx_cmd[2])
{
  uint8_t cmd[4];
  uint16_t cmd_pec;

  cmd[0] = tx_cmd[0];
  #if LTC6811_2
	    cmd[0] = cmd[0]|LTC6811_ADD;
   #endif
  cmd[1] =  tx_cmd[1];
  cmd_pec = pec15_calc(2, cmd);
  cmd[2] = (uint8_t)(cmd_pec >> 8);
  cmd[3] = (uint8_t)(cmd_pec);
  DASILY_RESET();
  delay_us(10);
  spi_write_array(4,cmd);
  delay_us(10);
  DASILY_SET();
}

void spi_write_read(uint8_t  tx_Data[],
                    uint8_t  tx_len, 
                    uint8_t* rx_data, 
                    uint8_t  rx_len)
{
    uint8_t i;
 //   uint8_t data;
    for ( i = 0; i < tx_len; i++ )
    {
        // Transmit byte.
				SPI1_SendByte(tx_Data[i]);
    }
    for ( i = 0; i < rx_len; i++ )
    {
        // Receive byte.
        rx_data[i]=SPI1_SendByte(0xff);
    } 
}

void ReserveByteOrder(u8 *pData,u8 len)//反转字节序
{
		u8 tmp,i,j;
		j=len-1;
		for(i=0;i<j;i++,j--)
		{ 
				tmp=pData[i];
				pData[i]=pData[j];
				pData[j]=tmp; 
		}
}

char check_crc(u8 *prxdata,u8 len,u8 *crccode)
{
	//len 为8的倍数
	u8 i,tmp;
	u16 crc;
	*crccode=0;	
	tmp=len/8-1;
	tmp=1<<tmp;
	for(i=0;i<len;i+=8)
	{
		*crccode>>=1;		
		crc=pec15_calc(6,prxdata);
		ReserveByteOrder((u8*)&crc,2);
		if(*(u16*)(prxdata+6)!=crc)
		{
			return 1;
		}	
		*crccode=*crccode|tmp;		
		 prxdata+=8;
	}
	return 0;
}

// Reads the raw cell voltage register data
void LTC681x_rdcv_reg(uint8_t reg, //Determines which cell voltage register is read back
                      uint8_t total_ic, //the number of ICs in the
                      uint8_t *data //An array of the unparsed cell codes
                     )
{
  const uint8_t REG_LEN = 8; //number of bytes in each ICs register + 2 bytes for the PEC
  uint8_t cmd[4];
  uint16_t cmd_pec;

  if (reg == 1)     //1: RDCVA
  {
    cmd[1] = 0x04;
    cmd[0] = 0x00;
  }
  else if (reg == 2) //2: RDCVB
  {
    cmd[1] = 0x06;
    cmd[0] = 0x00;
  }
  else if (reg == 3) //3: RDCVC
  {
    cmd[1] = 0x08;
    cmd[0] = 0x00;
  }
  else if (reg == 4) //4: RDCVD
  {
    cmd[1] = 0x0A;
    cmd[0] = 0x00;
  }
  else if (reg == 5) //4: RDCVE
  {
    cmd[1] = 0x09;
    cmd[0] = 0x00;
  }
  else if(reg == 6) //4: RDCVF
  {
    cmd[1] = 0x0B;
    cmd[0] = 0x00;
  }
	else
	{	
			cmd[1] = 0x00;
			cmd[0] = 0x00;
	}

	#if LTC6811_2
		cmd[0] = cmd[0]|LTC6811_ADD;
	#endif
  cmd_pec = pec15_calc(2, cmd);
  cmd[2] = (uint8_t)(cmd_pec >> 8);
  cmd[3] = (uint8_t)(cmd_pec);

  DASILY_RESET();
  delay_us(2);
  spi_write_read(cmd,4,data,(REG_LEN*total_ic));
  delay_us(2);
  DASILY_SET();

}

u8  Cmd_RecvData(u8 *mycmd,u8 *rx_data,u8 len,u8 *pcrccode)
{
		uint8_t cmd[4];
		uint16_t cmd_pec;
	    cmd[0] = mycmd[0];
		#if LTC6811_2
	   		 cmd[0] = cmd[0]|LTC6811_ADD;
		#endif
		cmd[1] =  mycmd[1];
		cmd_pec = pec15_calc(2, mycmd);
		cmd[2] = (uint8_t)(cmd_pec >> 8);
		cmd[3] = (uint8_t)(cmd_pec);
		
		DASILY_RESET();
		delay_us(2);
		spi_write_read(cmd,4,rx_data,8);
		delay_us(2);
		DASILY_SET();
		return check_crc(rx_data,len,pcrccode);	
}

int8_t LTC6804_rdcfg(uint8_t  *rx_Redata)
{
//  const uint8_t BYTES_IN_REG = 8;

  uint8_t cmd[4];
//  uint8_t *rx_data;
  int8_t pec_error = 0;
  uint16_t data_pec;
  uint16_t received_pec;
//  uint8_t current_byte;
  //1
  cmd[0] = LTC6811_ADD;
  cmd[1] = 0x02;
//  cmd[2] = 0x2b;
//  cmd[3] = 0x0A;

  //2
  wakeup_idle (1); //This will guarantee that the LTC6804 isoSPI port is awake. This command can be removed.
  //3
//  for (int current_ic = 0; current_ic<total_ic; current_ic++)
 // {
  //  cmd[0] = 0x80; //Setting address
//    cmd[0] = 0x80 ;
    data_pec = pec15_calc(2, cmd);
    cmd[2] = (uint8_t)(data_pec >> 8);
    cmd[3] = (uint8_t)(data_pec);
	DASILY_RESET();
    spi_write_read(cmd,4,rx_Redata,8);
	DASILY_SET();
 // }

  //for (uint8_t current_ic = 0; current_ic < total_ic; current_ic++) //executes for each LTC6804 in the stack
  //{
    //4.a
 //   for (current_byte = 0; current_byte < BYTES_IN_REG; current_byte++)
  //  {
  //    rx_Redata[current_byte] = rx_data[current_byte];
 //   }
    //4.b
    received_pec = (rx_Redata[6]<<8) + rx_Redata[7];
    data_pec = pec15_calc(6, &rx_Redata[0]);
    if (received_pec != data_pec)
    {
      pec_error = 0;
    }
	else
		{
			pec_error=1;
		}
 // }
  //5
  return(pec_error);
}

int8_t LTC6804_Revision_Get(uint8_t total_ic, uint8_t  *rx_Redata)
{
	uint16_t data_pec;
	uint16_t received_pec;
  	int8_t pec_error = 0;
	uint8_t cmd[4];
	wakeup_idle (1);
	cmd[0] = 0x80 + (total_ic<<3);
  	cmd[1] = LTC6804_COMMAND_CODE_BASE_RDSTATB&0xff;
	data_pec = pec15_calc(2, cmd);
    cmd[2] = (uint8_t)(data_pec >> 8);
    cmd[3] = (uint8_t)(data_pec);
	DASILY_RESET();
    spi_write_read(cmd,4,rx_Redata,8);
	DASILY_SET();
	received_pec = (rx_Redata[6]<<8) + rx_Redata[7];
    data_pec = pec15_calc(6, &rx_Redata[0]);
    if (received_pec != data_pec)
    {
      	pec_error=0;
    }
	else
	{
		pec_error=1;
	}
	return(pec_error);
}

int8_t LTC6804_RDAUXA_Get(uint8_t total_ic, uint8_t  *rx_Redata)
{
	uint16_t data_pec;
	uint16_t received_pec;
  	int8_t pec_error = 0;
	uint8_t cmd[4];
	wakeup_idle (1);
	cmd[0] = 0x80 + (total_ic<<3);
  	cmd[1] = LTC6804_COMMAND_CODE_BASE_RDSTATA&0xff;
	data_pec = pec15_calc(2, cmd);
    cmd[2] = (uint8_t)(data_pec >> 8);
    cmd[3] = (uint8_t)(data_pec);
	DASILY_RESET();
    spi_write_read(cmd,4,rx_Redata,8);
	DASILY_SET();
	received_pec = (rx_Redata[6]<<8) + rx_Redata[7];
    data_pec = pec15_calc(6, &rx_Redata[0]);
    if (received_pec != data_pec)
    {
      	pec_error=0;
    }
	else
	{
		pec_error=1;
	}
	return(pec_error);
	
}
//Starts cell voltage self test conversion
void LTC681x_cvst(
  uint8_t MD, //ADC Mode
  uint8_t ST //Self Test
)
{
  uint8_t cmd[2];
  uint8_t md_bits;

  md_bits = (MD & 0x02) >> 1;
  cmd[0] = md_bits + 0x02;
  md_bits = (MD & 0x01) << 7;
  cmd[1] =  md_bits + ((ST)<<5) +0x07;
  cmd_68(cmd);

}

//Starts cell voltage conversion
void LTC681x_adcv(
  uint8_t MD, //ADC Mode
  uint8_t DCP, //Discharge Permit
  uint8_t CH //Cell Channels to be measured
)
{
  uint8_t cmd[4];
  uint8_t md_bits;
  u16 cmd_pec;
  md_bits = (MD & 0x02) >> 1;
  cmd[0] = md_bits + 0x02;
  md_bits = (MD & 0x01) << 7;
  cmd[1] =  md_bits + 0x60 + (DCP<<4) + CH;
  #if LTC6811_2
	    cmd[0] = cmd[0]|LTC6811_ADD;
  #endif
  cmd_pec = pec15_calc(2, cmd);
  cmd[2] = (uint8_t)(cmd_pec >> 8);
  cmd[3] = (uint8_t)(cmd_pec);
  DASILY_RESET();
  delay_us(10);
  spi_write_array(4,cmd);
  delay_us(10);
  DASILY_SET();	
}
//Start a Status ADC Conversion
void LTC681x_adstat(
  uint8_t MD, //ADC Mode
  uint8_t CHST //GPIO Channels to be measured
)
{
  uint8_t cmd[4];
  uint8_t md_bits;

  md_bits = (MD & 0x02) >> 1;
  cmd[0] = md_bits + 0x04;
  md_bits = (MD & 0x01) << 7;
  cmd[1] = md_bits + 0x68 + CHST ;
  cmd_68(cmd);
}
//This function will block operation until the ADC has finished it's conversion
uint32_t LTC681x_pollAdc(void)
{
 // uint32_t counter = 0;
//  uint8_t finished = 0;
//  uint8_t current_time = 0;
  uint8_t cmd[4];
  uint16_t cmd_pec;


  cmd[0] = 0x07;
  cmd[1] = 0x14;
  cmd_pec = pec15_calc(2, cmd);
  cmd[2] = (uint8_t)(cmd_pec >> 8);
  cmd[3] = (uint8_t)(cmd_pec);

  DASILY_RESET();
  delay_us(10);
  spi_write_array(4,cmd);
  delay_us(10);
  DASILY_SET();

  return(0);
}

void LTC681x_adax(
  uint8_t MD, //ADC Mode
  uint8_t CHG //GPIO Channels to be measured)
)
{
  uint8_t cmd[4];
  uint8_t md_bits;

  md_bits = (MD & 0x02) >> 1;
  cmd[0] = md_bits + 0x04;
  md_bits = (MD & 0x01) << 7;
  cmd[1] = md_bits + 0x60 + CHG ;
  cmd_68(cmd);

}
// Starts cell voltage  and GPIO 1&2 conversion
void LTC681x_adcvax(
  uint8_t MD, //ADC Mode
  uint8_t DCP //Discharge Permit
)
{
  uint8_t cmd[4];
  uint8_t md_bits;
  md_bits = (MD & 0x02) >> 1;
  cmd[0] = md_bits | 0x04;
  md_bits = (MD & 0x01) << 7;
  cmd[1] =  md_bits | ((DCP&0x01)<<4) + 0x6F;
  cmd_68(cmd);
}
void LTC681x_clrcell(void)
{
  uint8_t cmd[2]= {0x07 , 0x11};
  cmd_68(cmd);
}
void LTC681x_clraux(void)
{
  uint8_t cmd[2]= {0x07 , 0x12};
  cmd_68(cmd);
}
void LTC681x_adcvsc(
  uint8_t MD, //ADC Mode
  uint8_t DCP //Discharge Permit
)
{
  uint8_t cmd[4];
  uint8_t md_bits;
  md_bits = (MD & 0x02) >> 1;
  cmd[0] = md_bits | 0x04;
  md_bits = (MD & 0x01) << 7;
  cmd[1] =  md_bits | 0x60 | (DCP<<4) | 0x07;
  cmd_68(cmd);

}


void LTC681x_rdstat_reg(uint8_t reg, //Determines which stat register is read back
                        uint8_t total_ic, //The number of ICs in the system
                        uint8_t *data //Array of the unparsed stat codes
                       )
{
  const uint8_t REG_LEN = 8; // number of bytes in the register + 2 bytes for the PEC
  uint8_t cmd[4];
  uint16_t cmd_pec;


  if (reg == 1)     //Read back statiliary group A
  {
    cmd[1] = 0x10;
    cmd[0] = 0x00;
  }
  else if (reg == 2)  //Read back statiliary group B
  {
    cmd[1] = 0x12;
    cmd[0] = 0x00;
  }

  else          //Read back statiliary group A
  {
    cmd[1] = 0x10;
    cmd[0] = 0x00;
  }
	#if LTC6811_2
	    cmd[0] = cmd[0]|LTC6811_ADD;
		#endif
  cmd_pec = pec15_calc(2, cmd);
  cmd[2] = (uint8_t)(cmd_pec >> 8);
  cmd[3] = (uint8_t)(cmd_pec);

  DASILY_RESET();
  spi_write_read(cmd,4,data,(REG_LEN*total_ic));
  DASILY_SET();

}

void LTC681x_rdaux_reg(uint8_t reg, //Determines which GPIO voltage register is read back
                       uint8_t total_ic, //The number of ICs in the system
                       uint8_t *data //Array of the unparsed auxiliary codes
                      )
{
  const uint8_t REG_LEN = 8; // number of bytes in the register + 2 bytes for the PEC
  uint8_t cmd[4];
  uint16_t cmd_pec;


  if (reg == 1)     //Read back auxiliary group A
  {
    cmd[1] = 0x0C;
    cmd[0] = 0x00;
  }
  else if (reg == 2)  //Read back auxiliary group B
  {
    cmd[1] = 0x0e;
    cmd[0] = 0x00;
  }
  else if (reg == 3)  //Read back auxiliary group C
  {
    cmd[1] = 0x0D;
    cmd[0] = 0x00;
  }
  else if (reg == 4)  //Read back auxiliary group D
  {
    cmd[1] = 0x0F;
    cmd[0] = 0x00;
  }
  else          //Read back auxiliary group A
  {
    cmd[1] = 0x0C;
    cmd[0] = 0x00;
  }
#if LTC6811_2
	    cmd[0] = cmd[0]|LTC6811_ADD;
#endif
  cmd_pec = pec15_calc(2, cmd);
  cmd[2] = (uint8_t)(cmd_pec >> 8);
  cmd[3] = (uint8_t)(cmd_pec);

  DASILY_RESET();
  delay_us(10);
  spi_write_read(cmd,4,data,(REG_LEN*total_ic));
  delay_us(10);
  DASILY_SET();
}
void LTC681x_adow(
	uint8_t MD, //ADC Mode
	uint8_t PUP //Discharge Permit
 )
 {
	uint8_t cmd[2];
	uint8_t md_bits;
	md_bits = (MD & 0x02) >> 1;
	cmd[0] = md_bits + 0x02;
	md_bits = (MD & 0x01) << 7;
	cmd[1] =  md_bits + 0x28 + (PUP<<6) ;//+ CH;
	cmd_68(cmd);
 }


//crccode bit 以位标识出错的在第几个group组

void ltc6811_wakeup_standy(void)
{
  	wakeup_idle(IC_COUNT);
	delay_us(300);//250us保证core 进入standby
}

//void init_ltc6811(void)
//{
////u8 i;
//	u32 v;

//	memset(&sample_stack,0,sizeof(STACK_CELL_SAMPLE)*STACK_SIZE);
//	SPI1_Init();
//	ltc6811_get_total_voltage(&v);
//	ltc6811_get_total_voltage(&v);
//	//init_ic_config();
//	//LTC681x_adstat(MD_26HZ_2KHZ,3);
//	/*
//	ltc6811_wakeup_standy(); 
//	LTC681x_clrcell(); 
//	delay_ms(5);
//	LTC681x_adcv(MD_26HZ_2KHZ,DCP_ENABLED,0);
//	delay_ms(5);
//	LTC681x_pollAdc();
//	delay_ms(300);
//	for(i=1;i<5;i++)
//		{
//			LTC681x_rdcv_reg(i,1,dasiy_rxdata);//读电池电压LTC6811-2
//		}
//		*/
//	
//}         
