/**
 ****************************************************************************
 * 文 件 名	: < oled.h >
 * 作    者	: 魏杰文
 * 文件版本	: v1.0
 * 微控制器	: STM32F103RCT6/STM32F103ZET6
 * 创建时间	: 2016/3/6
 ****************************************************************************
 */
/* Hfile_guard -------------------------------------------------------------*/
#ifndef _OLED_H
#define _OLED_H
/* includes ----------------------------------------------------------------*/
#include "sys.h"
//#include "gpio.h"
#include "delay.h"
/* 宏 ----------------------------------------------------------------------*/
//#define OLED_IIC_SCL    PBout(15) //OLED的SCL
//#define OLED_IIC_SDA    PBout(14) //OLED的SDA	 
//#define OLED_READ_SDA   PBin(14)  //输入SDA 
#define OLED_IIC_SCL    PBout(11) //OLED的SCL
#define OLED_IIC_SDA    PBout(10) //OLED的SDA	 
#define OLED_READ_SDA   PBin(10)  //输入SDA 
/* 函数声明 ----------------------------------------------------------------*/
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
