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
	if(huart->Instance==USART2)
	{
			osMessagePut(lidar_data_transmission_queueHandle,aRxBuffer[0],osWaitForever);
	}
	if(huart->Instance==USART3)
	{
		;
	}
}
