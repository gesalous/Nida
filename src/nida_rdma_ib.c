#include "nida_rdma_ib.h"

const char *__attribute_const__ nida_ib_event_msg(enum ib_event_type event) {
  return NULL;
}

struct ib_pd *__nida_ib_alloc_pd(struct ib_device *device, unsigned int flags,
                                 const char *caller) {
  return NULL;
}

