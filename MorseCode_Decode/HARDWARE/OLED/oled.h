/**
 ****************************************************************************
 * �� �� ��	: < oled.h >
 * ��    ��	: κ����
 * �ļ��汾	: v1.0
 * ΢������	: STM32F103RCT6/STM32F103ZET6
 * ����ʱ��	: 2016/3/6
 ****************************************************************************
 */
/* Hfile_guard -------------------------------------------------------------*/
#ifndef _OLED_H
#define _OLED_H
/* includes ----------------------------------------------------------------*/
#include "sys.h"
//#include "gpio.h"
#include "delay.h"
/* �� ----------------------------------------------------------------------*/
//#define OLED_IIC_SCL    PBout(15) //OLED��SCL
//#define OLED_IIC_SDA    PBout(14) //OLED��SDA	 
//#define OLED_READ_SDA   PBin(14)  //����SDA 
#define OLED_IIC_SCL    PBout(11) //OLED��SCL
#define OLED_IIC_SDA    PBout(10) //OLED��SDA	 
#define OLED_READ_SDA   PBin(10)  //����SDA 
/* �������� ----------------------------------------------------------------*/
void OLED_ON(void);
void OLED_OFF(void);
void OLED_Init(void);
void Initial_OLED(void);
void OLED_Clr(void);
void Full_OLED(u8 data1,u8 data2);
void OLED_address(u8 page,u8 column);
void SendCommand_To_OLED(u8 ins);
void SendData_To_OLED(u8 data);

void OLED_ShowString_16x32(u8 page,u8 column,u8 *text);
void OLED_ShowString_8x16(u8 page,u8 column,u8 *text);
void OLED_ShowString_5x8(u8 page,u8 column,u8 *text);
void OLED_ShowString_16x16(u8 page,u8 column,u8 *text);
void OLED_ShowString(u8 page,u8 column,u8 *text);
void display_32x32(u8 page,u8 column,u8 *dp);
#endif
/* END_FILE ----------------------------------------------------------------*/
