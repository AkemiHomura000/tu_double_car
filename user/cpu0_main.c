
#include "zf_common_headfile.h"
#include "camera.h"
#include "IfxCpu.h"

#pragma section all "cpu0_dsram"

int core0_main(void)
{
    clock_init(); // ��ȡʱ��Ƶ��<��ر���>
    debug_init(); // ��ʼ��Ĭ�ϵ��Դ���

    // �˴���д�û����� ���������ʼ�������
    cpu_wait_event_ready(); // �ȴ����к��ĳ�ʼ�����
    while (TRUE)
    {
        int center=0;
        // �˴���д��Ҫѭ��ִ�еĴ���
        if (IfxCpu_acquireMutex(&camera_mutex))
        {
            center =center_x_offset;
            IfxCpu_releaseMutex(&camera_mutex);
        }
        printf("%d \n", center);
        // �˴���д��Ҫѭ��ִ�еĴ���
        system_delay_ms(10);
    }
}

#pragma section all restore
