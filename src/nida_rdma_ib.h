#ifndef NIDA_RDMA_IB_H
#define NIDA_RDMA_IB_H

#include <rdma/ib_verbs.h>
#include <rdma/rdma_cm.h>


const char *__attribute_const__ nida_ib_event_msg(enum ib_event_type event);



struct ib_pd *__nida_ib_alloc_pd(struct ib_device *device, unsigned int flags,
		const char *caller);

/**
 * ib_alloc_pd - Allocates an unused protection domain.
 * @device: The device on which to allocate the protection domain.
 * @flags: protection domain flags
 *
 * A protection domain object provides an association between QPs, shared
 * receive queues, address handles, memory regions, and memory windows.
 *
 * Every PD has a local_dma_lkey which can be used as the lkey value for local
 * memory operations.
 */
#define nida_ib_alloc_pd(device, flags) \
	__nida_ib_alloc_pd((device), (flags), KBUILD_MODNAME)

/**
 * ib_dealloc_pd - Deallocate kernel PD
 * @pd: The protection domain
 *
 * NOTE: for user PD use ib_dealloc_pd_user with valid udata!
 */
static inline void nida_ib_dealloc_pd(struct ib_pd *pd)
{
	int ret = ib_dealloc_pd_user(pd, NULL);

	WARN_ONCE(ret, "Destroy of kernel PD shouldn't fail");
}

/**
 * ib_dma_map_single - Map a kernel virtual address to DMA address
 * @dev: The device for which the dma_addr is to be created
 * @cpu_addr: The kernel virtual address
 * @size: The size of the region in bytes
 * @direction: The direction of the DMA
 */
static inline u64 nida_ib_dma_map_single(struct ib_device *dev,
				    void *cpu_addr, size_t size,
				    enum dma_data_direction direction)
{
	if (ib_uses_virt_dma(dev))
		return (uintptr_t)cpu_addr;
	return dma_map_single(dev->dma_device, cpu_addr, size, direction);
}


/**
 * ib_dma_mapping_error - check a DMA addr for error
 * @dev: The device for which the dma_addr was created
 * @dma_addr: The DMA address to check
 */
static inline int nida_ib_dma_mapping_error(struct ib_device *dev, u64 dma_addr)
{
	if (ib_uses_virt_dma(dev))
		return 0;
	return dma_mapping_error(dev->dma_device, dma_addr);
}

/**
 * ib_dma_unmap_single - Destroy a mapping created by ib_dma_map_single()
 * @dev: The device for which the DMA address was created
 * @addr: The DMA address
 * @size: The size of the region in bytes
 * @direction: The direction of the DMA
 */
static inline void nida_ib_dma_unmap_single(struct ib_device *dev,
				       u64 addr, size_t size,
				       enum dma_data_direction direction)
{
	if (!ib_uses_virt_dma(dev))
		dma_unmap_single(dev->dma_device, addr, size, direction);
}
#endif
