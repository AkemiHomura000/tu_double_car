#ifndef CHASSIS_CONTROL_H
#define CHASSIS_CONTROL_H

#include "zf_common_headfile.h"
#include "IfxCpu.h"

/// @brief 获取图像中心线的偏移量（已确保线程安全）
void update_camera_data(void);
/// @brief 直线跟随
void straight_follow(void);
/// @brief 角度值映射到舵机控制值
/// @param angel 中值为0，左右范围为-45 ~ 45
/// @return 返回舵机控制值
int angel_mapping(float angel);
void gengeral_follow(void);
#endif