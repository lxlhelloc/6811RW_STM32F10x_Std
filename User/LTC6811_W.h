#ifndef _LTC6811_W_H
#define _LTC6811_W_H

#include "stm32f10x.h"
#include "Spi.h"
#include "systick.h"

#include "string.h"

#define DASILY_RESET()  GPIO_ResetBits(GPIOA, GPIO_Pin_4) 
#define DASILY_SET()    GPIO_SetBits(GPIOA, GPIO_Pin_4)


#define IC_COUNT 1
#define MD_422HZ_1KHZ 0
#define MD_27KHZ_14KHZ 1
#define MD_7KHZ_3KHZ 2
#define MD_26HZ_2KHZ 3

#define ADC_OPT_ENABLED 1
#define ADC_OPT_DISABLED 0

#define CELL_CH_ALL 0
#define CELL_CH_1and7 1
#define CELL_CH_2and8 2
#define CELL_CH_3and9 3
#define CELL_CH_4and10 4
#define CELL_CH_5and11 5
#define CELL_CH_6and12 6

#define SELFTEST_1 1
#define SELFTEST_2 2

#define AUX_CH_ALL 0
#define AUX_CH_GPIO1 1
#define AUX_CH_GPIO2 2
#define AUX_CH_GPIO3 3
#define AUX_CH_GPIO4 4
#define AUX_CH_GPIO5 5
#define AUX_CH_VREF2 6

#define STAT_CH_ALL 0
#define STAT_CH_SOC 1
#define STAT_CH_ITEMP 2
#define STAT_CH_VREGA 3
#define STAT_CH_VREGD 4

#define DCP_DISABLED 0
#define DCP_ENABLED 1

#define PULL_UP_CURRENT 1
#define PULL_DOWN_CURRENT 0
#define LTC6811_2 1


#define NUM_RX_BYT 8
#define LTC681x_CELL 1
#define LTC681x_AUX 2
#define LTC681x_STAT 3
#define LTC681x_CFGR 0
#define LTC681x_CFGRB 4
#define LTC681x_CS_PIN 10
#define LTC6804_COMMAND_CODE_BASE_RDSTATB   0x012 // Read Status Register Group B
#define LTC6804_COMMAND_CODE_BASE_RDSTATA   0x010 // Read Status Register Group A

u8 Cmd_RecvData(u8 *mycmd,u8 *rx_data,u8 len,u8 *pcrccode);
void ltc6811_wakeup_standy(void);
void ltc6811_delay_ms(u16 ms);//保证isospi不sleep,同时保证core不发生5.5ms  sleep
void LTC681x_adcvax(
  uint8_t MD, //ADC Mode
  uint8_t DCP //Discharge Permit
);
void LTC681x_cvst(
  uint8_t MD, //ADC Mode
  uint8_t ST //Self Test
);
void LTC681x_adow(
   uint8_t MD, //ADC Mode
   uint8_t PUP //Discharge Permit
);
uint32_t LTC681x_pollAdc(void);
void LTC681x_rdaux_reg(uint8_t reg, //Determines which GPIO voltage register is read back
                       uint8_t total_ic, //The number of ICs in the system
                       uint8_t *data //Array of the unparsed auxiliary codes
                      );
void LTC681x_rdcv_reg(uint8_t reg, //Determines which cell voltage register is read back
                      uint8_t total_ic, //the number of ICs in the
                      uint8_t *data //An array of the unparsed cell codes
                     );
//Starts cell voltage conversion
void LTC681x_adcv(
  uint8_t MD, //ADC Mode
  uint8_t DCP, //Discharge Permit
  uint8_t CH //Cell Channels to be measured
);
char check_crc(u8 *prxdata,u8 len,u8 *crccode);
void LTC681x_clrcell(void);
void LTC681x_adstat(
  uint8_t MD, //ADC Mode
  uint8_t CHST //GPIO Channels to be measured
);
void LTC681x_rdstat_reg(uint8_t reg, //Determines which stat register is read back
                        uint8_t total_ic, //The number of ICs in the system
                        uint8_t *data //Array of the unparsed stat codes
                       );

void init_ltc6811(void);
void spi_write_array(uint8_t len, 
                     uint8_t data[]);

void LTC681x_adax(
  uint8_t MD, //ADC Mode
  uint8_t CHG //GPIO Channels to be measured)
);
uint16_t pec15_calc(uint8_t len, //Number of bytes that will be used to calculate a PEC
                    uint8_t *data //Array of data that will be used to calculate  a PEC
                   );
extern void wakeup_idle(uint8_t total_ic);
extern void LTC681x_clraux(void);
extern int8_t LTC6804_rdcfg(uint8_t  *rx_Redata);
extern int8_t LTC6804_Revision_Get(uint8_t total_ic, uint8_t  *rx_Redata);
int8_t LTC6804_RDAUXA_Get(uint8_t total_ic, uint8_t  *rx_Redata);

#endif

