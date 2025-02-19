/*
 *  Copyright (c) 2012 The WebRTC project authors. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license
 *  that can be found in the LICENSE file in the root of the source
 *  tree. An additional intellectual property rights grant can be found
 *  in the file PATENTS.  All contributing project authors may
 *  be found in the AUTHORS file in the root of the source tree.
 */

#ifndef WEBRTC_MODULES_VIDEO_RENDER_MAIN_INTERFACE_VIDEO_RENDER_DEFINES_H_
#define WEBRTC_MODULES_VIDEO_RENDER_MAIN_INTERFACE_VIDEO_RENDER_DEFINES_H_

// Includes
#include "common_types.h"
#include "modules/interface/module_common_types.h"

namespace webrtc
{
// Defines
#ifndef NULL
#define NULL    0
#endif

// Enums
enum VideoRenderType
{
    kRenderExternal = 0, // External
    kRenderWindows = 1, // Windows
    kRenderCocoa = 2, // Mac
    kRenderCarbon = 3,
    kRenderiPhone = 4, // iPhone
    kRenderAndroid = 5, // Android
    kRenderX11 = 6, // Linux
    kRenderDefault
};

// Runtime errors
enum VideoRenderError
{
    kRenderShutDown = 0,
    kRenderPerformanceAlarm = 1
};

// The object a module user uses to send new frames to the renderer
// One object is used for each incoming stream
class VideoRenderCallback
{
public:
    virtual WebRtc_Word32 RenderFrame(const WebRtc_UWord32 streamId,
                                      VideoFrame& videoFrame) = 0;

protected:
    virtual ~VideoRenderCallback()
    {
    }
};

// Feedback class to be implemented by module user
class VideoRenderFeedback
{
public:
    virtual void OnRenderError(const WebRtc_Word32 streamId,
                               const VideoRenderError error) = 0;

protected:
    virtual ~VideoRenderFeedback()
    {
    }
};

// Mobile enums
enum StretchMode
{
    kStretchToInsideEdge = 1,
    kStretchToOutsideEdge = 2,
    kStretchMatchWidth = 3,
    kStretchMatchHeight = 4,
    kStretchNone = 5
};

enum Rotation
{
    kRotation0 = 0,
    kRotation90 = 1,
    kRotation180 = 2,
    kRotation270 = 3
};

} //namespace webrtc

#endif  // WEBRTC_MODULES_VIDEO_RENDER_MAIN_INTERFACE_VIDEO_RENDER_DEFINES_H_
