#ifndef __PWM_H
#define __PWM_H
#include "stm32f10x.h" //�ǵ���Ӵ�ͷ�ļ�����Ϊconfig.c�õ�GPIO��غ�����
#include "sys.h"

#define chNum 8		//����ͨ����

void TIM3_PB5_PWM_Init(u16 arr,u16 psc);//PB5��ʱ����ʼ��
void TIM2_PWM_Init(u16 arr,u16 psc);//PA0-3PWM�����ʼ��
void TIM3_PWM_Init(u16 arr,u16 psc);//PB5��ʱ����ʼ��
#endif
