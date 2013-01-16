#ifndef CCOCOS2DXTHREAD_H
#define CCOCOS2DXTHREAD_H

#include <QThread>


class CCocos2dxThread : public QThread
{
    //Q_OBJECT
public:
    explicit CCocos2dxThread(HWND hWnd, float fWidth, float fHeight, float fZoom, QObject* pParent = NULL);

    void run();


protected:
    HWND m_hWnd;
    float m_fWidth;
    float m_fHeight;
    float m_fZoom;
    CCEGLView* m_pEglView;

};

class CCocos2dxWidget : public QWidget
{
    //Q_OBJECT
public:
    explicit CCocos2dxWidget(QWidget* pParent = NULL);

protected:
    virtual void resizeEvent(QResizeEvent* pEvent);
};

#endif // CCOCOS2DXTHREAD_H
