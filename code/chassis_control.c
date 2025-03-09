#include "zf_common_headfile.h"
float center_x, center_y;
static pid_param_t liner_vel_pid = PID_CREATE(1.0, 1.0, 0.2, 0.1, 4.0, 3, 9.0); // 直线速度环
static pid_param_t angel_pid = PID_CREATE(1.0, 1.0, 0.2, 0.1, 4.0, 3, 9.0);     // 角度环

void update_camera_data(void)
{
    if (IfxCpu_acquireMutex(&camera_mutex))
    {
        center_x = center_x_offset;
        center_y = center_y_diff;
        IfxCpu_releaseMutex(&camera_mutex);
    }
}
void straight_follow(void)
{
    update_camera_data();
    float liner_vel_control = pid_solve(&liner_vel_pid, 30 - center_y) * 100;
    // float转int
    int liner_vel_control_int = (int)1000 + liner_vel_control;
    if (liner_vel_control_int < 0)
    {
        liner_vel_control_int = 0;
    }
    straight_drive(liner_vel_control_int);
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
    float liner_vel_control = pid_solve(&liner_vel_pid, 30 - center_y) * 100;
    // float转int
    int liner_vel_control_int = (int)1500 + liner_vel_control;
    if (liner_vel_control_int < 0)
    {
        liner_vel_control_int = 0;
    }
    float angel_control = 2*pid_solve(&angel_pid, center_x);
    // printf("angel_control: %f\n", angel_control);
    int angel_control_int = angel_mapping(angel_control);
    general_drive(liner_vel_control_int, angel_control_int);
}
