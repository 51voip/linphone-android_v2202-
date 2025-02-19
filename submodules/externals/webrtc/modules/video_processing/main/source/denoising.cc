/*
 *  Copyright (c) 2012 The WebRTC project authors. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license
 *  that can be found in the LICENSE file in the root of the source
 *  tree. An additional intellectual property rights grant can be found
 *  in the file PATENTS.  All contributing project authors may
 *  be found in the AUTHORS file in the root of the source tree.
 */

#include "denoising.h"
#include "trace.h"

#include <cstring>

namespace webrtc {

enum { kSubsamplingTime = 0 };       // Down-sampling in time (unit: number of frames)
enum { kSubsamplingWidth = 0 };      // Sub-sampling in width (unit: power of 2)
enum { kSubsamplingHeight = 0 };     // Sub-sampling in height (unit: power of 2)
enum { kDenoiseFiltParam = 179 };    // (Q8) De-noising filter parameter
enum { kDenoiseFiltParamRec = 77 };  // (Q8) 1 - filter parameter
enum { kDenoiseThreshold = 19200 };  // (Q8) De-noising threshold level

VPMDenoising::VPMDenoising() :
    _id(0),
    _moment1(NULL),
    _moment2(NULL)
{
    Reset();
}

VPMDenoising::~VPMDenoising()
{
    if (_moment1)
    {
        delete [] _moment1;
        _moment1 = NULL;
    }
    
    if (_moment2)
    {
        delete [] _moment2;
        _moment2 = NULL;
    }
}

WebRtc_Word32
VPMDenoising::ChangeUniqueId(const WebRtc_Word32 id)
{
    _id = id;
    return VPM_OK;
}

void
VPMDenoising::Reset()
{
    _frameSize = 0;
    _denoiseFrameCnt = 0;

    if (_moment1)
    {
        delete [] _moment1;
        _moment1 = NULL;
    }
    
    if (_moment2)
    {
        delete [] _moment2;
        _moment2 = NULL;
    }
}

WebRtc_Word32
VPMDenoising::ProcessFrame(WebRtc_UWord8* frame,
                           const WebRtc_UWord32 width,
                           const WebRtc_UWord32 height)
{
    WebRtc_Word32     thevar;
    WebRtc_UWord32    k;
    WebRtc_UWord32    jsub, ksub;
    WebRtc_Word32     diff0;
    WebRtc_UWord32    tmpMoment1;
    WebRtc_UWord32    tmpMoment2;
    WebRtc_UWord32    tmp;
    WebRtc_Word32     numPixelsChanged = 0;

    if (frame == NULL)
    {
        WEBRTC_TRACE(webrtc::kTraceError, webrtc::kTraceVideoPreocessing, _id, "Null frame pointer");
        return VPM_GENERAL_ERROR;
    }

    if (width == 0 || height == 0)
    {
        WEBRTC_TRACE(webrtc::kTraceError, webrtc::kTraceVideoPreocessing, _id, "Invalid frame size");
        return VPM_GENERAL_ERROR;
    }

    /* Size of luminance component */
    const WebRtc_UWord32 ysize  = height * width;

    /* Initialization */
    if (ysize != _frameSize)
    {
        delete [] _moment1;
        _moment1 = NULL;

        delete [] _moment2;
        _moment2 = NULL;
    }
    _frameSize = ysize;

    if (!_moment1)
    {
        _moment1 = new WebRtc_UWord32[ysize];
        memset(_moment1, 0, sizeof(WebRtc_UWord32)*ysize);
    }
    
    if (!_moment2)
    {
        _moment2 = new WebRtc_UWord32[ysize];
        memset(_moment2, 0, sizeof(WebRtc_UWord32)*ysize);
    }

    /* Apply de-noising on each pixel, but update variance sub-sampled */
    for (WebRtc_UWord32 i = 0; i < height; i++)
    { // Collect over height
        k = i * width;
        ksub = ((i >> kSubsamplingHeight) << kSubsamplingHeight) * width;
        for (WebRtc_UWord32 j = 0; j < width; j++)
        { // Collect over width
            jsub = ((j >> kSubsamplingWidth) << kSubsamplingWidth);
            /* Update mean value for every pixel and every frame */
            tmpMoment1 = _moment1[k + j];
            tmpMoment1 *= kDenoiseFiltParam; // Q16
            tmpMoment1 += ((kDenoiseFiltParamRec * ((WebRtc_UWord32)frame[k + j])) << 8);
            tmpMoment1 >>= 8; // Q8
            _moment1[k + j] = tmpMoment1;

            tmpMoment2 = _moment2[ksub + jsub];
            if ((ksub == k) && (jsub == j) && (_denoiseFrameCnt == 0))
            {
                tmp = ((WebRtc_UWord32)frame[k + j] * (WebRtc_UWord32)frame[k + j]);
                tmpMoment2 *= kDenoiseFiltParam; // Q16
                tmpMoment2 += ((kDenoiseFiltParamRec * tmp)<<8);
                tmpMoment2 >>= 8; // Q8
            }
            _moment2[k + j] = tmpMoment2;
            /* Current event = deviation from mean value */
            diff0 = ((WebRtc_Word32)frame[k + j] << 8) - _moment1[k + j];
            /* Recent events = variance (variations over time) */
            thevar = _moment2[k + j];
            thevar -= ((_moment1[k + j] * _moment1[k + j]) >> 8);
            /***************************************************************************
             * De-noising criteria, i.e., when should we replace a pixel by its mean
             *
             * 1) recent events are minor
             * 2) current events are minor
             ***************************************************************************/
            if ((thevar < kDenoiseThreshold)
                && ((diff0 * diff0 >> 8) < kDenoiseThreshold))
            { // Replace with mean
                frame[k + j] = (WebRtc_UWord8)(_moment1[k + j] >> 8);
                numPixelsChanged++;
            }
        }
    }

    /* Update frame counter */
    _denoiseFrameCnt++;
    if (_denoiseFrameCnt > kSubsamplingTime)
    {
        _denoiseFrameCnt = 0;
    }

    return numPixelsChanged;
}

} //namespace
