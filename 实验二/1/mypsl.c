#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/sched/signal.h>

static int __init mypsl_init(void)
{
    struct task_struct *p = NULL;
    printk(KERN_ALERT "%13s %12s %13s %14s %23s\n", "进程号", "状态", "优先级", "父进程号", "程序名");
    for_each_process(p) if (!p->mm)
        printk(KERN_ALERT "%10d %10ld %10d %10d %20s\n", p->pid, p->state, p->prio, p->parent->pid, p->comm);
    return 0;
}

static void __exit mypsl_exit(void)
{
    printk(KERN_ALERT "mypsl exit\n");
}

module_init(mypsl_init);
module_exit(mypsl_exit);
MODULE_LICENSE("GPL");