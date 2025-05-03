/* 
 * Group Name: CyberGuardians
 * Group Members: Hector Agwara, Michael Echebi, Ethan Ragno
 * Course: SCIA 360 - Operating System Security
 * Project: Linux Kernel Module for Real-Time Health Monitoring
 */
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include <linux/uaccess.h>
#include <linux/timer.h>
#include <linux/sysinfo.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("CyberGuardians");
MODULE_DESCRIPTION("Real-Time System Health Monitoring Module");

static struct proc_dir_entry *proc_entry;
static struct timer_list health_timer;
static int mem_threshold = 100; // MB
static char health_data[256];

module_param(mem_threshold, int, 0644);
MODULE_PARM_DESC(mem_threshold, "Memory usage threshold in MB");

static void collect_metrics(struct timer_list *t)
{
    struct sysinfo info;
    unsigned long free_mem_mb;
    si_meminfo(&info);
    free_mem_mb = (info.freeram * info.mem_unit) >> 20; // Convert to MB
    snprintf(health_data, sizeof(health_data), 
             "[CyberGuardians] Free Memory: %lu MB\n", free_mem_mb);
    if (free_mem_mb < mem_threshold) {
        printk(KERN_WARNING "[CyberGuardians] SCIA 360: Alert: Free memory (%lu MB) below threshold (%d MB). Team Members: Hector Agwara, Michael Echebi, Ethan Ragno\n",
               free_mem_mb, mem_threshold);
    }
    mod_timer(&health_timer, jiffies + msecs_to_jiffies(5000));
}

static ssize_t proc_read(struct file *file, char __user *buf, size_t count, loff_t *ppos)
{
    int len = strlen(health_data);
    if (*ppos > 0 || count < len)
        return 0;
    if (copy_to_user(buf, health_data, len))
        return -EFAULT;
    *ppos += len;
    return len;
}

static const struct proc_ops proc_file_ops = {
    .proc_read = proc_read,
};

static int __init sys_health_init(void)
{
    printk(KERN_INFO "[CyberGuardians] SCIA 360: Module loaded successfully. Team Members: Hector Agwara, Michael Echebi, Ethan Ragno\n");
    proc_entry = proc_create("sys_health", 0444, NULL, &proc_file_ops);
    if (!proc_entry) {
        printk(KERN_ERR "[CyberGuardians] SCIA 360: Failed to create /proc/sys_health\n");
        return -ENOMEM;
    }
    timer_setup(&health_timer, collect_metrics, 0);
    mod_timer(&health_timer, jiffies + msecs_to_jiffies(5000));
    return 0;
}

static void __exit sys_health_exit(void)
{
    del_timer(&health_timer);
    if (proc_entry)
        proc_remove(proc_entry);
    printk(KERN_INFO "[CyberGuardians] SCIA 360: Module unloaded successfully. Team Members: Hector Agwara, Michael Echebi, Ethano ragno\n");
}

module_init(sys_health_init);
module_exit(sys_health_exit);
