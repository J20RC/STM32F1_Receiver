/*
=============J20��ģң�������ն�-������V1.0==============
	оƬSTM32F103C8T6��ʵ����NRF24L01���߽��պ�8��ͨ����PWM���
	NRF24L01ģ�飺
				GND   	��Դ��
				VCC		��3.3V��Դ
				CSN		PB12
				SCK		PB13
				MISO	PB14
				MOSI	PB15
				CE		PA8
				IRQ		PA9
	PWM�����PA0-3��PA6-7��PB0-1
	OLED��ʾ����
				GND   	��Դ��
				VCC   	��3.3V��Դ
				SCL   	��PB8��SCL��
				SDA   	��PB9��SDA��
	����USB-TTL�ӷ���	
				GND   	��Դ��
				3V3   	��3.3V
				TXD   	��PB7
				RXD   	��PB6
	ST-LINK V2�ӷ���
				GND   	��Դ��
				3V3   	��3.3V
				SWCLK 	��DCLK
				SWDIO 	��DIO
	
	by J20�����Ŷ�
*/
#include "main.h"
#include "delay.h"
#include "usart.h"
#include "stm32f10x.h"
#include "rtc.h" 
#include "stdio.h"
#include "string.h"
#include "nrf24l01.h"
#include "led.h"
#include "tim.h"
#include "sbus.h"
#include "pwm.h"
#include "ppm.h"

u16 PWMvalue[SBUS_CHANNEL_NUMBER];// ����PWMռ�ձ�
u16 recPWMvalue[SBUS_CHANNEL_NUMBER];// ����PWMռ�ձ�
u8 sbusPacket[SBUS_PACKET_LENGTH];// 25���ֽڵ�SBUS���ݰ�
u8 signalLoss = 0;  // 1��ʾ��ʧ�ź�
u16 i=0,startIndex=0;
u32 lastTime = 0;
u32 sbusTime = 0;
u8 chPacket[32];

void PWM_reset(void);

int main()
{
//	delay_init();//��ʼ����ʱ����
	PWM_reset();
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //����NVIC�жϷ���2��2λ��ռ���ȼ���2λ�����ȼ�
	usart_init(100000);//��ʼ������1����Ϊsbus�������������Ϊ100000��8������λ��żУ�飬2��ֹͣλ��
	//usart_init(115200);
	TIM3_PWM_Init(19999,71);//Ԥ��Ƶ72��Ƶ��1MHz������1us���Զ�װ��ֵ20 000����PWM����1us*20 000
	TIM2_PWM_Init(19999,71);//PWM���
	TIM4_Counter_Init(9,71); //Ԥ��Ƶ1MHz������1us���Զ�װ��ֵ10������С������λ10us
	PPM_Pin_Init();//PPM���ų�ʼ��
	systick_init(10000);//PPM��ʱ��ʼ������ʼֵ��������
	LED_Init();		//LED��ʼ��
	NRF24L01_Init();//��ʼ��NRF24L01
	while(NRF24L01_Check())
	{
 		delay_ms(200);
	}
	
	NRF24L01_RX_Mode();
	while (1){ 	
		if(NRF24L01_RxPacket(chPacket)==0)
		{
			for(i=0;i<32-chNum*2;i++)//Ѱ����ʼλ�ã�һ���ڵ�2-3��Ԫ��
			{
				if(chPacket[i]==0x00 & chPacket[i+1]==0x00) 
				{
					startIndex = i+2;
					break;
				}
			}
			for(i=0;i<chNum;i++)//����ʼλ�ÿ�ʼ����
			{
				recPWMvalue[i] = ((u16)chPacket[startIndex] << 8) | ((u16)(chPacket[startIndex+1]));// �ϲ�u8Ϊu16
				if(recPWMvalue[i]!=0) PWMvalue[i] = recPWMvalue[i];
				startIndex = startIndex+2;
			}
			for (i=chNum; i<16; i++) 
			{
				PWMvalue[i] = 1500;//δ�õ���ͨ��ȫ������
			}
			//printf("%d,%d,%d,%d\n",PWMvalue[4],PWMvalue[5],PWMvalue[6],PWMvalue[7]);
			LED = 0;
			lastTime = nowTime;
		}
		
		if (nowTime > sbusTime) //���SBUS
		{
			sbusPreparePacket(sbusPacket, PWMvalue, signalLoss, 0); //chNumͨ����ֵת��ΪSBUS���ݰ�
			for(i=0;i<SBUS_PACKET_LENGTH;i++)
			{
				USART_SendData(USART1,sbusPacket[i]);//��SBUS���ݰ�ͨ������TX0���
				while( USART_GetFlagStatus(USART1,USART_FLAG_TC)!= SET);//�ȴ��������
			}
			sbusTime = nowTime + SBUS_UPDATE_RATE*100;//10ms����һ��
		}
//		printf("%d\t%d\n",nowTime,lastTime);
		if(nowTime-lastTime>100*2000)//�����ϴν���ʱ�����2s����˵��ʧȥ�ź�
		{
			LED=!LED;
			signalLoss = 1;//ʧȥ�źű�־
			PWM_reset();//ʧ�ر���
			delay_ms(200);
		}
		//PPM����ת��
		for (i=0;i<chNum;i++) {chTime[i] = PWMvalue[i]*9;}
		TIM_SetCompare1(TIM2,PWMvalue[0]);//�����PWM-PA0
		TIM_SetCompare2(TIM2,PWMvalue[1]);//�����PWM-PA1
		TIM_SetCompare3(TIM2,PWMvalue[2]);//�����PWM-PA2
		TIM_SetCompare4(TIM2,PWMvalue[3]);//�����PWM-PA3
		TIM_SetCompare1(TIM3,PWMvalue[4]);//�����PWM-PA6
		TIM_SetCompare2(TIM3,PWMvalue[5]);//�����PWM-PA7
		TIM_SetCompare3(TIM3,PWMvalue[6]);//�����PWM-PB0
		TIM_SetCompare4(TIM3,PWMvalue[7]);//�����PWM-PB1
	}
}
//��ʼ��PWM�����ʧ�ر���
void PWM_reset(void)
{
	PWMvalue[0] = 1500;//ͨ��1����
	PWMvalue[1] = 1500;//ͨ��2����
	PWMvalue[2] = 1000;//�������
	PWMvalue[3] = 1500;//ͨ��4����
	for (i=4; i<chNum; i++) 
	{
		PWMvalue[i] = 1500;//δ�õ���ͨ��ȫ������
	}
}

