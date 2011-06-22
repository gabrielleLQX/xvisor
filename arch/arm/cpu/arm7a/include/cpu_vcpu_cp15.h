/**
 * Copyright (c) 2011 Anup Patel.
 * All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2, or (at your option)
 * any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 * @file cpu_vcpu_cp15.h
 * @version 1.0
 * @author Anup Patel (anup@brainfault.org)
 * @brief Header File for VCPU cp15 emulation
 */
#ifndef _CPU_VCPU_CP15_H__
#define _CPU_VCPU_CP15_H__

#include <vmm_types.h>
#include <vmm_guest.h>

/** Read one registers from CP15 */
bool cpu_vcpu_cp15_read(vmm_vcpu_t * vcpu, 
			u32 opc1, u32 opc2, u32 CRn, u32 CRm, 
			u32 *data);

/** Write one registers to CP15 */
bool cpu_vcpu_cp15_write(vmm_vcpu_t * vcpu, 
			 u32 opc1, u32 opc2, u32 CRn, u32 CRm, 
			 u32 data);

/** Handle instruction fault for a VCPU */
int cpu_vcpu_cp15_ifault(vmm_vcpu_t * vcpu, 
			 vmm_user_regs_t * regs,
			 u32 ifsr, u32 ifar);

/** Handle data fault for a VCPU */
int cpu_vcpu_cp15_dfault(vmm_vcpu_t * vcpu, 
			 vmm_user_regs_t * regs,
			 u32 dfsr, u32 dfar);

/** Read from memory using VCPU CP15 */
int cpu_vcpu_cp15_mem_read(vmm_vcpu_t * vcpu, 
			   vmm_user_regs_t * regs,
			   virtual_addr_t addr, 
			   void *dst, u32 dst_len);

/** Write to memory using VCPU CP15 */
int cpu_vcpu_cp15_mem_write(vmm_vcpu_t * vcpu, 
			    vmm_user_regs_t * regs,
			    virtual_addr_t addr, 
			    void *src, u32 src_len);

/* Read from memory using VCPU CP15 */
virtual_addr_t cpu_vcpu_cp15_vector_addr(vmm_vcpu_t * vcpu, 
					 u32 irq_no);

/* Syncronize VCPU CP15 with change in VCPU mode */
void cpu_vcpu_cp15_sync_cpsr(vmm_vcpu_t * vcpu);

/** Switch MMU context between VCPUs */
void cpu_vcpu_cp15_context_switch(vmm_vcpu_t * tvcpu, 
				  vmm_vcpu_t * vcpu, 
				  vmm_user_regs_t * regs);

/** Initialize CP15 subsystem for a VCPU */
int cpu_vcpu_cp15_init(vmm_vcpu_t * vcpu, u32 cpuid);

#endif /* _CPU_VCPU_CP15_H */
