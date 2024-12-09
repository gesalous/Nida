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

#pragma once

#ifndef __KERNEL__
#include <stdbool.h>
#include <stdatomic.h>
#else
#include <linux/types.h>
#endif
#include "portals4.h"

#define PTL_STATS_VERSION 3

#define IF_MAX_SET(x, val)                                                                         \
	do {                                                                                       \
		if ((val) > (x))                                                                   \
			((x) = (val));                                                             \
	} while (0)

#define KERN_VAL_SET(x, val)                                                                       \
	do {                                                                                       \
	} while (0)
#define VAL_ATOMIC_ADD(x, val) __atomic_fetch_add(&x, val, __ATOMIC_RELAXED)
#define VAL_ATOMIC_SUB(x, val) __atomic_fetch_sub(&x, val, __ATOMIC_RELAXED)
#define STATS_LOG() ptl_statistics_log((struct ptl_stats *)&ptl_counters);

/* BXI Specific return codes */
#define PTL_UNIMPLEMENTED 0x300

typedef struct ptl_activate_hook *ptl_activate_hook_t;

int PtlActivateHookAdd(void (*callback)(void *arg, unsigned int nic, int what), void *arg,
		       ptl_activate_hook_t *hook);
int PtlActivateHookRemove(ptl_activate_hook_t hook);

int PtlNIAtomicSync(ptl_handle_ni_t nih);

enum ptl_str_type {
	PTL_STR_ERROR, /* Return codes */
	PTL_STR_EVENT, /* Events */
	PTL_STR_FAIL_TYPE /* Failure type */
};
const char *PtlToStr(int rc, enum ptl_str_type type);

/*
 * Statistics
 */
#define PTL_MAX_TX_LOOP_COUNT 20
#define PTL_MAX_RX_LOOP_COUNT 20

struct ptl_stats {
	uint64_t magic;
	int version;
	bool *print;
	/* API calls counters */
	struct ptl_api_calls {
		uint64_t init;
		uint64_t fini;
		uint64_t ni_init;
		uint64_t ni_fini;

		uint64_t hook_add;
		uint64_t hook_remove;

		uint64_t md_bind;
		uint64_t md_release;

		uint64_t me_append;
		uint64_t me_appendnb;
		uint64_t me_unlink;
		uint64_t me_search;
		uint64_t me_searchnb;

		uint64_t le_append;
		uint64_t le_appendnb;
		uint64_t le_unlink;
		uint64_t le_search;
		uint64_t le_searchnb;

		uint64_t eq_allocasync;
		uint64_t eq_alloc;
		uint64_t eq_free;
		uint64_t eq_get;
		uint64_t eq_poll;
		uint64_t eq_wait;

		uint64_t ct_alloc;
		uint64_t ct_free;
		uint64_t ct_get;
		uint64_t ct_set;
		uint64_t ct_poll;
		uint64_t ct_wait;
		uint64_t ct_inc;

		uint64_t pt_alloc;
		uint64_t pt_free;
		uint64_t pt_enable;
		uint64_t pt_enablenb;
		uint64_t pt_disable;
		uint64_t pt_disablenb;

		uint64_t atomic_sync;
		uint64_t ni_atomic_sync;

		uint64_t put;
		uint64_t putnb;

		uint64_t get;
		uint64_t getnb;

		uint64_t cache_cmd;
		uint64_t atinv;

		uint64_t swap;
		uint64_t swapnb;
		uint64_t atomic;
		uint64_t atomicnb;
		uint64_t fetch_atomic;
		uint64_t fetch_atomicnb;
		uint64_t ct_cancel_triggered;
		uint64_t triggered_put;
		uint64_t triggered_putnb;
		uint64_t triggered_get;
		uint64_t triggered_getnb;
		uint64_t triggered_atomic;
		uint64_t triggered_atomicnb;
		uint64_t triggered_fetch_atomic;
		uint64_t triggered_fetch_atomicnb;
		uint64_t triggered_swap;
		uint64_t triggered_swapnb;
		uint64_t triggered_ct_set;
		uint64_t triggered_ct_setnb;
		uint64_t triggered_ct_inc;
		uint64_t triggered_ct_incnb;
	} api;

	/* Resource utilization counters */
	struct ptl_resource_util {
		uint64_t avail_nme;
		uint64_t nme_on_nic;
		uint64_t nme_on_host;
		uint64_t lre_nme_on_nic;
		uint64_t lre_nme_on_host;
		uint64_t limit_nme;
		uint64_t nmd_inuse;
		uint64_t max_nmd_inuse;
		uint64_t max_md_length;
		uint64_t eq_sleeps;
		uint64_t intr;
		uint64_t intr_empty;
		uint64_t cq_tx_full;
		uint64_t cq_rx_full;
		uint64_t cq_tx_commit;
		uint64_t cq_rx_commit;
		uint64_t cq_tx_loop_counts[PTL_MAX_TX_LOOP_COUNT];
		uint64_t cq_rx_loop_counts[PTL_MAX_RX_LOOP_COUNT];
		uint64_t eq_discard;
		uint64_t eq_getev;
		uint64_t pio_put;
	} res;
};

extern volatile struct ptl_stats ptl_counters __attribute__((visibility("default")));

volatile struct ptl_stats *ptl_get_counters(ptl_handle_ni_t nih)
	__attribute__((visibility("default")));

/* Temporary: workaround to not check limits */
extern bool ptl_no_limits_check_workaround;

/*
 * Source Compatibility with BXIV2
 */

#define PTL_NI_TARGET_INVALID PTL_NI_UNDELIVERABLE
#define PTL_ME_MANAGE_LOCAL_STOP_IF_UH 0
#define PTL_ME_OV_RDV_PUT_ONLY 0
#define PTL_ME_OV_RDV_PUT_DISABLE 0
#define PTL_SIZE_INVALID 0x300
#define PTL_TRY_AGAIN 0x301

/*
 * BXI Specific Portals Features
 */

#define PTL_BXI3_MULTI_CQ (1 << 3)
#define PTL_BXI3_SERVICE (1 << 4)
/*
 * Enables PtlNIInit to return multiple pids for the same interface
 * (breaking the Portals behaviour)
 */
#define PTL_BXI3_MULTIPLE_PIDS (1 << 5)

void ptl_statistics_log(struct ptl_stats *);

/*
 * CQ Management
 *
 * Warning: If these functions are called then you will need to
 * re-call them if you close & re-open a BXI device, or your state
 * may be incoherent if you reduced the number of CQs.
 */

int PtlCQSet(ptl_interface_t iface, uint16_t index);
int PtlCQChooseBest(ptl_interface_t iface);
int PtlCQIndex(ptl_interface_t iface, uint16_t *index);
