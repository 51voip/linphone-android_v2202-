/*
 *  Copyright (c) 2010 The WebM project authors. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license
 *  that can be found in the LICENSE file in the root of the source
 *  tree. An additional intellectual property rights grant can be found
 *  in the file PATENTS.  All contributing project authors may
 *  be found in the AUTHORS file in the root of the source tree.
 */


#ifndef DBOOLHUFF_H
#define DBOOLHUFF_H
#include <stddef.h>
#include <limits.h>
#include "vpx_config.h"
#include "vpx_ports/mem.h"
#include "vpx/vpx_integer.h"

typedef size_t VP8_BD_VALUE;

# define VP8_BD_VALUE_SIZE ((int)sizeof(VP8_BD_VALUE)*CHAR_BIT)
/*This is meant to be a large, positive constant that can still be efficiently
   loaded as an immediate (on platforms like ARM, for example).
  Even relatively modest values like 100 would work fine.*/
# define VP8_LOTS_OF_BITS (0x40000000)

typedef struct
{
    const unsigned char *user_buffer_end;
    const unsigned char *user_buffer;
    VP8_BD_VALUE         value;
    int                  count;
    unsigned int         range;
} BOOL_DECODER;

DECLARE_ALIGNED(16, extern const unsigned char, vp8_norm[256]);

int vp8dx_start_decode(BOOL_DECODER *br,
                       const unsigned char *source,
                       unsigned int source_sz);

void vp8dx_bool_decoder_fill(BOOL_DECODER *br);

/*The refill loop is used in several places, so define it in a macro to make
   sure they're all consistent.
  An inline function would be cleaner, but has a significant penalty, because
   multiple BOOL_DECODER fields must be modified, and the compiler is not smart
   enough to eliminate the stores to those fields and the subsequent reloads
   from them when inlining the function.*/
#define VP8DX_BOOL_DECODER_FILL(_count,_value,_bufptr,_bufend) \
    do \
    { \
        int shift = VP8_BD_VALUE_SIZE - 8 - ((_count) + 8); \
        int loop_end, x; \
        size_t bits_left = ((_bufend)-(_bufptr))*CHAR_BIT; \
        \
        x = (int)(shift + CHAR_BIT - bits_left); \
        loop_end = 0; \
        if(x >= 0) \
        { \
            (_count) += VP8_LOTS_OF_BITS; \
            loop_end = x; \
            if(!bits_left) break; \
        } \
        while(shift >= loop_end) \
        { \
            (_count) += CHAR_BIT; \
            (_value) |= (VP8_BD_VALUE)*(_bufptr)++ << shift; \
            shift -= CHAR_BIT; \
        } \
    } \
    while(0) \


static int vp8dx_decode_bool(BOOL_DECODER *br, int probability) {
    unsigned int bit = 0;
    VP8_BD_VALUE value;
    unsigned int split;
    VP8_BD_VALUE bigsplit;
    int count;
    unsigned int range;

    split = 1 + (((br->range - 1) * probability) >> 8);

    if(br->count < 0)
        vp8dx_bool_decoder_fill(br);

    value = br->value;
    count = br->count;

    bigsplit = (VP8_BD_VALUE)split << (VP8_BD_VALUE_SIZE - 8);

    range = split;

    if (value >= bigsplit)
    {
        range = br->range - split;
        value = value - bigsplit;
        bit = 1;
    }

    {
        register unsigned int shift = vp8_norm[range];
        range <<= shift;
        value <<= shift;
        count -= shift;
    }
    br->value = value;
    br->count = count;
    br->range = range;

    return bit;
}

static int vp8_decode_value(BOOL_DECODER *br, int bits)
{
    int z = 0;
    int bit;

    for (bit = bits - 1; bit >= 0; bit--)
    {
        z |= (vp8dx_decode_bool(br, 0x80) << bit);
    }

    return z;
}

static int vp8dx_bool_error(BOOL_DECODER *br)
{
    /* Check if we have reached the end of the buffer.
     *
     * Variable 'count' stores the number of bits in the 'value' buffer, minus
     * 8. The top byte is part of the algorithm, and the remainder is buffered
     * to be shifted into it. So if count == 8, the top 16 bits of 'value' are
     * occupied, 8 for the algorithm and 8 in the buffer.
     *
     * When reading a byte from the user's buffer, count is filled with 8 and
     * one byte is filled into the value buffer. When we reach the end of the
     * data, count is additionally filled with VP8_LOTS_OF_BITS. So when
     * count == VP8_LOTS_OF_BITS - 1, the user's data has been exhausted.
     */
    if ((br->count > VP8_BD_VALUE_SIZE) && (br->count < VP8_LOTS_OF_BITS))
    {
       /* We have tried to decode bits after the end of
        * stream was encountered.
        */
        return 1;
    }

    /* No error. */
    return 0;
}
#endif
