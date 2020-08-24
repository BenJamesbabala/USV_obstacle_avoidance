#include "IIC.h"

#define IIC_PORT_CLK_ENABLE() __HAL_RCC_GPIOA_CLK_ENABLE()
#define IIC_PORT GPIOA
#define SCL GPIO_PIN_10
#define SDA GPIO_PIN_11
//ע��SCL��SDAӦ������ͬһ��GPIO

void IIC_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	//ʹ������ IO PORTC ʱ��
	IIC_PORT_CLK_ENABLE();
	GPIO_InitStructure.Pin = SCL|SDA;
	GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP ; //�������
	GPIO_InitStructure.Pull=GPIO_PULLUP; 
	GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(IIC_PORT, &GPIO_InitStructure);
	HAL_GPIO_WritePin(IIC_PORT,SCL,GPIO_PIN_SET);
	HAL_GPIO_WritePin(IIC_PORT,SDA,GPIO_PIN_SET);
	
}

void SDA_OUT()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	//ʹ������ IO PORTC ʱ��
	GPIO_InitStructure.Pin = SDA;
	GPIO_InitStructure.Pull=GPIO_PULLUP; 
	GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP ; //�������
	GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(IIC_PORT, &GPIO_InitStructure);
}

void SDA_IN()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	//ʹ������ IO PORT ʱ��
	GPIO_InitStructure.Pull=GPIO_PULLUP; 
	GPIO_InitStructure.Pin = SDA;
	GPIO_InitStructure.Mode = GPIO_MODE_INPUT ; //��������
	HAL_GPIO_Init(IIC_PORT, &GPIO_InitStructure);
}
//���� IIC ��ʼ�ź�

void IIC_Start(void)
{
	SDA_OUT(); //sda �����
	GPIO_SetBits(IIC_PORT,SDA);
	delay_us(2);
	GPIO_SetBits(IIC_PORT,SCL);
	delay_us(3);
	GPIO_ResetBits(IIC_PORT,SDA);//START:when CLK is high,DATA change form high to low
	delay_us(3);
	GPIO_ResetBits(IIC_PORT,SCL);//ǯס I2C ���ߣ�׼�����ͻ��������
}

//���� IIC ֹͣ�ź�
void IIC_Stop(void)
{
	SDA_OUT();//sda �����
	GPIO_ResetBits(IIC_PORT,SCL);
	GPIO_ResetBits(IIC_PORT,SDA);//STOP:when CLK is high DATA change form low to high
	delay_us(3);
	GPIO_SetBits(IIC_PORT,SCL);
	GPIO_SetBits(IIC_PORT,SDA);//���� I2C ���߽����ź�
	delay_us(3);
}

//�ȴ�Ӧ���źŵ���
//����ֵ��0������Ӧ��ʧ��
// 1������Ӧ��ɹ�
u8 IIC_Wait_Ack(void)
{
	u16 ucErrTime=0;
	SDA_IN(); //SDA ����Ϊ����
	GPIO_SetBits(IIC_PORT,SDA);delay_us(1);
	GPIO_SetBits(IIC_PORT,SCL);delay_us(1);
	while(GPIO_ReadInputDataBit(IIC_PORT,SDA))
	{
		ucErrTime++;
		if(ucErrTime>250)
		{
			IIC_Stop();
			return 0;
		}
	}
	GPIO_ResetBits(IIC_PORT,SCL);//ʱ����� 0
	return 1;
}

//���� ACK Ӧ��
void IIC_Ack(void)
{
	GPIO_ResetBits(IIC_PORT,SCL);
	SDA_OUT();
	GPIO_ResetBits(IIC_PORT,SDA);
	delay_us(2);
	GPIO_SetBits(IIC_PORT,SCL);
	delay_us(2);
	GPIO_ResetBits(IIC_PORT,SCL);
}
//������ ACK Ӧ��
void IIC_NAck(void)
{
	GPIO_ResetBits(IIC_PORT,SCL);
	SDA_OUT();
	GPIO_SetBits(IIC_PORT,SDA);
	delay_us(2);
	GPIO_SetBits(IIC_PORT,SCL);
	delay_us(2);
	GPIO_ResetBits(IIC_PORT,SCL);
}
//IIC ����һ���ֽ�
//���شӻ�����Ӧ��
//1����Ӧ��
//0����Ӧ��
void IIC_Send_Byte(u8 txd)
{
	u8 t;
	SDA_OUT();
	GPIO_ResetBits(IIC_PORT,SCL);//����ʱ�ӿ�ʼ���ݴ���
	for(t=0;t<8;t++)
	{
		switch((txd&0x80)>>7)
		{
			case 1: GPIO_SetBits(IIC_PORT,SDA);break;
			case 0:GPIO_ResetBits(IIC_PORT,SDA);break;
		}
		txd<<=1;
		delay_us(2); //�� TEA5767 ��������ʱ���Ǳ����
		GPIO_SetBits(IIC_PORT,SCL);
		delay_us(2);
		GPIO_ResetBits(IIC_PORT,SCL);
		delay_us(2);
	}
}

//�� 1 ���ֽڣ�ack=1 ʱ������ ACK��ack=0������ nACK
u8 IIC_Read_Byte(unsigned char ack)
{
	unsigned char i,receive=0;
	SDA_IN();//SDA ����Ϊ����
	for(i=0;i<8;i++ )
	{
		GPIO_ResetBits(IIC_PORT,SCL);
		delay_us(2);
		GPIO_SetBits(IIC_PORT,SCL);
		receive<<=1;
		if(GPIO_ReadInputDataBit(IIC_PORT,SDA))receive++;
		delay_us(1);
		
	}
	if (!ack)
	IIC_NAck();//���� nACK
	else
	IIC_Ack(); //���� ACK
	return receive;
}
