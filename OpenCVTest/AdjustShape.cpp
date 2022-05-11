// AdjustShape.cpp: 实现文件
//

#include "pch.h"
#include "OpenCVTest.h"
#include "AdjustShape.h"
#include "afxdialogex.h"


// AdjustShape 对话框

IMPLEMENT_DYNAMIC(CAdjustShape, CDialogEx)

CAdjustShape::CAdjustShape(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_ADJ_SHAPE, pParent)
	, width(_T(""))
	, height(_T(""))
	, bKeepShape(FALSE)
	,iwid(0)
	,ihei(0)
{

}

CAdjustShape::~CAdjustShape()
{
}

void CAdjustShape::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_WIDTH, width);
	DDX_Text(pDX, IDC_EDIT_HEIGHT, height);
	DDX_Check(pDX, IDC_KEEP_SHAPE, bKeepShape);
}


BEGIN_MESSAGE_MAP(CAdjustShape, CDialogEx)
	ON_BN_CLICKED(IDC_KEEP_SHAPE, &CAdjustShape::OnBnClickedKeepShape)
	ON_EN_KILLFOCUS(IDC_EDIT_WIDTH, &CAdjustShape::OnEnKillfocusEditWidth)
	ON_BN_CLICKED(IDOK, &CAdjustShape::OnBnClickedOk)
	ON_EN_KILLFOCUS(IDC_EDIT_HEIGHT, &CAdjustShape::OnEnKillfocusEditHeight)
END_MESSAGE_MAP()


// AdjustShape 消息处理程序


BOOL CAdjustShape::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 打开对话框时从文档中获取当前图像大小并显示在框中
	CMainFrame* pMain = (CMainFrame*)AfxGetApp()->m_pMainWnd;
	CChildFrame* pChild = (CChildFrame*)pMain->GetActiveFrame();
	COpenCVTestView* pView = (COpenCVTestView*)pChild->GetActiveView();
	if (pView==nullptr)
	{
		return FALSE;
	}

	COpenCVTestDoc *pDoc = (COpenCVTestDoc*)pView->GetDocument();
	iwid = pDoc->timg.cols;
	ihei = pDoc->timg.rows;
	width.Format(_T("%d"),iwid);
	height.Format(_T("%d"), ihei);
	bKeepShape = TRUE; // 打开对话框时默认勾选保持比
	UpdateData(FALSE); // 指定从变量更新Edit编辑框中的数值

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void CAdjustShape::OnBnClickedKeepShape()
{
	// 单击保持宽高比复选框时更改变量
	UpdateData(TRUE);
}


void CAdjustShape::OnEnKillfocusEditWidth()
{
	// 控件失去焦点时更新值
	UpdateData(TRUE); // 指定由控件更新变量
	int old = iwid;
	int tmp = _ttoi(width);
	if (tmp<1)
	{
		MessageBox(_T("必须输入一个介于1到99999之间的值！已插入最接近的值。"));
		tmp = 1;
		
	}
	else if (tmp>99999)
	{
		MessageBox(_T("必须输入一个介于1到99999之间的值！已插入最接近的值。"));
		tmp = 99999;
	}
	iwid = tmp;
	width.Format(_T("%d"), iwid);
	// 如果勾选了保持宽高比，则根据比例计算另一个框的值
	if (bKeepShape==TRUE)
	{
		// 此算式顺序不可更改，否则会因为整型相除丢失精度而发生错误
		ihei = iwid * ihei / old;
		if (ihei < 1)
			ihei = 1;
		else if (ihei > 99999)
			ihei = 99999;
		height.Format(_T("%d"), ihei);
		
	}
	UpdateData(FALSE); // 根据变量更新控件的值
}

void CAdjustShape::OnEnKillfocusEditHeight()
{
	// 控件失去焦点时更新值
	UpdateData(TRUE);
	int old = ihei;
	int tmp = _ttoi(height);
	if (tmp < 1)
	{
		MessageBox(_T("必须输入一个介于1到99999之间的值！已插入最接近的值。"));
		tmp = 1;

	}
	else if (tmp > 99999)
	{
		MessageBox(_T("必须输入一个介于1到99999之间的值！已插入最接近的值。"));
		tmp = 99999;
	}
	ihei = tmp;
	height.Format(_T("%d"), ihei);
	// 如果勾选了保持宽高比，则根据比例计算另一个框的值
	if (bKeepShape == TRUE)
	{
		// 此算式顺序不可更改，否则会因为整型相除丢失精度而发生错误
		iwid = ihei * iwid / old;
		if (iwid < 1)
			iwid = 1;
		else if (iwid > 99999)
			iwid = 99999;
		width.Format(_T("%d"), iwid);

	}
	UpdateData(FALSE); // 根据变量更新控件的值
}

void CAdjustShape::OnBnClickedOk()
{
	// 单击确定时执行调整大小指令
	cv::Size sz(iwid, ihei);
	CMainFrame* pMain = (CMainFrame*)AfxGetApp()->m_pMainWnd;
	CChildFrame* pChild = (CChildFrame*)pMain->GetActiveFrame();
	COpenCVTestView* pView = (COpenCVTestView*)pChild->GetActiveView();
	if (pView == nullptr)
	{
		MessageBox(_T("出现错误！pView 为 nullptr。"));
		return;
	}

	COpenCVTestDoc* pDoc = (COpenCVTestDoc*)pView->GetDocument();
	if (pDoc==nullptr)
	{
		MessageBox(_T("出现错误！pDoc 为 nullptr。"));
		return;
	}
	resize(pDoc->timg, pDoc->timg, sz);
	// 更改timg后需要根据当前显示比例更改simg，并更新docSize以正确重绘滚动条
	resize(pDoc->timg, pDoc->simg, cv::Size(0,0), pDoc->GetScale() * 0.01, pDoc->GetScale() * 0.01);
	pDoc->UpdateDocSize();
	// 强制更新所有视图以重绘滚动条
	pDoc->UpdateAllViews(NULL);
	pView->Invalidate();
	CDialogEx::OnOK();
}


