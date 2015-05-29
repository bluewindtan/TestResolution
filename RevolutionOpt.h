/////////////////////////////////////////////////////////////////
/// @file RevolutionOpt.h
///
/// @purpose 分辨率相关操作类
///
/// @author tanfeng
///
/// @date 12/30/2011
/// -------------------------------------------------------------
/// ChangeLog:
///
/// -------------------------------------------------------------
/// Copyright (C) 2011 - All Rights Reserved
/////////////////////////////////////////////////////////////////
#pragma once

class CRevolutionOpt
{
private:
	CRevolutionOpt(void);
	~CRevolutionOpt(void);

public:
	// 计算并且调整分辨率
	static bool _CalcAndAdjustResolution_ByNormalScreen(bool bNormalScreen, unsigned int& nWidth, unsigned int& nHeight);

	// 根据渲染分辨率来计算出窗口分辨率
	static bool _CalcWindowSize_ByRender(unsigned int& nWidthWindow, unsigned int& nHeightWindow,
		unsigned int& nWidthRender, unsigned int& nHeightRender);

	// 判断两个分辨率比例值之间是否“约等于”
	static bool _IsApproximate(float f1, float f2);

	// 查找当前支持的最接近的分辨率
	static bool _FindApproximateResolution(float fResolution, unsigned int& nWidth, unsigned int& nHeight, DEVMODE& outDevMode);

	// 调整全屏分辨率
	static LONG _ChangeResolutionWidthAndHeight(unsigned int nWidth, unsigned int nHeight, DEVMODE* pDevMode = NULL);

};
