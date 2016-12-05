/**
 ****************************************************************************
 * �� �� ��	: < oled.c >
 * ��    ��	: κ����
 * �ļ��汾	: v1.0
 * ΢������	: STM32F103RCT6/STM32F103ZET6
 * ����ʱ��	: 2016/3/6
 * ��������	: 0.69��OLED��ʾ������������\����IO��ģ��IIC����
 ****************************************************************************
 */
/* includes ----------------------------------------------------------------*/
#include "oled.h"
#include "ASCII_CODE_8X16_5X8_VERTICAL.h"
#include "Chinese_And_Graphic.h"

/***************************************
 * ��������OLED_Init(��ʼ��OLED)
 * ���ߣ�κ����
 * ��ע��
 *	   OLED_IIC_SCL   <---->   PA3
 *	   OLED_IIC_SDA   <---->   PA4
 ***************************************/
void OLED_Init(void)
{
//	MY_GPIO_Init(GPIOB,14,GPIO_Mode_Out_PP);	//SDA
//	MY_GPIO_Init(GPIOB,15,GPIO_Mode_Out_PP);	//SCL
	RCC->APB2ENR|=(1<<3);//��ʹ������IO PORTBʱ�� 							 
	GPIOB->CRH&=0XFFFF00FF;//PB11/10 �������
	GPIOB->CRH|=0X00003300;	   
	GPIOB->ODR|=3<<10;     //PB11,10 �����
	//����ʱIIC�߾�Ϊ�ߵ�ƽ
	OLED_IIC_SCL=1;
	OLED_IIC_SDA=1;
	
	Initial_OLED();		//�ϵ��ʼ��	
	OLED_Clr();	
}

/*----------iic����ʼ�ź�----------*/
void OLED_IIC_Start(void)
{
     OLED_IIC_SCL=1;
     OLED_IIC_SDA=1;
     OLED_IIC_SDA=0;
     OLED_IIC_SCL=0;
}

/*--------iic�Ľ���ֹͣ�ź�---------*/
void OLED_IIC_Stop()
{
     OLED_IIC_SCL=0;
     OLED_IIC_SDA=0;
     OLED_IIC_SDA=1;
     OLED_IIC_SCL=1;
}

/*------------����һ���ֽ����ݵ�OLED------------*/
void Send1Byte_To_OLED(u8 data)
{	
	u8 bit_cnt;
	for(bit_cnt=0;bit_cnt<8;bit_cnt++)
	{
		OLED_IIC_SCL=0;		
		if(data&0x80)		OLED_IIC_SDA=1;
		else 				OLED_IIC_SDA=0;
		data<<=1;
		OLED_IIC_SCL=1;			//����������
	}
	OLED_IIC_SCL=0;
	delay_us(10);
	OLED_IIC_SCL=1;
}

/*------------�������OLED------------*/
void SendCommand_To_OLED(u8 ins)
{
     OLED_IIC_Start();
     Send1Byte_To_OLED(0x78);
     Send1Byte_To_OLED(0x00);
     Send1Byte_To_OLED(ins);
     OLED_IIC_Stop();
}

/*------------�������ݵ�OLED------------*/
void SendData_To_OLED(u8 data)
{
     OLED_IIC_Start();
     Send1Byte_To_OLED(0x78);
     Send1Byte_To_OLED(0x40);
     Send1Byte_To_OLED(data);
     OLED_IIC_Stop();
}

/*------------OLEDģ����ϵ��ʼ��------------*/
void Initial_OLED(void)
{      
	SendCommand_To_OLED(0xae);	//����ʾ
	SendCommand_To_OLED(0xd5);	//����Ƶ��
	SendCommand_To_OLED(0x80);
	SendCommand_To_OLED(0xa8);	//duty����
	SendCommand_To_OLED(0x0f);	//duty=1/64  //������������ҳ�й�
	SendCommand_To_OLED(0xd3);	//��ʾƫ��
	SendCommand_To_OLED(0x00);	
	SendCommand_To_OLED(0x40);	//��ʼ��
	SendCommand_To_OLED(0x8d);	//��ѹ����
	SendCommand_To_OLED(0x14);	
	SendCommand_To_OLED(0x20);	//page address mode
	SendCommand_To_OLED(0x02);
	SendCommand_To_OLED(0xc8); //��ɨ��˳�򣺴��ϵ���
	SendCommand_To_OLED(0xa1); //��ɨ��˳�򣺴�����
	SendCommand_To_OLED(0xda); //sequential configuration
	SendCommand_To_OLED(0x02);
	SendCommand_To_OLED(0x81); //΢���Աȶ�,��ָ���0x81��Ҫ�Ķ����������ֵ
	SendCommand_To_OLED(0xcf); //΢���Աȶȵ�ֵ�������÷�Χ0x00��0xff
	SendCommand_To_OLED(0xd9); //Set Pre-Charge Period
	SendCommand_To_OLED(0xf1);
	SendCommand_To_OLED(0xdb); //Set VCOMH Deselect Level
	SendCommand_To_OLED(0x40);
	SendCommand_To_OLED(0xaf); //����ʾ
}

/**************************************************
 * ��������OLED_address(����OLED��ָ���ַ)
 * ���ߣ�κ����
 *-------------------------------------------------
 * ����1 page		:�У�����96*16��OLED��ֵΪ1/2
 * ����2 column		:�У�0~96
 **************************************************/
void OLED_address(u8 page,u8 column)
{
	column=column-1;  							//����ƽ����˵�ĵ�1�У���LCD����IC���ǵ�0�С������������ȥ1.
	page=page-1;
	SendCommand_To_OLED(0xb0+page);   			//����ҳ��ַ��ÿҳ��8�С�һ�������64�б��ֳ�8��ҳ������ƽ����˵�ĵ�1ҳ����LCD����IC���ǵ�0ҳ�������������ȥ1
	SendCommand_To_OLED(((column>>4)&0x0f)+0x10);	//�����е�ַ�ĸ�4λ
	SendCommand_To_OLED(column&0x0f);				//�����е�ַ�ĵ�4λ
}

/*------------OLED��������------------*/
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
 * ��������OLED_ShowString_8x16(��ʾ8*16���ַ���)
 * ���ߣ�κ����
 *-------------------------------------------------
 * ����1 page		:�У�����96*16��OLED��ֵΪ1/2
 * ����2 column		:�У�0~96
* ����3 *text		:��ʾ���ַ���
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
					SendData_To_OLED(ascii_table_8x16[j][k+8*n]);	//д���ݵ�LCD,ÿд��1�ֽڵ����ݺ��е�ַ�Զ���1
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
 * ��������OLED_ShowString_5x8(��ʾ5*8���ַ���)
 * ���ߣ�κ����
 *-------------------------------------------------
 * ����1 page		:�У�����96*16��OLED��ֵΪ1/2
 * ����2 column		:�У�0~96
* ����3 *text		:��ʾ���ַ���
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
				SendData_To_OLED(ascii_table_5x8[j][k]);	//д���ݵ�LCD,ÿд��1�ֽڵ����ݺ��е�ַ�Զ���1
			}
			i++;
			column+=5;
		}
		else
		i++;
	}
}

/**************************************************
 * ��������OLED_ShowString_16x16(��ʾ16*16���ַ���)
 * ���ߣ�κ����
 *-------------------------------------------------
 * ����1 page		:�У�����96*16��OLED��ֵΪ1/2
 * ����2 column		:�У�0~96
* ����3 *text		:��ʾ���ַ���
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
        while(Chinese_text_16x16[i] > 0x7e)	  // >0x7f��˵������ASCII���ַ�
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
        
        if(address != 1)// ��ʾ����                   
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
        else              //��ʾ�հ��ַ�            
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
 * ��������OLED_ShowString(��ʾ�ַ���)
 * ���ߣ�κ����
 *-------------------------------------------------
 * ����1 page		:�У�����96*16��OLED��ֵΪ1/2
 * ����2 column		:�У�0~96
 * ����3 *text		:��ʾ���ַ���
 *-------------------------------------------------
 * ��ע���ַ���û�м����8*16��16*16�����ʾ
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
            temp[2] = '\0';          //����Ϊ�����ֽ�
            OLED_ShowString_16x16(page,column,temp);  //��ʾ����
            column += 16;
            i += 2;
        }
        else
        {
            temp[0] = text[i];    
            temp[1] = '\0';          //��ĸռһ���ֽ�
            OLED_ShowString_8x16(page, column, temp);  //��ʾ��ĸ
            column += 8;
            i++;
        }
    }
}
/* END_FILE ----------------------------------------------------------------*/
