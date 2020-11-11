#ifndef __SBUS_H
#define __SBUS_H
#include "stm32f10x.h" //�ǵ���Ӵ�ͷ�ļ�����Ϊconfig.c�õ�GPIO��غ�����
#include "sys.h"
#define RC_CHANNEL_MIN 1000 	//ÿ��ͨ����PWM��Сֵ
#define RC_CHANNEL_MAX 2000 	//ÿ��ͨ����PWM���ֵ
#define SBUS_MIN_OFFSET 173 	//SBUSЭ�����Сֵ
#define SBUS_MID_OFFSET 992 	//SBUSЭ����м�ֵ
#define SBUS_MAX_OFFSET 1811 	//SBUSЭ������ֵ
#define SBUS_CHANNEL_NUMBER 16 	//SBUSͨ������16��ģ��ͨ����2������ͨ��
#define SBUS_PACKET_LENGTH 25	// SBUS���ݰ����ֽڳ���
#define SBUS_FRAME_HEADER 0x0f		// SBUS����ͷ����ʼ�ֽ�
#define SBUS_FRAME_FOOTER 0x00		// SBUS����β�������ֽ�
#define SBUS_FRAME_FOOTER_V2 0x04	// SBUS����β�������ֽ�2
#define SBUS_STATE_FAILSAFE 0x08	// ������ϰ�ȫ��־
#define SBUS_STATE_SIGNALLOSS 0x04 	// ��ʧ�źű�־
#define SBUS_UPDATE_RATE 10 //SBUS�������ʱ��ms

void sbusPreparePacket(u8 packet[], u16 channels[], u8 isSignalLoss, u8 isFailsafe);
void sbusData(u8 SBUS_DATA[], u16 CH_Rec[]);
float map(float value,float fromLow,float fromHigh,float toLow,float toHigh);

#endif

