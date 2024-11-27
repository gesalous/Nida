#include "nida_rdma_ib.h"

const char *__attribute_const__ nida_ib_event_msg(enum ib_event_type event) {
  pr_info("%s:%s:%d\n", __FILE__, __func__, __LINE__);
  return ib_event_msg(event);
}

struct ib_pd *__nida_ib_alloc_pd(struct ib_device *device, unsigned int flags,
                                 const char *caller) {
  pr_info("%s:%s:%d\n", __FILE__, __func__, __LINE__);
  return __ib_alloc_pd(device, flags, caller);
}

void nida_ib_free_cq(struct ib_cq *cq) {
  pr_info("%s:%s:%d\n", __FILE__, __func__, __LINE__);
  return ib_free_cq(cq);
}

void nida_ib_cq_pool_put(struct ib_cq *cq, unsigned int nr_cqe) {
  pr_info("%s:%s:%d\n", __FILE__, __func__, __LINE__);
  return ib_cq_pool_put(cq, nr_cqe);
}

struct ib_cq *nida_ib_cq_pool_get(struct ib_device *dev, unsigned int nr_cqe,
                                  int comp_vector_hint,
                                  enum ib_poll_context poll_ctx) {
  pr_info("%s:%s:%d\n", __FILE__, __func__, __LINE__);
  return ib_cq_pool_get(dev, nr_cqe, comp_vector_hint, poll_ctx);
}

void nida_ib_drain_qp(struct ib_qp *qp) {
  pr_info("%s:%s:%d\n", __FILE__, __func__, __LINE__);
  return ib_drain_qp(qp);
}
