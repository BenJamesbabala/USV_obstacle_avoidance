#include "MOTOR.h"


void TIM_SetTIM3Compare2(uint32_t compare)
{
	TIM3->CCR2=compare;
}
void TIM_SetTIM3Compare3(uint32_t compare)
{
	TIM3->CCR3=compare;
}

void left_wheel_forward(uint8_t speed_pwm)
{
	HAL_GPIO_WritePin(GPIOC,GPIO_PIN_6,GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOC,GPIO_PIN_7,GPIO_PIN_RESET);
	TIM_SetTIM3Compare2(speed_pwm);
}
void left_wheel_backward(uint8_t speed_pwm)
{
	HAL_GPIO_WritePin(GPIOC,GPIO_PIN_6,GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOC,GPIO_PIN_7,GPIO_PIN_SET);
	TIM_SetTIM3Compare2(speed_pwm);
}
void left_wheel_stop(void)
{
	HAL_GPIO_WritePin(GPIOC,GPIO_PIN_6,GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOC,GPIO_PIN_7,GPIO_PIN_RESET);	
}

void right_wheel_forward(uint8_t speed_pwm)
{
	HAL_GPIO_WritePin(GPIOC,GPIO_PIN_8,GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOC,GPIO_PIN_9,GPIO_PIN_RESET);
	TIM_SetTIM3Compare3(speed_pwm);
}
void right_wheel_backward(uint8_t speed_pwm)
{
	HAL_GPIO_WritePin(GPIOC,GPIO_PIN_8,GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOC,GPIO_PIN_9,GPIO_PIN_SET);
	TIM_SetTIM3Compare3(speed_pwm);
}
void right_wheel_stop(void)
{
	HAL_GPIO_WritePin(GPIOC,GPIO_PIN_8,GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOC,GPIO_PIN_9,GPIO_PIN_RESET);	
}


void forward(uint8_t speed_pwm)
{
	printf("forward\n");
	left_wheel_forward(speed_pwm);
	right_wheel_forward((uint8_t)((float)speed_pwm*1.3));
}

void backward(uint8_t speed_pwm)
{
	printf("backward\n");
	left_wheel_backward(speed_pwm);
	right_wheel_backward((uint8_t)((float)speed_pwm*1.5));	
}
void stop(void)
{
	printf("stop\n");
	right_wheel_stop();
	left_wheel_stop();
}

void turn(uint8_t left_speed_pwm,uint8_t right_speed_pwm)
{
	left_wheel_forward(left_speed_pwm);
	right_wheel_forward((uint8_t)((float)right_speed_pwm*1.4));
}
void left_steel(void)
{
	printf("left\n");
	turn(18,25);
}
void right_steel(void)
{
	printf("right\n");
	turn(25,18);
}