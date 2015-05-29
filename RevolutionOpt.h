/////////////////////////////////////////////////////////////////
/// @file RevolutionOpt.h
///
/// @purpose �ֱ�����ز�����
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
	// ���㲢�ҵ����ֱ���
	static bool _CalcAndAdjustResolution_ByNormalScreen(bool bNormalScreen, unsigned int& nWidth, unsigned int& nHeight);

	// ������Ⱦ�ֱ�������������ڷֱ���
	static bool _CalcWindowSize_ByRender(unsigned int& nWidthWindow, unsigned int& nHeightWindow,
		unsigned int& nWidthRender, unsigned int& nHeightRender);

	// �ж������ֱ��ʱ���ֵ֮���Ƿ�Լ���ڡ�
	static bool _IsApproximate(float f1, float f2);

	// ���ҵ�ǰ֧�ֵ���ӽ��ķֱ���
	static bool _FindApproximateResolution(float fResolution, unsigned int& nWidth, unsigned int& nHeight, DEVMODE& outDevMode);

	// ����ȫ���ֱ���
	static LONG _ChangeResolutionWidthAndHeight(unsigned int nWidth, unsigned int nHeight, DEVMODE* pDevMode = NULL);

};
