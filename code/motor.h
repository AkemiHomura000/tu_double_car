/*
 * motor.h
 *
 *  Created on: 2025年3月8日
 *      Author: zsz
 */

#ifndef CODE_MOTOR_H_
#define CODE_MOTOR_H_

#define left_wheel_pwm ATOM0_CH1_P21_3
#define left_wheel_dir P21_2
#define right_wheel_pwm ATOM0_CH3_P21_5
#define right_wheel_dir P21_4
#define steering_pwm ATOM1_CH1_P33_9

#define steering_middle 730 // 范围805左--730--650右
#define steering_lmax 805
#define steering_rmax 650
#define speed_max 4000
extern float speed_l;
extern float speed_r; // 左右轮速度
void encoder_Read(void);
void motor_init(void);
void straight_drive(int speed_l, int speed_r);
void general_drive(int speed_l, int speed_r, int steer);
void encoder_init(void);

#endif /* CODE_MOTOR_H_ */
