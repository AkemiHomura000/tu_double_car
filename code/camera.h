#ifndef TU_CAMERA_H
#define TU_CAMERA_H
#include "zf_common_headfile.h"
#include "IfxCpu.h"

#define LED1 (P33_9) // ���������
#define IPS200_TYPE (IPS200_TYPE_SPI)
#define M_PI 3.1415926
#define VISITED 127
#define QUEUE_SIZE (MT9V03X_W * MT9V03X_H)
/////////////////////////////���޸Ĳ���//////////////////////////////////////
// �˲���������ֵ�����Ƶ���״��ֵ���Ƶ�����x�������ֵ,�Ƶ����ص�����ֵ
#define x_threshold 10      // �����������ĵ�x�������ֵ
#define shape_threshold 1.2 // ��������״��ֵ
#define count_threshold 1.3 // ���������ص�����ֵ
#define THRESHOLD 254       // ��ֵ����ֵ
#define BOX_COLOR 200
#define center_x_set 89
///////////////////////////////////////////////////////////////////////////////
extern uint8 image_copy[MT9V03X_H][MT9V03X_W];
extern float center_x_offset; // ����������Ϊ����
extern float center_y_diff;   // ����ֵ
extern IfxCpu_mutexLock camera_mutex;

void draw_center_line(uint8 *image);
void binarize_image(uint8 *image);
void find_regions(uint8 *image);
void draw_box(uint8 *image, int x, int y, int r);
bool is_valid(int x, int y);
void bfs(uint8 *image, int start_x, int start_y, int *sum_x, int *sum_y, int *count);
/// @brief ���������ã���ʼ������ͷ��ips200��ʾ�����Լ����Դ���
/// @todo ȡ����������ĳ�ʼ����������Ļ
extern void camera_related_init(void);
/// @brief ���������ã�����ȫ�ֱ����е������ľ����趨ͼ�����ĵ�ƫ�����Լ�����֮���y�����
extern void camera_run(void);

#endif
