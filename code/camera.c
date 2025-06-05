#include "zf_common_headfile.h"
uint8 image_copy[MT9V03X_H][MT9V03X_W];
int center_x_offset; // 在中心线左为负数
int center_y_diff;
bool led_tracked = false; // 是否追踪到灯条
IfxCpu_mutexLock camera_mutex;

void draw_center_line(uint8 *image)
{
    for (int y = 0; y < MT9V03X_H; y++)
    {
        image[y * MT9V03X_W + center_x_set] = 255;
    }
}
void binarize_image(uint8 *image)
{
    for (int y = 0; y < MT9V03X_H; y++)
    {
        for (int x = 0; x < MT9V03X_W; x++)
        {
            int index = y * MT9V03X_W + x;
            image[index] = (image[index] >= THRESHOLD) ? 255 : 0;
        }
    }
}
void draw_box(uint8 *image, int x, int y, int r)
{
    int left = (x - r < 0) ? 0 : x - r;
    int right = (x + r >= MT9V03X_W) ? MT9V03X_W - 1 : x + r;
    int top = (y - r < 0) ? 0 : y - r;
    int bottom = (y + r >= MT9V03X_H) ? MT9V03X_H - 1 : y + r;
    for (int i = left; i <= right; i++)
    {
        image[top * MT9V03X_W + i] = BOX_COLOR;    // 上边
        image[bottom * MT9V03X_W + i] = BOX_COLOR; // 下边
    }
    for (int j = top; j <= bottom; j++)
    {
        image[j * MT9V03X_W + left] = BOX_COLOR;  // 左边
        image[j * MT9V03X_W + right] = BOX_COLOR; // 右边
    }
}
bool is_valid(int x, int y)
{
    return (x >= 0 && x < MT9V03X_W && y >= 0 && y < MT9V03X_H);
}
void bfs(uint8 *image, int start_x, int start_y, int *sum_x, int *sum_y, int *count)
{
    int queue_x[QUEUE_SIZE], queue_y[QUEUE_SIZE];
    int front = 0, rear = 0; // 队列的头尾索引

    queue_x[rear] = start_x;
    queue_y[rear] = start_y;
    rear++;

    image[start_y * MT9V03X_W + start_x] = VISITED; // 标记访问
    *sum_x = start_x;
    *sum_y = start_y;
    *count = 1;

    int min_x = start_x, max_x = start_x;
    int min_y = start_y, max_y = start_y;

    int dx[] = {1, -1, 0, 0};
    int dy[] = {0, 0, 1, -1};

    while (front < rear)
    {
        int x = queue_x[front];
        int y = queue_y[front];
        front++;

        for (int d = 0; d < 4; d++)
        {
            int nx = x + dx[d];
            int ny = y + dy[d];

            if (is_valid(nx, ny) && image[ny * MT9V03X_W + nx] == 255)
            {
                image[ny * MT9V03X_W + nx] = VISITED;
                queue_x[rear] = nx;
                queue_y[rear] = ny;
                rear++;

                *sum_x += nx;
                *sum_y += ny;
                (*count)++;

                if (nx < min_x)
                    min_x = nx;
                if (nx > max_x)
                    max_x = nx;
                if (ny < min_y)
                    min_y = ny;
                if (ny > max_y)
                    max_y = ny;
            }
        }
    }

    int width = max_x - min_x;
    int height = max_y - min_y;

    if (width > height * shape_threshold || height > width * shape_threshold)
    {
        *count = 0; // 直接标记此区域为无效
    }
}
void find_regions(uint8 *image)
{
    int center_raw[200]; // 每个点占三位，分别为x，y，r
    int region_num = 0;
    for (int y = 0; y < MT9V03X_H; y++)
    {
        for (int x = 0; x < MT9V03X_W; x++)
        {
            if (image[y * MT9V03X_W + x] == 255)
            { // 找到一个新的白色区域
                int sum_x = 0, sum_y = 0, count = 0;
                bfs(image, x, y, &sum_x, &sum_y, &count);

                if (count > 5 && count < 500)
                { // 确保找到的区域有效
                    int center_x = sum_x / count;
                    int center_y = sum_y / count;
                    // printf("Region center at: (%d, %d), size: %d pixels\n", center_x, center_y, count);
                    int r = (int)(sqrt(count / M_PI) + 5);
                    // draw_box(image,center_x,center_y,r);
                    if (region_num >= 200)
                        break; // 防止溢出
                    center_raw[region_num * 3] = center_x;
                    center_raw[region_num * 3 + 1] = center_y;
                    center_raw[region_num * 3 + 2] = r;
                    region_num++;
                }
            }
        }
    }
    for (int i = 0; i < region_num; i++)
    {
        bool valid = false;
        for (int j = i + 1; j < region_num; j++)
        {
            int x1 = center_raw[i * 3];
            int x2 = center_raw[j * 3];

            int count1 = center_raw[i * 3 + 2];
            int count2 = center_raw[j * 3 + 2];
            if (abs(x1 - x2) < x_threshold && count1 < count_threshold * count2 && count2 < count_threshold * count1)
            {
                valid = true;
                int y1 = center_raw[i * 3 + 1];
                int r1 = center_raw[i * 3 + 2];

                int y2 = center_raw[j * 3 + 1];
                int r2 = center_raw[j * 3 + 2];

                draw_box(image, x1, y1, r1);
                draw_box(image, x2, y2, r2);
                // printf("Drawing box for (%d, %d) and (%d, %d)\n", x1, y1, x2, y2);
                while (!IfxCpu_acquireMutex(&camera_mutex))
                {
                    system_delay_ms(1);
                }
                center_x_offset = (x1 + x2) / 2.0 - center_x_set;
                center_y_diff = abs(y1 - y2);
                led_tracked = true;
                seekfree_assistant_oscilloscope_data.data[0] = center_x_offset;
                seekfree_assistant_oscilloscope_data.data[1] = center_y_diff;
                IfxCpu_releaseMutex(&camera_mutex);
                untracked_count = 0;
                // seekfree_assistant_oscilloscope_data.data[2] =seekfree_assistant_parameter[2];
                // seekfree_assistant_oscilloscope_data.data[3] =seekfree_assistant_parameter[3];
                //    detector_oscilloscope_data.data[4] = 10;
                //    detector_oscilloscope_data.data[5] = 100;
                //    detector_oscilloscope_data.data[6] = 1000;
                //    detector_oscilloscope_data.data[7] = 10000;

                // 设置本次需要发送几个通道的数据
                seekfree_assistant_oscilloscope_data.channel_num = 2;

                // 这里进发送了4个通道的数据，最大支持8通道
                // seekfree_assistant_oscilloscope_send(&seekfree_assistant_oscilloscope_data);
            }
        }
        if (!valid)
        {
            untracked_count++;
            if (untracked_count > 5)
            {
                while (!IfxCpu_acquireMutex(&camera_mutex))
                {
                    system_delay_ms(1);
                }
                led_tracked = false;
                IfxCpu_releaseMutex(&camera_mutex);
                untracked_count = 100;
            }
        }
    }
}
void camera_related_init(void)
{
    // gpio_init(LED1, GPO, GPIO_HIGH, GPO_PUSH_PULL); // 初始化 LED1 输出 默认高电平 推挽输出模式
    printf("init");
    ips200_init(IPS200_TYPE);
    ips200_show_string(0, 0, "mt9v03x init.");
    while (1)
    {
        if (mt9v03x_init())
            {printf("reinit \n");
            ips200_show_string(0, 80, "mt9v03x reinit.");}
        else
            break;
        system_delay_ms(500); // 短延时快速闪灯表示异常
    }
    ips200_show_string(0, 16, "init success.");
    seekfree_assistant_interface_init(SEEKFREE_ASSISTANT_DEBUG_UART);
}
void camera_run(void)
{
    if (mt9v03x_finish_flag)
    {
        // ips200_displayimage03x((const uint8 *)mt9v03x_image, MT9V03X_W, MT9V03X_H); // 显示原始图像
        // ips200_show_gray_image(0, 0, (const uint8 *)mt9v03x_image, MT9V03X_W, MT9V03X_H, 240, 180, 254); // 显示二值化图像
        memcpy(image_copy[0], mt9v03x_image[0], MT9V03X_IMAGE_SIZE);

        // 1. 先二值化图像
        binarize_image(image_copy);
        // 2. 寻找所有白色区域，并计算中心点
        find_regions(image_copy);
        draw_center_line(image_copy);
        ips200_displayimage03x((uint8 *)image_copy, MT9V03X_W, MT9V03X_H); // 显示图像
        mt9v03x_finish_flag = 0;
    }
}
