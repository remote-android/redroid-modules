#include <linux/fdtable.h>

#include <linux/version.h>
#if LINUX_VERSION_CODE < KERNEL_VERSION(4, 20, 1)
extern int __close_fd_get_file(unsigned int fd, struct file **res);
#endif

