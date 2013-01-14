// Copyright (c) 2012-2013 PHP Desktop Authors. All rights reserved.
// License: New BSD License.
// Website: http://code.google.com/p/phpdesktop/

// Not used currently... as we implemented DocHostUIHandlerDispatch()
// and this is not needed anymore.

#pragma once

#include "browser_frame_interface.h"

template <class RootFrame>
class OleInPlaceSite : public IOleInPlaceSite
{
public:
    IOleClientSite* oleClientSite;
    IOleInPlaceFrame* oleInPlaceFrame;
    BrowserFrameInterface<RootFrame>* webFrame;

    OleInPlaceSite(
        IOleClientSite* inOleClientSite,
        IOleInPlaceFrame* inOleInPlaceFrame,
        BrowserFrameInterface<RootFrame>* inWebFrame)
    {
        oleClientSite = inOleClientSite;
        webFrame = inWebFrame;
        oleInPlaceFrame = inOleInPlaceFrame;
    }
public:

    // IUnknown

    HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, LPVOID FAR* ppvObj)
    {
        return oleClientSite->QueryInterface(riid, ppvObj);
    }
    ULONG STDMETHODCALLTYPE AddRef(void)
    {
        return 1;
    }
    ULONG STDMETHODCALLTYPE Release(void)
    {
        return 1;
    }

    // IOleWindow

    HRESULT STDMETHODCALLTYPE GetWindow(HWND FAR* lphwnd)
    {
        *lphwnd = webFrame->RootView_GetWindowHandle();
        return S_OK;
    }
    HRESULT STDMETHODCALLTYPE ContextSensitiveHelp(BOOL fEnterMode)
    {
        return E_NOTIMPL;
    }

    // IOleInPlaceSite

    HRESULT STDMETHODCALLTYPE CanInPlaceActivate(void)
    {
        return S_OK;
    }
    HRESULT STDMETHODCALLTYPE OnInPlaceActivate(void)
    {
        return S_OK;
    }
    HRESULT STDMETHODCALLTYPE OnUIActivate(void)
    {
        return S_OK;
    }
    HRESULT STDMETHODCALLTYPE GetWindowContext(LPOLEINPLACEFRAME FAR* lplpFrame,
        LPOLEINPLACEUIWINDOW FAR* lplpDoc, LPRECT lprcPosRect, LPRECT lprcClipRect,
        LPOLEINPLACEFRAMEINFO lpFrameInfo)
    {
        *lplpFrame = oleInPlaceFrame;
        *lplpDoc = 0;
        lpFrameInfo->fMDIApp = FALSE;
        lpFrameInfo->hwndFrame = webFrame->GetWindowHandle();
        lpFrameInfo->haccel = 0;
        lpFrameInfo->cAccelEntries = 0;
        return (S_OK);
    }

    HRESULT STDMETHODCALLTYPE Scroll(SIZE scrollExtent)
    {
        return E_NOTIMPL;
    }

    HRESULT STDMETHODCALLTYPE OnUIDeactivate(BOOL fUndoable)
    {
        return S_OK;
    }

    HRESULT STDMETHODCALLTYPE OnInPlaceDeactivate(void)
    {
        return S_OK;
    }

    HRESULT STDMETHODCALLTYPE DiscardUndoState(void)
    {
        return E_NOTIMPL;
    }

    HRESULT STDMETHODCALLTYPE DeactivateAndUndo(void)
    {
        return E_NOTIMPL;
    }

    HRESULT STDMETHODCALLTYPE OnPosRectChange(LPCRECT lprcPosRect)
    {
        HRESULT hr;

        CComQIPtr<IOleObject> oleObject;
        CComQIPtr<IWebBrowser2> webBrowser;
        int ctrlid = webFrame->RootView_GetDlgCtrlID();

        hr = webFrame->RootFrame_GetDlgControl(ctrlid, IID_IWebBrowser2, (void**) &webBrowser);
        ASSERT_EXIT(SUCCEEDED(hr), "webFrame->RootFrame_GetDlgControl(IID_IWebBrowser2) failed");

        hr = webBrowser->QueryInterface(IID_IOleObject, (void**) &oleObject);
        ASSERT_EXIT(SUCCEEDED(hr), "webBrowser->QueryInterface(IID_IOleObject)");

        CComQIPtr<IOleInPlaceObject> oleInPlaceObject;
        hr = oleObject->QueryInterface(IID_IOleInPlaceObject, (void**) &oleInPlaceObject);
        ASSERT_EXIT(SUCCEEDED(hr), "webBrowser->QueryInterface(IID_IOleINPlaceObject)");

        oleInPlaceObject->SetObjectRects(lprcPosRect, lprcPosRect);

        return S_OK;

        // CComQIPtr does automatic releases!
        // off: oleInPlaceObject->Release();
        // off: oleObject->Release();
        // off: webBrowser->Release();
    }

};