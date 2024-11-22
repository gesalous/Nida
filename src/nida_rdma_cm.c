#include "nida_rdma_cm.h"

int nida_rdma_resolve_addr(struct rdma_cm_id *id, struct sockaddr *src_addr,
                           const struct sockaddr *dst_addr,
                           unsigned long timeout_ms) {
  pr_info("%s:%s:%d\n", __FILE__, __func__, __LINE__);
  return rdma_resolve_addr(id, src_addr, dst_addr, timeout_ms);
}

int nida_rdma_resolve_route(struct rdma_cm_id *id, unsigned long timeout_ms) {
  pr_info("%s:%s:%d\n", __FILE__, __func__, __LINE__);
  rdma_resolve_route(id, timeout_ms);
  return 0;
}

void nida_rdma_set_service_type(struct rdma_cm_id *id, int tos) {
  pr_info("%s:%s:%d\n", __FILE__, __func__, __LINE__);
  return rdma_set_service_type(id, tos);
}

int nida_rdma_connect_locked(struct rdma_cm_id *id,
                             struct rdma_conn_param *conn_param) {
  pr_info("%s:%s:%d\n", __FILE__, __func__, __LINE__);
  return rdma_connect_locked(id, conn_param);
}

const char *__attribute_const__
nida_rdma_event_msg(enum rdma_cm_event_type event) {
  pr_info("%s:%s:%d\n", __FILE__, __func__, __LINE__);
  return rdma_event_msg(event);
}

int nida_rdma_create_qp(struct rdma_cm_id *id, struct ib_pd *pd,
                        struct ib_qp_init_attr *qp_init_attr) {
  pr_info("%s:%s:%d\n", __FILE__, __func__, __LINE__);
  return rdma_create_qp(id, pd, qp_init_attr);
}

void nida_rdma_destroy_qp(struct rdma_cm_id *id) {
  pr_info("%s:%s:%d\n", __FILE__, __func__, __LINE__);
  return rdma_destroy_qp(id);
}

void nida_rdma_destroy_id(struct rdma_cm_id *id) {
  pr_info("%s:%s:%d\n", __FILE__, __func__, __LINE__);
  return rdma_destroy_id(id);
}

int nida_rdma_disconnect(struct rdma_cm_id *id) {
  pr_info("%s:%s:%d\n", __FILE__, __func__, __LINE__);
  return rdma_disconnect(id);
}
