
// MFCCocos2dxDlg.h : ͷ�ļ�
//

#pragma once
#include "afxwin.h"


// CMFCCocos2dxDlg �Ի���
class CMFCCocos2dxDlg : public CDialogEx
{
// ����
public:
	CMFCCocos2dxDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_MFCCOCOS2DX_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnBnClickedShowcc();

    CMFCEGLView* m_pView;
    HANDLE m_hThrd;
    
    afx_msg void OnBnClickedTest();
};
