
#include "zf_common_headfile.h"
#include "camera.h"
#pragma section all "cpu0_dsram"

int core0_main(void)
{
    clock_init(); // ��ȡʱ��Ƶ��<��ر���>
    debug_init(); // ��ʼ��Ĭ�ϵ��Դ���

    camera_related_init();
    // �˴���д�û����� ���������ʼ�������
    cpu_wait_event_ready(); // �ȴ����к��ĳ�ʼ�����
    while (TRUE)
    {
        // �˴���д��Ҫѭ��ִ�еĴ���
        camera_run();
        // �˴���д��Ҫѭ��ִ�еĴ���
    }
}

#pragma section all restore
