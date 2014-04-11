/*
 * Copyright (c) 2004, 2005 Mellanox Technologies Ltd.  All rights reserved.
 * Copyright (c) 2004, 2005 Infinicon Corporation.  All rights reserved.
 * Copyright (c) 2004, 2005 Intel Corporation.  All rights reserved.
 * Copyright (c) 2004, 2005 Topspin Corporation.  All rights reserved.
 * Copyright (c) 2004-2007 Voltaire Corporation.  All rights reserved.
 * Copyright (c) 2005 Sun Microsystems, Inc. All rights reserved.
 *
 * This software is available to you under a choice of one of two
 * licenses.  You may choose to be licensed under the terms of the GNU
 * General Public License (GPL) Version 2, available from the file
 * COPYING in the main directory of this source tree, or the
 * OpenIB.org BSD license below:
 *
 *     Redistribution and use in source and binary forms, with or
 *     without modification, are permitted provided that the following
 *     conditions are met:
 *
 *      - Redistributions of source code must retain the above
 *        copyright notice, this list of conditions and the following
 *        disclaimer.
 *
 *      - Redistributions in binary form must reproduce the above
 *        copyright notice, this list of conditions and the following
 *        disclaimer in the documentation and/or other materials
 *        provided with the distribution.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 */

#include <rdma/ib_smi.h>
#include "smi.h"

enum smi_action smi_handle_dr_smp_send(struct ib_smp *smp,
				       u8 node_type, int port_num)
{
	u8 hop_ptr, hop_cnt;

	hop_ptr = smp->hop_ptr;
	hop_cnt = smp->hop_cnt;

	
	
	if (hop_cnt >= IB_SMP_MAX_PATH_HOPS)
		return IB_SMI_DISCARD;

	if (!ib_get_smp_direction(smp)) {
		
		if (hop_cnt && hop_ptr == 0) {
			smp->hop_ptr++;
			return (smp->initial_path[smp->hop_ptr] ==
				port_num ? IB_SMI_HANDLE : IB_SMI_DISCARD);
		}

		
		if (hop_ptr && hop_ptr < hop_cnt) {
			if (node_type != RDMA_NODE_IB_SWITCH)
				return IB_SMI_DISCARD;

			
			smp->hop_ptr++;
			return (smp->initial_path[smp->hop_ptr] ==
				port_num ? IB_SMI_HANDLE : IB_SMI_DISCARD);
		}

		
		if (hop_ptr == hop_cnt) {
			
			smp->hop_ptr++;
			return (node_type == RDMA_NODE_IB_SWITCH ||
				smp->dr_dlid == IB_LID_PERMISSIVE ?
				IB_SMI_HANDLE : IB_SMI_DISCARD);
		}

		
		
		return (hop_ptr == hop_cnt + 1 ? IB_SMI_HANDLE : IB_SMI_DISCARD);

	} else {
		
		if (hop_cnt && hop_ptr == hop_cnt + 1) {
			smp->hop_ptr--;
			return (smp->return_path[smp->hop_ptr] ==
				port_num ? IB_SMI_HANDLE : IB_SMI_DISCARD);
		}

		
		if (2 <= hop_ptr && hop_ptr <= hop_cnt) {
			if (node_type != RDMA_NODE_IB_SWITCH)
				return IB_SMI_DISCARD;

			smp->hop_ptr--;
			return (smp->return_path[smp->hop_ptr] ==
				port_num ? IB_SMI_HANDLE : IB_SMI_DISCARD);
		}

		
		if (hop_ptr == 1) {
			smp->hop_ptr--;
			
			return (node_type == RDMA_NODE_IB_SWITCH ||
				smp->dr_slid == IB_LID_PERMISSIVE ?
				IB_SMI_HANDLE : IB_SMI_DISCARD);
		}

		
		if (hop_ptr == 0)
			return IB_SMI_HANDLE;

		
		return IB_SMI_DISCARD;
	}
}

enum smi_action smi_handle_dr_smp_recv(struct ib_smp *smp, u8 node_type,
				       int port_num, int phys_port_cnt)
{
	u8 hop_ptr, hop_cnt;

	hop_ptr = smp->hop_ptr;
	hop_cnt = smp->hop_cnt;

	
	
	if (hop_cnt >= IB_SMP_MAX_PATH_HOPS)
		return IB_SMI_DISCARD;

	if (!ib_get_smp_direction(smp)) {
		
		if (hop_cnt && hop_ptr == 0)
			return IB_SMI_DISCARD;

		
		if (hop_ptr && hop_ptr < hop_cnt) {
			if (node_type != RDMA_NODE_IB_SWITCH)
				return IB_SMI_DISCARD;

			smp->return_path[hop_ptr] = port_num;
			
			return (smp->initial_path[hop_ptr+1] <= phys_port_cnt ?
				IB_SMI_HANDLE : IB_SMI_DISCARD);
		}

		
		if (hop_ptr == hop_cnt) {
			if (hop_cnt)
				smp->return_path[hop_ptr] = port_num;
			

			return (node_type == RDMA_NODE_IB_SWITCH ||
				smp->dr_dlid == IB_LID_PERMISSIVE ?
				IB_SMI_HANDLE : IB_SMI_DISCARD);
		}

		
		
		return (hop_ptr == hop_cnt + 1 ? IB_SMI_HANDLE : IB_SMI_DISCARD);

	} else {

		
		if (hop_cnt && hop_ptr == hop_cnt + 1) {
			smp->hop_ptr--;
			return (smp->return_path[smp->hop_ptr] ==
				port_num ? IB_SMI_HANDLE : IB_SMI_DISCARD);
		}

		
		if (2 <= hop_ptr && hop_ptr <= hop_cnt) {
			if (node_type != RDMA_NODE_IB_SWITCH)
				return IB_SMI_DISCARD;

			
			return (smp->return_path[hop_ptr-1] <= phys_port_cnt ?
				IB_SMI_HANDLE : IB_SMI_DISCARD);
		}

		
		if (hop_ptr == 1) {
			if (smp->dr_slid == IB_LID_PERMISSIVE) {
				
				smp->hop_ptr--;
				return IB_SMI_HANDLE;
			}
			
			return (node_type == RDMA_NODE_IB_SWITCH ?
				IB_SMI_HANDLE : IB_SMI_DISCARD);
		}

		
		
		return (hop_ptr == 0 ? IB_SMI_HANDLE : IB_SMI_DISCARD);
	}
}

enum smi_forward_action smi_check_forward_dr_smp(struct ib_smp *smp)
{
	u8 hop_ptr, hop_cnt;

	hop_ptr = smp->hop_ptr;
	hop_cnt = smp->hop_cnt;

	if (!ib_get_smp_direction(smp)) {
		
		if (hop_ptr && hop_ptr < hop_cnt)
			return IB_SMI_FORWARD;

		
		if (hop_ptr == hop_cnt)
			return (smp->dr_dlid == IB_LID_PERMISSIVE ?
				IB_SMI_SEND : IB_SMI_LOCAL);

		
		if (hop_ptr == hop_cnt + 1)
			return IB_SMI_SEND;
	} else {
		
		if (2 <= hop_ptr && hop_ptr <= hop_cnt)
			return IB_SMI_FORWARD;

		
		if (hop_ptr == 1)
			return (smp->dr_slid != IB_LID_PERMISSIVE ?
				IB_SMI_SEND : IB_SMI_LOCAL);
	}
	return IB_SMI_LOCAL;
}

int smi_get_fwd_port(struct ib_smp *smp)
{
	return (!ib_get_smp_direction(smp) ? smp->initial_path[smp->hop_ptr+1] :
		smp->return_path[smp->hop_ptr-1]);
}