;
;  Copyright (c) 2010 The WebM project authors. All Rights Reserved.
;
;  Use of this source code is governed by a BSD-style license
;  that can be found in the LICENSE file in the root of the source
;  tree. An additional intellectual property rights grant can be found
;  in the file PATENTS.  All contributing project authors may
;  be found in the AUTHORS file in the root of the source tree.
;


%include "vpx_ports/x86_abi_support.asm"

;macro in deblock functions
%macro FIRST_2_ROWS 0
        movdqa      xmm4,       xmm0
        movdqa      xmm6,       xmm0
        movdqa      xmm5,       xmm1
        pavgb       xmm5,       xmm3

        ;calculate absolute value
        psubusb     xmm4,       xmm1
        psubusb     xmm1,       xmm0
        psubusb     xmm6,       xmm3
        psubusb     xmm3,       xmm0
        paddusb     xmm4,       xmm1
        paddusb     xmm6,       xmm3

        ;get threshold
        movdqa      xmm2,       flimit
        pxor        xmm1,       xmm1
        movdqa      xmm7,       xmm2

        ;get mask
        psubusb     xmm2,       xmm4
        psubusb     xmm7,       xmm6
        pcmpeqb     xmm2,       xmm1
        pcmpeqb     xmm7,       xmm1
        por         xmm7,       xmm2
%endmacro

%macro SECOND_2_ROWS 0
        movdqa      xmm6,       xmm0
        movdqa      xmm4,       xmm0
        movdqa      xmm2,       xmm1
        pavgb       xmm1,       xmm3

        ;calculate absolute value
        psubusb     xmm6,       xmm2
        psubusb     xmm2,       xmm0
        psubusb     xmm4,       xmm3
        psubusb     xmm3,       xmm0
        paddusb     xmm6,       xmm2
        paddusb     xmm4,       xmm3

        pavgb       xmm5,       xmm1

        ;get threshold
        movdqa      xmm2,       flimit
        pxor        xmm1,       xmm1
        movdqa      xmm3,       xmm2

        ;get mask
        psubusb     xmm2,       xmm6
        psubusb     xmm3,       xmm4
        pcmpeqb     xmm2,       xmm1
        pcmpeqb     xmm3,       xmm1

        por         xmm7,       xmm2
        por         xmm7,       xmm3

        pavgb       xmm5,       xmm0

        ;decide if or not to use filtered value
        pand        xmm0,       xmm7
        pandn       xmm7,       xmm5
        paddusb     xmm0,       xmm7
%endmacro

%macro UPDATE_FLIMIT 0
        movdqa      xmm2,       XMMWORD PTR [rbx]
        movdqa      [rsp],      xmm2
        add         rbx,        16
%endmacro

;void vp8_post_proc_down_and_across_mb_row_sse2
;(
;    unsigned char *src_ptr,
;    unsigned char *dst_ptr,
;    int src_pixels_per_line,
;    int dst_pixels_per_line,
;    int cols,
;    int *flimits,
;    int size
;)
global sym(vp8_post_proc_down_and_across_mb_row_sse2) PRIVATE
sym(vp8_post_proc_down_and_across_mb_row_sse2):
    push        rbp
    mov         rbp, rsp
    SHADOW_ARGS_TO_STACK 7
    SAVE_XMM 7
    push        rbx
    push        rsi
    push        rdi
    ; end prolog
    ALIGN_STACK 16, rax
    sub         rsp, 16

        ; put flimit on stack
        mov         rbx,        arg(5)           ;flimits ptr
        UPDATE_FLIMIT

%define flimit [rsp]

        mov         rsi,        arg(0)           ;src_ptr
        mov         rdi,        arg(1)           ;dst_ptr

        movsxd      rax,        DWORD PTR arg(2) ;src_pixels_per_line
        movsxd      rcx,        DWORD PTR arg(6) ;rows in a macroblock
.nextrow:
        xor         rdx,        rdx              ;col
.nextcol:
        ;load current and next 2 rows
        movdqu      xmm0,       XMMWORD PTR [rsi]
        movdqu      xmm1,       XMMWORD PTR [rsi + rax]
        movdqu      xmm3,       XMMWORD PTR [rsi + 2*rax]

        FIRST_2_ROWS

        ;load above 2 rows
        neg         rax
        movdqu      xmm1,       XMMWORD PTR [rsi + 2*rax]
        movdqu      xmm3,       XMMWORD PTR [rsi + rax]

        SECOND_2_ROWS

        movdqu      XMMWORD PTR [rdi], xmm0

        neg         rax                          ; positive stride
        add         rsi,        16
        add         rdi,        16

        add         rdx,        16
        cmp         edx,        dword arg(4)     ;cols
        jge         .downdone
        UPDATE_FLIMIT
        jmp         .nextcol

.downdone:
        ; done with the all cols, start the across filtering in place
        sub         rsi,        rdx
        sub         rdi,        rdx

        mov         rbx,        arg(5) ; flimits
        UPDATE_FLIMIT

        ; dup the first byte into the left border 8 times
        movq        mm1,   [rdi]
        punpcklbw   mm1,   mm1
        punpcklwd   mm1,   mm1
        punpckldq   mm1,   mm1
        mov         rdx,    -8
        movq        [rdi+rdx], mm1

        ; dup the last byte into the right border
        movsxd      rdx,    dword arg(4)
        movq        mm1,   [rdi + rdx + -1]
        punpcklbw   mm1,   mm1
        punpcklwd   mm1,   mm1
        punpckldq   mm1,   mm1
        movq        [rdi+rdx], mm1

        xor         rdx,        rdx
        movq        mm0,        QWORD PTR [rdi-16];
        movq        mm1,        QWORD PTR [rdi-8];

.acrossnextcol:
        movdqu      xmm0,       XMMWORD PTR [rdi + rdx]
        movdqu      xmm1,       XMMWORD PTR [rdi + rdx -2]
        movdqu      xmm3,       XMMWORD PTR [rdi + rdx -1]

        FIRST_2_ROWS

        movdqu      xmm1,       XMMWORD PTR [rdi + rdx +1]
        movdqu      xmm3,       XMMWORD PTR [rdi + rdx +2]

        SECOND_2_ROWS

        movq        QWORD PTR [rdi+rdx-16], mm0  ; store previous 8 bytes
        movq        QWORD PTR [rdi+rdx-8], mm1   ; store previous 8 bytes
        movdq2q     mm0,        xmm0
        psrldq      xmm0,       8
        movdq2q     mm1,        xmm0

        add         rdx,        16
        cmp         edx,        dword arg(4)     ;cols
        jge         .acrossdone
        UPDATE_FLIMIT
        jmp         .acrossnextcol

.acrossdone
        ; last 16 pixels
        movq        QWORD PTR [rdi+rdx-16], mm0

        cmp         edx,        dword arg(4)
        jne         .throw_last_8
        movq        QWORD PTR [rdi+rdx-8], mm1
.throw_last_8:
        ; done with this rwo
        add         rsi,rax                      ;next src line
        mov         eax, dword arg(3)            ;dst_pixels_per_line
        add         rdi,rax                      ;next destination
        mov         eax, dword arg(2)            ;src_pixels_per_line

        mov         rbx,        arg(5)           ;flimits
        UPDATE_FLIMIT

        dec         rcx                          ;decrement count
        jnz         .nextrow                     ;next row

    add rsp, 16
    pop rsp
    ; begin epilog
    pop rdi
    pop rsi
    pop rbx
    RESTORE_XMM
    UNSHADOW_ARGS
    pop         rbp
    ret
%undef flimit

;void vp8_mbpost_proc_down_xmm(unsigned char *dst,
;                            int pitch, int rows, int cols,int flimit)
extern sym(vp8_rv)
global sym(vp8_mbpost_proc_down_xmm) PRIVATE
sym(vp8_mbpost_proc_down_xmm):
    push        rbp
    mov         rbp, rsp
    SHADOW_ARGS_TO_STACK 5
    SAVE_XMM 7
    GET_GOT     rbx
    push        rsi
    push        rdi
    ; end prolog

    ALIGN_STACK 16, rax
    sub         rsp, 128+16

    ; unsigned char d[16][8] at [rsp]
    ; create flimit2 at [rsp+128]
    mov         eax, dword ptr arg(4) ;flimit
    mov         [rsp+128], eax
    mov         [rsp+128+4], eax
    mov         [rsp+128+8], eax
    mov         [rsp+128+12], eax
%define flimit4 [rsp+128]

%if ABI_IS_32BIT=0
    lea         r8,       [GLOBAL(sym(vp8_rv))]
%endif

    ;rows +=8;
    add         dword arg(2), 8

    ;for(c=0; c<cols; c+=8)
.loop_col:
            mov         rsi,        arg(0) ; s
            pxor        xmm0,       xmm0        ;

            movsxd      rax,        dword ptr arg(1) ;pitch       ;

            ; this copies the last row down into the border 8 rows
            mov         rdi,        rsi
            mov         rdx,        arg(2)
            sub         rdx,        9
            imul        rdx,        rax
            lea         rdi,        [rdi+rdx]
            movq        xmm1,       QWORD ptr[rdi]              ; first row
            mov         rcx,        8
.init_borderd                                                    ; initialize borders
            lea         rdi,        [rdi + rax]
            movq        [rdi],      xmm1

            dec         rcx
            jne         .init_borderd

            neg         rax                                     ; rax = -pitch

            ; this copies the first row up into the border 8 rows
            mov         rdi,        rsi
            movq        xmm1,       QWORD ptr[rdi]              ; first row
            mov         rcx,        8
.init_border                                                    ; initialize borders
            lea         rdi,        [rdi + rax]
            movq        [rdi],      xmm1

            dec         rcx
            jne         .init_border



            lea         rsi,        [rsi + rax*8];              ; rdi = s[-pitch*8]
            neg         rax

            pxor        xmm5,       xmm5
            pxor        xmm6,       xmm6        ;

            pxor        xmm7,       xmm7        ;
            mov         rdi,        rsi

            mov         rcx,        15          ;

.loop_initvar:
            movq        xmm1,       QWORD PTR [rdi];
            punpcklbw   xmm1,       xmm0        ;

            paddw       xmm5,       xmm1        ;
            pmullw      xmm1,       xmm1        ;

            movdqa      xmm2,       xmm1        ;
            punpcklwd   xmm1,       xmm0        ;

            punpckhwd   xmm2,       xmm0        ;
            paddd       xmm6,       xmm1        ;

            paddd       xmm7,       xmm2        ;
            lea         rdi,        [rdi+rax]   ;

            dec         rcx
            jne         .loop_initvar
            ;save the var and sum
            xor         rdx,        rdx
.loop_row:
            movq        xmm1,       QWORD PTR [rsi]     ; [s-pitch*8]
            movq        xmm2,       QWORD PTR [rdi]     ; [s+pitch*7]

            punpcklbw   xmm1,       xmm0
            punpcklbw   xmm2,       xmm0

            paddw       xmm5,       xmm2
            psubw       xmm5,       xmm1

            pmullw      xmm2,       xmm2
            movdqa      xmm4,       xmm2

            punpcklwd   xmm2,       xmm0
            punpckhwd   xmm4,       xmm0

            paddd       xmm6,       xmm2
            paddd       xmm7,       xmm4

            pmullw      xmm1,       xmm1
            movdqa      xmm2,       xmm1

            punpcklwd   xmm1,       xmm0
            psubd       xmm6,       xmm1

            punpckhwd   xmm2,       xmm0
            psubd       xmm7,       xmm2


            movdqa      xmm3,       xmm6
            pslld       xmm3,       4

            psubd       xmm3,       xmm6
            movdqa      xmm1,       xmm5

            movdqa      xmm4,       xmm5
            pmullw      xmm1,       xmm1

            pmulhw      xmm4,       xmm4
            movdqa      xmm2,       xmm1

            punpcklwd   xmm1,       xmm4
            punpckhwd   xmm2,       xmm4

            movdqa      xmm4,       xmm7
            pslld       xmm4,       4

            psubd       xmm4,       xmm7

            psubd       xmm3,       xmm1
            psubd       xmm4,       xmm2

            psubd       xmm3,       flimit4
            psubd       xmm4,       flimit4

            psrad       xmm3,       31
            psrad       xmm4,       31

            packssdw    xmm3,       xmm4
            packsswb    xmm3,       xmm0

            movq        xmm1,       QWORD PTR [rsi+rax*8]

            movq        xmm2,       xmm1
            punpcklbw   xmm1,       xmm0

            paddw       xmm1,       xmm5
            mov         rcx,        rdx

            and         rcx,        127
%if ABI_IS_32BIT=1 && CONFIG_PIC=1
            push        rax
            lea         rax,        [GLOBAL(sym(vp8_rv))]
            movdqu      xmm4,       [rax + rcx*2] ;vp8_rv[rcx*2]
            pop         rax
%elif ABI_IS_32BIT=0
            movdqu      xmm4,       [r8 + rcx*2] ;vp8_rv[rcx*2]
%else
            movdqu      xmm4,       [sym(vp8_rv) + rcx*2]
%endif

            paddw       xmm1,       xmm4
            ;paddw     xmm1,       eight8s
            psraw       xmm1,       4

            packuswb    xmm1,       xmm0
            pand        xmm1,       xmm3

            pandn       xmm3,       xmm2
            por         xmm1,       xmm3

            and         rcx,        15
            movq        QWORD PTR   [rsp + rcx*8], xmm1 ;d[rcx*8]

            mov         rcx,        rdx
            sub         rcx,        8

            and         rcx,        15
            movq        mm0,        [rsp + rcx*8] ;d[rcx*8]

            movq        [rsi],      mm0
            lea         rsi,        [rsi+rax]

            lea         rdi,        [rdi+rax]
            add         rdx,        1

            cmp         edx,        dword arg(2) ;rows
            jl          .loop_row

        add         dword arg(0), 8 ; s += 8
        sub         dword arg(3), 8 ; cols -= 8
        cmp         dword arg(3), 0
        jg          .loop_col

    add         rsp, 128+16
    pop         rsp

    ; begin epilog
    pop rdi
    pop rsi
    RESTORE_GOT
    RESTORE_XMM
    UNSHADOW_ARGS
    pop         rbp
    ret
%undef flimit4


;void vp8_mbpost_proc_across_ip_xmm(unsigned char *src,
;                                int pitch, int rows, int cols,int flimit)
global sym(vp8_mbpost_proc_across_ip_xmm) PRIVATE
sym(vp8_mbpost_proc_across_ip_xmm):
    push        rbp
    mov         rbp, rsp
    SHADOW_ARGS_TO_STACK 5
    SAVE_XMM 7
    GET_GOT     rbx
    push        rsi
    push        rdi
    ; end prolog

    ALIGN_STACK 16, rax
    sub         rsp, 16

    ; create flimit4 at [rsp]
    mov         eax, dword ptr arg(4) ;flimit
    mov         [rsp], eax
    mov         [rsp+4], eax
    mov         [rsp+8], eax
    mov         [rsp+12], eax
%define flimit4 [rsp]


    ;for(r=0;r<rows;r++)
.ip_row_loop:

        xor         rdx,    rdx ;sumsq=0;
        xor         rcx,    rcx ;sum=0;
        mov         rsi,    arg(0); s


        ; dup the first byte into the left border 8 times
        movq        mm1,   [rsi]
        punpcklbw   mm1,   mm1
        punpcklwd   mm1,   mm1
        punpckldq   mm1,   mm1

        mov         rdi,    -8
        movq        [rsi+rdi], mm1

        ; dup the last byte into the right border
        movsxd      rdx,    dword arg(3)
        movq        mm1,   [rsi + rdx + -1]
        punpcklbw   mm1,   mm1
        punpcklwd   mm1,   mm1
        punpckldq   mm1,   mm1
        movq        [rsi+rdx], mm1

.ip_var_loop:
        ;for(i=-8;i<=6;i++)
        ;{
        ;    sumsq += s[i]*s[i];
        ;    sum   += s[i];
        ;}
        movzx       eax, byte [rsi+rdi]
        add         ecx, eax
        mul         al
        add         edx, eax
        add         rdi, 1
        cmp         rdi, 6
        jle         .ip_var_loop


            ;mov         rax,    sumsq
            ;movd        xmm7,   rax
            movd        xmm7,   edx

            ;mov         rax,    sum
            ;movd        xmm6,   rax
            movd        xmm6,   ecx

            mov         rsi,    arg(0) ;s
            xor         rcx,    rcx

            movsxd      rdx,    dword arg(3) ;cols
            add         rdx,    8
            pxor        mm0,    mm0
            pxor        mm1,    mm1

            pxor        xmm0,   xmm0
.nextcol4:

            movd        xmm1,   DWORD PTR [rsi+rcx-8]   ; -8 -7 -6 -5
            movd        xmm2,   DWORD PTR [rsi+rcx+7]   ; +7 +8 +9 +10

            punpcklbw   xmm1,   xmm0                    ; expanding
            punpcklbw   xmm2,   xmm0                    ; expanding

            punpcklwd   xmm1,   xmm0                    ; expanding to dwords
            punpcklwd   xmm2,   xmm0                    ; expanding to dwords

            psubd       xmm2,   xmm1                    ; 7--8   8--7   9--6 10--5
            paddd       xmm1,   xmm1                    ; -8*2   -7*2   -6*2 -5*2

            paddd       xmm1,   xmm2                    ; 7+-8   8+-7   9+-6 10+-5
            pmaddwd     xmm1,   xmm2                    ; squared of 7+-8   8+-7   9+-6 10+-5

            paddd       xmm6,   xmm2
            paddd       xmm7,   xmm1

            pshufd      xmm6,   xmm6,   0               ; duplicate the last ones
            pshufd      xmm7,   xmm7,   0               ; duplicate the last ones

            psrldq      xmm1,       4                   ; 8--7   9--6 10--5  0000
            psrldq      xmm2,       4                   ; 8--7   9--6 10--5  0000

            pshufd      xmm3,   xmm1,   3               ; 0000  8--7   8--7   8--7 squared
            pshufd      xmm4,   xmm2,   3               ; 0000  8--7   8--7   8--7 squared

            paddd       xmm6,   xmm4
            paddd       xmm7,   xmm3

            pshufd      xmm3,   xmm1,   01011111b       ; 0000  0000   9--6   9--6 squared
            pshufd      xmm4,   xmm2,   01011111b       ; 0000  0000   9--6   9--6 squared

            paddd       xmm7,   xmm3
            paddd       xmm6,   xmm4

            pshufd      xmm3,   xmm1,   10111111b       ; 0000  0000   8--7   8--7 squared
            pshufd      xmm4,   xmm2,   10111111b       ; 0000  0000   8--7   8--7 squared

            paddd       xmm7,   xmm3
            paddd       xmm6,   xmm4

            movdqa      xmm3,   xmm6
            pmaddwd     xmm3,   xmm3

            movdqa      xmm5,   xmm7
            pslld       xmm5,   4

            psubd       xmm5,   xmm7
            psubd       xmm5,   xmm3

            psubd       xmm5,   flimit4
            psrad       xmm5,   31

            packssdw    xmm5,   xmm0
            packsswb    xmm5,   xmm0

            movd        xmm1,   DWORD PTR [rsi+rcx]
            movq        xmm2,   xmm1

            punpcklbw   xmm1,   xmm0
            punpcklwd   xmm1,   xmm0

            paddd       xmm1,   xmm6
            paddd       xmm1,   [GLOBAL(four8s)]

            psrad       xmm1,   4
            packssdw    xmm1,   xmm0

            packuswb    xmm1,   xmm0
            pand        xmm1,   xmm5

            pandn       xmm5,   xmm2
            por         xmm5,   xmm1

            movd        [rsi+rcx-8],  mm0
            movq        mm0,    mm1

            movdq2q     mm1,    xmm5
            psrldq      xmm7,   12

            psrldq      xmm6,   12
            add         rcx,    4

            cmp         rcx,    rdx
            jl          .nextcol4

        ;s+=pitch;
        movsxd rax, dword arg(1)
        add    arg(0), rax

        sub dword arg(2), 1 ;rows-=1
        cmp dword arg(2), 0
        jg .ip_row_loop

    add         rsp, 16
    pop         rsp

    ; begin epilog
    pop rdi
    pop rsi
    RESTORE_GOT
    RESTORE_XMM
    UNSHADOW_ARGS
    pop         rbp
    ret
%undef flimit4


;void vp8_plane_add_noise_wmt (unsigned char *Start, unsigned char *noise,
;                            unsigned char blackclamp[16],
;                            unsigned char whiteclamp[16],
;                            unsigned char bothclamp[16],
;                            unsigned int Width, unsigned int Height, int Pitch)
extern sym(rand)
global sym(vp8_plane_add_noise_wmt) PRIVATE
sym(vp8_plane_add_noise_wmt):
    push        rbp
    mov         rbp, rsp
    SHADOW_ARGS_TO_STACK 8
    GET_GOT     rbx
    push        rsi
    push        rdi
    ; end prolog

.addnoise_loop:
    call sym(rand) WRT_PLT
    mov     rcx, arg(1) ;noise
    and     rax, 0xff
    add     rcx, rax

    ; we rely on the fact that the clamping vectors are stored contiguously
    ; in black/white/both order. Note that we have to reload this here because
    ; rdx could be trashed by rand()
    mov     rdx, arg(2) ; blackclamp


            mov     rdi, rcx
            movsxd  rcx, dword arg(5) ;[Width]
            mov     rsi, arg(0) ;Pos
            xor         rax,rax

.addnoise_nextset:
            movdqu      xmm1,[rsi+rax]         ; get the source

            psubusb     xmm1, [rdx]    ;blackclamp        ; clamp both sides so we don't outrange adding noise
            paddusb     xmm1, [rdx+32] ;bothclamp
            psubusb     xmm1, [rdx+16] ;whiteclamp

            movdqu      xmm2,[rdi+rax]         ; get the noise for this line
            paddb       xmm1,xmm2              ; add it in
            movdqu      [rsi+rax],xmm1         ; store the result

            add         rax,16                 ; move to the next line

            cmp         rax, rcx
            jl          .addnoise_nextset

    movsxd  rax, dword arg(7) ; Pitch
    add     arg(0), rax ; Start += Pitch
    sub     dword arg(6), 1   ; Height -= 1
    jg      .addnoise_loop

    ; begin epilog
    pop rdi
    pop rsi
    RESTORE_GOT
    UNSHADOW_ARGS
    pop         rbp
    ret


SECTION_RODATA
align 16
four8s:
    times 4 dd 8
