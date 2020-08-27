#include "MMWAVE.h"

/*�������MAXIMUM_ANGLE�������ú��ײ��״�ɨ�������ĽǶ�*/
/*����MAXIMUM_ANGLE=100�������״�����ǰ�����Ҹ���ɨ�赽50�ȵĻ���*/

#define MAXIMUM_ANGLE 100

/**/
#define ANGLE_SLICE MAXIMUM_ANGLE/5



uint16_t obstacle_array[5];
static uint8_t obstacle_array_counter[5];//��������������˲�

/*
�������ܣ��Ӵ��ڵĻ������ж���ȡ�������ϰ�����Ϣ
����˵����
obstacle_array_counter_mean_distance:һ���ⲿ������ָ������������������ַ����ž���ƽ�����˲���ľ�������
frame_number_to_filter���˲�ʹ�õ�֡��������frame_number_to_filter=10������ȡ10֡�״���ϰ��������ٽ��д����Է�����
obstacle_confirm_threshold��ȷ��һ����λ�����ϰ��������ֵ�����frame_number_to_filterһ������
���磺frame_number_to_filter=10  obstacle_confirm_threshold=5
�����ۼƶ�ȡ��10֡�״��ϰ������ݺ����5֡����ĳ����λ�����ϰ�����ڣ���ô��ȷ�ϴ��ϰ�����ڣ�������Ϊ�ϰ���Ϊ�龯��
*/
uint8_t mmwave_process_data(uint16_t *obstacle_array_counter_mean_distance,uint8_t frame_number_to_filter,uint8_t obstacle_confirm_threshold)
{
	/*��ȡmmwave��Ϣ������洢�ĺ��ײ��״���Ϣ*/
	osEvent message;
	static uint8_t rx_data_header[2];
	static uint8_t rx_data_package[8];
	static uint8_t frames_rx_counter=0;
//	uint8_t object_number = 0;
	uint8_t temp,temp1;
	uint16_t distance;
	int8_t angle;
	rx_data_header[0] = 0;
	rx_data_header[1] = 1;

	if(frames_rx_counter == 0)
	{
		for(temp1=0;temp1<5;temp1++)
		{
			obstacle_array_counter[temp1] = 0;
			obstacle_array_counter_mean_distance[temp1] = 0xFFFF;
		}		
	}

	message = osMessageGet(mmwave_data_transmission_queueHandle,osWaitForever);
	if(message.status == osEventMessage)
	{
		rx_data_header[0] = message.value.v;
	}	
	
	if(rx_data_header[0] == 0x0C || rx_data_header[0] == 0x0B)
	{
		message = osMessageGet(mmwave_data_transmission_queueHandle,osWaitForever);
		if(message.status == osEventMessage)
		{
			rx_data_header[1] = message.value.v;
		}				
	}
	
	if(rx_data_header[1] == 0x07)//��ͷ���յ�
	{
		switch(rx_data_header[0])
		{
			case 0x0B:
			{  /*��0x0B���ڶ�ȡ��⵽��Ŀ�����*/
				/*------��ʱ����ҪĿ��������ݣ�������ε�-----*/
//				message = osMessageGet(mmwave_data_transmission_queueHandle,osWaitForever);
//				if(message.status == osEventMessage)
//				{
//					object_number = message.value.v;//��ü��Ŀ�����
//				}			
//				printf("Ŀ�������%d \n",object_number);
				break;
			}
			case 0x0C:
			{
				for(temp = 0;temp<8;temp++)
				{//����������һ֡
					message = osMessageGet(mmwave_data_transmission_queueHandle,osWaitForever);
					if(message.status == osEventMessage)
					{
						rx_data_package[temp] = message.value.v;
					}
				}
				frames_rx_counter += 1;
				distance = (((uint16_t)rx_data_package[2])<<8) +	rx_data_package[3];	
				angle = ((int8_t)rx_data_package[4])*2 - 90;
//				printf("��ǰĿ�귽λ��:%d,����:%d\n",angle,distance);
				
				for(temp1=0;temp1<5;temp1++)
				{
					obstacle_array[temp1] = 0;
				}
				
				for(temp1=0;temp1<5;temp1++)
				{
					if(-(MAXIMUM_ANGLE/2)+temp1*ANGLE_SLICE <=angle && angle<=-(MAXIMUM_ANGLE/2)+(temp1+1)*ANGLE_SLICE)
					{
						obstacle_array_counter[temp1] += 1;
						obstacle_array_counter_mean_distance[temp1] += distance;//��ʼֵΪ0xFFFF���ټӻᵼ��������������������
						obstacle_array[temp1] = distance;
						break;
					}
				}
				
				if(frames_rx_counter >= frame_number_to_filter)
				{
					frames_rx_counter = 0;
					for(temp1=0;temp1<5;temp1++)
					{
						if(obstacle_array_counter[temp1] >= obstacle_confirm_threshold)
							obstacle_array_counter_mean_distance[temp1] /= frame_number_to_filter;
						else
							obstacle_array_counter_mean_distance[temp1] = 0xFFFF;
					}	
					printf("\n");
					for(temp1=0;temp1<5;temp1++)
					{
						printf(" %d ",obstacle_array_counter_mean_distance[temp1]);
					}printf("\n");					
					return 1;
				}

			}
		}
	}
	return 0;
}

/*
�������ܣ���������ʽ�ľ�������ת��Ϊ�����������͵����ݣ�ͬʱ��ȡ��������������ϰ������
������
minimum_dis��ָ�������ݣ�ָ��һ���ⲿ�ı����������������������ȡ����С�ľ���ֵ
avoid_params:���ϵĲ������ã������ڼ���֮����Ҫ��ʼ���
obstacle_array��ԭʼ��������ʽ�ľ�������
*/
uint16_t process_obstacle_array(uint16_t *minimum_dis,avoidance_obj avoid_params,uint16_t *obstacle_array)
{
	uint8_t temp1,temp2;
	uint16_t byte_form_array;
	uint8_t obstacle_bitmask_array[5];
	*minimum_dis = obstacle_array[0];//�ҳ��ϰ������С����
	for(temp1=0;temp1<5;temp1++)
	{
		/*�ҳ���С���ϰ������*/
		if(obstacle_array[temp1]<=*minimum_dis)
			*minimum_dis = obstacle_array[temp1];
		
		/*���ϰ�����Ϣת��Ϊ�����ʽ*/
		if(obstacle_array[temp1] <= avoid_params.secure_distance)
			obstacle_bitmask_array[temp1] = 1;
		else
			obstacle_bitmask_array[temp1] = 0; 
	}
	/*����*/
	printf("\n");
	for(temp1=0;temp1<5;temp1++)
	{
		printf(" %d ",obstacle_bitmask_array[temp1]);
	}printf("\n");	
		
		/*�������ʽת��Ϊ�ֽ���ʽ��{1,0,1,0,1} -> (uint16_t)10101*/
		byte_form_array = 0;
		for(temp1=0;temp1<5;temp1++)
		{
			temp2 = obstacle_bitmask_array[temp1];
			byte_form_array |= temp2 << (4-temp1);
		}
			
		return byte_form_array;
}

