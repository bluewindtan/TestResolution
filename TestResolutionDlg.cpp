// TestResolutionDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "TestResolution.h"
#include "TestResolutionDlg.h"
#include "RevolutionOpt.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialog
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

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CTestResolutionDlg 对话框




CTestResolutionDlg::CTestResolutionDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTestResolutionDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CTestResolutionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CTestResolutionDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON_CALC, &CTestResolutionDlg::OnBnClickedButtonCalc)
	ON_BN_CLICKED(IDC_BUTTON_CHANGE, &CTestResolutionDlg::OnBnClickedButtonChange)
END_MESSAGE_MAP()


// CTestResolutionDlg 消息处理程序

BOOL CTestResolutionDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
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
	GetDlgItem(IDC_EDIT1)->SetWindowText(_T("1280"));
	GetDlgItem(IDC_EDIT2)->SetWindowText(_T("720"));
	GetDlgItem(IDC_EDIT3)->SetWindowText(_T("1018"));
	GetDlgItem(IDC_EDIT4)->SetWindowText(_T("706"));

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CTestResolutionDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CTestResolutionDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作矩形中居中
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
		CDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标显示。
//
HCURSOR CTestResolutionDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CTestResolutionDlg::OnBnClickedButtonCalc()
{
	// TODO: Add your control notification handler code here
	bool bNormalScreen = false;
	CButton* pBtn = (CButton*)GetDlgItem(IDC_CHECK1);
	if (pBtn)
	{
		if (BST_CHECKED == pBtn->GetCheck())
		{
			bNormalScreen = true;
		}
	}
	// 计算开始
	unsigned int nWidthRender = 0, nHeightRender = 0;
	CRevolutionOpt::_CalcAndAdjustResolution_ByNormalScreen(bNormalScreen, nWidthRender, nHeightRender);
	unsigned int nWidthWindow = 1018, nHeightWindow = 706;
	CRevolutionOpt::_CalcWindowSize_ByRender(nWidthWindow, nHeightWindow, nWidthRender, nHeightRender);
	
	// 显示出分辨率
	CString strTemp("");
	strTemp.Format(_T("%d"), nWidthRender);
	GetDlgItem(IDC_EDIT1)->SetWindowText(strTemp);
	strTemp.Format(_T("%d"), nHeightRender);
	GetDlgItem(IDC_EDIT2)->SetWindowText(strTemp);
	strTemp.Format(_T("%d"), nWidthWindow);
	GetDlgItem(IDC_EDIT3)->SetWindowText(strTemp);
	strTemp.Format(_T("%d"), nHeightWindow);
	GetDlgItem(IDC_EDIT4)->SetWindowText(strTemp);

}

void CTestResolutionDlg::OnBnClickedButtonChange()
{
	// TODO: Add your control notification handler code here
	CString strTemp("");
	// 宽
	GetDlgItem(IDC_EDIT1)->GetWindowText(strTemp);
	int nWidth = _ttoi(strTemp.GetBuffer());
	strTemp.ReleaseBuffer();

	// 高
	GetDlgItem(IDC_EDIT2)->GetWindowText(strTemp);
	int nHeight = _ttoi(strTemp.GetBuffer());
	strTemp.ReleaseBuffer();
	
	// 调整分辨率
	CRevolutionOpt::_ChangeResolutionWidthAndHeight(nWidth, nHeight);
}
