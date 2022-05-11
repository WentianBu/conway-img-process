// CDlgBlur.cpp: 实现文件
//

#include "pch.h"
#include "OpenCVTest.h"
#include "CDlgBlur.h"
#include "afxdialogex.h"


// CDlgBlur 对话框

IMPLEMENT_DYNAMIC(CDlgBlur, CDialogEx)

CDlgBlur::CDlgBlur(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_BLUR, pParent)
	, kw(1)
	, kh(1)
{

}

CDlgBlur::~CDlgBlur()
{
}

void CDlgBlur::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_BLUR1, kw);
	DDV_MinMaxInt(pDX, kw, 1, 200);
	DDX_Control(pDX, IDC_SPIN1, m_spin1);
	DDX_Text(pDX, IDC_BLUR2, kh);
	DDV_MinMaxInt(pDX, kh, 1, 200);
	DDX_Control(pDX, IDC_SPIN2, m_spin2);
}


BEGIN_MESSAGE_MAP(CDlgBlur, CDialogEx)
END_MESSAGE_MAP()


// CDlgBlur 消息处理程序


BOOL CDlgBlur::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	m_spin1.SetRange(1, 200);
	m_spin1.SetBuddy(this->GetDlgItem(IDC_BLUR1));
	m_spin2.SetRange(1, 200);
	m_spin2.SetBuddy(this->GetDlgItem(IDC_BLUR2));
	// TODO:  在此添加额外的初始化

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void CDlgBlur::OnOK()
{
	UpdateData(TRUE);
	// 当数据无效时尽管弹窗但也会触发这个方法，所以必须处理
	if ( kw < 1 || kw >200||kh<1||kh>200)
	{
		return;
	}
	// 点击确认时进行高斯模糊
	CMainFrame* pMain = (CMainFrame*)AfxGetApp()->m_pMainWnd;
	CChildFrame* pChild = (CChildFrame*)pMain->GetActiveFrame();
	COpenCVTestView* pView = (COpenCVTestView*)pChild->GetActiveView();
	if (pView == nullptr)
	{
		MessageBox(_T("发生意料之外的错误。pView为 nullptr。"));
		return;
	}
	COpenCVTestDoc* pDoc = (COpenCVTestDoc*)pView->GetDocument();
	blur(pDoc->timg, pDoc->timg, cv::Size(kw,kh));
	resize(pDoc->timg, pDoc->simg, cv::Size(0, 0), pDoc->GetScale() * 0.01, pDoc->GetScale() * 0.01);
	pView->Invalidate();

	CDialogEx::OnOK();
}
