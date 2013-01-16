
// MFCCocos2dxDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "MFCCocos2dx.h"
#include "MFCEGLView.h"
#include "MFCCocos2dxDlg.h"
#include "afxdialogex.h"
#include "AppDelegate.h"
#include "CodeTank.h"
#include "GameLogic.h"
#include "GameDisplay.h"
#include "TankSprite.h"
#include "Unit.h"
#include "Skill.h"
#include "HelloWorldScene.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CMFCCocos2dxDlg 对话框



CMFCCocos2dxDlg::CMFCCocos2dxDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMFCCocos2dxDlg::IDD, pParent)
    , m_pView(NULL)
    , m_hThrd(NULL)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMFCCocos2dxDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CMFCCocos2dxDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
    ON_BN_CLICKED(IDC_SHOWCC, &CMFCCocos2dxDlg::OnBnClickedShowcc)
    ON_BN_CLICKED(IDC_TEST, &CMFCCocos2dxDlg::OnBnClickedTest)
END_MESSAGE_MAP()


// CMFCCocos2dxDlg 消息处理程序

BOOL CMFCCocos2dxDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CMFCCocos2dxDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CMFCCocos2dxDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CMFCCocos2dxDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

DWORD WINAPI Cocos2dThreadProc(PVOID pParam)
{

    CMFCEGLView* pView = (CMFCEGLView*)pParam;
    pView->setFrameSize(800, 480);
    int ret = CCApplication::sharedApplication()->run();
    return 0;
}

AppDelegate g_oCCApp;

void CMFCCocos2dxDlg::OnBnClickedShowcc()
{
    // TODO: Add your control notification handler code here

    if (m_hThrd)
    {
        ::ShowWindow(m_pView->getHWnd(), SW_SHOWNORMAL);
        //m_pView->setFrameSizeNoMove(100, 100);
        return;
    }
    else
    {
        m_pView = new CMFCEGLView;
        //m_pView->m_hParent = GetSafeHwnd();
        CCEGLView::sharedOpenGLView(m_pView);
        m_hThrd = ::CreateThread(NULL, 0, Cocos2dThreadProc, m_pView, 0, NULL);
    }
}


void CMFCCocos2dxDlg::OnBnClickedTest()
{
    // TODO: Add your control notification handler code here
    CCGameManager* pGm = CCGameManager::shareGameManager();
    CCCtrlLayer* pCtrlLayer = pGm->m_pCtrlLayer;
    //pCtrlLayer->setVisible(!pCtrlLayer->isVisible());
    CGameUnit* pTankU = dynamic_cast<CGameUnit*>(pGm->getTankByKey(pGm->m_iPlayerKey)->getLogicBody());
    pTankU->delSkill(dynamic_cast<CSkill*>(pTankU->m_oArrSkill.lastObject()));
}
