/*
 * Copyright (C) Bull S.A.S - 2023
 *
 * This program is free software; you can redistribute it and/or modify it under the terms of the
 * GNU General Public License as published by the Free Software Foundation; either version 2 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without
 * even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with this program; if
 * not, write to the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301, USA.
 *
 * BXI Low Level Team <dl-bxi-sw-ll@atos.net>
 *
 */
#ifndef PORTALS4_H
#define PORTALS4_H

#ifndef __KERNEL__
#include <stdint.h>
#else
#include <linux/types.h>
#endif

/*
 * API version
 */
#define PTL_MAJOR_VERSION 4
#define PTL_MINOR_VERSION 3

/*
 * Scalar types
 */
typedef void *ptl_addr_t;
typedef uint64_t ptl_hdr_data_t;
typedef uint8_t ptl_interface_t;
typedef uint64_t ptl_match_bits_t;
typedef uint32_t ptl_nid_t;
typedef uint16_t ptl_pid_t;
typedef uint32_t ptl_pt_index_t;
typedef uint32_t ptl_pt_index_t;
typedef uint32_t ptl_rank_t;
typedef uint64_t ptl_size_t;
typedef uint32_t ptl_sr_value_t;
typedef uint64_t ptl_time_t;
typedef uint32_t ptl_uid_t;

/*
 * Return codes
 *
 * Values over 0x100 are software only.
 *
 * HAS: Table 59, section 4.4.3.4
 */
#define PTL_OK 0x0
#define PTL_NO_INIT 0x1
#define PTL_ARG_INVALID 0x2
#define PTL_NO_SPACE 0x3
#define PTL_IN_USE 0x4
#define PTL_LIST_TOO_LONG 0x5

#define PTL_ABORTED 0x100
#define PTL_CT_NONE_REACHED 0x101
#define PTL_EQ_DROPPED 0x102
#define PTL_EQ_EMPTY 0x103
#define PTL_FAIL 0x104
#define PTL_IGNORED 0x105
#define PTL_PID_IN_USE 0x106
#define PTL_PT_EQ_NEEDED 0x107
#define PTL_PT_FULL 0x108
#define PTL_PT_IN_USE 0x109

/*
 * NI Options
 *
 * HAS: Table 2, section 4.1
 */
#define PTL_NI_LOGICAL (0 << 0)
#define PTL_NI_MATCHING (1 << 1)
#define PTL_NI_NO_MATCHING (0 << 1)
#define PTL_NI_PHYSICAL (1 << 0)

/*
 * NI Features
 *
 * These values are defined SW only
 */
#define PTL_COHERENT_ATOMICS (1 << 0)
#define PTL_TARGET_BIND_INACCESSIBLE (1 << 1)
#define PTL_TOTAL_DATA_ORDERING (1 << 2)

struct ptl_ni_limits {
	int max_entries;
	int max_unexpected_headers;
	int max_mds;
	int max_cts;
	int max_eqs;
	int max_pt_index;
	int max_iovecs;
	int max_list_size;
	int max_triggered_ops;
	ptl_size_t max_msg_size;
	ptl_size_t max_atomic_size;
	ptl_size_t max_fetch_atomic_size;
	ptl_size_t max_waw_ordered_size;
	ptl_size_t max_war_ordered_size;
	ptl_size_t max_volatile_size;
	unsigned int features;
	int max_cqs;
};
typedef struct ptl_ni_limits ptl_ni_limits_t;

typedef union {
	struct {
		ptl_nid_t nid;
		ptl_pid_t pid;
	} phys;
	ptl_rank_t rank;
} ptl_process_t;

/* Status registers */
typedef enum {
	PTL_SR_DROP_COUNT,
	PTL_SR_OPERATION_VIOLATIONS,
	PTL_SR_PERMISSION_VIOLATIONS,
} ptl_sr_index_t;

/*
 * Atomic operations & datatypes
 *
 * TODO: no values are defined by HW yet
 */
typedef enum {
	PTL_BAND,
	PTL_BOR,
	PTL_BXOR,
	PTL_CSWAP,
	PTL_CSWAP_GE,
	PTL_CSWAP_GT,
	PTL_CSWAP_LE,
	PTL_CSWAP_LT,
	PTL_CSWAP_NE,
	PTL_DIFF,
	PTL_LAND,
	PTL_LOR,
	PTL_LXOR,
	PTL_MAX,
	PTL_MIN,
	PTL_MSWAP,
	PTL_PROD,
	PTL_SUM,
	PTL_SWAP,
} ptl_op_t;

typedef enum {
	PTL_DOUBLE,
	PTL_DOUBLE_COMPLEX,
	PTL_FLOAT,
	PTL_FLOAT_COMPLEX,
	PTL_INT16_T,
	PTL_INT32_T,
	PTL_INT64_T,
	PTL_INT8_T,
	PTL_LONG_DOUBLE,
	PTL_LONG_DOUBLE_COMPLEX,
	PTL_UINT16_T,
	PTL_UINT32_T,
	PTL_UINT64_T,
	PTL_UINT8_T,
} ptl_datatype_t;

/*
 * Handle types
 *
 * SW handles are not the same as HW handles
 */
typedef struct {
	void *handle;
	uint64_t incarnation;
} ptl_handle_any_t;
typedef ptl_handle_any_t ptl_handle_ct_t;
typedef ptl_handle_any_t ptl_handle_eq_t;
typedef ptl_handle_any_t ptl_handle_le_t;
typedef ptl_handle_any_t ptl_handle_md_t;
typedef ptl_handle_any_t ptl_handle_me_t;
typedef ptl_handle_any_t ptl_handle_ni_t;

#define PTL_INVALID_HANDLE ((ptl_handle_any_t){ .incarnation = 0, .handle = NULL })
#define PTL_EQ_NONE ((ptl_handle_any_t){ .incarnation = 0, .handle = NULL })
#define PTL_CT_NONE ((ptl_handle_any_t){ .incarnation = 0, .handle = NULL })

/*
 * PT Options
 *
 * HAS: Table 53, section 4.4.3.2
 */

#define PTL_PT_ANY ((1 << 21) + 1)

#define PTL_PT_ONLY_USE_ONCE (1 << 0)
#define PTL_PT_ONLY_TRUNCATE (1 << 1)
#define PTL_PT_FLOWCTRL (1 << 2)
#define PTL_PT_ALLOC_DISABLED (1 << 3)

/*
 * MD Options
 *
 * HAS: Table 45, section 4.4.3.2
 */
#define PTL_MD_EVENT_SEND_DISABLE (1 << 0)
#define PTL_MD_EVENT_SUCCESS_DISABLE (1 << 1)
#define PTL_MD_EVENT_CT_SEND (1 << 2)
#define PTL_MD_EVENT_CT_REPLY (1 << 3)
#define PTL_MD_EVENT_CT_ACK (1 << 4)
#define PTL_MD_EVENT_CT_BYTES (1 << 5)
#define PTL_MD_UNORDERED (1 << 6)
#define PTL_MD_UNRELIABLE (1 << 8)
#define PTL_MD_VOLATILE (1 << 9)

typedef struct {
	ptl_addr_t start;
	ptl_size_t length;
	unsigned int options;
	ptl_handle_eq_t eq_handle;
	ptl_handle_ct_t ct_handle;
} ptl_md_t;

/*
 * ME/LE Options & Type
 *
 * HAS: Table 50, section 4.4.3.2
 */
#define PTL_ME_OP_PUT (1 << 0)
#define PTL_ME_OP_GET (1 << 1)
#define PTL_ME_MANAGE_LOCAL (1 << 2)
#define PTL_ME_LOCAL_INC_UH_RLENGTH (1 << 3)
#define PTL_ME_NO_TRUNCATE (1 << 4)
#define PTL_ME_USE_ONCE (1 << 5)
#define PTL_ME_UNEXPECTED_HDR_DISABLE (1 << 6)
#define PTL_ME_EVENT_LINK_DISABLE (1 << 8)
#define PTL_ME_EVENT_FLOWCTRL_DISABLE (1 << 9)
#define PTL_ME_EVENT_UNLINK_DISABLE (1 << 10)
#define PTL_ME_EVENT_COMM_DISABLE (1 << 11)
#define PTL_ME_EVENT_SUCCESS_DISABLE (1 << 12)
#define PTL_ME_EVENT_OVER_DISABLE (1 << 13)
#define PTL_ME_EVENT_CT_COMM (1 << 14)
#define PTL_ME_EVENT_CT_OVERFLOW (1 << 15)
#define PTL_ME_EVENT_CT_BYTES (1 << 16)
#define PTL_ME_MAY_ALIGN (1 << 17)
#define PTL_ME_IS_ACCESSIBLE (1 << 18)

#define PTL_LE_OP_PUT PTL_ME_OP_PUT
#define PTL_LE_OP_GET PTL_ME_OP_GET
#define PTL_LE_USE_ONCE PTL_ME_USE_ONCE
#define PTL_LE_UNEXPECTED_HDR_DISABLE PTL_ME_UNEXPECTED_HDR_DISABLE
#define PTL_LE_EVENT_LINK_DISABLE PTL_ME_EVENT_LINK_DISABLE
#define PTL_LE_EVENT_FLOWCTRL_DISABLE PTL_ME_EVENT_FLOWCTRL_DISABLE
#define PTL_LE_EVENT_UNLINK_DISABLE PTL_ME_EVENT_UNLINK_DISABLE
#define PTL_LE_EVENT_COMM_DISABLE PTL_ME_EVENT_COMM_DISABLE
#define PTL_LE_EVENT_SUCCESS_DISABLE PTL_ME_EVENT_SUCCESS_DISABLE
#define PTL_LE_EVENT_OVER_DISABLE PTL_ME_EVENT_OVER_DISABLE
#define PTL_LE_EVENT_CT_COMM PTL_ME_EVENT_CT_COMM
#define PTL_LE_EVENT_CT_OVERFLOW PTL_ME_EVENT_CT_OVERFLOW
#define PTL_LE_EVENT_CT_BYTES PTL_ME_EVENT_CT_BYTES
#define PTL_LE_IS_ACCESSIBLE PTL_ME_IS_ACCESSIBLE

/*
 * ME/LE structures
 *
 * HAS: Table 4, section 4.2.7
 */
typedef enum {
	PTL_PRIORITY_LIST = 0,
	PTL_OVERFLOW_LIST = 1,
} ptl_list_t;

typedef struct {
	ptl_addr_t start;
	ptl_size_t length;
	ptl_handle_ct_t ct_handle;
	ptl_uid_t uid;
	unsigned int options;
	ptl_process_t match_id;
	ptl_match_bits_t match_bits;
	ptl_match_bits_t ignore_bits;
	ptl_size_t min_free;
} ptl_me_t;
typedef ptl_me_t ptl_le_t;

typedef enum {
	PTL_SEARCH_ONLY = 0,
	PTL_SEARCH_DELETE = 1,
} ptl_search_op_t;

/*
 * Acknowledgment modes
 *
 * HAS: Table 4, section 4.2.7
 */
typedef enum {
	PTL_NO_ACK_REQ = 0,
	PTL_CT_ACK_REQ = 1,
	PTL_OC_ACK_REQ = 2,
	PTL_ACK_REQ = 3,
} ptl_ack_req_t;

/*
 * Event structures
 *
 * HAS: Table 82, section 4.5.3
 */
typedef enum {
	PTL_EVENT_GET = 0x0,
	PTL_EVENT_GET_OVERFLOW = 0x1,
	PTL_EVENT_PUT = 0x2,
	PTL_EVENT_PUT_OVERFLOW = 0x3,
	PTL_EVENT_ATOMIC = 0x4,
	PTL_EVENT_ATOMIC_OVERFLOW = 0x5,
	PTL_EVENT_FETCH_ATOMIC = 0x6,
	PTL_EVENT_FETCH_ATOMIC_OVERFLOW = 0x7,
	PTL_EVENT_REPLY = 0x8,
	PTL_EVENT_SEND = 0x9,
	PTL_EVENT_ACK = 0xA,
	PTL_EVENT_PT_DISABLED = 0xB,
	PTL_EVENT_AUTO_UNLINK = 0xC,
	PTL_EVENT_AUTO_FREE = 0xD,
	PTL_EVENT_SEARCH = 0xE,
	PTL_EVENT_LINK = 0xF,
	PTL_EVENT_ERROR = 0x10,
} ptl_event_kind_t;

/* HAS: Table 59, section 4.4.3.4 */
typedef enum {
	PTL_NI_OK = PTL_OK,
	PTL_NI_UNDELIVERABLE = 0x6,
	PTL_NI_PT_DISABLED = 0x7,
	PTL_NI_DROPPED = 0x8,
	PTL_NI_PERM_VIOLATION = 0x9,
	PTL_NI_OP_VIOLATION = 0xA,
	PTL_NI_SEGV = 0xB,
	PTL_NI_NO_MATCH = 0xC,
} ptl_ni_fail_t;

typedef struct {
	ptl_addr_t start;
	void *user_ptr;
	ptl_hdr_data_t hdr_data;
	ptl_match_bits_t match_bits;
	ptl_size_t rlength, mlength, remote_offset;
	ptl_uid_t uid;
	ptl_process_t initiator;
	ptl_event_kind_t type;
	ptl_list_t ptl_list;
	ptl_pt_index_t pt_index;
	ptl_ni_fail_t ni_fail_type;
	ptl_op_t atomic_operation;
	ptl_datatype_t atomic_type;
} ptl_event_t;

typedef struct {
	ptl_size_t success;
	ptl_size_t failure;
} ptl_ct_event_t;

/*
 * Miscellaneous constants
 *
 * HAS: Mostly Table 4, section 4.2.7
 */
#define PTL_IFACE_DEFAULT 0
/* HAS: Table 45 and Table 50, section 4.4.3.2 */
#define PTL_IOVEC (1 << 7)
#define PTL_NID_ANY 0x3fffffff
#define PTL_PID_MAX 1022
#define PTL_PID_ANY (PTL_PID_MAX + 1)
/*
 * In the portals spec sizes can be a number of entry, an offset, a buffer length, ...
 * This define is the largest size acceptable from any of the functions, but some functions
 * may not accept as much as this.
 */
#define PTL_SIZE_MAX ((1 << 57) - 1)
#define PTL_UID_ANY 0xffffffff

/*
 * Miscellaneous structures
 */
typedef struct {
	ptl_addr_t iov_base;
	ptl_size_t iov_len;
} ptl_iovec_t;

int PtlInit(void);
void PtlFini(void);
void PtlAbort(void);

int PtlNIInit(ptl_interface_t iface, unsigned int options, ptl_pid_t pid,
	      const ptl_ni_limits_t *desired, ptl_ni_limits_t *actual, ptl_handle_ni_t *ni_handle);
int PtlNIFini(ptl_handle_ni_t ni_handle);
int PtlNIHandle(ptl_handle_any_t handle, ptl_handle_ni_t *ni_handle);
int PtlNIStatus(ptl_handle_ni_t ni_handle, ptl_sr_index_t status_register, ptl_sr_value_t *status);
int PtlSetMap(ptl_handle_ni_t ni_handle, ptl_size_t map_size, const ptl_process_t *mapping);
int PtlGetMap(ptl_handle_ni_t ni_handle, ptl_size_t map_size, ptl_process_t *mapping,
	      ptl_size_t *actual_map_size);

int PtlPTAlloc(ptl_handle_ni_t ni_handle, unsigned int options, ptl_handle_eq_t eq_handle,
	       ptl_pt_index_t pt_index_req, ptl_pt_index_t *pt_index);
int PtlPTFree(ptl_handle_ni_t ni_handle, ptl_pt_index_t pt_index);
int PtlPTEnable(ptl_handle_ni_t ni_handle, ptl_pt_index_t pt_index);
int PtlPTDisable(ptl_handle_ni_t ni_handle, ptl_pt_index_t pt_index);

int PtlGetUid(ptl_handle_ni_t ni_handle, ptl_uid_t *uid);
int PtlGetId(ptl_handle_ni_t ni_handle, ptl_process_t *id);
int PtlGetPhysId(ptl_handle_ni_t ni_handle, ptl_process_t *id);

int PtlMDBind(ptl_handle_ni_t ni_handle, const ptl_md_t *md, ptl_handle_md_t *md_handle);
int PtlMDRelease(ptl_handle_md_t md_handle);

int PtlLEAppend(ptl_handle_ni_t ni_handle, ptl_pt_index_t pt_index, const ptl_le_t *le,
		ptl_list_t ptl_list, void *user_ptl, ptl_handle_le_t *le_handle);
int PtlLEUnlink(ptl_handle_le_t le_handle);
int PtlLESearch(ptl_handle_ni_t ni_handle, ptl_pt_index_t pt_index, const ptl_le_t *le,
		ptl_search_op_t ptl_search_op, void *user_ptr);
int PtlMEAppend(ptl_handle_ni_t ni_handle, ptl_pt_index_t pt_index, const ptl_me_t *me,
		ptl_list_t ptl_list, void *user_ptr, ptl_handle_me_t *me_handle);
int PtlMEUnlink(ptl_handle_me_t me_handle);
int PtlMESearch(ptl_handle_ni_t ni_handle, ptl_pt_index_t pt_index, const ptl_me_t *me,
		ptl_search_op_t ptl_search_op, void *user_ptr);

int PtlEQAlloc(ptl_handle_ni_t ni_handle, ptl_size_t count, ptl_handle_eq_t *eq_handle);
int PtlEQFree(ptl_handle_eq_t eq_handle);
int PtlEQGet(ptl_handle_eq_t eq_handle, ptl_event_t *event);
int PtlEQWait(ptl_handle_eq_t eq_handle, ptl_event_t *event);
int PtlEQPoll(const ptl_handle_eq_t *eq_handles, unsigned int size, ptl_time_t timeout,
	      ptl_event_t *event, unsigned int *which);

int PtlCTAlloc(ptl_handle_ni_t ni_handle, ptl_handle_ct_t *ct_handle);
int PtlCTFree(ptl_handle_ct_t ct_handle);
int PtlCTCancelTriggered(ptl_handle_ct_t ct_handle);
int PtlCTGet(ptl_handle_ct_t ct_handle, ptl_ct_event_t *event);
int PtlCTWait(ptl_handle_ct_t ct_handle, ptl_size_t test, ptl_ct_event_t *event);
int PtlCTPoll(const ptl_handle_ct_t *ct_handles, const ptl_size_t *tests, unsigned int size,
	      ptl_time_t timeout, ptl_ct_event_t *event, unsigned int *which);
int PtlCTSet(ptl_handle_ct_t ct_handle, ptl_ct_event_t new_ct);
int PtlCTInc(ptl_handle_ct_t ct_handle, ptl_ct_event_t increment);

int PtlPut(ptl_handle_md_t md_handle, ptl_size_t local_offset, ptl_size_t length,
	   ptl_ack_req_t ack_req, ptl_process_t target_id, ptl_pt_index_t pt_index,
	   ptl_match_bits_t match_bits, ptl_size_t remote_offset, void *user_ptr,
	   ptl_hdr_data_t hdr_data);
int PtlGet(ptl_handle_md_t md_handle, ptl_size_t local_offset, ptl_size_t length,
	   ptl_process_t target_id, ptl_pt_index_t pt_index, ptl_match_bits_t match_bits,
	   ptl_size_t remote_offset, void *user_ptr);
int PtlAtomic(ptl_handle_md_t md_handle, ptl_size_t local_offset, ptl_size_t length,
	      ptl_ack_req_t ack_req, ptl_process_t target_id, ptl_pt_index_t pt_index,
	      ptl_match_bits_t match_bits, ptl_size_t remote_offset, void *user_ptr,
	      ptl_hdr_data_t hdr_data, ptl_op_t operation, ptl_datatype_t datatype);
int PtlFetchAtomic(ptl_handle_md_t get_md_handle, ptl_size_t local_get_offset,
		   ptl_handle_md_t put_md_handle, ptl_size_t local_put_offset, ptl_size_t length,
		   ptl_process_t target_id, ptl_pt_index_t pt_index, ptl_match_bits_t match_bits,
		   ptl_size_t remote_offset, void *user_ptr, ptl_hdr_data_t hdr_data,
		   ptl_op_t operation, ptl_datatype_t datatype);
int PtlSwap(ptl_handle_md_t get_md_handle, ptl_size_t local_get_offset,
	    ptl_handle_md_t put_md_handle, ptl_size_t local_put_offset, ptl_size_t length,
	    ptl_process_t target_id, ptl_pt_index_t pt_index, ptl_match_bits_t match_bits,
	    ptl_size_t remote_offset, void *user_ptr, ptl_hdr_data_t hdr_data, const void *operand,
	    ptl_op_t operation, ptl_datatype_t datatype);
int PtlAtomicSync(void);

int PtlTriggeredPut(ptl_handle_md_t md_handle, ptl_size_t local_offset, ptl_size_t length,
		    ptl_ack_req_t ack_req, ptl_process_t target_id, ptl_pt_index_t pt_index,
		    ptl_match_bits_t match_bits, ptl_size_t remote_offset, void *user_ptr,
		    ptl_hdr_data_t hdr_data, ptl_handle_ct_t trig_ct_handle, ptl_size_t threshold);
int PtlTriggeredGet(ptl_handle_md_t md_handle, ptl_size_t local_offset, ptl_size_t length,
		    ptl_process_t target_id, ptl_pt_index_t pt_index, ptl_match_bits_t match_bits,
		    ptl_size_t remote_offset, void *user_ptr, ptl_handle_ct_t ct_handle,
		    ptl_size_t threshold);
int PtlTriggeredAtomic(ptl_handle_md_t md_handle, ptl_size_t local_offset, ptl_size_t length,
		       ptl_ack_req_t ack_req, ptl_process_t target_id, ptl_pt_index_t pt_index,
		       ptl_match_bits_t match_bits, ptl_size_t remote_offset, void *user_ptr,
		       ptl_hdr_data_t hdr_data, ptl_op_t operation, ptl_datatype_t datatype,
		       ptl_handle_ct_t trig_ct_handle, ptl_size_t threshold);
int PtlTriggeredFetchAtomic(ptl_handle_md_t get_md_handle, ptl_size_t local_get_offset,
			    ptl_handle_md_t put_md_handle, ptl_size_t local_put_offset,
			    ptl_size_t length, ptl_process_t target_id, ptl_pt_index_t pt_index,
			    ptl_match_bits_t match_bits, ptl_size_t remote_offset, void *user_ptr,
			    ptl_hdr_data_t hdr_data, ptl_op_t operation, ptl_datatype_t datatype,
			    ptl_handle_ct_t trig_ct_handle, ptl_size_t threshold);
int PtlTriggeredSwap(ptl_handle_md_t get_md_handle, ptl_size_t local_get_offset,
		     ptl_handle_md_t put_md_handle, ptl_size_t local_put_offset, ptl_size_t length,
		     ptl_process_t target_id, ptl_pt_index_t pt_index, ptl_match_bits_t match_bits,
		     ptl_size_t remote_offset, void *user_ptr, ptl_hdr_data_t hdr_data,
		     const void *operand, ptl_op_t operation, ptl_datatype_t datatype,
		     ptl_handle_ct_t trig_ct_handle, ptl_size_t threshold);
int PtlTriggeredCTSet(ptl_handle_ct_t ct_handle, ptl_ct_event_t new_ct,
		      ptl_handle_ct_t trig_ct_handle, ptl_size_t threshold);
int PtlTriggeredCTInc(ptl_handle_ct_t ct_handle, ptl_ct_event_t increment,
		      ptl_handle_ct_t trig_ct_handle, ptl_size_t threshold);

int PtlStartBundle(ptl_handle_ni_t ni_handle);
int PtlEndBundle(ptl_handle_ni_t ni_handle);
int PtlHandleIsEqual(ptl_handle_any_t handle1, ptl_handle_any_t handle2);

/*
 * Deprecated Values
 */

#define PTL_INTERRUPTED 0x200

#endif /* PORTALS4_H */
