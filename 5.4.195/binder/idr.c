#include <linux/version.h>
#if LINUX_VERSION_CODE < KERNEL_VERSION(4, 19, 0)
#include "linux/idr.h"
#include <linux/spinlock.h>
#include <linux/xarray.h>
/**
 * ida_alloc_range() - Allocate an unused ID.
 * @ida: IDA handle.
 * @min: Lowest ID to allocate.
 * @max: Highest ID to allocate.
 * @gfp: Memory allocation flags.
 *
 * Allocate an ID between @min and @max, inclusive.  The allocated ID will
 * not exceed %INT_MAX, even if @max is larger.
 *
 * Context: Any context.
 * Return: The allocated ID, or %-ENOMEM if memory could not be allocated,
 * or %-ENOSPC if there are no free IDs.
 */
int ida_alloc_range(struct ida *ida, unsigned int min, unsigned int max,
			gfp_t gfp)
{
	int id = 0;
	unsigned long flags;

	if ((int)min < 0)
		return -ENOSPC;

	if ((int)max < 0)
		max = INT_MAX;

again:
	xa_lock_irqsave(&ida->ida_rt, flags);
	ida_get_new_above(ida, min, &id);
	if (id > (int)max) {
		ida_remove(ida, id);
		id = -ENOSPC;
	}
	xa_unlock_irqrestore(&ida->ida_rt, flags);

	if (unlikely(id == -EAGAIN)) {
		if (!ida_pre_get(ida, gfp))
			return -ENOMEM;
		goto again;
	}

	return id;
}

/**
 * ida_free() - Release an allocated ID.
 * @ida: IDA handle.
 * @id: Previously allocated ID.
 *
 * Context: Any context.
 */
void ida_free(struct ida *ida, unsigned int id)
{
	unsigned long flags;

	BUG_ON((int)id < 0);
	xa_lock_irqsave(&ida->ida_rt, flags);
	ida_remove(ida, id);
	xa_unlock_irqrestore(&ida->ida_rt, flags);
}
#endif
