;*****************************************************************************
;* MMX/SSE2-optimized H.264 iDCT
;*****************************************************************************
;* Copyright (C) 2004-2005 Michael Niedermayer, Loren Merritt
;* Copyright (C) 2003-2008 x264 project
;*
;* Authors: Laurent Aimar <fenrir@via.ecp.fr>
;*          Loren Merritt <lorenm@u.washington.edu>
;*          Holger Lubitz <hal@duncan.ol.sub.de>
;*          Min Chen <chenm001.163.com>
;*
;* This file is part of FFmpeg.
;*
;* FFmpeg is free software; you can redistribute it and/or
;* modify it under the terms of the GNU Lesser General Public
;* License as published by the Free Software Foundation; either
;* version 2.1 of the License, or (at your option) any later version.
;*
;* FFmpeg is distributed in the hope that it will be useful,
;* but WITHOUT ANY WARRANTY; without even the implied warranty of
;* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
;* Lesser General Public License for more details.
;*
;* You should have received a copy of the GNU Lesser General Public
;* License along with FFmpeg; if not, write to the Free Software
;* 51, Inc., Foundation Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
;*****************************************************************************

%include "x86inc.asm"
%include "x86util.asm"

SECTION_RODATA

; FIXME this table is a duplicate from h264data.h, and will be removed once the tables from, h264 have been split
scan8_mem: db 4+1*8, 5+1*8, 4+2*8, 5+2*8
           db 6+1*8, 7+1*8, 6+2*8, 7+2*8
           db 4+3*8, 5+3*8, 4+4*8, 5+4*8
           db 6+3*8, 7+3*8, 6+4*8, 7+4*8
           db 1+1*8, 2+1*8
           db 1+2*8, 2+2*8
           db 1+4*8, 2+4*8
           db 1+5*8, 2+5*8
%ifdef PIC
%define scan8 r11
%else
%define scan8 scan8_mem
%endif

cextern pw_32

SECTION .text

; %1=uint8_t *dst, %2=int16_t *block, %3=int stride
%macro IDCT4_ADD 3
    ; Load dct coeffs
    movq         m0, [%2]
    movq         m1, [%2+8]
    movq         m2, [%2+16]
    movq         m3, [%2+24]

    IDCT4_1D      0, 1, 2, 3, 4, 5
    mova         m6, [pw_32]
    TRANSPOSE4x4W 0, 1, 2, 3, 4
    paddw        m0, m6
    IDCT4_1D      0, 1, 2, 3, 4, 5
    pxor         m7, m7

    STORE_DIFFx2 m0, m1, m4, m5, m7, 6, %1, %3
    lea          %1, [%1+%3*2]
    STORE_DIFFx2 m2, m3, m4, m5, m7, 6, %1, %3
%endmacro

INIT_MMX
; ff_h264_idct_add_mmx(uint8_t *dst, int16_t *block, int stride)
cglobal h264_idct_add_mmx, 3, 3, 0
    IDCT4_ADD    r0, r1, r2
    RET

%macro IDCT8_1D 2
    mova         m4, m5
    mova         m0, m1
    psraw        m4, 1
    psraw        m1, 1
    paddw        m4, m5
    paddw        m1, m0
    paddw        m4, m7
    paddw        m1, m5
    psubw        m4, m0
    paddw        m1, m3

    psubw        m0, m3
    psubw        m5, m3
    paddw        m0, m7
    psubw        m5, m7
    psraw        m3, 1
    psraw        m7, 1
    psubw        m0, m3
    psubw        m5, m7

    mova         m3, m4
    mova         m7, m1
    psraw        m1, 2
    psraw        m3, 2
    paddw        m3, m0
    psraw        m0, 2
    paddw        m1, m5
    psraw        m5, 2
    psubw        m0, m4
    psubw        m7, m5

    mova         m4, m2
    mova         m5, m6
    psraw        m4, 1
    psraw        m6, 1
    psubw        m4, m5
    paddw        m6, m2

    mova         m2, %1
    mova         m5, %2
    SUMSUB_BA    m5, m2
    SUMSUB_BA    m6, m5
    SUMSUB_BA    m4, m2
    SUMSUB_BA    m7, m6
    SUMSUB_BA    m0, m4
    SUMSUB_BA    m3, m2
    SUMSUB_BA    m1, m5
    SWAP          7, 6, 4, 5, 2, 3, 1, 0 ; 70315246 -> 01234567
%endmacro

%macro IDCT8_1D_FULL 1
    mova         m7, [%1+112]
    mova         m6, [%1+ 96]
    mova         m5, [%1+ 80]
    mova         m3, [%1+ 48]
    mova         m2, [%1+ 32]
    mova         m1, [%1+ 16]
    IDCT8_1D   [%1], [%1+ 64]
%endmacro

; %1=int16_t *block, %2=int16_t *dstblock
%macro IDCT8_ADD_MMX_START 2
    IDCT8_1D_FULL %1
    mova       [%1], m7
    TRANSPOSE4x4W 0, 1, 2, 3, 7
    mova         m7, [%1]
    mova    [%2   ], m0
    mova    [%2+16], m1
    mova    [%2+32], m2
    mova    [%2+48], m3
    TRANSPOSE4x4W 4, 5, 6, 7, 3
    mova    [%2+ 8], m4
    mova    [%2+24], m5
    mova    [%2+40], m6
    mova    [%2+56], m7
%endmacro

; %1=uint8_t *dst, %2=int16_t *block, %3=int stride
%macro IDCT8_ADD_MMX_END 3
    IDCT8_1D_FULL %2
    mova    [%2   ], m5
    mova    [%2+16], m6
    mova    [%2+32], m7

    pxor         m7, m7
    STORE_DIFFx2 m0, m1, m5, m6, m7, 6, %1, %3
    lea          %1, [%1+%3*2]
    STORE_DIFFx2 m2, m3, m5, m6, m7, 6, %1, %3
    mova         m0, [%2   ]
    mova         m1, [%2+16]
    mova         m2, [%2+32]
    lea          %1, [%1+%3*2]
    STORE_DIFFx2 m4, m0, m5, m6, m7, 6, %1, %3
    lea          %1, [%1+%3*2]
    STORE_DIFFx2 m1, m2, m5, m6, m7, 6, %1, %3
%endmacro

INIT_MMX
; ff_h264_idct8_add_mmx(uint8_t *dst, int16_t *block, int stride)
cglobal h264_idct8_add_mmx, 3, 4, 0
    %assign pad 128+4-(stack_offset&7)
    SUB         rsp, pad

    add   word [r1], 32
    IDCT8_ADD_MMX_START r1  , rsp
    IDCT8_ADD_MMX_START r1+8, rsp+64
    lea          r3, [r0+4]
    IDCT8_ADD_MMX_END   r0  , rsp,   r2
    IDCT8_ADD_MMX_END   r3  , rsp+8, r2

    ADD         rsp, pad
    RET

; %1=uint8_t *dst, %2=int16_t *block, %3=int stride
%macro IDCT8_ADD_SSE 4
    IDCT8_1D_FULL %2
%ifdef ARCH_X86_64
    TRANSPOSE8x8W 0, 1, 2, 3, 4, 5, 6, 7, 8
%else
    TRANSPOSE8x8W 0, 1, 2, 3, 4, 5, 6, 7, [%2], [%2+16]
%endif
    paddw        m0, [pw_32]

%ifndef ARCH_X86_64
    mova    [%2   ], m0
    mova    [%2+16], m4
    IDCT8_1D   [%2], [%2+ 16]
    mova    [%2   ], m6
    mova    [%2+16], m7
%else
    SWAP          0, 8
    SWAP          4, 9
    IDCT8_1D     m8, m9
    SWAP          6, 8
    SWAP          7, 9
%endif

    pxor         m7, m7
    lea          %4, [%3*3]
    STORE_DIFF   m0, m6, m7, [%1     ]
    STORE_DIFF   m1, m6, m7, [%1+%3  ]
    STORE_DIFF   m2, m6, m7, [%1+%3*2]
    STORE_DIFF   m3, m6, m7, [%1+%4  ]
%ifndef ARCH_X86_64
    mova         m0, [%2   ]
    mova         m1, [%2+16]
%else
    SWAP          0, 8
    SWAP          1, 9
%endif
    lea          %1, [%1+%3*4]
    STORE_DIFF   m4, m6, m7, [%1     ]
    STORE_DIFF   m5, m6, m7, [%1+%3  ]
    STORE_DIFF   m0, m6, m7, [%1+%3*2]
    STORE_DIFF   m1, m6, m7, [%1+%4  ]
%endmacro

INIT_XMM
; ff_h264_idct8_add_sse2(uint8_t *dst, int16_t *block, int stride)
cglobal h264_idct8_add_sse2, 3, 4, 10
    IDCT8_ADD_SSE r0, r1, r2, r3
    RET

%macro DC_ADD_MMX2_INIT 2-3
%if %0 == 2
    movsx        %1, word [%1]
    add          %1, 32
    sar          %1, 6
    movd         m0, %1d
    lea          %1, [%2*3]
%else
    add          %3, 32
    sar          %3, 6
    movd         m0, %3d
    lea          %3, [%2*3]
%endif
    pshufw       m0, m0, 0
    pxor         m1, m1
    psubw        m1, m0
    packuswb     m0, m0
    packuswb     m1, m1
%endmacro

%macro DC_ADD_MMX2_OP 3-4
    %1           m2, [%2     ]
    %1           m3, [%2+%3  ]
    %1           m4, [%2+%3*2]
    %1           m5, [%2+%4  ]
    paddusb      m2, m0
    paddusb      m3, m0
    paddusb      m4, m0
    paddusb      m5, m0
    psubusb      m2, m1
    psubusb      m3, m1
    psubusb      m4, m1
    psubusb      m5, m1
    %1    [%2     ], m2
    %1    [%2+%3  ], m3
    %1    [%2+%3*2], m4
    %1    [%2+%4  ], m5
%endmacro

INIT_MMX
; ff_h264_idct_dc_add_mmx2(uint8_t *dst, int16_t *block, int stride)
cglobal h264_idct_dc_add_mmx2, 3, 3, 0
    DC_ADD_MMX2_INIT r1, r2
    DC_ADD_MMX2_OP movh, r0, r2, r1
    RET

; ff_h264_idct8_dc_add_mmx2(uint8_t *dst, int16_t *block, int stride)
cglobal h264_idct8_dc_add_mmx2, 3, 3, 0
    DC_ADD_MMX2_INIT r1, r2
    DC_ADD_MMX2_OP mova, r0, r2, r1
    lea          r0, [r0+r2*4]
    DC_ADD_MMX2_OP mova, r0, r2, r1
    RET

; ff_h264_idct_add16_mmx(uint8_t *dst, const int *block_offset,
;             DCTELEM *block, int stride, const uint8_t nnzc[6*8])
cglobal h264_idct_add16_mmx, 5, 7, 0
    xor          r5, r5
%ifdef PIC
    lea         r11, [scan8_mem]
%endif
.nextblock
    movzx        r6, byte [scan8+r5]
    movzx        r6, byte [r4+r6]
    test         r6, r6
    jz .skipblock
    mov         r6d, dword [r1+r5*4]
    lea          r6, [r0+r6]
    IDCT4_ADD    r6, r2, r3
.skipblock
    inc          r5
    add          r2, 32
    cmp          r5, 16
    jl .nextblock
    REP_RET

; ff_h264_idct8_add4_mmx(uint8_t *dst, const int *block_offset,
;                        DCTELEM *block, int stride, const uint8_t nnzc[6*8])
cglobal h264_idct8_add4_mmx, 5, 7, 0
    %assign pad 128+4-(stack_offset&7)
    SUB         rsp, pad

    xor          r5, r5
%ifdef PIC
    lea         r11, [scan8_mem]
%endif
.nextblock
    movzx        r6, byte [scan8+r5]
    movzx        r6, byte [r4+r6]
    test         r6, r6
    jz .skipblock
    mov         r6d, dword [r1+r5*4]
    lea          r6, [r0+r6]
    add   word [r2], 32
    IDCT8_ADD_MMX_START r2  , rsp
    IDCT8_ADD_MMX_START r2+8, rsp+64
    IDCT8_ADD_MMX_END   r6  , rsp,   r3
    mov         r6d, dword [r1+r5*4]
    lea          r6, [r0+r6+4]
    IDCT8_ADD_MMX_END   r6  , rsp+8, r3
.skipblock
    add          r5, 4
    add          r2, 128
    cmp          r5, 16
    jl .nextblock
    ADD         rsp, pad
    RET

; ff_h264_idct_add16_mmx2(uint8_t *dst, const int *block_offset,
;                         DCTELEM *block, int stride, const uint8_t nnzc[6*8])
cglobal h264_idct_add16_mmx2, 5, 7, 0
    xor          r5, r5
%ifdef PIC
    lea         r11, [scan8_mem]
%endif
.nextblock
    movzx        r6, byte [scan8+r5]
    movzx        r6, byte [r4+r6]
    test         r6, r6
    jz .skipblock
    cmp          r6, 1
    jnz .no_dc
    movsx        r6, word [r2]
    test         r6, r6
    jz .no_dc
    DC_ADD_MMX2_INIT r2, r3, r6
%ifdef ARCH_X86_64
%define dst_reg  r10
%define dst_regd r10d
%else
%define dst_reg  r1
%define dst_regd r1d
%endif
    mov    dst_regd, dword [r1+r5*4]
    lea     dst_reg, [r0+dst_reg]
    DC_ADD_MMX2_OP movh, dst_reg, r3, r6
%ifndef ARCH_X86_64
    mov          r1, r1m
%endif
    inc          r5
    add          r2, 32
    cmp          r5, 16
    jl .nextblock
    REP_RET
.no_dc
    mov         r6d, dword [r1+r5*4]
    lea          r6, [r0+r6]
    IDCT4_ADD    r6, r2, r3
.skipblock
    inc          r5
    add          r2, 32
    cmp          r5, 16
    jl .nextblock
    REP_RET

; ff_h264_idct_add16intra_mmx(uint8_t *dst, const int *block_offset,
;                             DCTELEM *block, int stride, const uint8_t nnzc[6*8])
cglobal h264_idct_add16intra_mmx, 5, 7, 0
    xor          r5, r5
%ifdef PIC
    lea         r11, [scan8_mem]
%endif
.nextblock
    movzx        r6, byte [scan8+r5]
    movzx        r6, byte [r4+r6]
    or          r6w, word [r2]
    test         r6, r6
    jz .skipblock
    mov         r6d, dword [r1+r5*4]
    lea          r6, [r0+r6]
    IDCT4_ADD    r6, r2, r3
.skipblock
    inc          r5
    add          r2, 32
    cmp          r5, 16
    jl .nextblock
    REP_RET

; ff_h264_idct_add16intra_mmx2(uint8_t *dst, const int *block_offset,
;                              DCTELEM *block, int stride, const uint8_t nnzc[6*8])
cglobal h264_idct_add16intra_mmx2, 5, 7, 0
    xor          r5, r5
%ifdef PIC
    lea         r11, [scan8_mem]
%endif
.nextblock
    movzx        r6, byte [scan8+r5]
    movzx        r6, byte [r4+r6]
    test         r6, r6
    jz .try_dc
    mov         r6d, dword [r1+r5*4]
    lea          r6, [r0+r6]
    IDCT4_ADD    r6, r2, r3
    inc          r5
    add          r2, 32
    cmp          r5, 16
    jl .nextblock
    REP_RET
.try_dc
    movsx        r6, word [r2]
    test         r6, r6
    jz .skipblock
    DC_ADD_MMX2_INIT r2, r3, r6
%ifdef ARCH_X86_64
%define dst_reg  r10
%define dst_regd r10d
%else
%define dst_reg  r1
%define dst_regd r1d
%endif
    mov    dst_regd, dword [r1+r5*4]
    lea     dst_reg, [r0+dst_reg]
    DC_ADD_MMX2_OP movh, dst_reg, r3, r6
%ifndef ARCH_X86_64
    mov          r1, r1m
%endif
.skipblock
    inc          r5
    add          r2, 32
    cmp          r5, 16
    jl .nextblock
    REP_RET

; ff_h264_idct8_add4_mmx2(uint8_t *dst, const int *block_offset,
;                         DCTELEM *block, int stride, const uint8_t nnzc[6*8])
cglobal h264_idct8_add4_mmx2, 5, 7, 0
    %assign pad 128+4-(stack_offset&7)
    SUB         rsp, pad

    xor          r5, r5
%ifdef PIC
    lea         r11, [scan8_mem]
%endif
.nextblock
    movzx        r6, byte [scan8+r5]
    movzx        r6, byte [r4+r6]
    test         r6, r6
    jz .skipblock
    cmp          r6, 1
    jnz .no_dc
    movsx        r6, word [r2]
    test         r6, r6
    jz .no_dc
    DC_ADD_MMX2_INIT r2, r3, r6
%ifdef ARCH_X86_64
%define dst_reg  r10
%define dst_regd r10d
%else
%define dst_reg  r1
%define dst_regd r1d
%endif
    mov    dst_regd, dword [r1+r5*4]
    lea     dst_reg, [r0+dst_reg]
    DC_ADD_MMX2_OP mova, dst_reg, r3, r6
    lea     dst_reg, [dst_reg+r3*4]
    DC_ADD_MMX2_OP mova, dst_reg, r3, r6
%ifndef ARCH_X86_64
    mov          r1, r1m
%endif
    add          r5, 4
    add          r2, 128
    cmp          r5, 16
    jl .nextblock

    ADD         rsp, pad
    RET
.no_dc
    mov         r6d, dword [r1+r5*4]
    lea          r6, [r0+r6]
    add   word [r2], 32
    IDCT8_ADD_MMX_START r2  , rsp
    IDCT8_ADD_MMX_START r2+8, rsp+64
    IDCT8_ADD_MMX_END   r6  , rsp,   r3
    mov         r6d, dword [r1+r5*4]
    lea          r6, [r0+r6+4]
    IDCT8_ADD_MMX_END   r6  , rsp+8, r3
.skipblock
    add          r5, 4
    add          r2, 128
    cmp          r5, 16
    jl .nextblock

    ADD         rsp, pad
    RET

INIT_XMM
; ff_h264_idct8_add4_sse2(uint8_t *dst, const int *block_offset,
;                         DCTELEM *block, int stride, const uint8_t nnzc[6*8])
cglobal h264_idct8_add4_sse2, 5, 7, 10
    xor          r5, r5
%ifdef PIC
    lea         r11, [scan8_mem]
%endif
.nextblock
    movzx        r6, byte [scan8+r5]
    movzx        r6, byte [r4+r6]
    test         r6, r6
    jz .skipblock
    cmp          r6, 1
    jnz .no_dc
    movsx        r6, word [r2]
    test         r6, r6
    jz .no_dc
INIT_MMX
    DC_ADD_MMX2_INIT r2, r3, r6
%ifdef ARCH_X86_64
%define dst_reg  r10
%define dst_regd r10d
%else
%define dst_reg  r1
%define dst_regd r1d
%endif
    mov    dst_regd, dword [r1+r5*4]
    lea     dst_reg, [r0+dst_reg]
    DC_ADD_MMX2_OP mova, dst_reg, r3, r6
    lea     dst_reg, [dst_reg+r3*4]
    DC_ADD_MMX2_OP mova, dst_reg, r3, r6
%ifndef ARCH_X86_64
    mov          r1, r1m
%endif
    add          r5, 4
    add          r2, 128
    cmp          r5, 16
    jl .nextblock
    REP_RET
.no_dc
INIT_XMM
    mov    dst_regd, dword [r1+r5*4]
    lea     dst_reg, [r0+dst_reg]
    IDCT8_ADD_SSE dst_reg, r2, r3, r6
%ifndef ARCH_X86_64
    mov          r1, r1m
%endif
.skipblock
    add          r5, 4
    add          r2, 128
    cmp          r5, 16
    jl .nextblock
    REP_RET

INIT_MMX
h264_idct_add8_mmx_plane:
.nextblock
    movzx        r6, byte [scan8+r5]
    movzx        r6, byte [r4+r6]
    or          r6w, word [r2]
    test         r6, r6
    jz .skipblock
%ifdef ARCH_X86_64
    mov         r0d, dword [r1+r5*4]
    add          r0, [r10]
%else
    mov          r0, r1m ; XXX r1m here is actually r0m of the calling func
    mov          r0, [r0]
    add          r0, dword [r1+r5*4]
%endif
    IDCT4_ADD    r0, r2, r3
.skipblock
    inc          r5
    add          r2, 32
    test         r5, 3
    jnz .nextblock
    rep ret

; ff_h264_idct_add8_mmx(uint8_t **dest, const int *block_offset,
;                       DCTELEM *block, int stride, const uint8_t nnzc[6*8])
cglobal h264_idct_add8_mmx, 5, 7, 0
    mov          r5, 16
    add          r2, 512
%ifdef PIC
    lea         r11, [scan8_mem]
%endif
%ifdef ARCH_X86_64
    mov         r10, r0
%endif
    call         h264_idct_add8_mmx_plane
%ifdef ARCH_X86_64
    add         r10, gprsize
%else
    add        r0mp, gprsize
%endif
    call         h264_idct_add8_mmx_plane
    RET

h264_idct_add8_mmx2_plane
.nextblock
    movzx        r6, byte [scan8+r5]
    movzx        r6, byte [r4+r6]
    test         r6, r6
    jz .try_dc
%ifdef ARCH_X86_64
    mov         r0d, dword [r1+r5*4]
    add          r0, [r10]
%else
    mov          r0, r1m ; XXX r1m here is actually r0m of the calling func
    mov          r0, [r0]
    add          r0, dword [r1+r5*4]
%endif
    IDCT4_ADD    r0, r2, r3
    inc          r5
    add          r2, 32
    test         r5, 3
    jnz .nextblock
    rep ret
.try_dc
    movsx        r6, word [r2]
    test         r6, r6
    jz .skipblock
    DC_ADD_MMX2_INIT r2, r3, r6
%ifdef ARCH_X86_64
    mov         r0d, dword [r1+r5*4]
    add          r0, [r10]
%else
    mov          r0, r1m ; XXX r1m here is actually r0m of the calling func
    mov          r0, [r0]
    add          r0, dword [r1+r5*4]
%endif
    DC_ADD_MMX2_OP movh, r0, r3, r6
.skipblock
    inc          r5
    add          r2, 32
    test         r5, 3
    jnz .nextblock
    rep ret

; ff_h264_idct_add8_mmx2(uint8_t **dest, const int *block_offset,
;                        DCTELEM *block, int stride, const uint8_t nnzc[6*8])
cglobal h264_idct_add8_mmx2, 5, 7, 0
    mov          r5, 16
    add          r2, 512
%ifdef ARCH_X86_64
    mov         r10, r0
%endif
%ifdef PIC
    lea         r11, [scan8_mem]
%endif
    call h264_idct_add8_mmx2_plane
%ifdef ARCH_X86_64
    add         r10, gprsize
%else
    add        r0mp, gprsize
%endif
    call h264_idct_add8_mmx2_plane
    RET

INIT_MMX
; r0 = uint8_t *dst, r2 = int16_t *block, r3 = int stride, r6=clobbered
h264_idct_dc_add8_mmx2:
    movd         m0, [r2   ]          ;  0 0 X D
    punpcklwd    m0, [r2+32]          ;  x X d D
    paddsw       m0, [pw_32]
    psraw        m0, 6
    punpcklwd    m0, m0               ;  d d D D
    pxor         m1, m1               ;  0 0 0 0
    psubw        m1, m0               ; -d-d-D-D
    packuswb     m0, m1               ; -d-d-D-D d d D D
    pshufw       m1, m0, 0xFA         ; -d-d-d-d-D-D-D-D
    punpcklwd    m0, m0               ;  d d d d D D D D
    lea          r6, [r3*3]
    DC_ADD_MMX2_OP movq, r0, r3, r6
    ret

ALIGN 16
INIT_XMM
; r0 = uint8_t *dst (clobbered), r2 = int16_t *block, r3 = int stride
x264_add8x4_idct_sse2:
    movq   m0, [r2+ 0]
    movq   m1, [r2+ 8]
    movq   m2, [r2+16]
    movq   m3, [r2+24]
    movhps m0, [r2+32]
    movhps m1, [r2+40]
    movhps m2, [r2+48]
    movhps m3, [r2+56]
    IDCT4_1D 0,1,2,3,4,5
    TRANSPOSE2x4x4W 0,1,2,3,4
    paddw m0, [pw_32]
    IDCT4_1D 0,1,2,3,4,5
    pxor  m7, m7
    STORE_DIFFx2 m0, m1, m4, m5, m7, 6, r0, r3
    lea   r0, [r0+r3*2]
    STORE_DIFFx2 m2, m3, m4, m5, m7, 6, r0, r3
    ret

%macro add16_sse2_cycle 2
    movzx       r0, word [r4+%2]
    test        r0, r0
    jz .cycle%1end
    mov        r0d, dword [r1+%1*8]
%ifdef ARCH_X86_64
    add         r0, r10
%else
    add         r0, r0m
%endif
    call        x264_add8x4_idct_sse2
.cycle%1end
%if %1 < 7
    add         r2, 64
%endif
%endmacro

; ff_h264_idct_add16_sse2(uint8_t *dst, const int *block_offset,
;                         DCTELEM *block, int stride, const uint8_t nnzc[6*8])
cglobal h264_idct_add16_sse2, 5, 5, 8
%ifdef ARCH_X86_64
    mov        r10, r0
%endif
    ; unrolling of the loop leads to an average performance gain of
    ; 20-25%
    add16_sse2_cycle 0, 0xc
    add16_sse2_cycle 1, 0x14
    add16_sse2_cycle 2, 0xe
    add16_sse2_cycle 3, 0x16
    add16_sse2_cycle 4, 0x1c
    add16_sse2_cycle 5, 0x24
    add16_sse2_cycle 6, 0x1e
    add16_sse2_cycle 7, 0x26
    RET

%macro add16intra_sse2_cycle 2
    movzx       r0, word [r4+%2]
    test        r0, r0
    jz .try%1dc
    mov        r0d, dword [r1+%1*8]
%ifdef ARCH_X86_64
    add         r0, r10
%else
    add         r0, r0m
%endif
    call        x264_add8x4_idct_sse2
    jmp .cycle%1end
.try%1dc
    movsx       r0, word [r2   ]
    or         r0w, word [r2+32]
    jz .cycle%1end
    mov        r0d, dword [r1+%1*8]
%ifdef ARCH_X86_64
    add         r0, r10
%else
    add         r0, r0m
%endif
    call        h264_idct_dc_add8_mmx2
.cycle%1end
%if %1 < 7
    add         r2, 64
%endif
%endmacro

; ff_h264_idct_add16intra_sse2(uint8_t *dst, const int *block_offset,
;                              DCTELEM *block, int stride, const uint8_t nnzc[6*8])
cglobal h264_idct_add16intra_sse2, 5, 7, 8
%ifdef ARCH_X86_64
    mov        r10, r0
%endif
    add16intra_sse2_cycle 0, 0xc
    add16intra_sse2_cycle 1, 0x14
    add16intra_sse2_cycle 2, 0xe
    add16intra_sse2_cycle 3, 0x16
    add16intra_sse2_cycle 4, 0x1c
    add16intra_sse2_cycle 5, 0x24
    add16intra_sse2_cycle 6, 0x1e
    add16intra_sse2_cycle 7, 0x26
    RET

%macro add8_sse2_cycle 2
    movzx       r0, word [r4+%2]
    test        r0, r0
    jz .try%1dc
%ifdef ARCH_X86_64
    mov        r0d, dword [r1+%1*8+64]
    add         r0, [r10]
%else
    mov         r0, r0m
    mov         r0, [r0]
    add         r0, dword [r1+%1*8+64]
%endif
    call        x264_add8x4_idct_sse2
    jmp .cycle%1end
.try%1dc
    movsx       r0, word [r2   ]
    or         r0w, word [r2+32]
    jz .cycle%1end
%ifdef ARCH_X86_64
    mov        r0d, dword [r1+%1*8+64]
    add         r0, [r10]
%else
    mov         r0, r0m
    mov         r0, [r0]
    add         r0, dword [r1+%1*8+64]
%endif
    call        h264_idct_dc_add8_mmx2
.cycle%1end
%if %1 < 3
    add         r2, 64
%endif
%endmacro

; ff_h264_idct_add8_sse2(uint8_t **dest, const int *block_offset,
;                        DCTELEM *block, int stride, const uint8_t nnzc[6*8])
cglobal h264_idct_add8_sse2, 5, 7, 8
    add          r2, 512
%ifdef ARCH_X86_64
    mov         r10, r0
%endif
    add8_sse2_cycle 0, 0x09
    add8_sse2_cycle 1, 0x11
%ifdef ARCH_X86_64
    add         r10, gprsize
%else
    add        r0mp, gprsize
%endif
    add8_sse2_cycle 2, 0x21
    add8_sse2_cycle 3, 0x29
    RET
