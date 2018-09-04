/*
 * Copyright © 2018, VideoLAN and dav1d authors
 * Copyright © 2018, Two Orioles, LLC
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef __DAV1D_SRC_LOOPRESTORATION_H__
#define __DAV1D_SRC_LOOPRESTORATION_H__

#include <stdint.h>
#include <stddef.h>

#include "common/bitdepth.h"

enum LrEdgeFlags {
    LR_HAVE_LEFT = 1 << 0,
    LR_HAVE_RIGHT = 1 << 1,
    LR_HAVE_TOP = 1 << 2,
    LR_HAVE_BOTTOM = 1 << 3,
};

// Although the spec applies restoration filters over 4x4 blocks, the wiener
// filter can be applied to a bigger surface.
//    * w is constrained by the restoration unit size (w <= 256)
//    * h is constrained by the stripe height (h <= 64)
typedef void (*wienerfilter_fn)(pixel *dst, ptrdiff_t dst_stride,
                                const pixel *lpf, ptrdiff_t lpf_stride,
                                int w, int h, const int16_t filterh[7],
                                const int16_t filterv[7], enum LrEdgeFlags edges);

typedef void (*selfguided_fn)(pixel *dst, ptrdiff_t dst_stride,
                              const pixel *lpf, ptrdiff_t lpf_stride,
                              int w, int h, int sgr_idx, const int16_t sgr_w[2],
                              const enum LrEdgeFlags edges);

typedef struct Dav1dLoopRestorationDSPContext {
    wienerfilter_fn wiener;
    selfguided_fn selfguided;
} Dav1dLoopRestorationDSPContext;

void dav1d_loop_restoration_dsp_init_8bpc(Dav1dLoopRestorationDSPContext *c);
void dav1d_loop_restoration_dsp_init_10bpc(Dav1dLoopRestorationDSPContext *c);

#endif /* __DAV1D_SRC_LOOPRESTORATION_H__ */
