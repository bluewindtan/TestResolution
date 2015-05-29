// TestResolutionDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "TestResolution.h"
#include "TestResolutionDlg.h"
#include "RevolutionOpt.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialog
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

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CTestResolutionDlg �Ի���




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


// CTestResolutionDlg ��Ϣ�������

BOOL CTestResolutionDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
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

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	GetDlgItem(IDC_EDIT1)->SetWindowText(_T("1280"));
	GetDlgItem(IDC_EDIT2)->SetWindowText(_T("720"));
	GetDlgItem(IDC_EDIT3)->SetWindowText(_T("1018"));
	GetDlgItem(IDC_EDIT4)->SetWindowText(_T("706"));

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CTestResolutionDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ��������о���
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
		CDialog::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù����ʾ��
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
	// ���㿪ʼ
	unsigned int nWidthRender = 0, nHeightRender = 0;
	CRevolutionOpt::_CalcAndAdjustResolution_ByNormalScreen(bNormalScreen, nWidthRender, nHeightRender);
	unsigned int nWidthWindow = 1018, nHeightWindow = 706;
	CRevolutionOpt::_CalcWindowSize_ByRender(nWidthWindow, nHeightWindow, nWidthRender, nHeightRender);
	
	// ��ʾ���ֱ���
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
	// ��
	GetDlgItem(IDC_EDIT1)->GetWindowText(strTemp);
	int nWidth = _ttoi(strTemp.GetBuffer());
	strTemp.ReleaseBuffer();

	// ��
	GetDlgItem(IDC_EDIT2)->GetWindowText(strTemp);
	int nHeight = _ttoi(strTemp.GetBuffer());
	strTemp.ReleaseBuffer();
	
	// �����ֱ���
	CRevolutionOpt::_ChangeResolutionWidthAndHeight(nWidth, nHeight);
}
