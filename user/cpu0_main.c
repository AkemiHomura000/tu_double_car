
#include "zf_common_headfile.h"
#include "IfxCpu.h"

#pragma section all "cpu0_dsram"

int core0_main(void)
{
    clock_init(); // ��ȡʱ��Ƶ��<��ر���>
    debug_init(); // ��ʼ��Ĭ�ϵ��Դ���
    motor_init(); // ��ʼ�����
    // �˴���д�û����� ���������ʼ�������
    cpu_wait_event_ready(); // �ȴ����к��ĳ�ʼ�����
    while (TRUE)
    {
        straight_follow();
        // system_delay_ms(10);
    }
}

#pragma section all restore
