// CDlgMidBlur.cpp: 实现文件
//

#include "pch.h"
#include "OpenCVTest.h"
#include "CDlgMidBlur.h"
#include "afxdialogex.h"


// CDlgMidBlur 对话框

IMPLEMENT_DYNAMIC(CDlgMidBlur, CDialogEx)

CDlgMidBlur::CDlgMidBlur(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MID_BLR, pParent)
	, kw(3)
{

}

CDlgMidBlur::~CDlgMidBlur()
{
}

void CDlgMidBlur::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_MID_BLUR1, kw);
	DDV_MinMaxInt(pDX, kw, 1, 200);
	DDX_Control(pDX, IDC_SPIN1, m_spin1);
}


BEGIN_MESSAGE_MAP(CDlgMidBlur, CDialogEx)
	ON_BN_CLICKED(IDOK, &CDlgMidBlur::OnBnClickedOk)
END_MESSAGE_MAP()


// CDlgMidBlur 消息处理程序


BOOL CDlgMidBlur::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_spin1.SetRange(1, 200);
	m_spin1.SetBuddy(this->GetDlgItem(IDC_MID_BLUR1));

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void CDlgMidBlur::OnBnClickedOk()
{
	UpdateData(TRUE);
	// 当数据无效时尽管弹窗但也会触发这个方法，所以必须处理
	if (kw < 1 || kw >200)
	{
		return;
	}
	// 点击确认时进行中值模糊
	CMainFrame* pMain = (CMainFrame*)AfxGetApp()->m_pMainWnd;
	CChildFrame* pChild = (CChildFrame*)pMain->GetActiveFrame();
	COpenCVTestView* pView = (COpenCVTestView*)pChild->GetActiveView();
	if (pView == nullptr)
	{
		MessageBox(_T("发生意料之外的错误。pView为 nullptr。"));
		return;
	}
	COpenCVTestDoc* pDoc = (COpenCVTestDoc*)pView->GetDocument();
	medianBlur(pDoc->timg, pDoc->timg, 2*kw+1);
	resize(pDoc->timg, pDoc->simg, cv::Size(0, 0), pDoc->GetScale() * 0.01, pDoc->GetScale() * 0.01);
	pView->Invalidate();
	CDialogEx::OnOK();
}
