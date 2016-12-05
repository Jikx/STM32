#include "timer.h"
//定时器2输入捕获
void TIM2_Cap_Init(u16 arr,u16 psc)
{
	RCC->APB1ENR|=1<<0;   	//TIM2 时钟使能 
	RCC->APB2ENR|=1<<2;    	//使能PORTA时钟  
	 
	GPIOA->CRL&=0XFFFFFF0F;	//PA1 清除之前设置  
	GPIOA->CRL|=0X00000080;	//PA1 输入   
	GPIOA->ODR|=0<<1;		//PA1 下拉
	  
 	TIM2->ARR=arr;  		//设定计数器自动重装值   
	TIM2->PSC=psc;  		//预分频器 

	TIM2->CCMR1|=1<<8;		//CC2S=01 	选择输入端 IC2映射到TI2上
 //	TIM2->CCMR1|=1<<4; 		//IC1F=0001 配置输入滤波器 以Fck_int采样，2个事件后有效
 	TIM2->CCMR1|=0<<10; 	//IC2PS=00 	配置输入分频,不分频 

	TIM2->CCER|=0<<1; 		//CC1P=0	上升沿捕获
	TIM2->CCER|=1<<4; 		//CC2E=1 	允许捕获计数器的值到捕获寄存器中

	TIM2->DIER|=1<<2;   	//允许捕获中断				
	//TIM2->DIER|=1<<0;   	//允许更新中断	
	TIM2->CR1|=0x01;    	//使能定时器2
	MY_NVIC_Init(2,0,TIM2_IRQn,2);//抢占2，子优先级0，组2	  
}

//捕获状态
//[7]:0,没有成功的捕获;1,成功捕获到一次.
//[6]:0,还没捕获到高电平;1,已经捕获到高电平了.
//[5:0]:捕获高电平后溢出的次数
u8  TIM2CH1_CAPTURE_STA=0;	//输入捕获状态		    				
u32	TIM2CH1_CAPTURE_VAL;	//输入捕获值
//定时器2中断服务程序	 
void TIM2_IRQHandler(void)
{ 		    
	u16 tsr;
	tsr=TIM2->SR;
// 	if((TIM2CH1_CAPTURE_STA&0X80)==0)//还未成功捕获	
//	{
//		if(tsr&0X01)//溢出
//		{	    
//			if(TIM2CH1_CAPTURE_STA&0X40)//已经捕获到高电平了
//			{
//				if((TIM2CH1_CAPTURE_STA&0X3F)==0X3F)//高电平太长了
//				{
//					TIM2CH1_CAPTURE_STA|=0X80;//标记成功捕获了一次
//					TIM2CH1_CAPTURE_VAL=0XFFFF;
//				}else TIM2CH1_CAPTURE_STA++;
//			}	 
//		}
		if(tsr&0x04)//捕获2发生捕获事件
		{	
			//if(TIM2CH1_CAPTURE_STA&0X40)		//捕获到一个下降沿 		
			//{	  			
				TIM2CH1_CAPTURE_STA|=0X80;		//标记成功捕获到一次高电平脉宽
			  TIM2CH1_CAPTURE_VAL=TIM2->CCR2;	//获取当前的捕获值.
	 		//	TIM2->CCER&=~(1<<1);			//CC1P=0 设置为上升沿捕获
			//}
			//else  								//还未开始,第一次捕获上升沿
			{ 
				//TIM2CH1_CAPTURE_VAL=0;
			//	TIM2CH1_CAPTURE_STA=0X40;		//标记捕获到了上升沿
				TIM2->CNT=0;					//计数器清空
			//	TIM2->CCER|=1<<1; 				//CC1P=1 设置为下降沿捕获 
			}		    
	  }			     	    					   
 //	}
	TIM2->SR=0;//清除中断标志位 	    
}
