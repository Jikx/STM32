/**
 ****************************************************************************
 * 文 件 名	: < oled.c >
 * 作    者	: 魏杰文
 * 文件版本	: v1.0
 * 微控制器	: STM32F103RCT6/STM32F103ZET6
 * 创建时间	: 2016/3/6
 * 内容描述	: 0.69寸OLED显示屏的驱动程序\采用IO口模拟IIC驱动
 ****************************************************************************
 */
/* includes ----------------------------------------------------------------*/
#include "oled.h"
#include "ASCII_CODE_8X16_5X8_VERTICAL.h"
#include "Chinese_And_Graphic.h"

/***************************************
 * 函数名：OLED_Init(初始化OLED)
 * 作者：魏杰文
 * 备注：
 *	   OLED_IIC_SCL   <---->   PA3
 *	   OLED_IIC_SDA   <---->   PA4
 ***************************************/
void OLED_Init(void)
{
//	MY_GPIO_Init(GPIOB,14,GPIO_Mode_Out_PP);	//SDA
//	MY_GPIO_Init(GPIOB,15,GPIO_Mode_Out_PP);	//SCL
	RCC->APB2ENR|=(1<<3);//先使能外设IO PORTB时钟 							 
	GPIOB->CRH&=0XFFFF00FF;//PB11/10 推挽输出
	GPIOB->CRH|=0X00003300;	   
	GPIOB->ODR|=3<<10;     //PB11,10 输出高
	//空闲时IIC线均为高电平
	OLED_IIC_SCL=1;
	OLED_IIC_SDA=1;
	
	Initial_OLED();		//上电初始化	
	OLED_Clr();	
}

/*----------iic的起始信号----------*/
void OLED_IIC_Start(void)
{
     OLED_IIC_SCL=1;
     OLED_IIC_SDA=1;
     OLED_IIC_SDA=0;
     OLED_IIC_SCL=0;
}

/*--------iic的结束停止信号---------*/
void OLED_IIC_Stop()
{
     OLED_IIC_SCL=0;
     OLED_IIC_SDA=0;
     OLED_IIC_SDA=1;
     OLED_IIC_SCL=1;
}

/*------------发送一个字节数据到OLED------------*/
void Send1Byte_To_OLED(u8 data)
{	
	u8 bit_cnt;
	for(bit_cnt=0;bit_cnt<8;bit_cnt++)
	{
		OLED_IIC_SCL=0;		
		if(data&0x80)		OLED_IIC_SDA=1;
		else 				OLED_IIC_SDA=0;
		data<<=1;
		OLED_IIC_SCL=1;			//产生上升沿
	}
	OLED_IIC_SCL=0;
	delay_us(10);
	OLED_IIC_SCL=1;
}

/*------------发送命令到OLED------------*/
void SendCommand_To_OLED(u8 ins)
{
     OLED_IIC_Start();
     Send1Byte_To_OLED(0x78);
     Send1Byte_To_OLED(0x00);
     Send1Byte_To_OLED(ins);
     OLED_IIC_Stop();
}

/*------------发送数据到OLED------------*/
void SendData_To_OLED(u8 data)
{
     OLED_IIC_Start();
     Send1Byte_To_OLED(0x78);
     Send1Byte_To_OLED(0x40);
     Send1Byte_To_OLED(data);
     OLED_IIC_Stop();
}

/*------------OLED模块的上电初始化------------*/
void Initial_OLED(void)
{      
	SendCommand_To_OLED(0xae);	//关显示
	SendCommand_To_OLED(0xd5);	//晶振频率
	SendCommand_To_OLED(0x80);
	SendCommand_To_OLED(0xa8);	//duty设置
	SendCommand_To_OLED(0x0f);	//duty=1/64  //复用率设置与页有关
	SendCommand_To_OLED(0xd3);	//显示偏移
	SendCommand_To_OLED(0x00);	
	SendCommand_To_OLED(0x40);	//起始行
	SendCommand_To_OLED(0x8d);	//升压允许
	SendCommand_To_OLED(0x14);	
	SendCommand_To_OLED(0x20);	//page address mode
	SendCommand_To_OLED(0x02);
	SendCommand_To_OLED(0xc8); //行扫描顺序：从上到下
	SendCommand_To_OLED(0xa1); //列扫描顺序：从左到右
	SendCommand_To_OLED(0xda); //sequential configuration
	SendCommand_To_OLED(0x02);
	SendCommand_To_OLED(0x81); //微调对比度,本指令的0x81不要改动，改下面的值
	SendCommand_To_OLED(0xcf); //微调对比度的值，可设置范围0x00～0xff
	SendCommand_To_OLED(0xd9); //Set Pre-Charge Period
	SendCommand_To_OLED(0xf1);
	SendCommand_To_OLED(0xdb); //Set VCOMH Deselect Level
	SendCommand_To_OLED(0x40);
	SendCommand_To_OLED(0xaf); //开显示
}

/**************************************************
 * 函数名：OLED_address(设置OLED的指针地址)
 * 作者：魏杰文
 *-------------------------------------------------
 * 参数1 page		:行，对于96*16的OLED，值为1/2
 * 参数2 column		:列，0~96
 **************************************************/
void OLED_address(u8 page,u8 column)
{
	column=column-1;  							//我们平常所说的第1列，在LCD驱动IC里是第0列。所以在这里减去1.
	page=page-1;
	SendCommand_To_OLED(0xb0+page);   			//设置页地址。每页是8行。一个画面的64行被分成8个页。我们平常所说的第1页，在LCD驱动IC里是第0页，所以在这里减去1
	SendCommand_To_OLED(((column>>4)&0x0f)+0x10);	//设置列地址的高4位
	SendCommand_To_OLED(column&0x0f);				//设置列地址的低4位
}

/*------------OLED清屏函数------------*/
void OLED_Clr(void)
{
	u8 i,j;
 	for(j=0;j<8;j++)
	{
		OLED_address(1+j,1);
		for(i=0;i<96;i++)
		{
			SendData_To_OLED(0x00);
		}
	}
}

/**************************************************
 * 函数名：OLED_ShowString_8x16(显示8*16的字符串)
 * 作者：魏杰文
 *-------------------------------------------------
 * 参数1 page		:行，对于96*16的OLED，值为1/2
 * 参数2 column		:列，0~96
* 参数3 *text		:显示的字符串
 **************************************************/
void OLED_ShowString_8x16(u8 page,u8 column,u8 *text)
{
	u8 i=0,j,k,n;
	while(text[i]>0x00)
	{	
		if((text[i]>=0x20)&&(text[i]<=0x7e))
		{
			j=text[i]-0x20;
			for(n=0;n<2;n++)
			{			
				OLED_address(page+n,column);
				for(k=0;k<8;k++)
				{					
					SendData_To_OLED(ascii_table_8x16[j][k+8*n]);	//写数据到LCD,每写完1字节的数据后列地址自动加1
				}
			}
			i++;
			column+=8;
		}
		else
		i++;
	}
}

/**************************************************
 * 函数名：OLED_ShowString_5x8(显示5*8的字符串)
 * 作者：魏杰文
 *-------------------------------------------------
 * 参数1 page		:行，对于96*16的OLED，值为1/2
 * 参数2 column		:列，0~96
* 参数3 *text		:显示的字符串
 **************************************************/
void OLED_ShowString_5x8(u8 page,u8 column,u8 *text)
{
	u8 i=0,j,k;
	while(text[i]>0x00)
	{	
		if((text[i]>=0x20)&&(text[i]<=0x7e))
		{
			j=text[i]-0x20;		
			OLED_address(page,column);
			for(k=0;k<5;k++)
			{					
				SendData_To_OLED(ascii_table_5x8[j][k]);	//写数据到LCD,每写完1字节的数据后列地址自动加1
			}
			i++;
			column+=5;
		}
		else
		i++;
	}
}

/**************************************************
 * 函数名：OLED_ShowString_16x16(显示16*16的字符串)
 * 作者：魏杰文
 *-------------------------------------------------
 * 参数1 page		:行，对于96*16的OLED，值为1/2
 * 参数2 column		:列，0~96
* 参数3 *text		:显示的字符串
 **************************************************/
void OLED_ShowString_16x16(u8 page,u8 column,u8 *text)
{
    u8 i,j,k;
    u8 address; 
    
    j = 0;
    while(text[j] != '\0')
    {
        i = 0;
        address = 1;
        while(Chinese_text_16x16[i] > 0x7e)	  // >0x7f即说明不是ASCII码字符
        {
            if(Chinese_text_16x16[i] == text[j])
            {
                if(Chinese_text_16x16[i + 1] == text[j + 1])
                {
                    address = i * 16;
                    break;
                }
            }
            i += 2;            
        }
        
        if(column > 113)
        {
            column = 0;
            page += 2;
        }
        
        if(address != 1)// 显示汉字                   
        {
            
			for(k=0;k<2;k++)
			{
				OLED_address(page+k,column);
	            for(i = 0; i < 16; i++)               
	            {
	                SendData_To_OLED(Chinese_16x16[address]);   
	                address++;
	            }
   			}
            j += 2;        
        }
        else              //显示空白字符            
        {
			for(k=0;k<2;k++)
			{
				OLED_address(page+k,column);
	            for(i = 0; i < 16; i++)               
	            {
	                SendData_To_OLED(0x00);   
	            }
   			}
            
            j++;
        }
        
        column+=16;
    }
}

/**************************************************
 * 函数名：OLED_ShowString(显示字符串)
 * 作者：魏杰文
 *-------------------------------------------------
 * 参数1 page		:行，对于96*16的OLED，值为1/2
 * 参数2 column		:列，0~96
 * 参数3 *text		:显示的字符串
 *-------------------------------------------------
 * 备注：字符间没有间隔，8*16和16*16混合显示
 **************************************************/
void OLED_ShowString(u8 page,u8 column,u8 *text)
{
    u8 temp[3];
    u8 i = 0;    
    
    while(text[i] != '\0')
    {
        if(text[i] > 0x7e)
        {
            temp[0] = text[i];
            temp[1] = text[i + 1];
            temp[2] = '\0';          //汉字为两个字节
            OLED_ShowString_16x16(page,column,temp);  //显示汉字
            column += 16;
            i += 2;
        }
        else
        {
            temp[0] = text[i];    
            temp[1] = '\0';          //字母占一个字节
            OLED_ShowString_8x16(page, column, temp);  //显示字母
            column += 8;
            i++;
        }
    }
}
/* END_FILE ----------------------------------------------------------------*/
