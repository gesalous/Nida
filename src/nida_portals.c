#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/nvme.h>
#include "nvme.h"
#include "fabrics.h"

/* Module information */
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Giorgis Saloustros (Anogeia)");
MODULE_DESCRIPTION("NVMeOF Portals Transport Driver");

#define NVME_PORTALS_TRANSPORT_NAME "nida-portals"

/* Create controller function */
static struct nvme_ctrl *nvme_portals_create_ctrl(struct device *dev,
                                                 struct nvmf_ctrl_options *opts)
{
    pr_debug("nvme-portals: create_ctrl called\n");
    return ERR_PTR(-EINVAL); // Dummy implementation
}

/* Transport operations structure */
static struct nvmf_transport_ops nvme_portals_transport_ops = {
    .name           = NVME_PORTALS_TRANSPORT_NAME,
    .module         = THIS_MODULE,
    .required_opts  = NVMF_OPT_TRANSPORT,
    .allowed_opts   = NVMF_OPT_TRANSPORT | NVMF_OPT_NQN | 
                     NVMF_OPT_TRADDR | NVMF_OPT_TRSVCID,
    .create_ctrl    = nvme_portals_create_ctrl,
};

/* Module initialization and cleanup */
static int __init nvme_portals_init(void)
{
    pr_info("NVMeOF Portals transport driver initialized\n");
    return nvmf_register_transport(&nvme_portals_transport_ops);
}

static void __exit nvme_portals_exit(void)
{
    nvmf_unregister_transport(&nvme_portals_transport_ops);
    pr_info("NVMeOF Portals transport driver unloaded\n");
}

module_init(nvme_portals_init);
module_exit(nvme_portals_exit);

