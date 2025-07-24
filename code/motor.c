/*
 * motor.c
 *
 *  Created on: 2025��3��8��
 *      Author: zsz
 */

#include "zf_common_headfile.h"

#define left_wheel_pwm ATOM0_CH1_P21_3
#define left_wheel_dir P21_2
#define right_wheel_pwm ATOM0_CH3_P21_5
#define right_wheel_dir P21_4
#define steering_pwm ATOM1_CH1_P33_9

#define steering_middle 815 // ��Χ885��--815--740��

#define PIT_60_0_PERIOD 10    // 10ms ������
#define ENCODER_LINE_NUM 2340 //  ȷ������������
#define WHEEL_RADIUS 0.0325   // ȷ�� ���Ӱ뾶
int16 encoder_count_l = 0;    // ��������������
int16 encoder_count_r = 0;    // �ҵ������������
float speed_l = 0.0;
float speed_r = 0.0; // �������ٶ�
void encoder_Read()
{
    encoder_count_l = encoder_get_count(TIM2_ENCODER);  // ��ȡ����������ֵ
    encoder_clear_count(TIM2_ENCODER);                  // �����������������
    encoder_count_r = -encoder_get_count(TIM4_ENCODER); // ��ȡ�ҵ��������ֵ������
    encoder_clear_count(TIM4_ENCODER);                  // ����ҵ������������
    // ��������
    speed_l = 100.0 * (float)encoder_count_l / ENCODER_LINE_NUM * 2 * 3.14 / (PIT_60_0_PERIOD * 0.001) * WHEEL_RADIUS; // �����ٶ�
    speed_r = 100.0 * (float)encoder_count_r / ENCODER_LINE_NUM * 2 * 3.14 / (PIT_60_0_PERIOD * 0.001) * WHEEL_RADIUS; // �����ٶ�
}
// ���������Ͷ��
void motor_init(void)
{
    gpio_init(left_wheel_dir, GPO, 1, GPO_PUSH_PULL);
    gpio_init(right_wheel_dir, GPO, 1, GPO_PUSH_PULL);
    pwm_init(left_wheel_pwm, 17000, 0);
    pwm_init(right_wheel_pwm, 17000, 0);
    pwm_init(steering_pwm, 50, steering_middle); // �ź�����20ms
    encoder_init();
}

void straight_drive(int speed_l, int speed_r)
{
    if (speed_l > speed_max)
        speed_l = speed_max;
    if (speed_r > speed_max)
        speed_r = speed_max;
    pwm_set_duty(steering_pwm, steering_middle);
    pwm_set_duty(left_wheel_pwm, speed_l);
    pwm_set_duty(right_wheel_pwm, speed_r);
}

void general_drive(int speed_l, int speed_r, int steer)
{

    if (speed_l > speed_max)
        speed_l = speed_max;
    if (speed_r > speed_max)
        speed_r = speed_max;
    if (steer > steering_lmax)
        steer = steering_lmax;
    if (steer < steering_rmax)
        steer = steering_rmax;
    pwm_set_duty(steering_pwm, steer);
    if (speed_l > 0)
    {
        gpio_set_level(left_wheel_dir, 1);
        pwm_set_duty(left_wheel_pwm, speed_l);
    }
    else
    {
        gpio_set_level(left_wheel_dir, 0);
        pwm_set_duty(left_wheel_pwm, -speed_l);
    }
    if (speed_r > 0)
    {
        gpio_set_level(right_wheel_dir, 1);
        pwm_set_duty(right_wheel_pwm, speed_r);
    }
    else
    {
        gpio_set_level(right_wheel_dir, 0);
        pwm_set_duty(right_wheel_pwm, -speed_r);
    }
}

static void encoder_init(void)
{
    encoder_dir_init(TIM2_ENCODER, TIM2_ENCODER_CH1_P33_7, TIM2_ENCODER_CH2_P33_6);  // left
    encoder_quad_init(TIM4_ENCODER, TIM4_ENCODER_CH1_P02_8, TIM4_ENCODER_CH2_P00_9); // �ұ�
}
