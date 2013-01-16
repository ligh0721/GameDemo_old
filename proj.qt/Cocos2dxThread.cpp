#include "CommInc.h"
#include "mainwindow.h"
#include "Cocos2dxThread.h"
#include "AppDelegate.h"


CCocos2dxThread::CCocos2dxThread(HWND hWnd, float fWidth, float fHeight, float fZoom, QObject* pParent)
    : QThread(pParent)
    , m_hWnd(hWnd)
    , m_fWidth(fWidth)
    , m_fHeight(fHeight)
    , m_fZoom(fZoom)
{
}

void CCocos2dxThread::run()
{
    AppDelegate app;
    CC_UNUSED_PARAM(app);
    static CCEGLView* pEglView = CCEGLView::sharedOpenGLView(m_hWnd);
    pEglView->setFrameSize(m_fWidth, m_fHeight);
    // The resolution of ipad3 is very large. In general, PC's resolution is smaller than it.
    // So we need to invoke 'setFrameZoomFactor'(only valid on desktop(win32, mac, linux)) to make the window smaller.
    pEglView->setFrameZoomFactor(m_fZoom);

    CCApplication::sharedApplication()->run();

    qApp->quit();
}

CCocos2dxWidget::CCocos2dxWidget(QWidget* pParent)
    : QWidget(pParent)
{
}

void CCocos2dxWidget::resizeEvent(QResizeEvent* pEvent)
{
    return;
    CCEGLView* pEglView = CCDirector::sharedDirector()->getOpenGLView();
    if (!pEglView)
    {
        return;
    }

    CCSize oSz = pEglView->getFrameSize();
    float fW = pEvent->size().width() / oSz.width;
    float fH = pEvent->size().height() / oSz.height;
    float fZoom = MIN(fW, fH);
    //pEglView->resize(pEvent->size().width(), pEvent->size().height());
    //pEglView->setFrameSize(pEvent->size().width() / fZoom, pEvent->size().height() / fZoom);
    pEglView->setFrameZoomFactor(fZoom);

    //CCDirector* pDirector = CCDirector::sharedDirector();
}

