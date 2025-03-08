/*
 * motor.c
 *
 *  Created on: 2025年3月8日
 *      Author: zsz
 */


#include "zf_common_headfile.h"

#define left_wheel_pwm ATOM0_CH1_P21_3
#define left_wheel_dir P21_2
#define right_wheel_pwm ATOM0_CH3_P21_5
#define right_wheel_dir P21_4
#define steering_pwm ATOM1_CH1_P33_9

#define steering_middle 730//范围805左--730--650右

//包括驱动和舵机
void motor_init(void)
{
    gpio_init(left_wheel_dir, GPO, 1, GPO_PUSH_PULL);
    gpio_init(right_wheel_dir, GPO, 1, GPO_PUSH_PULL);
    pwm_init(left_wheel_pwm, 17000, 0);
    pwm_init(right_wheel_pwm, 17000, 0);
    //
    pwm_init(steering_pwm, 50, steering_middle);//信号周期20ms
    //
    encoder_init();
}

void straight_drive(int speed)
{
    if(speed > speed_max) speed = speed_max;
    pwm_set_duty(steering_pwm, steering_middle);
    pwm_set_duty(left_wheel_pwm, speed);
    pwm_set_duty(right_wheel_pwm, speed);
}

void general_drive(int speed, int steer)
{
    if(speed > speed_max) speed = speed_max;
    if(steer > steering_lmax) steer = steering_lmax;
    if(steer < steering_rmax) steer = steering_rmax;
    pwm_set_duty(steering_pwm, steer);
    pwm_set_duty(left_wheel_pwm, speed);
    pwm_set_duty(right_wheel_pwm, speed);
}

static void encoder_init(void)
{
    encoder_dir_init(TIM2_ENCODER, TIM2_ENCODER_CH1_P33_7, TIM2_ENCODER_CH2_P33_6);//left
    encoder_dir_init(TIM5_ENCODER, TIM5_ENCODER_CH1_P10_3, TIM5_ENCODER_CH2_P10_1);//right需要取负
}

