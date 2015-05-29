#include "StdAfx.h"
#include "RevolutionOpt.h"

CRevolutionOpt::CRevolutionOpt(void)
{
}

CRevolutionOpt::~CRevolutionOpt(void)
{
}

LONG CRevolutionOpt::_ChangeResolutionWidthAndHeight(unsigned int nWidth, unsigned int nHeight, DEVMODE* pDevMode /* = NULL */)
{
	DEVMODE devM;
	memset(&devM, 0 ,sizeof(DEVMODE));
	// ���������ָ�룬��ʹ�������趨
	if (pDevMode)
	{
		memcpy_s(&devM, sizeof(DEVMODE), pDevMode, sizeof(DEVMODE));
	}
	else
	{
		devM.dmSize = sizeof(DEVMODE);
	}
	// ֻ�ı���
	devM.dmPelsWidth = nWidth;
	devM.dmPelsHeight = nHeight;
	devM.dmFields = DM_PELSWIDTH | DM_PELSHEIGHT;
	LONG lReturn = ::ChangeDisplaySettings(&devM, CDS_FULLSCREEN);
	if (DISP_CHANGE_SUCCESSFUL != lReturn)
	{
		// ������Ϣ
		CString strMsg("");
		CString strMsgAll("");
		switch (lReturn)
		{
		case DISP_CHANGE_BADDUALVIEW:
			strMsg = _T("[Result] DISP_CHANGE_BADDUALVIEW");
			break;
		case DISP_CHANGE_BADFLAGS:
			strMsg = _T("[Result] DISP_CHANGE_BADFLAGS");
			break;
		case DISP_CHANGE_BADMODE:
			strMsg = _T("[Result] DISP_CHANGE_BADMODE");
			break;
		case DISP_CHANGE_BADPARAM:
			strMsg = _T("[Result] DISP_CHANGE_BADPARAM");
			break;
		case DISP_CHANGE_FAILED:
			strMsg = _T("[Result] DISP_CHANGE_FAILED");
			break;
		case DISP_CHANGE_NOTUPDATED:
			strMsg = _T("[Result] DISP_CHANGE_NOTUPDATED");
			break;
		case DISP_CHANGE_RESTART:
			strMsg = _T("[Result] DISP_CHANGE_RESTART");
			break;
		default:
			strMsg = _T("[Result] Unknown");
			break;
		}
		// ��һ�ֵ�����Ϣ��ķ�������ʾ�Լ�����Ĵ�����Ϣ
		strMsgAll.Format(_T("ChangeDisplaySettings(%d, %d) failed with reason: %s"),
			nWidth, nHeight, strMsg);
		AfxMessageBox(strMsgAll, MB_OK | MB_ICONERROR);
		
		// �ڶ��ֵ�����Ϣ��ķ��������������Ϣ
		DWORD dw = GetLastError();
		if (0 < dw)
		{
			TCHAR szBuf[80];
			LPVOID lpMsgBuf;
			FormatMessage(
				FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
				NULL,
				dw,
				MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
				(LPTSTR) &lpMsgBuf,
				0, 
				NULL 
				);
			wsprintf(szBuf, 
				_T("ChangeDisplaySettings(%d, %d) failed with error %d: %s"),
				nWidth, nHeight,
				dw, lpMsgBuf);
			AfxMessageBox(szBuf, MB_OK | MB_ICONERROR);
			LocalFree(lpMsgBuf);
		}

	}

	return lReturn;
}

bool CRevolutionOpt::_IsApproximate(float f1, float f2)
{
	float fDiff = 0.05f; // �趨���ֵ
	// f1����С��f2 ���Ҵ���f2�������趨���ֵ
	return ((f1 - fDiff) <= f2 && (f1 + fDiff) > f2);
}

bool CRevolutionOpt::_FindApproximateResolution(float fResolution, unsigned int& nWidth, unsigned int& nHeight, DEVMODE& outDevMode)
{
	bool bFind			= false;	// �ҵ��ֱ�����
	bool bFindScale		= false;	// �ҵ�ͬ�������ķֱ���
	bool bNeedFindScale = true;		// �Ƿ���Ҫ��������ͬ�������ķֱ���

	DEVMODE devM;
	memset(&devM, 0, sizeof(DEVMODE));
	int iModeNum = 0;
	CSize sizeTemp(0, 0);
	while (::EnumDisplaySettings(NULL, iModeNum++, &devM))
	{
		if (devM.dmPelsWidth == nWidth && devM.dmPelsHeight == nHeight)
		{
			bNeedFindScale = false;
			bFindScale = true;
			bFind = true;
			memcpy_s(&outDevMode, sizeof(DEVMODE), &devM, sizeof(DEVMODE));
			break; // �ҵ���
		}
		// û���ҵ�������£���һ������ͬ������
		if (bNeedFindScale)
		{
			float fTemp = (float)devM.dmPelsWidth / (float)devM.dmPelsHeight;
			if (_IsApproximate(fTemp, fResolution))
			{
				sizeTemp.cx = devM.dmPelsWidth;
				sizeTemp.cy = devM.dmPelsHeight;
				memcpy_s(&outDevMode, sizeof(DEVMODE), &devM, sizeof(DEVMODE));
				bFindScale = true;
				if (devM.dmPelsWidth >= nWidth && devM.dmPelsHeight >= nHeight)
				{
					bNeedFindScale = false;
				}
			}
		}
	}
	// ����ҵ��������ֱ�ӷ���
	if (bFind)
	{
		return true;
	}
	// ����Ҳ��������ǿ����ҵ�ͬ�������ģ�Ҳ�����ҵ�
	 if (bFindScale)
	 {
		 nWidth = sizeTemp.cx;
		 nHeight = sizeTemp.cy;
		 // {Test By TanFeng 2011-12-30 // Ӧ��Ҫ����true���ǲ�����������أ�
		 CString strTemp("");
		 strTemp.Format(_T("�ҵ���ͬ��������:��=%d,��=%d"), nWidth, nHeight);
		 AfxMessageBox(strTemp, MB_OK);
		 return true;
		 // ~}Test By TanFeng
	 }

	 // ʲô����ƥ�䣬�򷵻��Ҳ���
	 return false;
}

bool CRevolutionOpt::_CalcWindowSize_ByRender(unsigned int& nWidthWindow, unsigned int& nHeightWindow,
											  unsigned int& nWidthRender, unsigned int& nHeightRender)
{
	// ���ֵ�����⣬��ֱ�ӷ���false
	if (0 == nWidthWindow || 0 == nHeightWindow
		|| 0 == nWidthRender || 0 == nHeightRender)
	{
		return false;
	}
	// �������ѵ���ʾ����
	float fBest = (float)nWidthWindow / (float)nHeightWindow;
	// �������Ⱦ����ʾ����
	float fRender = (float)nWidthRender / (float)nHeightRender;

	// �Ƚ����߱���
	if (fBest == fRender)
	{
		nWidthWindow = nWidthRender;
		nHeightWindow = nHeightRender;
	}
	else if (fBest < fRender)
	{
		// �Էֱ��ʵĸ�Ϊ��׼
		nHeightWindow = nHeightRender;
		nWidthWindow = (unsigned int)((float)nHeightWindow * fBest);
	} 
	else if (fBest > fRender)
	{
		// �Էֱ��ʵĸ�Ϊ��׼
		nWidthWindow = nWidthRender;
		nHeightWindow = (unsigned int)((float)nWidthWindow / fBest);
	}

	return true;
}

bool CRevolutionOpt::_CalcAndAdjustResolution_ByNormalScreen(bool bNormalScreen, unsigned int& nWidth, unsigned int& nHeight)
{
	// Ĭ�Ϸ���ֵ
	bool bReturn = true;

	// �õ���ǰ�ֱ���
	int cx = ::GetSystemMetrics(SM_CXSCREEN);
	int cy = ::GetSystemMetrics(SM_CYSCREEN);
	// �������ǰ����ʾ����
	float fScreen = (float)cx / (float)cy;

	// �����׼�ֱ���ֵ�Լ�����
	CSize arrSize[2][2] = {
		CSize(1280, 1024), CSize(1280, 768), // normal screen
		CSize(1280, 800), CSize(1280, 720), // wide screen
	};
	float arrResolution[2][2] = {
		5.0f/4.0f, 4.0f/3.0f, // normal screen
		16.0f/10.0f, 16.0f/9.0f, // wide screen
	};

	// ��Ļ�Ƿ������ı߽�ֵ
	float fBoundary = (arrResolution[0][1] + arrResolution[1][0]) / 2;
	// Ĭ����������ֱ���
	bool bNeedAdjust = false;
	// ��⵱ǰ��Ļ�ֱ����Ƿ���Ҫ�������
	if ((bNormalScreen && fScreen > fBoundary)
		|| (!bNormalScreen && fScreen <= fBoundary))
	{
		bNeedAdjust = true;
	}
	// �����Ƿ���������ȷ���ֱ���
	CSize* pSize = NULL;
	float* pResolution = NULL;
	if (bNormalScreen)
	{
		pSize = arrSize[0];
		pResolution = arrResolution[0];
	}
	else
	{
		pSize = arrSize[1];
		pResolution = arrResolution[1];
	}
	// ���¼���ֱ��ʵı߽�ֵ
	fBoundary = (pResolution[0] + pResolution[1]) / 2;
	// ���ݱ߽�ֵ���Լ���ǰ�ķֱ�����ȷ����Ⱦ�ķֱ���
	int nIndex = 1; // Ĭ��ѡ�к�һ���ֱ���
	// ֻ���ڽӽ���һ���ֱ��ʵ�ʱ�򣬲Ż�ѡ���һ���ֱ���
	if ((pResolution[0] < fScreen && fScreen < fBoundary)
		|| _IsApproximate(pResolution[0], fScreen))
	{
		nIndex = 0;
	}
	// ������ֵ
	nWidth = pSize[nIndex].cx;
	nHeight = pSize[nHeight].cy;
	// ���ҵ�ǰ֧�ֵķֱ��ʣ��ҵ���ӽ��ķֱ���
	DEVMODE devM;
	memset(&devM, 0, sizeof(DEVMODE));
	bool bFindResolution = _FindApproximateResolution(pResolution[nIndex], nWidth, nHeight, devM);
	if (!bFindResolution)
	{
		// ����Ҳ�����������Σ���ͬ�����͵�����һ�ֱַ���
		if (0 == nIndex)
		{
			nIndex = 1;
		}
		else
		{
			nIndex = 0;
		}
		// �ٴθ�ֵ
		nWidth = pSize[nIndex].cx;
		nHeight = pSize[nHeight].cy;
		memset(&devM, 0, sizeof(DEVMODE));
		// �ٴβ���
		bFindResolution = _FindApproximateResolution(pResolution[nIndex], nWidth, nHeight, devM);
	}
	// ��������Ҳ�����˵���û����õ���Ļ���ʹ�����
	if (!bFindResolution)
	{
		bReturn = false;
		ASSERT(0);
	}
	else
	{
		// �����Ҫ�����ֱ��ʵģ������������������ֱ��ʰ�
		if (bNeedAdjust)
		{
			LONG lReturn = _ChangeResolutionWidthAndHeight(nWidth, nHeight, &devM);
			if (DISP_CHANGE_SUCCESSFUL != lReturn)
			{
				bReturn = false;
			}
		}
	}

	return bReturn;
}