#ifndef OA
#define OA
#include "main.h"
#include "MOTOR.h"
#include "SERVO.h"

//�����궨��
#define HOT_ZONE 0
#define COLD_ZONE 1
#define TIMER_OVERFLOW_TIME 100
#define HOT_ZONE_SPIN_DELAY 100
#define COLD_ZONE_SPIN_DELAY 200


typedef struct
{
	volatile uint16_t secure_distance;	//��ȫ���룺�ڰ�ȫ�������ڵ�����ᱻ��Ϊ���ϰ���
	volatile uint16_t hotzone_limit;		//�����ж����룺ƽ���ϰ������С����������ʱ���������ģʽ��ɨ���ٶȸ��죩
	volatile  uint16_t minimum_distance;//��;��룺��������С����;���ʱ����ȡֹͣ���������Զ��
}avoidance_obj;


extern uint16_t obstacle_array[20];

#endif
