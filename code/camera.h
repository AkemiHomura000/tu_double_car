#ifndef TU_CAMERA_H
#define TU_CAMERA_H
#include "zf_common_headfile.h"
#include "IfxCpu.h"

#define LED1 (P33_9) // 驱动红外灯
#define IPS200_TYPE (IPS200_TYPE_SPI)
#define M_PI 3.1415926
#define VISITED 127
#define QUEUE_SIZE (MT9V03X_W * MT9V03X_H)
/////////////////////////////可修改参数//////////////////////////////////////
// 滤波方法：二值化，灯的形状阈值，灯的中心x坐标差阈值,灯的像素点数阈值
#define x_threshold 10      // 两个灯条中心的x坐标差阈值
#define shape_threshold 1.2 // 灯条的形状阈值
#define count_threshold 1.3 // 灯条的像素点数阈值
#define THRESHOLD 254       // 二值化阈值
#define BOX_COLOR 200
#define center_x_set 89
///////////////////////////////////////////////////////////////////////////////
extern uint8 image_copy[MT9V03X_H][MT9V03X_W];
extern float center_x_offset; // 在中心线左为负数
extern float center_y_diff;   // 绝对值
extern IfxCpu_mutexLock camera_mutex;

void draw_center_line(uint8 *image);
void binarize_image(uint8 *image);
void find_regions(uint8 *image);
void draw_box(uint8 *image, int x, int y, int r);
bool is_valid(int x, int y);
void bfs(uint8 *image, int start_x, int start_y, int *sum_x, int *sum_y, int *count);
/// @brief 主函数调用，初始化摄像头，ips200显示屏，以及调试串口
/// @todo 取消红外灯条的初始化，解耦屏幕
extern void camera_related_init(void);
/// @brief 主函数调用，更新全局变量中灯条中心距离设定图像中心的偏移量以及亮灯之间的y坐标差
extern void camera_run(void);

#endif
