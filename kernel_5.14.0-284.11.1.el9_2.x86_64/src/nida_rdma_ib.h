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
  pr_info("[%s:%s:%d] IB API\n",__FILE__,__func__,__LINE__);
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
  pr_info("[%s:%s:%d] IB API virtual kernel addr: %lu size: %lu\n", __FILE__, __func__,
          __LINE__, (unsigned long)cpu_addr, size);
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
  // pr_info("%s:%s:%d check for error for physical dma_addr: %llu\n", __FILE__,
  //         __func__, __LINE__, dma_addr);
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
  pr_info("[%s:%s:%d] IB API\n",__FILE__,__func__,__LINE__);
	if (!ib_uses_virt_dma(dev))
		dma_unmap_single(dev->dma_device, addr, size, direction);
}

static inline int nida_ib_post_send(struct ib_qp *qp,
                                    const struct ib_send_wr *send_wr,
                                    const struct ib_send_wr **bad_send_wr) {
  pr_info("[%s:%s:%d] IB API\n",__FILE__,__func__,__LINE__);
  return ib_post_send(qp, send_wr, bad_send_wr);
}

static inline int nida_ib_post_recv(struct ib_qp *qp,
                                    const struct ib_recv_wr *recv_wr,
                                    const struct ib_recv_wr **bad_recv_wr) {
  pr_info("[%s:%s:%d] IB API\n",__FILE__,__func__,__LINE__);
  return ib_post_recv(qp, recv_wr, bad_recv_wr);
}

void nida_ib_free_cq(struct ib_cq *cq);


void nida_ib_cq_pool_put(struct ib_cq *cq, unsigned int nr_cqe);


/**
 * ib_destroy_qp - Destroys the specified kernel QP.
 * @qp: The QP to destroy.
 *
 * NOTE: for user qp use ib_destroy_qp_user with valid udata!
 */
static inline int nida_ib_destroy_qp(struct ib_qp *qp)
{
	return ib_destroy_qp_user(qp, NULL);
}

static inline struct ib_cq *nida_ib_alloc_cq(struct ib_device *dev,
                                             void *private, int nr_cqe,
                                             int comp_vector,
                                             enum ib_poll_context poll_ctx) {
  pr_info("[%s:%s:%d] IB API\n", __FILE__, __func__, __LINE__);
  return ib_alloc_cq(dev, private, nr_cqe, comp_vector, poll_ctx);
}


struct ib_cq *nida_ib_cq_pool_get(struct ib_device *dev, unsigned int nr_cqe,
			     int comp_vector_hint,
			     enum ib_poll_context poll_ctx);


void nida_ib_drain_qp(struct ib_qp *qp);


/**
 * ib_dma_map_sg - Map a scatter/gather list to DMA addresses
 * @dev: The device for which the DMA addresses are to be created
 * @sg: The array of scatter/gather entries
 * @nents: The number of scatter/gather entries
 * @direction: The direction of the DMA
 */
static inline int nida_ib_dma_map_sg(struct ib_device *dev,
                                     struct scatterlist *sg, int nents,
                                     enum dma_data_direction direction) {

  pr_info("[%s:%s:%d] IB API\n", __FILE__, __func__, __LINE__);
  return ib_dma_map_sg(dev, sg, nents, direction);
}

static inline void nida_ib_dma_unmap_sg(struct ib_device *dev,
				   struct scatterlist *sg, int nents,
				   enum dma_data_direction direction)
{
  pr_info("[%s:%s:%d] IB API\n", __FILE__, __func__, __LINE__);
	ib_dma_unmap_sg_attrs(dev, sg, nents, direction, 0);
}



/**
 * ib_dma_sync_single_for_cpu - Prepare DMA region to be accessed by CPU
 * @dev: The device for which the DMA address was created
 * @addr: The DMA address
 * @size: The size of the region in bytes
 * @dir: The direction of the DMA
 */
static inline void
nida_ib_dma_sync_single_for_cpu(struct ib_device *dev, u64 addr, size_t size,
                                enum dma_data_direction dir) {
  pr_info("[%s:%s:%d] IB API\n", __FILE__, __func__, __LINE__);
  ib_dma_sync_single_for_cpu(dev, addr, size, dir);
}


/**
 * ib_dma_sync_single_for_device - Prepare DMA region to be accessed by device
 * @dev: The device for which the DMA address was created
 * @addr: The DMA address
 * @size: The size of the region in bytes
 * @dir: The direction of the DMA
 */
static inline void nida_ib_dma_sync_single_for_device(struct ib_device *dev,
						 u64 addr,
						 size_t size,
						 enum dma_data_direction dir)
{
  pr_info("[%s:%s:%d] IB API\n", __FILE__, __func__, __LINE__);
  ib_dma_sync_single_for_device(dev,addr,size,dir);
}


/**
 * ibdev_to_node - return the NUMA node for a given ib_device
 * @dev:	device to get the NUMA node for.
 */
static inline int nida_ibdev_to_node(struct ib_device *ibdev)
{
  pr_info("[%s:%s:%d] IB API\n", __FILE__, __func__, __LINE__);
	struct device *parent = ibdev->dev.parent;

	if (!parent)
		return NUMA_NO_NODE;
	return dev_to_node(parent);
}

#endif
