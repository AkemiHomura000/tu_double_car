#ifndef CHASSIS_CONTROL_H
#define CHASSIS_CONTROL_H

#include "zf_common_headfile.h"
#include "IfxCpu.h"

/// @brief ��ȡͼ�������ߵ�ƫ��������ȷ���̰߳�ȫ��
void update_camera_data(void);
/// @brief ֱ�߸���
void straight_follow(void);
/// @brief �Ƕ�ֵӳ�䵽�������ֵ
/// @param angel ��ֵΪ0�����ҷ�ΧΪ-45 ~ 45
/// @return ���ض������ֵ
int angel_mapping(float angel);
void gengeral_follow(void);
#endif