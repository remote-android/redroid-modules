#include <linux/sched.h>
#include <linux/file.h>
#include <linux/fdtable.h>
#include <linux/atomic.h>
#include <linux/mm.h>
#include <linux/slab.h>
#include <linux/spinlock.h>
#include <linux/kallsyms.h>
#include <linux/version.h>
#include <linux/ipc_namespace.h>
#include <linux/task_work.h>

typedef void (*zap_page_range_ptr_t)(struct vm_area_struct *, unsigned long, unsigned long);
static zap_page_range_ptr_t zap_page_range_ptr = NULL;
void zap_page_range(struct vm_area_struct *vma, unsigned long address, unsigned long size)
{
	if (!zap_page_range_ptr)
		zap_page_range_ptr = (zap_page_range_ptr_t) kallsyms_lookup_name("zap_page_range");
	zap_page_range_ptr(vma, address, size);
}

typedef int (*can_nice_ptr_t)(const struct task_struct *, const int);
static can_nice_ptr_t can_nice_ptr = NULL;
int can_nice(const struct task_struct *p, const int nice)
{
	if (!can_nice_ptr)
		can_nice_ptr = (can_nice_ptr_t) kallsyms_lookup_name("can_nice");
	return can_nice_ptr(p, nice);
}

typedef int (*security_binder_set_context_mgr_ptr_t)(struct task_struct *mgr);
security_binder_set_context_mgr_ptr_t security_binder_set_context_mgr_ptr = NULL;
int security_binder_set_context_mgr(struct task_struct *mgr)
{
	if (!security_binder_set_context_mgr_ptr)
		security_binder_set_context_mgr_ptr = (security_binder_set_context_mgr_ptr_t) kallsyms_lookup_name("security_binder_set_context_mgr");
	return security_binder_set_context_mgr_ptr(mgr);
}

typedef int (*security_binder_transaction_ptr_t)(struct task_struct *from, struct task_struct *to);
static security_binder_transaction_ptr_t security_binder_transaction_ptr = NULL;
int security_binder_transaction(struct task_struct *from, struct task_struct *to)
{
	if (!security_binder_transaction_ptr)
		security_binder_transaction_ptr = (security_binder_transaction_ptr_t) kallsyms_lookup_name("security_binder_transaction");
	return security_binder_transaction_ptr(from, to);
}

typedef int (*security_binder_transfer_binder_ptr_t)(struct task_struct *from, struct task_struct *to);
static security_binder_transfer_binder_ptr_t security_binder_transfer_binder_ptr = NULL;
int security_binder_transfer_binder(struct task_struct *from, struct task_struct *to)
{
	if (!security_binder_transfer_binder_ptr)
		security_binder_transfer_binder_ptr = (security_binder_transfer_binder_ptr_t) kallsyms_lookup_name("security_binder_transfer_binder");
	return security_binder_transfer_binder_ptr(from, to);
}

typedef int (*security_binder_transfer_file_ptr_t)(struct task_struct *from, struct task_struct *to, struct file *file);
static security_binder_transfer_file_ptr_t security_binder_transfer_file_ptr = NULL;
int security_binder_transfer_file(struct task_struct *from, struct task_struct *to, struct file *file)
{
	if (!security_binder_transfer_file_ptr)
		security_binder_transfer_file_ptr = (security_binder_transfer_file_ptr_t) kallsyms_lookup_name("security_binder_transfer_file");
	return security_binder_transfer_file_ptr(from, to, file);
}

typedef void (*put_ipc_ns_ptr_t)(struct ipc_namespace *ns);
static put_ipc_ns_ptr_t put_ipc_ns_ptr = NULL;
void put_ipc_ns(struct ipc_namespace *ns)
{
    if (!put_ipc_ns_ptr)
        put_ipc_ns_ptr = (put_ipc_ns_ptr_t) kallsyms_lookup_name("put_ipc_ns");
    put_ipc_ns_ptr(ns);
}

// struct ipc_namespace init_ipc_ns;
typedef struct ipc_namespace *init_ipc_ns_ptr_t;
static init_ipc_ns_ptr_t init_ipc_ns_ptr = NULL;
init_ipc_ns_ptr_t get_init_ipc_ns_ptr(void)
{
    if (!init_ipc_ns_ptr) init_ipc_ns_ptr = (init_ipc_ns_ptr_t) kallsyms_lookup_name("init_ipc_ns");
    return init_ipc_ns_ptr;
}

typedef int (*task_work_add_ptr_t)(struct task_struct *task, struct callback_head *twork, bool notify);
static task_work_add_ptr_t task_work_add_ptr = NULL;
int task_work_add(struct task_struct *task, struct callback_head *twork, bool notify)
{
    if (!task_work_add_ptr)
        task_work_add_ptr = (task_work_add_ptr_t) kallsyms_lookup_name("task_work_add");
    return task_work_add_ptr(task, twork, notify);
}

typedef void (*mmput_async_ptr_t)(struct mm_struct *);
static mmput_async_ptr_t mmput_async_ptr = NULL;
void mmput_async(struct mm_struct *mm)
{
    if (!mmput_async_ptr)
       mmput_async_ptr = (mmput_async_ptr_t) kallsyms_lookup_name("mmput_async");
    mmput_async_ptr(mm);
}

#if LINUX_VERSION_CODE < KERNEL_VERSION(4, 20, 1)
static inline void __clear_open_fd(unsigned int fd, struct fdtable *fdt)
{
	__clear_bit(fd, fdt->open_fds);
	__clear_bit(fd / BITS_PER_LONG, fdt->full_fds_bits);
}

static void __put_unused_fd(struct files_struct *files, unsigned int fd)
{
	struct fdtable *fdt = files_fdtable(files);
	__clear_open_fd(fd, fdt);
	if (fd < files->next_fd)
		files->next_fd = fd;
}

int __close_fd_get_file(unsigned int fd, struct file **res)
{
	struct files_struct *files = current->files;
	struct file *file;
	struct fdtable *fdt;

	spin_lock(&files->file_lock);
	fdt = files_fdtable(files);
	if (fd >= fdt->max_fds)
		goto out_unlock;
	file = fdt->fd[fd];
	if (!file)
		goto out_unlock;
	rcu_assign_pointer(fdt->fd[fd], NULL);
	__put_unused_fd(files, fd);
	spin_unlock(&files->file_lock);
	get_file(file);
	*res = file;
	return filp_close(file, files);

out_unlock:
	spin_unlock(&files->file_lock);
	*res = NULL;
	return -ENOENT;
}
#else
typedef int (*__close_fd_get_file_ptr_t)(unsigned int fd, struct file **res);
static __close_fd_get_file_ptr_t __close_fd_get_file_ptr = NULL;
int __close_fd_get_file(unsigned int fd, struct file **res)
{
    if (!__close_fd_get_file_ptr)
        __close_fd_get_file_ptr = (__close_fd_get_file_ptr_t) kallsyms_lookup_name("__close_fd_get_file");

    return __close_fd_get_file_ptr(fd, res);

}
#endif // LINUX_VERSION_CODE < KERNEL_VERSION(4, 20, 1)

