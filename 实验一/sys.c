SYSCALL_DEFINE1(myuname, struct new_utsname __user *, name)
{
	struct new_utsname tmp;

	down_read(&uts_sem);
	memcpy(&tmp, utsname(), sizeof(tmp));
	up_read(&uts_sem);
	if (copy_to_user(name, &tmp, sizeof(tmp)))
		return -EFAULT;
    
    printk("Sysname:%s\nRelease:%s\nVersion:%s\n",tmp.sysname,tmp.release,tmp.version);
	return 0;
}

SYSCALL_DEFINE4(mysetnice, pid_t, pid, int, newnice, int __user *, prio, int __user *, nice)
{

    struct task_struct *task;

    task = pid_task(find_get_pid(pid), PIDTYPE_PID);

    int n = task_nice(task),
        p = task_prio(task);

    printk("before: the nice is %d and prio is %d\n", n, p);

    set_user_nice(task, newnice);

    n = task_nice(task);
    p = task_prio(task);

    printk("after: the nice is %d and prio is %d\n", n, p);

    if (copy_to_user(nice, &n, sizeof(n)) || copy_to_user(prio, &p, sizeof(p)))
        return -EFAULT;

    return 0;
}

SYSCALL_DEFINE2(mysethostname, char __user *, name, int, len)
{
	int errno;
	char tmp[__NEW_UTS_LEN];

	if (!ns_capable(current->nsproxy->uts_ns->user_ns, CAP_SYS_ADMIN))
		return -EPERM;

	if (len < 0 || len > __NEW_UTS_LEN)
		return -EINVAL;
	errno = -EFAULT;
	if (!copy_from_user(tmp, name, len)) {
		struct new_utsname *u;

		down_write(&uts_sem);
		u = utsname();
        printk("The hostname changed from %s\n",u->nodename);
		memcpy(u->nodename, tmp, len);
		memset(u->nodename + len, 0, sizeof(u->nodename) - len);
		errno = 0;
		uts_proc_notify(UTS_PROC_HOSTNAME);
		up_write(&uts_sem);
        printk("The hostname changed to %s\n",tmp);
	}
	return errno;
}