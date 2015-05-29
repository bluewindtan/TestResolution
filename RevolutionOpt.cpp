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
	// 如果传入了指针，则使用它的设定
	if (pDevMode)
	{
		memcpy_s(&devM, sizeof(DEVMODE), pDevMode, sizeof(DEVMODE));
	}
	else
	{
		devM.dmSize = sizeof(DEVMODE);
	}
	// 只改变宽高
	devM.dmPelsWidth = nWidth;
	devM.dmPelsHeight = nHeight;
	devM.dmFields = DM_PELSWIDTH | DM_PELSHEIGHT;
	LONG lReturn = ::ChangeDisplaySettings(&devM, CDS_FULLSCREEN);
	if (DISP_CHANGE_SUCCESSFUL != lReturn)
	{
		// 错误信息
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
		// 第一种弹出信息框的方法：显示自己保存的错误信息
		strMsgAll.Format(_T("ChangeDisplaySettings(%d, %d) failed with reason: %s"),
			nWidth, nHeight, strMsg);
		AfxMessageBox(strMsgAll, MB_OK | MB_ICONERROR);
		
		// 第二种弹出信息框的方法：处理错误信息
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
	float fDiff = 0.05f; // 设定误差值
	// f1不能小于f2 并且大于f2，超过设定误差值
	return ((f1 - fDiff) <= f2 && (f1 + fDiff) > f2);
}

bool CRevolutionOpt::_FindApproximateResolution(float fResolution, unsigned int& nWidth, unsigned int& nHeight, DEVMODE& outDevMode)
{
	bool bFind			= false;	// 找到分辨率了
	bool bFindScale		= false;	// 找到同样比例的分辨率
	bool bNeedFindScale = true;		// 是否需要继续查找同样比例的分辨率

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
			break; // 找到了
		}
		// 没有找到的情况下，退一步，找同样比例
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
	// 如果找到，则可以直接返回
	if (bFind)
	{
		return true;
	}
	// 如果找不到，但是可以找到同样比例的，也算是找到
	 if (bFindScale)
	 {
		 nWidth = sizeTemp.cx;
		 nHeight = sizeTemp.cy;
		 // {Test By TanFeng 2011-12-30 // 应该要返回true，是不是这个问题呢？
		 CString strTemp("");
		 strTemp.Format(_T("找到了同样比例的:宽=%d,高=%d"), nWidth, nHeight);
		 AfxMessageBox(strTemp, MB_OK);
		 return true;
		 // ~}Test By TanFeng
	 }

	 // 什么都不匹配，则返回找不到
	 return false;
}

bool CRevolutionOpt::_CalcWindowSize_ByRender(unsigned int& nWidthWindow, unsigned int& nHeightWindow,
											  unsigned int& nWidthRender, unsigned int& nHeightRender)
{
	// 如果值有问题，则直接返回false
	if (0 == nWidthWindow || 0 == nHeightWindow
		|| 0 == nWidthRender || 0 == nHeightRender)
	{
		return false;
	}
	// 计算出最佳的显示比例
	float fBest = (float)nWidthWindow / (float)nHeightWindow;
	// 计算出渲染的显示比例
	float fRender = (float)nWidthRender / (float)nHeightRender;

	// 比较两者比例
	if (fBest == fRender)
	{
		nWidthWindow = nWidthRender;
		nHeightWindow = nHeightRender;
	}
	else if (fBest < fRender)
	{
		// 以分辨率的高为基准
		nHeightWindow = nHeightRender;
		nWidthWindow = (unsigned int)((float)nHeightWindow * fBest);
	} 
	else if (fBest > fRender)
	{
		// 以分辨率的高为基准
		nWidthWindow = nWidthRender;
		nHeightWindow = (unsigned int)((float)nWidthWindow / fBest);
	}

	return true;
}

bool CRevolutionOpt::_CalcAndAdjustResolution_ByNormalScreen(bool bNormalScreen, unsigned int& nWidth, unsigned int& nHeight)
{
	// 默认返回值
	bool bReturn = true;

	// 得到当前分辨率
	int cx = ::GetSystemMetrics(SM_CXSCREEN);
	int cy = ::GetSystemMetrics(SM_CYSCREEN);
	// 计算出当前的显示比例
	float fScreen = (float)cx / (float)cy;

	// 定义标准分辨率值以及比例
	CSize arrSize[2][2] = {
		CSize(1280, 1024), CSize(1280, 768), // normal screen
		CSize(1280, 800), CSize(1280, 720), // wide screen
	};
	float arrResolution[2][2] = {
		5.0f/4.0f, 4.0f/3.0f, // normal screen
		16.0f/10.0f, 16.0f/9.0f, // wide screen
	};

	// 屏幕是否普屏的边界值
	float fBoundary = (arrResolution[0][1] + arrResolution[1][0]) / 2;
	// 默认无需调整分辨率
	bool bNeedAdjust = false;
	// 检测当前屏幕分辨率是否需要程序调整
	if ((bNormalScreen && fScreen > fBoundary)
		|| (!bNormalScreen && fScreen <= fBoundary))
	{
		bNeedAdjust = true;
	}
	// 根据是否普屏，来确定分辨率
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
	// 重新计算分辨率的边界值
	fBoundary = (pResolution[0] + pResolution[1]) / 2;
	// 根据边界值，以及当前的分辨率来确定渲染的分辨率
	int nIndex = 1; // 默认选中后一个分辨率
	// 只有在接近第一个分辨率的时候，才会选择第一个分辨率
	if ((pResolution[0] < fScreen && fScreen < fBoundary)
		|| _IsApproximate(pResolution[0], fScreen))
	{
		nIndex = 0;
	}
	// 初步赋值
	nWidth = pSize[nIndex].cx;
	nHeight = pSize[nHeight].cy;
	// 查找当前支持的分辨率，找到最接近的分辨率
	DEVMODE devM;
	memset(&devM, 0, sizeof(DEVMODE));
	bool bFindResolution = _FindApproximateResolution(pResolution[nIndex], nWidth, nHeight, devM);
	if (!bFindResolution)
	{
		// 如果找不到，则退其次，找同样类型的另外一种分辨率
		if (0 == nIndex)
		{
			nIndex = 1;
		}
		else
		{
			nIndex = 0;
		}
		// 再次赋值
		nWidth = pSize[nIndex].cx;
		nHeight = pSize[nHeight].cy;
		memset(&devM, 0, sizeof(DEVMODE));
		// 再次查找
		bFindResolution = _FindApproximateResolution(pResolution[nIndex], nWidth, nHeight, devM);
	}
	// 如果还是找不到，说明用户设置的屏幕类型错误了
	if (!bFindResolution)
	{
		bReturn = false;
		ASSERT(0);
	}
	else
	{
		// 如果需要调整分辨率的，则别忘了最后程序调整分辨率啊
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