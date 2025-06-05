#include "zf_common_headfile.h"

IfxCpu_mutexLock param_mutex;
float line_kp = 3.0, line_ki = 0.1, line_kd = 2.0; // PID参数
float set_line_speed = 70.0;                       // 设定线速度
int delta_y = 30;                                  // 设定偏移量
int y_offset_ = 0;                                 // 偏移量
int center_x, center_y;
int angle_yaw = 730;
bool caemra_tracked = false;
static pid_param_t liner_vel_pid = PID_CREATE(1.0, 1.0, 0.2, 0.1, 7.0, 3, 9.0); // 直线速度环
PID_Datatypedef l_liner_vel_pid, r_liner_vel_pid, delta_vel_pid;                // 直线速度环
#define PWM_PID_P 16.0
#define PWM_PID_I 2.0
#define PWM_PID_D 20.0
#define DELTA_VEL_KP 0.2
#define DELTA_VEL_KI 1.5
#define DELTA_VEL_KD 0.0
int16 pwm_r = 750;                                                          // 右电机PWM值
int16 pwm_l = 750;                                                          // 左电机PWM值
static pid_param_t angel_pid = PID_CREATE(1.0, 1.0, 0.2, 0.1, 4.0, 3, 9.0); // 角度环

#define SWITCH2 (P33_12)
void update_camera_data(void)
{
    if (IfxCpu_acquireMutex(&camera_mutex))
    {
        center_x = center_x_offset;
        center_y = center_y_diff;
        caemra_tracked = led_tracked;
        IfxCpu_releaseMutex(&camera_mutex);
    }
}
void straight_follow(void)
{
    // update_camera_data();
    //    float liner_vel_control = pid_solve(&liner_vel_pid, 30 - center_y) * 100;
    // float转int
    //    int liner_vel_control_int = (int)2000 + liner_vel_control;
    // if (liner_vel_control_int < 0)
    // {
    //     liner_vel_control_int = 0;
    // }
    //    straight_drive(liner_vel_control_int);
}
int angel_mapping(float angel)
{
    float k = 0.6;
    int reslut = (int)(730 - angel * k);
    if (reslut > 805)
        reslut = 805;
    if (reslut < 650)
        reslut = 650;
    return reslut;
}
void gengeral_follow(void)
{
    update_camera_data();
    bool switch2 = gpio_get_level(SWITCH2); // 获取开关状态
    pwm_set_duty(steering_pwm, steering_middle);
    if (!switch2)
    {
        float target_speed = set_line_speed;
        static bool initialized = false; // 只在第一次进入时为 false
        if (!initialized)
        {
            PID_Init_Add(&l_liner_vel_pid);
            PID_Init_Add(&r_liner_vel_pid);
            // 线速度环
            l_liner_vel_pid.P = PWM_PID_P;
            l_liner_vel_pid.I = PWM_PID_I;
            l_liner_vel_pid.D = PWM_PID_D;
            r_liner_vel_pid.P = l_liner_vel_pid.P;
            r_liner_vel_pid.I = l_liner_vel_pid.I;
            r_liner_vel_pid.D = l_liner_vel_pid.D;
            PID_Init_Add(&delta_vel_pid);
            delta_vel_pid.P = DELTA_VEL_KP;
            delta_vel_pid.I = DELTA_VEL_KI;
            delta_vel_pid.D = DELTA_VEL_KD;
            initialized = true;
        }
        if (IfxCpu_acquireMutex(&param_mutex))
        {
            // l_liner_vel_pid.P = line_kp;
            // l_liner_vel_pid.I = line_ki;
            // l_liner_vel_pid.D = line_kd;
            // r_liner_vel_pid.P = l_liner_vel_pid.P;
            // r_liner_vel_pid.I = l_liner_vel_pid.I;
            // r_liner_vel_pid.D = l_liner_vel_pid.D;
            delta_vel_pid.P = line_kd;
            delta_vel_pid.I = line_ki;
            delta_vel_pid.D = line_kp;
            y_offset_ = delta_y;
            IfxCpu_releaseMutex(&param_mutex);
        }
        float delta_liner_vel = MotorPID_Output_Add(&delta_vel_pid, y_offset_, 30.0); // 计算线速度环输出
        int16 pwm_line_add_l = (int)MotorPID_Output_Add(&l_liner_vel_pid, speed_l, target_speed);
        int16 pwm_line_add_r = (int)MotorPID_Output_Add(&r_liner_vel_pid, speed_r, target_speed);
        // 更新PWM值
        pwm_l += pwm_line_add_l;
        pwm_r += pwm_line_add_r;
        if (pwm_l > 4000)
            pwm_l = 4000;
        if (pwm_r > 4000)
            pwm_r = 4000;
        seekfree_assistant_oscilloscope_struct oscilloscope_data;
        oscilloscope_data.data[0] = speed_l;
        oscilloscope_data.data[1] = speed_r;
        oscilloscope_data.data[2] = set_line_speed;
        oscilloscope_data.data[3] = pwm_l;
        oscilloscope_data.data[4] = pwm_r;
        oscilloscope_data.data[5] = delta_liner_vel;
        oscilloscope_data.data[6] = angle_yaw;
        oscilloscope_data.channel_num = 7;
        seekfree_assistant_oscilloscope_send(&oscilloscope_data);

        if (angle_yaw > 900)
            angle_yaw = 900;
        if (angle_yaw < 550)
            angle_yaw = 550;
        general_drive(pwm_l, pwm_r, angle_yaw);
    }
    else
    {
        general_drive(0, 0, 730);
    }
    // if (!caemra_tracked)
    // {
    //     float delta_liner_vel = pid_solve(&liner_vel_pid, 30 - center_y) * 50;

    //     float angel_control = pid_solve(&angel_pid, center_x);
    //     int angel_control_int = angel_mapping(angel_control);
    // }
    // else
    // {

    // }
}
