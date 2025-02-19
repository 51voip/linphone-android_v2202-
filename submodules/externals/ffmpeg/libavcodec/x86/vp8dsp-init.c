/*
 * VP8 DSP functions x86-optimized
 * Copyright (c) 2010 Ronald S. Bultje <rsbultje@gmail.com>
 * Copyright (c) 2010 Jason Garrett-Glaser <darkshikari@gmail.com>
 *
 * This file is part of FFmpeg.
 *
 * FFmpeg is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * FFmpeg is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with FFmpeg; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */

#include "libavutil/cpu.h"
#include "libavutil/x86_cpu.h"
#include "libavcodec/vp8dsp.h"

#if HAVE_YASM

/*
 * MC functions
 */
extern void ff_put_vp8_epel4_h4_mmxext(uint8_t *dst, int dststride,
                                       uint8_t *src, int srcstride,
                                       int height, int mx, int my);
extern void ff_put_vp8_epel4_h6_mmxext(uint8_t *dst, int dststride,
                                       uint8_t *src, int srcstride,
                                       int height, int mx, int my);
extern void ff_put_vp8_epel4_v4_mmxext(uint8_t *dst, int dststride,
                                       uint8_t *src, int srcstride,
                                       int height, int mx, int my);
extern void ff_put_vp8_epel4_v6_mmxext(uint8_t *dst, int dststride,
                                       uint8_t *src, int srcstride,
                                       int height, int mx, int my);

extern void ff_put_vp8_epel8_h4_sse2  (uint8_t *dst, int dststride,
                                       uint8_t *src, int srcstride,
                                       int height, int mx, int my);
extern void ff_put_vp8_epel8_h6_sse2  (uint8_t *dst, int dststride,
                                       uint8_t *src, int srcstride,
                                       int height, int mx, int my);
extern void ff_put_vp8_epel8_v4_sse2  (uint8_t *dst, int dststride,
                                       uint8_t *src, int srcstride,
                                       int height, int mx, int my);
extern void ff_put_vp8_epel8_v6_sse2  (uint8_t *dst, int dststride,
                                       uint8_t *src, int srcstride,
                                       int height, int mx, int my);

extern void ff_put_vp8_epel4_h4_ssse3 (uint8_t *dst, int dststride,
                                       uint8_t *src, int srcstride,
                                       int height, int mx, int my);
extern void ff_put_vp8_epel4_h6_ssse3 (uint8_t *dst, int dststride,
                                       uint8_t *src, int srcstride,
                                       int height, int mx, int my);
extern void ff_put_vp8_epel4_v4_ssse3 (uint8_t *dst, int dststride,
                                       uint8_t *src, int srcstride,
                                       int height, int mx, int my);
extern void ff_put_vp8_epel4_v6_ssse3 (uint8_t *dst, int dststride,
                                       uint8_t *src, int srcstride,
                                       int height, int mx, int my);
extern void ff_put_vp8_epel8_h4_ssse3 (uint8_t *dst, int dststride,
                                       uint8_t *src, int srcstride,
                                       int height, int mx, int my);
extern void ff_put_vp8_epel8_h6_ssse3 (uint8_t *dst, int dststride,
                                       uint8_t *src, int srcstride,
                                       int height, int mx, int my);
extern void ff_put_vp8_epel8_v4_ssse3 (uint8_t *dst, int dststride,
                                       uint8_t *src, int srcstride,
                                       int height, int mx, int my);
extern void ff_put_vp8_epel8_v6_ssse3 (uint8_t *dst, int dststride,
                                       uint8_t *src, int srcstride,
                                       int height, int mx, int my);

extern void ff_put_vp8_bilinear4_h_mmxext(uint8_t *dst, int dststride,
                                          uint8_t *src, int srcstride,
                                          int height, int mx, int my);
extern void ff_put_vp8_bilinear8_h_sse2  (uint8_t *dst, int dststride,
                                          uint8_t *src, int srcstride,
                                          int height, int mx, int my);
extern void ff_put_vp8_bilinear4_h_ssse3 (uint8_t *dst, int dststride,
                                          uint8_t *src, int srcstride,
                                          int height, int mx, int my);
extern void ff_put_vp8_bilinear8_h_ssse3 (uint8_t *dst, int dststride,
                                          uint8_t *src, int srcstride,
                                          int height, int mx, int my);

extern void ff_put_vp8_bilinear4_v_mmxext(uint8_t *dst, int dststride,
                                          uint8_t *src, int srcstride,
                                          int height, int mx, int my);
extern void ff_put_vp8_bilinear8_v_sse2  (uint8_t *dst, int dststride,
                                          uint8_t *src, int srcstride,
                                          int height, int mx, int my);
extern void ff_put_vp8_bilinear4_v_ssse3 (uint8_t *dst, int dststride,
                                          uint8_t *src, int srcstride,
                                          int height, int mx, int my);
extern void ff_put_vp8_bilinear8_v_ssse3 (uint8_t *dst, int dststride,
                                          uint8_t *src, int srcstride,
                                          int height, int mx, int my);


extern void ff_put_vp8_pixels8_mmx (uint8_t *dst, int dststride,
                                    uint8_t *src, int srcstride,
                                    int height, int mx, int my);
extern void ff_put_vp8_pixels16_mmx(uint8_t *dst, int dststride,
                                    uint8_t *src, int srcstride,
                                    int height, int mx, int my);
extern void ff_put_vp8_pixels16_sse(uint8_t *dst, int dststride,
                                    uint8_t *src, int srcstride,
                                    int height, int mx, int my);

#define TAP_W16(OPT, FILTERTYPE, TAPTYPE) \
static void ff_put_vp8_ ## FILTERTYPE ## 16_ ## TAPTYPE ## _ ## OPT( \
    uint8_t *dst,  int dststride, uint8_t *src, \
    int srcstride, int height, int mx, int my) \
{ \
    ff_put_vp8_ ## FILTERTYPE ## 8_ ## TAPTYPE ## _ ## OPT( \
        dst,     dststride, src,     srcstride, height, mx, my); \
    ff_put_vp8_ ## FILTERTYPE ## 8_ ## TAPTYPE ## _ ## OPT( \
        dst + 8, dststride, src + 8, srcstride, height, mx, my); \
}
#define TAP_W8(OPT, FILTERTYPE, TAPTYPE) \
static void ff_put_vp8_ ## FILTERTYPE ## 8_ ## TAPTYPE ## _ ## OPT( \
    uint8_t *dst,  int dststride, uint8_t *src, \
    int srcstride, int height, int mx, int my) \
{ \
    ff_put_vp8_ ## FILTERTYPE ## 4_ ## TAPTYPE ## _ ## OPT( \
        dst,     dststride, src,     srcstride, height, mx, my); \
    ff_put_vp8_ ## FILTERTYPE ## 4_ ## TAPTYPE ## _ ## OPT( \
        dst + 4, dststride, src + 4, srcstride, height, mx, my); \
}

TAP_W8 (mmxext, epel, h4)
TAP_W8 (mmxext, epel, h6)
TAP_W16(mmxext, epel, h6)
TAP_W8 (mmxext, epel, v4)
TAP_W8 (mmxext, epel, v6)
TAP_W16(mmxext, epel, v6)
TAP_W8 (mmxext, bilinear, h)
TAP_W16(mmxext, bilinear, h)
TAP_W8 (mmxext, bilinear, v)
TAP_W16(mmxext, bilinear, v)

TAP_W16(sse2,   epel, h6)
TAP_W16(sse2,   epel, v6)
TAP_W16(sse2,   bilinear, h)
TAP_W16(sse2,   bilinear, v)

TAP_W16(ssse3,  epel, h6)
TAP_W16(ssse3,  epel, v6)
TAP_W16(ssse3,  bilinear, h)
TAP_W16(ssse3,  bilinear, v)

#define HVTAP(OPT, ALIGN, TAPNUMX, TAPNUMY, SIZE, MAXHEIGHT) \
static void ff_put_vp8_epel ## SIZE ## _h ## TAPNUMX ## v ## TAPNUMY ## _ ## OPT( \
    uint8_t *dst, int dststride, uint8_t *src, \
    int srcstride, int height, int mx, int my) \
{ \
    DECLARE_ALIGNED(ALIGN, uint8_t, tmp)[SIZE * (MAXHEIGHT + TAPNUMY - 1)]; \
    uint8_t *tmpptr = tmp + SIZE * (TAPNUMY / 2 - 1); \
    src -= srcstride * (TAPNUMY / 2 - 1); \
    ff_put_vp8_epel ## SIZE ## _h ## TAPNUMX ## _ ## OPT( \
        tmp, SIZE,      src,    srcstride, height + TAPNUMY - 1, mx, my); \
    ff_put_vp8_epel ## SIZE ## _v ## TAPNUMY ## _ ## OPT( \
        dst, dststride, tmpptr, SIZE,      height,               mx, my); \
}

#define HVTAPMMX(x, y) \
HVTAP(mmxext, 8, x, y,  4,  8) \
HVTAP(mmxext, 8, x, y,  8, 16)

HVTAPMMX(4, 4)
HVTAPMMX(4, 6)
HVTAPMMX(6, 4)
HVTAPMMX(6, 6)
HVTAP(mmxext, 8, 6, 6, 16, 16)

#define HVTAPSSE2(x, y, w) \
HVTAP(sse2,  16, x, y, w, 16) \
HVTAP(ssse3, 16, x, y, w, 16)

HVTAPSSE2(4, 4, 8)
HVTAPSSE2(4, 6, 8)
HVTAPSSE2(6, 4, 8)
HVTAPSSE2(6, 6, 8)
HVTAPSSE2(6, 6, 16)

HVTAP(ssse3, 16, 4, 4, 4, 8)
HVTAP(ssse3, 16, 4, 6, 4, 8)
HVTAP(ssse3, 16, 6, 4, 4, 8)
HVTAP(ssse3, 16, 6, 6, 4, 8)

#define HVBILIN(OPT, ALIGN, SIZE, MAXHEIGHT) \
static void ff_put_vp8_bilinear ## SIZE ## _hv_ ## OPT( \
    uint8_t *dst, int dststride, uint8_t *src, \
    int srcstride, int height, int mx, int my) \
{ \
    DECLARE_ALIGNED(ALIGN, uint8_t, tmp)[SIZE * (MAXHEIGHT + 2)]; \
    ff_put_vp8_bilinear ## SIZE ## _h_ ## OPT( \
        tmp, SIZE,      src, srcstride, height + 1, mx, my); \
    ff_put_vp8_bilinear ## SIZE ## _v_ ## OPT( \
        dst, dststride, tmp, SIZE,      height,     mx, my); \
}

HVBILIN(mmxext, 8,  4,  8)
HVBILIN(mmxext, 8,  8, 16)
HVBILIN(mmxext, 8, 16, 16)
HVBILIN(sse2,   8,  8, 16)
HVBILIN(sse2,   8, 16, 16)
HVBILIN(ssse3,  8,  4,  8)
HVBILIN(ssse3,  8,  8, 16)
HVBILIN(ssse3,  8, 16, 16)

extern void ff_vp8_idct_dc_add_mmx(uint8_t *dst, DCTELEM block[16], int stride);
extern void ff_vp8_idct_dc_add_sse4(uint8_t *dst, DCTELEM block[16], int stride);
extern void ff_vp8_idct_dc_add4y_mmx(uint8_t *dst, DCTELEM block[4][16], int stride);
extern void ff_vp8_idct_dc_add4y_sse2(uint8_t *dst, DCTELEM block[4][16], int stride);
extern void ff_vp8_idct_dc_add4uv_mmx(uint8_t *dst, DCTELEM block[2][16], int stride);
extern void ff_vp8_luma_dc_wht_mmx(DCTELEM block[4][4][16], DCTELEM dc[16]);
extern void ff_vp8_luma_dc_wht_sse(DCTELEM block[4][4][16], DCTELEM dc[16]);
extern void ff_vp8_idct_add_mmx(uint8_t *dst, DCTELEM block[16], int stride);
extern void ff_vp8_idct_add_sse(uint8_t *dst, DCTELEM block[16], int stride);

#define DECLARE_LOOP_FILTER(NAME)\
extern void ff_vp8_v_loop_filter_simple_ ## NAME(uint8_t *dst, int stride, int flim);\
extern void ff_vp8_h_loop_filter_simple_ ## NAME(uint8_t *dst, int stride, int flim);\
extern void ff_vp8_v_loop_filter16y_inner_ ## NAME (uint8_t *dst, int stride,\
                                                    int e, int i, int hvt);\
extern void ff_vp8_h_loop_filter16y_inner_ ## NAME (uint8_t *dst, int stride,\
                                                    int e, int i, int hvt);\
extern void ff_vp8_v_loop_filter8uv_inner_ ## NAME (uint8_t *dstU, uint8_t *dstV,\
                                                    int s, int e, int i, int hvt);\
extern void ff_vp8_h_loop_filter8uv_inner_ ## NAME (uint8_t *dstU, uint8_t *dstV,\
                                                    int s, int e, int i, int hvt);\
extern void ff_vp8_v_loop_filter16y_mbedge_ ## NAME(uint8_t *dst, int stride,\
                                                    int e, int i, int hvt);\
extern void ff_vp8_h_loop_filter16y_mbedge_ ## NAME(uint8_t *dst, int stride,\
                                                    int e, int i, int hvt);\
extern void ff_vp8_v_loop_filter8uv_mbedge_ ## NAME(uint8_t *dstU, uint8_t *dstV,\
                                                    int s, int e, int i, int hvt);\
extern void ff_vp8_h_loop_filter8uv_mbedge_ ## NAME(uint8_t *dstU, uint8_t *dstV,\
                                                    int s, int e, int i, int hvt);

DECLARE_LOOP_FILTER(mmx)
DECLARE_LOOP_FILTER(mmxext)
DECLARE_LOOP_FILTER(sse2)
DECLARE_LOOP_FILTER(ssse3)
DECLARE_LOOP_FILTER(sse4)

#endif

#define VP8_LUMA_MC_FUNC(IDX, SIZE, OPT) \
    c->put_vp8_epel_pixels_tab[IDX][0][2] = ff_put_vp8_epel ## SIZE ## _h6_ ## OPT; \
    c->put_vp8_epel_pixels_tab[IDX][2][0] = ff_put_vp8_epel ## SIZE ## _v6_ ## OPT; \
    c->put_vp8_epel_pixels_tab[IDX][2][2] = ff_put_vp8_epel ## SIZE ## _h6v6_ ## OPT

#define VP8_MC_FUNC(IDX, SIZE, OPT) \
    c->put_vp8_epel_pixels_tab[IDX][0][1] = ff_put_vp8_epel ## SIZE ## _h4_ ## OPT; \
    c->put_vp8_epel_pixels_tab[IDX][1][0] = ff_put_vp8_epel ## SIZE ## _v4_ ## OPT; \
    c->put_vp8_epel_pixels_tab[IDX][1][1] = ff_put_vp8_epel ## SIZE ## _h4v4_ ## OPT; \
    c->put_vp8_epel_pixels_tab[IDX][1][2] = ff_put_vp8_epel ## SIZE ## _h6v4_ ## OPT; \
    c->put_vp8_epel_pixels_tab[IDX][2][1] = ff_put_vp8_epel ## SIZE ## _h4v6_ ## OPT; \
    VP8_LUMA_MC_FUNC(IDX, SIZE, OPT)

#define VP8_BILINEAR_MC_FUNC(IDX, SIZE, OPT) \
    c->put_vp8_bilinear_pixels_tab[IDX][0][1] = ff_put_vp8_bilinear ## SIZE ## _h_ ## OPT; \
    c->put_vp8_bilinear_pixels_tab[IDX][0][2] = ff_put_vp8_bilinear ## SIZE ## _h_ ## OPT; \
    c->put_vp8_bilinear_pixels_tab[IDX][1][0] = ff_put_vp8_bilinear ## SIZE ## _v_ ## OPT; \
    c->put_vp8_bilinear_pixels_tab[IDX][1][1] = ff_put_vp8_bilinear ## SIZE ## _hv_ ## OPT; \
    c->put_vp8_bilinear_pixels_tab[IDX][1][2] = ff_put_vp8_bilinear ## SIZE ## _hv_ ## OPT; \
    c->put_vp8_bilinear_pixels_tab[IDX][2][0] = ff_put_vp8_bilinear ## SIZE ## _v_ ## OPT; \
    c->put_vp8_bilinear_pixels_tab[IDX][2][1] = ff_put_vp8_bilinear ## SIZE ## _hv_ ## OPT; \
    c->put_vp8_bilinear_pixels_tab[IDX][2][2] = ff_put_vp8_bilinear ## SIZE ## _hv_ ## OPT


av_cold void ff_vp8dsp_init_x86(VP8DSPContext* c)
{
    int mm_flags = av_get_cpu_flags();

#if HAVE_YASM
    if (mm_flags & AV_CPU_FLAG_MMX) {
        c->vp8_idct_dc_add    = ff_vp8_idct_dc_add_mmx;
        c->vp8_idct_dc_add4y  = ff_vp8_idct_dc_add4y_mmx;
        c->vp8_idct_dc_add4uv = ff_vp8_idct_dc_add4uv_mmx;
        c->vp8_idct_add       = ff_vp8_idct_add_mmx;
        c->vp8_luma_dc_wht    = ff_vp8_luma_dc_wht_mmx;
        c->put_vp8_epel_pixels_tab[0][0][0]     =
        c->put_vp8_bilinear_pixels_tab[0][0][0] = ff_put_vp8_pixels16_mmx;
        c->put_vp8_epel_pixels_tab[1][0][0]     =
        c->put_vp8_bilinear_pixels_tab[1][0][0] = ff_put_vp8_pixels8_mmx;

        c->vp8_v_loop_filter_simple = ff_vp8_v_loop_filter_simple_mmx;
        c->vp8_h_loop_filter_simple = ff_vp8_h_loop_filter_simple_mmx;

        c->vp8_v_loop_filter16y_inner = ff_vp8_v_loop_filter16y_inner_mmx;
        c->vp8_h_loop_filter16y_inner = ff_vp8_h_loop_filter16y_inner_mmx;
        c->vp8_v_loop_filter8uv_inner = ff_vp8_v_loop_filter8uv_inner_mmx;
        c->vp8_h_loop_filter8uv_inner = ff_vp8_h_loop_filter8uv_inner_mmx;

        c->vp8_v_loop_filter16y       = ff_vp8_v_loop_filter16y_mbedge_mmx;
        c->vp8_h_loop_filter16y       = ff_vp8_h_loop_filter16y_mbedge_mmx;
        c->vp8_v_loop_filter8uv       = ff_vp8_v_loop_filter8uv_mbedge_mmx;
        c->vp8_h_loop_filter8uv       = ff_vp8_h_loop_filter8uv_mbedge_mmx;
    }

    /* note that 4-tap width=16 functions are missing because w=16
     * is only used for luma, and luma is always a copy or sixtap. */
    if (mm_flags & AV_CPU_FLAG_MMX2) {
        VP8_LUMA_MC_FUNC(0, 16, mmxext);
        VP8_MC_FUNC(1, 8, mmxext);
        VP8_MC_FUNC(2, 4, mmxext);
        VP8_BILINEAR_MC_FUNC(0, 16, mmxext);
        VP8_BILINEAR_MC_FUNC(1, 8, mmxext);
        VP8_BILINEAR_MC_FUNC(2, 4, mmxext);

        c->vp8_v_loop_filter_simple = ff_vp8_v_loop_filter_simple_mmxext;
        c->vp8_h_loop_filter_simple = ff_vp8_h_loop_filter_simple_mmxext;

        c->vp8_v_loop_filter16y_inner = ff_vp8_v_loop_filter16y_inner_mmxext;
        c->vp8_h_loop_filter16y_inner = ff_vp8_h_loop_filter16y_inner_mmxext;
        c->vp8_v_loop_filter8uv_inner = ff_vp8_v_loop_filter8uv_inner_mmxext;
        c->vp8_h_loop_filter8uv_inner = ff_vp8_h_loop_filter8uv_inner_mmxext;

        c->vp8_v_loop_filter16y       = ff_vp8_v_loop_filter16y_mbedge_mmxext;
        c->vp8_h_loop_filter16y       = ff_vp8_h_loop_filter16y_mbedge_mmxext;
        c->vp8_v_loop_filter8uv       = ff_vp8_v_loop_filter8uv_mbedge_mmxext;
        c->vp8_h_loop_filter8uv       = ff_vp8_h_loop_filter8uv_mbedge_mmxext;
    }

    if (mm_flags & AV_CPU_FLAG_SSE) {
        c->vp8_idct_add                         = ff_vp8_idct_add_sse;
        c->vp8_luma_dc_wht                      = ff_vp8_luma_dc_wht_sse;
        c->put_vp8_epel_pixels_tab[0][0][0]     =
        c->put_vp8_bilinear_pixels_tab[0][0][0] = ff_put_vp8_pixels16_sse;
    }

    if (mm_flags & (AV_CPU_FLAG_SSE2|AV_CPU_FLAG_SSE2SLOW)) {
        VP8_LUMA_MC_FUNC(0, 16, sse2);
        VP8_MC_FUNC(1, 8, sse2);
        VP8_BILINEAR_MC_FUNC(0, 16, sse2);
        VP8_BILINEAR_MC_FUNC(1, 8, sse2);

        c->vp8_v_loop_filter_simple = ff_vp8_v_loop_filter_simple_sse2;

        c->vp8_v_loop_filter16y_inner = ff_vp8_v_loop_filter16y_inner_sse2;
        c->vp8_v_loop_filter8uv_inner = ff_vp8_v_loop_filter8uv_inner_sse2;

        c->vp8_v_loop_filter16y       = ff_vp8_v_loop_filter16y_mbedge_sse2;
        c->vp8_v_loop_filter8uv       = ff_vp8_v_loop_filter8uv_mbedge_sse2;
    }

    if (mm_flags & AV_CPU_FLAG_SSE2) {
        c->vp8_idct_dc_add4y          = ff_vp8_idct_dc_add4y_sse2;

        c->vp8_h_loop_filter_simple = ff_vp8_h_loop_filter_simple_sse2;

        c->vp8_h_loop_filter16y_inner = ff_vp8_h_loop_filter16y_inner_sse2;
        c->vp8_h_loop_filter8uv_inner = ff_vp8_h_loop_filter8uv_inner_sse2;

        c->vp8_h_loop_filter16y       = ff_vp8_h_loop_filter16y_mbedge_sse2;
        c->vp8_h_loop_filter8uv       = ff_vp8_h_loop_filter8uv_mbedge_sse2;
    }

    if (mm_flags & AV_CPU_FLAG_SSSE3) {
        VP8_LUMA_MC_FUNC(0, 16, ssse3);
        VP8_MC_FUNC(1, 8, ssse3);
        VP8_MC_FUNC(2, 4, ssse3);
        VP8_BILINEAR_MC_FUNC(0, 16, ssse3);
        VP8_BILINEAR_MC_FUNC(1, 8, ssse3);
        VP8_BILINEAR_MC_FUNC(2, 4, ssse3);

        c->vp8_v_loop_filter_simple = ff_vp8_v_loop_filter_simple_ssse3;
        c->vp8_h_loop_filter_simple = ff_vp8_h_loop_filter_simple_ssse3;

        c->vp8_v_loop_filter16y_inner = ff_vp8_v_loop_filter16y_inner_ssse3;
        c->vp8_h_loop_filter16y_inner = ff_vp8_h_loop_filter16y_inner_ssse3;
        c->vp8_v_loop_filter8uv_inner = ff_vp8_v_loop_filter8uv_inner_ssse3;
        c->vp8_h_loop_filter8uv_inner = ff_vp8_h_loop_filter8uv_inner_ssse3;

        c->vp8_v_loop_filter16y       = ff_vp8_v_loop_filter16y_mbedge_ssse3;
        c->vp8_h_loop_filter16y       = ff_vp8_h_loop_filter16y_mbedge_ssse3;
        c->vp8_v_loop_filter8uv       = ff_vp8_v_loop_filter8uv_mbedge_ssse3;
        c->vp8_h_loop_filter8uv       = ff_vp8_h_loop_filter8uv_mbedge_ssse3;
    }

    if (mm_flags & AV_CPU_FLAG_SSE4) {
        c->vp8_idct_dc_add                  = ff_vp8_idct_dc_add_sse4;

        c->vp8_h_loop_filter_simple   = ff_vp8_h_loop_filter_simple_sse4;
        c->vp8_h_loop_filter16y       = ff_vp8_h_loop_filter16y_mbedge_sse4;
        c->vp8_h_loop_filter8uv       = ff_vp8_h_loop_filter8uv_mbedge_sse4;
    }
#endif
}
