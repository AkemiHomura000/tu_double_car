#ifndef CHASSIS_CONTROL_H
#define CHASSIS_CONTROL_H

#include "zf_common_headfile.h"
#include "IfxCpu.h"

/// @brief 获取图像中心线的偏移量（已确保线程安全）
void update_camera_data(void);
/// @brief 直线跟随
void straight_follow(void);

#endif