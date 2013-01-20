#include "CommInc.h"
#include "mainwindow.h"
#include "Cocos2dxThread.h"
#include "ui_mainwindow.h"
#include "UipEditor.h"
#include "AppDelegate.h"


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    Ui::MainWindow* pUi = w.getUi();
    QWidget* pDisplay = pUi->wndCocos2dxDisplay;
    HWND hWnd = (HWND)(pDisplay->winId());
    CCocos2dxThread oThrd(hWnd, 800, 480, 1);
    oThrd.start();
    
    return a.exec();
}
