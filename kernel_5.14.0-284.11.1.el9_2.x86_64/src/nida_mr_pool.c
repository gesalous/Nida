#include "nida_mr_pool.h"
#include <rdma/mr_pool.h>

struct ib_mr *nida_ib_mr_pool_get(struct ib_qp *qp, struct list_head *list){
  pr_info("%s:%s:%d\n", __FILE__, __func__, __LINE__);
  return ib_mr_pool_get(qp,list);

}

void nida_ib_mr_pool_put(struct ib_qp *qp, struct list_head *list,
                         struct ib_mr *mr) {
  pr_info("%s:%s:%d\n", __FILE__, __func__, __LINE__);
  ib_mr_pool_put(qp, list, mr);
}

int nida_ib_mr_pool_init(struct ib_qp *qp, struct list_head *list, int nr,
                         enum ib_mr_type type, u32 max_num_sg,
                         u32 max_num_meta_sg) {

  pr_info("%s:%s:%d\n", __FILE__, __func__, __LINE__);
  return ib_mr_pool_init(qp, list, nr, type, max_num_sg, max_num_meta_sg);
}

void nida_ib_mr_pool_destroy(struct ib_qp *qp, struct list_head *list) {
  pr_info("%s:%s:%d\n", __FILE__, __func__, __LINE__);
  ib_mr_pool_destroy(qp, list);
}
