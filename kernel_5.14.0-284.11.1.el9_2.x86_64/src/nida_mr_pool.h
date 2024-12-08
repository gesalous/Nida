#ifndef NIDA_MR_POOL_H
#define NIDA_MR_POOL_H
#include <rdma/ib_verbs.h>

struct ib_mr *nida_ib_mr_pool_get(struct ib_qp *qp, struct list_head *list);
void nida_ib_mr_pool_put(struct ib_qp *qp, struct list_head *list, struct ib_mr *mr);

int nida_ib_mr_pool_init(struct ib_qp *qp, struct list_head *list, int nr,
		enum ib_mr_type type, u32 max_num_sg, u32 max_num_meta_sg);
void nida_ib_mr_pool_destroy(struct ib_qp *qp, struct list_head *list);

#endif
