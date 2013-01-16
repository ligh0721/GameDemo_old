#include "stdafx.h"
#include "MFCEGLView.h"


CMFCEGLView::CMFCEGLView(void)
    : m_hParent(NULL)
{
}


CMFCEGLView::~CMFCEGLView(void)
{
}

bool CMFCEGLView::Create( LPCTSTR pTitle, int w, int h)
{
    bool bRet = false;
    do 
    {
        CC_BREAK_IF(getHWnd());

        HINSTANCE hInstance = GetModuleHandle( NULL );
        WNDCLASS  wc;        // Windows Class Structure

        // Redraw On Size, And Own DC For Window.
        wc.style          = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;  
        //wc.style          = CS_DBLCLKS | CS_PARENTDC | CS_GLOBALCLASS;
        wc.lpfnWndProc    = CCEGLView::getWindowProc();                    // WndProc Handles Messages
        wc.cbClsExtra     = 0;                              // No Extra Window Data
        wc.cbWndExtra     = 0;                                // No Extra Window Data
        wc.hInstance      = hInstance;                        // Set The Instance
        wc.hIcon          = LoadIcon( NULL, IDI_WINLOGO );    // Load The Default Icon
        wc.hCursor        = LoadCursor( NULL, IDC_ARROW );    // Load The Arrow Pointer
        wc.hbrBackground  = NULL;                           // No Background Required For GL
        wc.lpszMenuName   = m_menu;                         // 
        wc.lpszClassName  = CCEGLView::getWindowClassName();               // Set The Class Name

        CC_BREAK_IF(! RegisterClass(&wc) && 1410 != GetLastError());        

        // center window position
        RECT rcDesktop;
        GetWindowRect(GetDesktopWindow(), &rcDesktop);

        WCHAR wszBuf[50] = {0};
        MultiByteToWideChar(CP_UTF8, 0, m_szViewName, -1, wszBuf, sizeof(wszBuf));

        // create window
        m_hWnd = CreateWindowExW(
            WS_EX_APPWINDOW | WS_EX_WINDOWEDGE,    // Extended Style For The Window
            CCEGLView::getWindowClassName(),                                    // Class Name
            wszBuf,                                                // Window Title
            WS_CAPTION | WS_POPUPWINDOW | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_SIZEBOX,        // Defined Window Style
            0, 0,                                                // Window Position
            0,                                                  // Window Width
            0,                                                  // Window Height
            m_hParent,                                          // No Parent Window
            NULL,                                                // No Menu
            hInstance,                                            // Instance
            NULL );

        CC_BREAK_IF(! m_hWnd);

        resize(w, h);

        bRet = initGL();
        CC_BREAK_IF(!bRet);

        CCEGLView::setMainWindow(this);
        bRet = true;
    } while (0);

    return bRet;
}

void CMFCEGLView::setHWnd( HWND hWnd )
{
    m_hWnd = hWnd;
}

void CMFCEGLView::setFrameSizeNoMove( float width, float height )
{
    CCEGLViewProtocol::setFrameSize(width, height);
    resize(width, height); // adjust window size for menubar
}


LRESULT CMFCEGLView::WindowProc( UINT message, WPARAM wParam, LPARAM lParam )
{
    int iWidth = 0;
    int iHeight = 0;
    switch (message)
    {
    case WM_CLOSE:
        ShowWindow(getHWnd(), SW_HIDE);
        break;

    case WM_SIZE:
        iWidth = LOWORD(lParam); // width of client area
        iHeight = HIWORD(lParam); // height of client area
        if (iWidth != m_windowWidth || iHeight != m_windowHeight)
        {
            setFrameSizeNoMove(iWidth, iHeight);
        }
        break;

    default:
        return CCEGLView::WindowProc(message, wParam, lParam);
    }

    return 0;
}
