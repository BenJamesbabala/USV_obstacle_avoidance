#ifndef UTILITIES
#define UTILITIES

#include "stdio.h"
#include "stm32f1xx_hal.h"
#include "main.h"
#include "cmsis_os.h"

//void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart);

#define USART_REC_LEN  			200  	//�����������ֽ��� 200
#define RXBUFFERSIZE   1 //�����С
	  	
extern uint8_t  USART_RX_BUF[USART_REC_LEN]; //���ջ���,���USART_REC_LEN���ֽ�.ĩ�ֽ�Ϊ���з� 
extern uint16_t USART_RX_STA;         		//����״̬���	
extern uint8_t aRxBuffer[RXBUFFERSIZE];//HAL��USART����Buffer

#endif
