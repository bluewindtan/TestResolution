// TestResolution.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CTestResolutionApp:
// �йش����ʵ�֣������ TestResolution.cpp
//

class CTestResolutionApp : public CWinApp
{
public:
	CTestResolutionApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CTestResolutionApp theApp;