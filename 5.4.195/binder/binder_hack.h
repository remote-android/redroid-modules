#ifndef REDROID_HACK_H
#define REDROID_HACK_H

#include <linux/version.h>

#if LINUX_VERSION_CODE < KERNEL_VERSION(4, 16, 0)
#ifdef __CHECK_POLL
typedef unsigned __bitwise __poll_t;
#else
typedef unsigned __poll_t;
#endif
#endif

#if LINUX_VERSION_CODE < KERNEL_VERSION(4, 17, 0)
typedef int vm_fault_t;
#endif

#ifndef CONFIG_ANDROID_BINDER_DEVICES
#define CONFIG_ANDROID_BINDER_DEVICES "binder,hwbinder,vndbinder"
#endif

extern void __exit binderfs_exit(void);
extern void binder_alloc_shrinker_exit(void);
extern struct ipc_namespace* get_init_ipc_ns_ptr(void);

#if LINUX_VERSION_CODE < KERNEL_VERSION(4, 16, 0)
#define DEFINE_SHOW_ATTRIBUTE(__name)                                   \
	static int __name ## _open(struct inode *inode, struct file *file)      \
{                                                                       \
	return single_open(file, __name ## _show, inode->i_private);    \
}                                                                       \
\
static const struct file_operations __name ## _fops = {                 \
	.owner          = THIS_MODULE,                                  \
	.open           = __name ## _open,                              \
	.read           = seq_read,                                     \
	.llseek         = seq_lseek,                                    \
	.release        = single_release,                               \
}
#endif
#endif // REDROID_HACK_H
