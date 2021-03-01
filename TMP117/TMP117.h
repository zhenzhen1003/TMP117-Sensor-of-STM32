#ifndef __TMP117_H
#define __TMP117_H
#include "sys.h"

#define ACK	 0
#define	NACK 1  //��Ӧ���񶨵�Ӧ��
#define SA				0x48  //�ӻ���ַ

									/*
									0x48    //	GND
									0x49    //	VCC
									0x4A    //	SDA
									0x4B    //	SCL
									*/

/*
Pointer Registers
*/
#define     TMP117_TemperatureRegister     0x00
#define     TMP117_ConfigurationRegister   0x01
#define     TMP117_TemperatureHighLimit    0x02
#define     TMP117_TemperatureLowLimit     0x03

#define     TMP117_EEPROM_Uclock           0x04
#define     TMP117_EEPROM1                 0x05
#define     TMP117_EEPROM2                 0x06
#define     TMP117_EEPROM3                 0x08

#define     TMP117_Temperature_Offset      0x07
#define     TMP117_ID_Register             0x0F

#define SMBUS_PORT	    GPIOB
#define SMBUS_SCK				GPIO_Pin_6
#define SMBUS_SDA				GPIO_Pin_7
#define SMBUS_RCC				RCC_APB2Periph_GPIOB


#define SMBUS_SCK_H()	    SMBUS_PORT->BSRR = SMBUS_SCK
#define SMBUS_SCK_L()	    SMBUS_PORT->BRR = SMBUS_SCK
#define SMBUS_SDA_H()	    SMBUS_PORT->BSRR = SMBUS_SDA
#define SMBUS_SDA_L()	    SMBUS_PORT->BRR = SMBUS_SDA

#define SMBUS_SDA_PIN()	    SMBUS_PORT->IDR & SMBUS_SDA //��ȡ���ŵ�ƽ

/*SMBus��д����Э�����*/
void SMBus_StartBit(void);
void SMBus_StopBit(void);
void SMBus_SendBit(u8);
u8 SMBus_SendByte(u8);
u8 SMBus_ReceiveBit(void);
u8 SMBus_ReceiveByte(u8);
void SMBus_Delay(u16);
void SMBus_Init(void);
u16 SMBus_ReadMemory(u8, u8);
void SMBus_WriteMemory(u8, u8, u8, u8);
u8 PEC_Calculation(u8*);

/*TMP117�Ĵ�����д���ܺ���*/
float TMP117_get_Temperature(void);    //��ȡ�¶�ֵ
u16 TMP117_get_Configuration(void);         //��ȡ������ֵ
void TMP117_Initialization_DEFAULT(void);    //��ʼ�����趨���������õ�����
void TMP117_set_Temperature_Offset(u8, u8);  //�趨�¶�ƫ������ֵ
u16 TMP117_get_Temperature_Offset(void);     //��ȡ�����õ��¶�ƫ������ֵ
void TMP117_set_EEPROM_Unlock(u8, u8);        
void TMP117_set_LowLimit(u8, u8);
u16 TMP117_get_LowLimit(void);
void TMP117_set_HighLimit(u8, u8);
u16 TMP117_get_HighLimit(void);
void TMP117_set_Configuration(u8, u8);
u16 TMP117_get_Configuration(void);
u16 TMP117_get_ID_Register(void);
void TMP117_set_EEPROM1(u8, u8);
u16 TMP117_get_EEPROM1(void);
void TMP117_set_EEPROM2(u8, u8);
u16 TMP117_get_EEPROM2(void);
void TMP117_set_EEPROM3(u8, u8);
u16 TMP117_get_EEPROM3(void);
#endif
