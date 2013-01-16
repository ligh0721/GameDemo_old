
// MFCCocos2dxDlg.cpp : ʵ���ļ�
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


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
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


// CMFCCocos2dxDlg �Ի���



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


// CMFCCocos2dxDlg ��Ϣ�������

BOOL CMFCCocos2dxDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
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

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CMFCCocos2dxDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
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
