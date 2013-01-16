#pragma once

class CMFCEGLView : public CCEGLView
{
public:
    CMFCEGLView(void);
    virtual ~CMFCEGLView(void);

    void setHWnd(HWND hWnd);
    virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
    void setFrameSizeNoMove(float width, float height);

private:
    virtual bool Create(LPCTSTR pTitle, int w, int h);

public:
    HWND m_hParent;
};

