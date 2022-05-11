// CDlgGauss.cpp: 实现文件
//

#include "pch.h"
#include "OpenCVTest.h"
#include "CDlgGauss.h"
#include "afxdialogex.h"


// CDlgGauss 对话框

IMPLEMENT_DYNAMIC(CDlgGauss, CDialogEx)

CDlgGauss::CDlgGauss(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_GAUSS, pParent)
	, r_edit(1)
{

}

CDlgGauss::~CDlgGauss()
{
}

void CDlgGauss::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_GAUSS, r_edit);
	DDV_MinMaxInt(pDX, r_edit, 1, 200);
	DDX_Control(pDX, IDC_SPIN1, m_spin);
}


BEGIN_MESSAGE_MAP(CDlgGauss, CDialogEx)
	ON_BN_CLICKED(IDOK, &CDlgGauss::OnBnClickedOk)
END_MESSAGE_MAP()


// CDlgGauss 消息处理程序


BOOL CDlgGauss::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	m_spin.SetRange(1, 200);
	m_spin.SetBuddy(this->GetDlgItem(IDC_EDIT_GAUSS));

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void CDlgGauss::OnBnClickedOk()
{
	UpdateData(TRUE);
	// 当数据无效时尽管弹窗但也会触发这个方法，所以必须处理
	if (r_edit<1||r_edit>200)
	{
		return;
	}
	// 点击确认时进行高斯模糊
	CMainFrame* pMain = (CMainFrame*)AfxGetApp()->m_pMainWnd;
	CChildFrame* pChild = (CChildFrame*)pMain->GetActiveFrame();
	COpenCVTestView* pView = (COpenCVTestView*)pChild->GetActiveView();
	if (pView==nullptr)
	{
		MessageBox(_T("发生意料之外的错误。pView为 nullptr。"));
		return;
	}
	COpenCVTestDoc* pDoc = (COpenCVTestDoc*)pView->GetDocument();
	cv::Size sz(r_edit * 2 + 1, r_edit * 2 + 1);
	GaussianBlur(pDoc->timg, pDoc->timg, sz,0,0);
	resize(pDoc->timg, pDoc->simg, cv::Size(0, 0), pDoc->GetScale() * 0.01, pDoc->GetScale() * 0.01);
	pView->Invalidate();
	CDialogEx::OnOK();
}
