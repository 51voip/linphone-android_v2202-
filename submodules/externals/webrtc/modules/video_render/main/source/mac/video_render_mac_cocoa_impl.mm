/*
 *  Copyright (c) 2012 The WebRTC project authors. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license
 *  that can be found in the LICENSE file in the root of the source
 *  tree. An additional intellectual property rights grant can be found
 *  in the file PATENTS.  All contributing project authors may
 *  be found in the AUTHORS file in the root of the source tree.
 */

#include "engine_configurations.h"
#if defined(COCOA_RENDERING)

#import "cocoa_render_view.h"

#include "video_render_mac_cocoa_impl.h"
#include "critical_section_wrapper.h"
#include "video_render_nsopengl.h"
#include "trace.h"

namespace webrtc {

VideoRenderMacCocoaImpl::VideoRenderMacCocoaImpl(const WebRtc_Word32 id,
        const VideoRenderType videoRenderType,
        void* window,
        const bool fullscreen) :
_id(id),
_renderMacCocoaCritsect(*CriticalSectionWrapper::CreateCriticalSection()),
_fullScreen(fullscreen),
_ptrWindow(window)
{

    WEBRTC_TRACE(kTraceInfo, kTraceVideoRenderer, _id, "Constructor %s:%d", __FUNCTION__, __LINE__);
}

VideoRenderMacCocoaImpl::~VideoRenderMacCocoaImpl()
{
    WEBRTC_TRACE(kTraceInfo, kTraceVideoRenderer, _id, "Destructor %s:%d", __FUNCTION__, __LINE__);
    delete &_renderMacCocoaCritsect;
    if (_ptrCocoaRender)
    {
        delete _ptrCocoaRender;
        _ptrCocoaRender = NULL;
    }
}

WebRtc_Word32
VideoRenderMacCocoaImpl::Init()
{

    CriticalSectionScoped cs(&_renderMacCocoaCritsect);
    WEBRTC_TRACE(kTraceInfo, kTraceVideoRenderer, _id, "%s:%d", __FUNCTION__, __LINE__);

    // cast ptrWindow from void* to CocoaRenderer. Void* was once NSOpenGLView, and CocoaRenderer is NSOpenGLView.
    _ptrCocoaRender = new VideoRenderNSOpenGL((CocoaRenderView*)_ptrWindow, _fullScreen, _id);
    if (!_ptrWindow)
    {
        WEBRTC_TRACE(kTraceWarning, kTraceVideoRenderer, _id, "Constructor %s:%d", __FUNCTION__, __LINE__);
        return -1;
    }
    int retVal = _ptrCocoaRender->Init();
    if (retVal == -1)
    {
        WEBRTC_TRACE(kTraceInfo, kTraceVideoRenderer, _id, "Failed to init %s:%d", __FUNCTION__, __LINE__);
        return -1;
    }

    return 0;
}

WebRtc_Word32
VideoRenderMacCocoaImpl::ChangeUniqueId(const WebRtc_Word32 id)
{
    CriticalSectionScoped cs(&_renderMacCocoaCritsect);
    WEBRTC_TRACE(kTraceInfo, kTraceVideoRenderer, _id, "%s", __FUNCTION__);
    _id = id;

    if(_ptrCocoaRender)
    {
        _ptrCocoaRender->ChangeUniqueID(_id);
    }

    return 0;
}

WebRtc_Word32
VideoRenderMacCocoaImpl::ChangeWindow(void* window)
{

    CriticalSectionScoped cs(&_renderMacCocoaCritsect);
    WEBRTC_TRACE(kTraceInfo, kTraceVideoRenderer, _id, "%s changing ID to ", __FUNCTION__, window);

    if (window == NULL)
    {
        return -1;
    }
    _ptrWindow = window;


    _ptrWindow = window;
    _ptrCocoaRender->ChangeWindow((CocoaRenderView*)_ptrWindow);

    return 0;
}

VideoRenderCallback*
VideoRenderMacCocoaImpl::AddIncomingRenderStream(const WebRtc_UWord32 streamId,
        const WebRtc_UWord32 zOrder,
        const float left,
        const float top,
        const float right,
        const float bottom)
{
    CriticalSectionScoped cs(&_renderMacCocoaCritsect);
    WEBRTC_TRACE(kTraceDebug, kTraceVideoRenderer, _id, "%s", __FUNCTION__);
    VideoChannelNSOpenGL* nsOpenGLChannel = NULL;

    if(!_ptrWindow)
    {
    }

    if(!nsOpenGLChannel)
    {
        nsOpenGLChannel = _ptrCocoaRender->CreateNSGLChannel(streamId, zOrder, left, top, right, bottom);
    }

    return nsOpenGLChannel;

}

WebRtc_Word32
VideoRenderMacCocoaImpl::DeleteIncomingRenderStream(const WebRtc_UWord32 streamId)
{
    WEBRTC_TRACE(kTraceDebug, kTraceVideoRenderer, _id, "Constructor %s:%d", __FUNCTION__, __LINE__);
    CriticalSectionScoped cs(&_renderMacCocoaCritsect);
    _ptrCocoaRender->DeleteNSGLChannel(streamId);

    return 0;
}

WebRtc_Word32
VideoRenderMacCocoaImpl::GetIncomingRenderStreamProperties(const WebRtc_UWord32 streamId,
        WebRtc_UWord32& zOrder,
        float& left,
        float& top,
        float& right,
        float& bottom) const
{
    return _ptrCocoaRender->GetChannelProperties(streamId, zOrder, left, top, right, bottom);
}

WebRtc_Word32
VideoRenderMacCocoaImpl::StartRender()
{
    return _ptrCocoaRender->StartRender();
}

WebRtc_Word32
VideoRenderMacCocoaImpl::StopRender()
{
    return _ptrCocoaRender->StopRender();
}

VideoRenderType
VideoRenderMacCocoaImpl::RenderType()
{
    return kRenderCocoa;
}

RawVideoType
VideoRenderMacCocoaImpl::PerferedVideoType()
{
    return kVideoI420;
}

bool
VideoRenderMacCocoaImpl::FullScreen()
{
    return false;
}

WebRtc_Word32
VideoRenderMacCocoaImpl::GetGraphicsMemory(WebRtc_UWord64& totalGraphicsMemory,
        WebRtc_UWord64& availableGraphicsMemory) const
{
    totalGraphicsMemory = 0;
    availableGraphicsMemory = 0;
    return 0;
}

WebRtc_Word32
VideoRenderMacCocoaImpl::GetScreenResolution(WebRtc_UWord32& screenWidth,
        WebRtc_UWord32& screenHeight) const
{
    CriticalSectionScoped cs(&_renderMacCocoaCritsect);
    NSScreen* mainScreen = [NSScreen mainScreen];

    NSRect frame = [mainScreen frame];

    screenWidth = frame.size.width;
    screenHeight = frame.size.height;
    return 0;
}

WebRtc_UWord32
VideoRenderMacCocoaImpl::RenderFrameRate(const WebRtc_UWord32 streamId)
{
    CriticalSectionScoped cs(&_renderMacCocoaCritsect);
    return 0;
}

WebRtc_Word32
VideoRenderMacCocoaImpl::SetStreamCropping(const WebRtc_UWord32 streamId,
        const float left,
        const float top,
        const float right,
        const float bottom)
{
    return 0;
}

WebRtc_Word32 VideoRenderMacCocoaImpl::ConfigureRenderer(const WebRtc_UWord32 streamId,
        const unsigned int zOrder,
        const float left,
        const float top,
        const float right,
        const float bottom)
{
    return 0;
}

WebRtc_Word32
VideoRenderMacCocoaImpl::SetTransparentBackground(const bool enable)
{
    return 0;
}

WebRtc_Word32 VideoRenderMacCocoaImpl::SetText(const WebRtc_UWord8 textId,
        const WebRtc_UWord8* text,
        const WebRtc_Word32 textLength,
        const WebRtc_UWord32 textColorRef,
        const WebRtc_UWord32 backgroundColorRef,
        const float left,
        const float top,
        const float right,
        const float bottom)
{
    return _ptrCocoaRender->SetText(textId, text, textLength, textColorRef, backgroundColorRef, left, top, right, bottom);
}

WebRtc_Word32 VideoRenderMacCocoaImpl::SetBitmap(const void* bitMap,
        const WebRtc_UWord8 pictureId,
        const void* colorKey,
        const float left,
        const float top,
        const float right,
        const float bottom)
{
    return 0;
}

WebRtc_Word32 VideoRenderMacCocoaImpl::FullScreenRender(void* window, const bool enable)
{
    return -1;
}

} //namespace webrtc

#endif // COCOA_RENDERING
