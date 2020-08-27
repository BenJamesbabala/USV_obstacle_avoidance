#include "util.h"

//==============�ļ��ڰ������õĺ�������===============
int fputc(int ch, FILE *f)
{ 	
	while((USART1->SR&0X40)==0);//ѭ������,ֱ���������   
	USART1->DR = (uint8_t) ch;      
	return ch;
}

//�����жϷ������
uint8_t USART_RX_BUF[USART_REC_LEN];     //���ջ���,���USART_REC_LEN���ֽ�.
uint8_t aRxBuffer[RXBUFFERSIZE];//HAL��ʹ�õĴ��ڽ��ջ���

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	static uint8_t usart_buff_last_rx_byte;
	static uint8_t usart_buff_current_rx_byte;
	static uint8_t package_head_received = 0; //�յ�0xAAAA��ͷ��־λ
	static uint8_t in_rx_progress_flag = 0; //���ڽ���һ�����ݱ�־λ
	static uint8_t rx_data_counter = 0;
	
	if(huart->Instance==USART2)
	{
			osMessagePut(lidar_data_transmission_queueHandle,aRxBuffer[0],50);
	}
	
	
/*---���ײ��״�ʹ�õĴ���ISR����---*/	
/*���ײ��״ﷵ��һ֡����*/
/*  AA AA 0C 07 01 79 01 D8 2D 02 BC 94 55 55 */
/*  AA AA 0B 07 02 00 00 00 00 00 00 00 55 55 */
/*  AA AA 0A 06 01 13 00 00 00 00 00 00 55 55 AA AA 0B 07 02 00 00 00 00 00 00 00 55 55 */
	if(huart->Instance==USART3)
	{
		usart_buff_last_rx_byte = usart_buff_current_rx_byte;
		usart_buff_current_rx_byte = aRxBuffer[0];
		rx_data_counter += 1;

		if(usart_buff_current_rx_byte == 0xAA && usart_buff_last_rx_byte == 0xAA)
		{
			in_rx_progress_flag = 0;//��ʼ�����ͷ�����������ֽڣ�����������
			package_head_received = 1;//�յ���ͷ
			rx_data_counter = 0;
		}
		else if (usart_buff_current_rx_byte == 0x55 && usart_buff_last_rx_byte == 0x55)
		{
			in_rx_progress_flag = 0;//���������ֽڣ�����������
			package_head_received = 0;//�յ���ͷ
			rx_data_counter = 0;	
		}
		
		if(in_rx_progress_flag == 1)
		{
			//HAL_UART_Transmit(&huart1,&aRxBuffer[0],1,50);
			osMessagePut(mmwave_data_transmission_queueHandle,aRxBuffer[0],150);
		}
		
		if((package_head_received == 1 && usart_buff_current_rx_byte == 0x07 && rx_data_counter <= 2))
		{/*���������ϸ�һЩ����ֹ���ݽ��ճ���*/
			switch(usart_buff_last_rx_byte)
			{
				case 0x0C:
				case 0x0B:
				{
					osMessagePut(mmwave_data_transmission_queueHandle,usart_buff_last_rx_byte,150);//�����ͷ
					//HAL_UART_Transmit(&huart1,&usart_buff_last_rx_byte,1,50);
					osMessagePut(mmwave_data_transmission_queueHandle,0x07,50);//�����ͷ
					//HAL_UART_Transmit(&huart1,&usart_buff_current_rx_byte,1,150);
					in_rx_progress_flag = 1;
					break;
				}
				default:
				{
					in_rx_progress_flag = 0;
					package_head_received = 0;
					break;
				}
			}
		}
		
	}
}
