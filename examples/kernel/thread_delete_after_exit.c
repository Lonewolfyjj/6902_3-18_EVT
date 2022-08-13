#include <rtthread.h>

static void thread1_entry(void* parameter);
static void thread2_entry(void* parameter);

static rt_thread_t tid1 = RT_NULL, tid2 = RT_NULL;

static void thread1_entry(void* parameter)
{
    rt_kprintf("thread 1 enter\n");

	tid2 = rt_thread_create("t2",
        thread2_entry, RT_NULL,
        1024, 4, 10);
    if (tid2 != RT_NULL)
    {
        rt_thread_startup(tid2);
    }
    else {
		rt_kprintf("create thread 2 failed\n");
        return;
    }

    /* ��Ϊt2�����ȼ����ߣ�����������t2�Ѿ��˳��ˡ�
	 * ��ԭ�ȵĳ���ʵ������t1��t2ͨ��event��ͬ����t1���t2�˳���ȥɾ��t2*/
	rt_kprintf("delete thread2\n");
    rt_thread_delete(tid2);
}

static void thread2_entry(void* parameter)
{
    rt_kprintf("thread2 exit\n");
}

void thread_delete_after_exit_test()
{
    tid1 = rt_thread_create("t1",
        thread1_entry, RT_NULL,
        1024, 5, 10);
    if (tid1 != RT_NULL)
    {
        rt_thread_startup(tid1);
    }
    else {
		rt_kprintf("create thread 1 failed\n");
        return;
    }
}

#ifndef RT_USING_UTEST
#include "finsh.h"
MSH_CMD_EXPORT_ALIAS(thread_delete_after_exit_test, td_test, test thread delete)
#else
#include "utest.h"
UTEST_TC_EXPORT(thread_delete_after_exit_test, "thread_delete", NULL, NULL, 100);
#endif
