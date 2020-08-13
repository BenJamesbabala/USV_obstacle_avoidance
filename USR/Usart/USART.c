#include "USART.h"

int fputc(int ch, FILE *f)
{ 	
	while((USART1->SR&0X40)==0);//ѭ������,ֱ���������   
	USART1->DR = (uint8_t) ch;      
	return ch;
}

//�����жϷ������
u8 USART_RX_BUF[USART_REC_LEN];     //���ջ���,���USART_REC_LEN���ֽ�.
u8 aRxBuffer[RXBUFFERSIZE];//HAL��ʹ�õĴ��ڽ��ջ���

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	HAL_GPIO_TogglePin(GPIOB,GPIO_PIN_11);
	if(huart->Instance==USART2)
	{
		WriteRingBuff(&RINGBUFF_UART2,LENGTH_OF_BUFF,aRxBuffer[0]);
	}
	if(huart->Instance==USART3)
	{
		WriteRingBuff(&RINGBUFF_UART3,LENGTH_OF_BUFF,aRxBuffer[0]);
	}
}
