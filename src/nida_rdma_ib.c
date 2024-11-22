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
