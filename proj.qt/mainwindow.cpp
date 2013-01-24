#include "CommInc.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "UipEditor.h"
#include "UnitEditorScene.h"
#include "GameCtrl.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_pUe(NULL)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

Ui::MainWindow* MainWindow::getUi() const
{
    return ui;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    event->ignore();
    if (m_pUe && !m_pUe->saveCheck())
    {
        return;
    }
    TerminateProcess(GetCurrentProcess(), 0);
    //CCUnitEditorSceneCtrlLayer::m_bExit = true;
}

void MainWindow::on_btnUnitEditor_clicked()
{
    while (!CCUnitEditorSceneCtrlLayer::m_bReady)
    {
        if (QMessageBox::information(this, A2U("单位编辑器"), A2U("游戏尚未加载完毕,\n无法打开单位编辑器"), A2U("重试(&R)"), A2U("取消(&C)"), QString(), 0, 1) != 0)
        {
            return;
        }
    }

    if (!m_pUe)
    {
        m_pUe = new CUipEditor();
    }
    m_pUe->showNormal();
    SetWindowPos((HWND)m_pUe->winId(), HWND_TOP, 0, 0, 0, 0, SWP_SHOWWINDOW | SWP_NOMOVE | SWP_NOSIZE);
    //m_pUe->setFocus();
}

void MainWindow::on_btnRunEdit_clicked()
{
    M_DEF_GM(pGm);
    if (ui->btnRunEdit->isChecked())
    {
        ui->btnRunEdit->setText(A2U("运行中"));
        pGm->setCmd(CGameManager::kCmdRun);
    }
    else
    {
        ui->btnRunEdit->setText(A2U("运行"));
        pGm->setCmd(CGameManager::kCmdEdit);
    }
}
