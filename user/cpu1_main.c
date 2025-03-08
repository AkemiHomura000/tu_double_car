#include "zf_common_headfile.h"
#pragma section all "cpu1_dsram"

void core1_main(void)
{
    disable_Watchdog();                     // �رտ��Ź�
    interrupt_global_enable(0);             // ��ȫ���ж�
    // �˴���д�û����� ���������ʼ�������
    camera_related_init();
    // �˴���д�û����� ���������ʼ�������
    cpu_wait_event_ready();                 // �ȴ����к��ĳ�ʼ�����
    while (TRUE)
    {
        // �˴���д��Ҫѭ��ִ�еĴ���
        camera_run();
        // �˴���д��Ҫѭ��ִ�еĴ���
    }
}
#pragma section all restore
