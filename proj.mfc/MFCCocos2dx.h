
// MFCCocos2dx.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CMFCCocos2dxApp:
// �йش����ʵ�֣������ MFCCocos2dx.cpp
//

class CMFCCocos2dxApp : public CWinApp
{
public:
	CMFCCocos2dxApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CMFCCocos2dxApp theApp;