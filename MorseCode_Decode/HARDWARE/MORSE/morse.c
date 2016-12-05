#include "morse.h"

u8 msc_buff[10];
u8 Msc_NumTable[33]={'5','4','0','3','0','0','0','2','0','0','o','0','0','0','0','1','6','0','0','0','0','0','0','0','7','0','0','0','8','0','9','0'};
u8 Msc_EngTable[4][14]={{'E','T','0','0','0','0'},{'I','A','N','M','0','0'},{'S','U','R','W','D','K','G','o'},{'H','V','F','0','L','0','P','J','B','X','C','Y','Z','Q'}};u8 msc_buff[10];


int Morse_Judge(u16 now_counter,u16 *last_counter,u32 *last_val,u32 now_val)
{
	static u8 start_sta=0,msc_dida_num=0,msc_sign_sta=0,msc_sign_times=0;
	u8 da_sta = 0,di_sta =0;
		//滴答判断
		if(now_counter > 40)
		{
			da_sta = 1;
			di_sta = 0;
		}else 
		{
			if((now_counter > ((*last_counter) - 10)) && (now_counter < ((*last_counter) + 10)))		//在上次的范围内
			{
				if(now_counter>20)		//这里是为了特殊情况处理
				{
					da_sta = 1;
					di_sta = 0;
				}else 
				{
					di_sta = 1;
					da_sta = 0;
				}
				//还不能判断是滴还是搭
			}else 
			{
				if(now_counter > (*last_counter)) //搭，如果不在上次范围内，则大的一定是搭
				{
					da_sta = 1;
					di_sta = 0;
					*last_counter = now_counter ;
				}else		//否则为滴
				{
					di_sta = 1;//滴
					da_sta = 0;
					*last_counter = now_counter ;
				}
			}
		}
	//进行滴答信号标记
	if(start_sta == 1)
	{
		if(da_sta == 1)
		{
			msc_sign_sta = (msc_sign_sta<<1);	//搭信号，置1
			msc_sign_sta++;
			msc_dida_num++;
		}else 
		{
			if(di_sta  == 1)
			{
				msc_sign_sta = (msc_sign_sta<<1);//滴信号，置0
				msc_dida_num++;
			}
		}
	}
		//判断间隔
		if((now_val>((*last_val)-200))&&(now_val<((*last_val)+200)))	//在上次范围内
		{
			//不操作
		}else
		{
			if((now_val>((*last_val)*3-200))&&(now_val<((*last_val)*3+200)))//last 是单位，VAL为字符间隔
			{
					if(start_sta == 1)
					{
						if(msc_dida_num == 5)
						{
							msc_buff[msc_sign_times] = Msc_NumTable[msc_sign_sta];
						}else 
						{
							msc_buff[msc_sign_times] = Msc_EngTable[msc_dida_num-1][msc_sign_sta];
						}
						msc_sign_times++;	//信号个数加1
						//一个字符信号结束，将信号标志位和数量位清0
						msc_dida_num=0;
						msc_sign_sta=0;
					}
			}else
			{
				if((now_val>((*last_val)*5-200))&&(now_val<((*last_val)*5+200)))//last 是单位，VAL是句间隔
				{
					if(start_sta == 1)
					{
						if(msc_dida_num == 5)
						{
							msc_buff[msc_sign_times] = Msc_NumTable[msc_sign_sta];	//莫尔斯码为5位，数字
						}else 
						{
							msc_buff[msc_sign_times] = Msc_EngTable[msc_dida_num-1][msc_sign_sta];	//莫尔斯为4位，表示为英文
						}
						//if((msc_buff[msc_sign_times]=='E')||(msc_buff[msc_sign_times]=='I')||(msc_buff[msc_sign_times]=='O')||(msc_buff[msc_sign_times]=='S'))
						{
								switch(msc_buff[msc_sign_times])
								{
									case 'E':msc_buff[msc_sign_times+1]='1';msc_buff[msc_sign_times+2]='\0';break;//1号
									case 'I':msc_buff[msc_sign_times+1]='2';msc_buff[msc_sign_times+2]='\0';break;//2号
									case 'O':msc_buff[msc_sign_times+1]='3';msc_buff[msc_sign_times+2]='\0';break;//3号
									case 'H':msc_buff[msc_sign_times+1]='4';msc_buff[msc_sign_times+2]='\0';break;//4号
									default:msc_buff[msc_sign_times+1]='\0';break;
								}
						}//else
						{
							//msc_buff[msc_sign_times+1]='\0';
							//在最后一个字符后面加上\0
						}
						//这里进行显示
						Morse_Disp();
						//一个完整的信号结束，将所有计数位清0
						msc_sign_times = 0;
						msc_dida_num = 0;
						msc_sign_sta = 0;
						return 1;
					}
					start_sta = 1;
				}else 
				{	
					(*last_val) = now_val;	//赋值，这里确保了会找到单位
				}
			}
		}
		return 0;
}

int Morse_Disp(void)
{
	OLED_Clr();	
	OLED_ShowString(1,32 ,msc_buff);
	return 0;
}
