
#include "zf_common_headfile.h"
#include "camera.h"
#include "IfxCpu.h"

#pragma section all "cpu0_dsram"

int core0_main(void)
{
    clock_init(); // 获取时钟频率<务必保留>
    debug_init(); // 初始化默认调试串口

    // 此处编写用户代码 例如外设初始化代码等
    cpu_wait_event_ready(); // 等待所有核心初始化完毕
    while (TRUE)
    {
        int center=0;
        // 此处编写需要循环执行的代码
        if (IfxCpu_acquireMutex(&camera_mutex))
        {
            center =center_x_offset;
            IfxCpu_releaseMutex(&camera_mutex);
        }
        printf("%d \n", center);
        // 此处编写需要循环执行的代码
        system_delay_ms(10);
    }
}

#pragma section all restore
