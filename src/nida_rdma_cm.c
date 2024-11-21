#include "nida_rdma_cm.h"

int nida_rdma_resolve_addr(struct rdma_cm_id *id, struct sockaddr *src_addr,
                           const struct sockaddr *dst_addr,
                           unsigned long timeout_ms) {
  return 0;
}

int nida_rdma_resolve_route(struct rdma_cm_id *id, unsigned long timeout_ms) {
  return 0;
}

void nida_rdma_set_service_type(struct rdma_cm_id *id, int tos) {}

int nida_rdma_connect_locked(struct rdma_cm_id *id,
                             struct rdma_conn_param *conn_param) {
  return 0;
}

const char *__attribute_const__
nida_rdma_event_msg(enum rdma_cm_event_type event) {
  return "lala";
}

int nida_rdma_create_qp(struct rdma_cm_id *id, struct ib_pd *pd,
                        struct ib_qp_init_attr *qp_init_attr) {
  return 0;
}

void nida_rdma_destroy_qp(struct rdma_cm_id *id) {}

void nida_rdma_destroy_id(struct rdma_cm_id *id){}

int nida_rdma_disconnect(struct rdma_cm_id *id) { return 0; }
