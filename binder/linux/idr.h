#include <linux/idr.h>

#include <linux/version.h>
#if LINUX_VERSION_CODE < KERNEL_VERSION(4, 19, 0)
int ida_alloc_range(struct ida *, unsigned int min, unsigned int max, gfp_t);
void ida_free(struct ida *ida, unsigned int id);

/**
 * ida_alloc_max() - Allocate an unused ID.
 * @ida: IDA handle.
 * @max: Highest ID to allocate.
 * @gfp: Memory allocation flags.
 *
 * Allocate an ID between 0 and @max, inclusive.
 *
 * Context: Any context.
 * Return: The allocated ID, or %-ENOMEM if memory could not be allocated,
 * or %-ENOSPC if there are no free IDs.
 */
static inline int ida_alloc_max(struct ida *ida, unsigned int max, gfp_t gfp)
{
	return ida_alloc_range(ida, 0, max, gfp);
}

#endif
