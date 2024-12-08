#ifndef NIDA_RDMA_CM_H
#define NIDA_RDMA_CM_H
#include <rdma/ib_verbs.h>
#include <rdma/rdma_cm.h>


/**
 * rdma_resolve_addr - Resolve destination and optional source addresses
 *   from IP addresses to an RDMA address.  If successful, the specified
 *   rdma_cm_id will be bound to a local device.
 *
 * @id: RDMA identifier.
 * @src_addr: Source address information.  This parameter may be NULL.
 * @dst_addr: Destination address information.
 * @timeout_ms: Time to wait for resolution to complete.
 */
int nida_rdma_resolve_addr(struct rdma_cm_id *id, struct sockaddr *src_addr,
		      const struct sockaddr *dst_addr,
		      unsigned long timeout_ms);



/**
 * rdma_resolve_route - Resolve the RDMA address bound to the RDMA identifier
 *   into route information needed to establish a connection.
 *
 * This is called on the client side of a connection.
 * Users must have first called rdma_resolve_addr to resolve a dst_addr
 * into an RDMA address before calling this routine.
 */
int nida_rdma_resolve_route(struct rdma_cm_id *id, unsigned long timeout_ms);


/**
 * rdma_set_service_type - Set the type of service associated with a
 *   connection identifier.
 * @id: Communication identifier to associated with service type.
 * @tos: Type of service.
 *
 * The type of service is interpretted as a differentiated service
 * field (RFC 2474).  The service type should be specified before
 * performing route resolution, as existing communication on the
 * connection identifier may be unaffected.  The type of service
 * requested may not be supported by the network to all destinations.
 */
void nida_rdma_set_service_type(struct rdma_cm_id *id, int tos);


int nida_rdma_connect_locked(struct rdma_cm_id *id,
			struct rdma_conn_param *conn_param);


const char *__attribute_const__ nida_rdma_event_msg(enum rdma_cm_event_type event);

/**
 * rdma_create_id - Create an RDMA identifier.
 *
 * @net: The network namespace in which to create the new id.
 * @event_handler: User callback invoked to report events associated with the
 *   returned rdma_id.
 * @context: User specified context associated with the id.
 * @ps: RDMA port space.
 * @qp_type: type of queue pair associated with the id.
 *
 * Returns a new rdma_cm_id. The id holds a reference on the network
 * namespace until it is destroyed.
 *
 * The event handler callback serializes on the id's mutex and is
 * allowed to sleep.
 */
#define nida_rdma_create_id(net, event_handler, context, ps, qp_type)               \
	__rdma_create_kernel_id(net, event_handler, context, ps, qp_type,      \
				KBUILD_MODNAME)

/**
 * nida_rdma_create_qp - Allocate a QP and associate it with the specified RDMA
 * identifier.
 *
 * QPs allocated to an rdma_cm_id will automatically be transitioned by the CMA
 * through their states.
 */
int nida_rdma_create_qp(struct rdma_cm_id *id, struct ib_pd *pd,
		   struct ib_qp_init_attr *qp_init_attr);


/**
 * rdma_destroy_qp - Deallocate the QP associated with the specified RDMA
 * identifier.
 *
 * Users must destroy any QP associated with an RDMA identifier before
 * destroying the RDMA ID.
 */
void nida_rdma_destroy_qp(struct rdma_cm_id *id);


/**
  * rdma_destroy_id - Destroys an RDMA identifier.
  *
  * @id: RDMA identifier.
  *
  * Note: calling this function has the effect of canceling in-flight
  * asynchronous operations associated with the id.
  */
void nida_rdma_destroy_id(struct rdma_cm_id *id);

/**
 * rdma_disconnect - This function disconnects the associated QP and
 *   transitions it into the error state.
 */
int nida_rdma_disconnect(struct rdma_cm_id *id);

#endif


