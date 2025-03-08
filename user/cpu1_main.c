#include "zf_common_headfile.h"
#pragma section all "cpu1_dsram"

void core1_main(void)
{
    disable_Watchdog();                     // 关闭看门狗
    interrupt_global_enable(0);             // 打开全局中断
    // 此处编写用户代码 例如外设初始化代码等
    camera_related_init();
    // 此处编写用户代码 例如外设初始化代码等
    cpu_wait_event_ready();                 // 等待所有核心初始化完毕
    while (TRUE)
    {
        // 此处编写需要循环执行的代码
        camera_run();
        // 此处编写需要循环执行的代码
    }
}
#pragma section all restore
