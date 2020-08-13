#ifndef OA
#define OA
#include "sys.h"
#include "USART.h"
#include "RINGBUFF.h"
#include "main.h"
#include "SERVO.h"
#include "MOTOR.h"

//�����궨��
#define HOT_ZONE 0
#define COLD_ZONE 1
#define TIMER_OVERFLOW_TIME 100
#define HOT_ZONE_SPIN_DELAY 100
#define COLD_ZONE_SPIN_DELAY 200
extern u8 ZONE_FLAG;
extern u8 deducted_obstacle_array[5];
u16 read_one_frame(u16 *distance,UART_HandleTypeDef *huart,RingBuff_t * ringbuff,u16 RINGBUFF_LEN,u8 *frame_data);

typedef struct
{
	volatile u16 secure_distance;	//��ȫ���룺�ڰ�ȫ�������ڵ�����ᱻ��Ϊ���ϰ���
	volatile u8 hotzone_limit;		//�����ж����룺ƽ���ϰ������С����������ʱ���������ģʽ��ɨ���ٶȸ��죩
	volatile  u16 minimum_distance;//��;��룺��������С����;���ʱ����ȡֹͣ���������Զ��
}avoidance_obj;

extern avoidance_obj lidar_avoid_params;
extern u16 obstacle_array[20];
u8 obstacle_detection(avoidance_obj lidar_avoid_params,u8 sample_rate,float f_c,
											UART_HandleTypeDef *huart,RingBuff_t * ringbuff,u16 RINGBUFF_LEN,u8 *frame_data);
void obstacle_avoidance(avoidance_obj lidar_avoid_params,u8 * deducted_obstacle_array,u8 minimumm_distance);
#endif
