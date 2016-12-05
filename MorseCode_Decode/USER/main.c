#include "sys.h"
#include "delay.h"
#include "OLED.h"
#include "usart.h"
#include "timer.h"
#include "morse.h"
extern u8  TIM2CH1_CAPTURE_STA;		//输入捕获状态		    				
extern u16	TIM2CH1_CAPTURE_VAL;	//输入捕获值
extern u8 msc_buff[10];
extern u8 Msc_NumTable[33];//莫尔斯译码表
extern u8 Msc_EngTable[4][14];
int main(void)
{
	u8 p[16]="Decoding....";
	u32 buf = 0,last_val=0;
	u16 counter = 0,now_sta,last_counter;
	Stm32_Clock_Init(9); 	//系统时钟设置
	delay_init(72);	     	//延时初始化
	uart_init(72,9600);	 			//串口初始化为9600
	OLED_Init();			//OLD初始化
	TIM2_Cap_Init(0xffff,7200-1);//输入捕捉初始化
  OLED_ShowString(1,2,p);
	while(1)	
	{	
		if(TIM2CH1_CAPTURE_STA&0X80)//成功捕获到了一次高电平
		{
			if(buf == 0)	//第一个数据
			{
				if(TIM2CH1_CAPTURE_VAL>1000)	//第一个计数值大于1000就丢弃,无效数据
				{
					counter = 0;
					TIM2CH1_CAPTURE_VAL = 0;
					TIM2CH1_CAPTURE_STA=0;			//开启下一次捕获
				}else			//VAL值在正常范围内
				{
					buf = buf + TIM2CH1_CAPTURE_VAL; //存起来
					counter++;							//计数值加1
					TIM2CH1_CAPTURE_VAL = 0;
					TIM2CH1_CAPTURE_STA=0;			//开启下一次捕获
				}				
			}else		//已经不是第一个数值了
			{
				if(TIM2CH1_CAPTURE_VAL > (buf/counter+100))//检测到间隔+100避免干扰
				{
					if(now_sta < 2)		//第一次进来.这里将启动时的第一次数据丢弃
					{
						now_sta++;
						last_counter = counter ;
						last_val = TIM2CH1_CAPTURE_VAL;
					}else
					{
						Morse_Judge(counter,&last_counter,&last_val,TIM2CH1_CAPTURE_VAL);		//尔斯电码判断
					}
					printf(" ------------- %d \r\n",TIM2CH1_CAPTURE_VAL);
					buf = 0;
					counter = 0;
					TIM2CH1_CAPTURE_VAL = 0;
					TIM2CH1_CAPTURE_STA=0;			//开启下一次捕获
				}else
				{
					buf = buf + TIM2CH1_CAPTURE_VAL; //存起来
					counter++;
					printf("test11:%d \r\n",TIM2CH1_CAPTURE_VAL);	//打印总的高点平时间
					TIM2CH1_CAPTURE_VAL = 0;
					TIM2CH1_CAPTURE_STA=0;			//开启下一次捕获
				}
			}
 		}
	}
}



