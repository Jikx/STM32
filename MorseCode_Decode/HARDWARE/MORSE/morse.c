#include "morse.h"

u8 msc_buff[10];
u8 Msc_NumTable[33]={'5','4','0','3','0','0','0','2','0','0','o','0','0','0','0','1','6','0','0','0','0','0','0','0','7','0','0','0','8','0','9','0'};
u8 Msc_EngTable[4][14]={{'E','T','0','0','0','0'},{'I','A','N','M','0','0'},{'S','U','R','W','D','K','G','o'},{'H','V','F','0','L','0','P','J','B','X','C','Y','Z','Q'}};u8 msc_buff[10];


int Morse_Judge(u16 now_counter,u16 *last_counter,u32 *last_val,u32 now_val)
{
	static u8 start_sta=0,msc_dida_num=0,msc_sign_sta=0,msc_sign_times=0;
	u8 da_sta = 0,di_sta =0;
		//�δ��ж�
		if(now_counter > 40)
		{
			da_sta = 1;
			di_sta = 0;
		}else 
		{
			if((now_counter > ((*last_counter) - 10)) && (now_counter < ((*last_counter) + 10)))		//���ϴεķ�Χ��
			{
				if(now_counter>20)		//������Ϊ�������������
				{
					da_sta = 1;
					di_sta = 0;
				}else 
				{
					di_sta = 1;
					da_sta = 0;
				}
				//�������ж��ǵλ��Ǵ�
			}else 
			{
				if(now_counter > (*last_counter)) //���������ϴη�Χ�ڣ�����һ���Ǵ�
				{
					da_sta = 1;
					di_sta = 0;
					*last_counter = now_counter ;
				}else		//����Ϊ��
				{
					di_sta = 1;//��
					da_sta = 0;
					*last_counter = now_counter ;
				}
			}
		}
	//���еδ��źű��
	if(start_sta == 1)
	{
		if(da_sta == 1)
		{
			msc_sign_sta = (msc_sign_sta<<1);	//���źţ���1
			msc_sign_sta++;
			msc_dida_num++;
		}else 
		{
			if(di_sta  == 1)
			{
				msc_sign_sta = (msc_sign_sta<<1);//���źţ���0
				msc_dida_num++;
			}
		}
	}
		//�жϼ��
		if((now_val>((*last_val)-200))&&(now_val<((*last_val)+200)))	//���ϴη�Χ��
		{
			//������
		}else
		{
			if((now_val>((*last_val)*3-200))&&(now_val<((*last_val)*3+200)))//last �ǵ�λ��VALΪ�ַ����
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
						msc_sign_times++;	//�źŸ�����1
						//һ���ַ��źŽ��������źű�־λ������λ��0
						msc_dida_num=0;
						msc_sign_sta=0;
					}
			}else
			{
				if((now_val>((*last_val)*5-200))&&(now_val<((*last_val)*5+200)))//last �ǵ�λ��VAL�Ǿ���
				{
					if(start_sta == 1)
					{
						if(msc_dida_num == 5)
						{
							msc_buff[msc_sign_times] = Msc_NumTable[msc_sign_sta];	//Ī��˹��Ϊ5λ������
						}else 
						{
							msc_buff[msc_sign_times] = Msc_EngTable[msc_dida_num-1][msc_sign_sta];	//Ī��˹Ϊ4λ����ʾΪӢ��
						}
						//if((msc_buff[msc_sign_times]=='E')||(msc_buff[msc_sign_times]=='I')||(msc_buff[msc_sign_times]=='O')||(msc_buff[msc_sign_times]=='S'))
						{
								switch(msc_buff[msc_sign_times])
								{
									case 'E':msc_buff[msc_sign_times+1]='1';msc_buff[msc_sign_times+2]='\0';break;//1��
									case 'I':msc_buff[msc_sign_times+1]='2';msc_buff[msc_sign_times+2]='\0';break;//2��
									case 'O':msc_buff[msc_sign_times+1]='3';msc_buff[msc_sign_times+2]='\0';break;//3��
									case 'H':msc_buff[msc_sign_times+1]='4';msc_buff[msc_sign_times+2]='\0';break;//4��
									default:msc_buff[msc_sign_times+1]='\0';break;
								}
						}//else
						{
							//msc_buff[msc_sign_times+1]='\0';
							//�����һ���ַ��������\0
						}
						//���������ʾ
						Morse_Disp();
						//һ���������źŽ����������м���λ��0
						msc_sign_times = 0;
						msc_dida_num = 0;
						msc_sign_sta = 0;
						return 1;
					}
					start_sta = 1;
				}else 
				{	
					(*last_val) = now_val;	//��ֵ������ȷ���˻��ҵ���λ
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
