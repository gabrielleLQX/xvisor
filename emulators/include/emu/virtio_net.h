/**
 * Copyright (c) 2013 Pranav Sawargaonkar.
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
 * @file virtio_net.h
 * @author Pranav Sawargaonkar (pranav.sawargaonkar@gmail.com)
 * @brief VirtIO Net Framework Interface.
 *
 * This header has been derived from linux kernel source:
 * <linux_source>/include/uapi/linux/virtio_net.h
 *
 * The original header is BSD licensed. 
 */

/* This header is BSD licensed so anyone can use the definitions to implement
 * compatible drivers/servers.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of IBM nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL IBM OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE. 
 */

#ifndef __VIRTIO_NET_H_
#define __VIRTIO_NET_H_

#include <vmm_types.h>

#define ETH_ALEN 6

/* The feature bitmap for virtio net */
#define VIRTIO_NET_F_CSUM	0	/* Host handles pkts w/ partial csum */
#define VIRTIO_NET_F_GUEST_CSUM	1	/* Guest handles pkts w/ partial csum */
#define VIRTIO_NET_F_MAC	5	/* Host has given MAC address. */
#define VIRTIO_NET_F_GSO	6	/* Host handles pkts w/ any GSO type */
#define VIRTIO_NET_F_GUEST_TSO4	7	/* Guest can handle TSOv4 in. */
#define VIRTIO_NET_F_GUEST_TSO6	8	/* Guest can handle TSOv6 in. */
#define VIRTIO_NET_F_GUEST_ECN	9	/* Guest can handle TSO[6] w/ ECN in. */
#define VIRTIO_NET_F_GUEST_UFO	10	/* Guest can handle UFO in. */
#define VIRTIO_NET_F_HOST_TSO4	11	/* Host can handle TSOv4 in. */
#define VIRTIO_NET_F_HOST_TSO6	12	/* Host can handle TSOv6 in. */
#define VIRTIO_NET_F_HOST_ECN	13	/* Host can handle TSO[6] w/ ECN in. */
#define VIRTIO_NET_F_HOST_UFO	14	/* Host can handle UFO in. */
#define VIRTIO_NET_F_MRG_RXBUF	15	/* Host can merge receive buffers. */
#define VIRTIO_NET_F_STATUS	16	/* virtio_net_config.status available */
#define VIRTIO_NET_F_CTRL_VQ	17	/* Control channel available */
#define VIRTIO_NET_F_CTRL_RX	18	/* Control channel RX mode support */
#define VIRTIO_NET_F_CTRL_VLAN	19	/* Control channel VLAN filtering */
#define VIRTIO_NET_F_CTRL_RX_EXTRA 20	/* Extra RX mode control support */

#define VIRTIO_NET_S_LINK_UP	1	/* Link is up */

struct virtio_net_config {
	/* The config defining mac address (if VIRTIO_NET_F_MAC) */
	u8 mac[6];
	/* See VIRTIO_NET_F_STATUS and VIRTIO_NET_S_* above */
	u16 status;
} __attribute__((packed));

/* This is the first element of the scatter-gather list.  If you don't
 * specify GSO or CSUM features, you can simply ignore the header. */
struct virtio_net_hdr {
#define VIRTIO_NET_HDR_F_NEEDS_CSUM	1	// Use csum_start, csum_offset
#define VIRTIO_NET_HDR_F_DATA_VALID	2	// Csum is valid
	u8 flags;
#define VIRTIO_NET_HDR_GSO_NONE		0	// Not a GSO frame
#define VIRTIO_NET_HDR_GSO_TCPV4	1	// GSO frame, IPv4 TCP (TSO)
#define VIRTIO_NET_HDR_GSO_UDP		3	// GSO frame, IPv4 UDP (UFO)
#define VIRTIO_NET_HDR_GSO_TCPV6	4	// GSO frame, IPv6 TCP
#define VIRTIO_NET_HDR_GSO_ECN		0x80	// TCP has ECN set
	u8 gso_type;
	u16 hdr_len;		/* Ethernet + IP + tcp/udp hdrs */
	u16 gso_size;		/* Bytes to append to hdr_len per frame */
	u16 csum_start;	/* Position to start checksumming from */
	u16 csum_offset;	/* Offset after that to place checksum */
};

/* This is the version of the header to use when the MRG_RXBUF
 * feature has been negotiated. */
struct virtio_net_hdr_mrg_rxbuf {
	struct virtio_net_hdr hdr;
	u16 num_buffers;	/* Number of merged rx buffers */
};

/*
 * Control virtqueue data structures
 *
 * The control virtqueue expects a header in the first sg entry
 * and an ack/status response in the last entry.  Data for the
 * command goes in between.
 */
struct virtio_net_ctrl_hdr {
#if 0
	__u8 class;
#endif
	u8 cmd;
} __attribute__((packed));

typedef u8 virtio_net_ctrl_ack;

#define VIRTIO_NET_OK     0
#define VIRTIO_NET_ERR    1

/*
 * Control the RX mode, ie. promisucous, allmulti, etc...
 * All commands require an "out" sg entry containing a 1 byte
 * state value, zero = disable, non-zero = enable.  Commands
 * 0 and 1 are supported with the VIRTIO_NET_F_CTRL_RX feature.
 * Commands 2-5 are added with VIRTIO_NET_F_CTRL_RX_EXTRA.
 */
#define VIRTIO_NET_CTRL_RX		0
#define VIRTIO_NET_CTRL_RX_PROMISC      0
#define VIRTIO_NET_CTRL_RX_ALLMULTI     1
#define VIRTIO_NET_CTRL_RX_ALLUNI       2
#define VIRTIO_NET_CTRL_RX_NOMULTI      3
#define VIRTIO_NET_CTRL_RX_NOUNI        4
#define VIRTIO_NET_CTRL_RX_NOBCAST      5

/*
 * Control the MAC filter table.
 *
 * The MAC filter table is managed by the hypervisor, the guest should
 * assume the size is infinite.  Filtering should be considered
 * non-perfect, ie. based on hypervisor resources, the guest may
 * received packets from sources not specified in the filter list.
 *
 * In addition to the class/cmd header, the TABLE_SET command requires
 * two out scatterlists.  Each contains a 4 byte count of entries followed
 * by a concatenated byte stream of the ETH_ALEN MAC addresses.  The
 * first sg list contains unicast addresses, the second is for multicast.
 * This functionality is present if the VIRTIO_NET_F_CTRL_RX feature
 * is available.
 */
struct virtio_net_ctrl_mac {
	u32 entries;
	u8 macs[][ETH_ALEN];
} __attribute__((packed));

#define VIRTIO_NET_CTRL_MAC		1
#define VIRTIO_NET_CTRL_MAC_TABLE_SET	0

/*
 * Control VLAN filtering
 *
 * The VLAN filter table is controlled via a simple ADD/DEL interface.
 * VLAN IDs not added may be filterd by the hypervisor.  Del is the
 * opposite of add.  Both commands expect an out entry containing a 2
 * byte VLAN ID.  VLAN filterting is available with the
 * VIRTIO_NET_F_CTRL_VLAN feature bit.
 */
#define VIRTIO_NET_CTRL_VLAN			2
#define VIRTIO_NET_CTRL_VLAN_ADD		0
#define VIRTIO_NET_CTRL_VLAN_DEL		1

#endif /* __VIRTIO_NET_H_ */
