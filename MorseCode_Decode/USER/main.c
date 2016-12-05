#include "sys.h"
#include "delay.h"
#include "OLED.h"
#include "usart.h"
#include "timer.h"
#include "morse.h"
extern u8  TIM2CH1_CAPTURE_STA;		//���벶��״̬		    				
extern u16	TIM2CH1_CAPTURE_VAL;	//���벶��ֵ
extern u8 msc_buff[10];
extern u8 Msc_NumTable[33];//Ī��˹�����
extern u8 Msc_EngTable[4][14];
int main(void)
{
	u8 p[16]="Decoding....";
	u32 buf = 0,last_val=0;
	u16 counter = 0,now_sta,last_counter;
	Stm32_Clock_Init(9); 	//ϵͳʱ������
	delay_init(72);	     	//��ʱ��ʼ��
	uart_init(72,9600);	 			//���ڳ�ʼ��Ϊ9600
	OLED_Init();			//OLD��ʼ��
	TIM2_Cap_Init(0xffff,7200-1);//���벶׽��ʼ��
  OLED_ShowString(1,2,p);
	while(1)	
	{	
		if(TIM2CH1_CAPTURE_STA&0X80)//�ɹ�������һ�θߵ�ƽ
		{
			if(buf == 0)	//��һ������
			{
				if(TIM2CH1_CAPTURE_VAL>1000)	//��һ������ֵ����1000�Ͷ���,��Ч����
				{
					counter = 0;
					TIM2CH1_CAPTURE_VAL = 0;
					TIM2CH1_CAPTURE_STA=0;			//������һ�β���
				}else			//VALֵ��������Χ��
				{
					buf = buf + TIM2CH1_CAPTURE_VAL; //������
					counter++;							//����ֵ��1
					TIM2CH1_CAPTURE_VAL = 0;
					TIM2CH1_CAPTURE_STA=0;			//������һ�β���
				}				
			}else		//�Ѿ����ǵ�һ����ֵ��
			{
				if(TIM2CH1_CAPTURE_VAL > (buf/counter+100))//��⵽���+100�������
				{
					if(now_sta < 2)		//��һ�ν���.���ｫ����ʱ�ĵ�һ�����ݶ���
					{
						now_sta++;
						last_counter = counter ;
						last_val = TIM2CH1_CAPTURE_VAL;
					}else
					{
						Morse_Judge(counter,&last_counter,&last_val,TIM2CH1_CAPTURE_VAL);		//��˹�����ж�
					}
					printf(" ------------- %d \r\n",TIM2CH1_CAPTURE_VAL);
					buf = 0;
					counter = 0;
					TIM2CH1_CAPTURE_VAL = 0;
					TIM2CH1_CAPTURE_STA=0;			//������һ�β���
				}else
				{
					buf = buf + TIM2CH1_CAPTURE_VAL; //������
					counter++;
					printf("test11:%d \r\n",TIM2CH1_CAPTURE_VAL);	//��ӡ�ܵĸߵ�ƽʱ��
					TIM2CH1_CAPTURE_VAL = 0;
					TIM2CH1_CAPTURE_STA=0;			//������һ�β���
				}
			}
 		}
	}
}



