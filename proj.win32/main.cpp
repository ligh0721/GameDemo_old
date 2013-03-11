#include "CommDef.h"

#include "main.h"
#include "AppDelegate.h"
#include "CCEGLView.h"

USING_NS_CC;

// uncomment below line, open debug console
// #define USE_WIN32_CONSOLE

int APIENTRY _tWinMain(HINSTANCE hInstance,
                       HINSTANCE hPrevInstance,
                       LPTSTR    lpCmdLine,
                       int       nCmdShow)
{    
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);
#define USE_WIN32_CONSOLE
#ifdef USE_WIN32_CONSOLE
    AllocConsole();
    freopen("CONIN$", "r", stdin);
    freopen("CONOUT$", "w", stdout);
    freopen("CONOUT$", "w", stderr);

#endif

    srand(time(NULL));
    int a = 0, b = 0;
    for (int i = 0; i < 100; ++i)
    {
        int n = rand() % 100;

        if (n < 50)
        {
            ++a;
        }
        else
        {
            ++b;
        }
    }
    printf("%d/%d\n\n\n\n\n", a, b);




    // create the application instance
    AppDelegate app;
    CCEGLView* eglView = CCEGLView::sharedOpenGLView();
    eglView->setFrameSize(1024, 768);
    //eglView->setFrameSize(800 * 2, 480 * 2);

    int ret = CCApplication::sharedApplication()->run();

#ifdef USE_WIN32_CONSOLE
    FreeConsole();
#endif

    return ret;
}
