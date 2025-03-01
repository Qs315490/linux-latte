/* SPDX-License-Identifier: GPL-2.0 */
/*
 * Support for Intel Camera Imaging ISP subsystem.
 * Copyright (c) 2015, Intel Corporation.
 */

#ifndef __ISYS_DMA_PUBLIC_H_INCLUDED__
#define __ISYS_DMA_PUBLIC_H_INCLUDED__


#include "system_local.h"
#include "type_support.h"

extern void isys2401_dma_reg_store(
    const isys2401_dma_ID_t dma_id,
    const unsigned int	reg,
    const hrt_data		value);

extern hrt_data isys2401_dma_reg_load(
    const isys2401_dma_ID_t dma_id,
    const unsigned int	reg);

void isys2401_dma_set_max_burst_size(
    const isys2401_dma_ID_t dma_id,
    uint32_t		max_burst_size);


#endif /* __ISYS_DMA_PUBLIC_H_INCLUDED__ */
