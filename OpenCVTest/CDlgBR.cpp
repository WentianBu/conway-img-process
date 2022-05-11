// CDlgBR.cpp: 实现文件
//

#include "pch.h"
#include "OpenCVTest.h"
#include "CDlgBR.h"
#include "afxdialogex.h"


// CDlgBR 对话框

IMPLEMENT_DYNAMIC(CDlgBR, CDialogEx)

CDlgBR::CDlgBR(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_BR, pParent)
	, bright(0)
	, contr(0)
{

}

CDlgBR::~CDlgBR()
{
}

void CDlgBR::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, bright);
	DDV_MinMaxInt(pDX, bright, -255, 255);
	DDX_Text(pDX, IDC_EDIT2, contr);
	DDV_MinMaxInt(pDX, contr, -255, 255);
	DDX_Control(pDX, IDC_SLIDER2, m_slider1);
	DDX_Control(pDX, IDC_SLIDER3, m_slider2);
}


BEGIN_MESSAGE_MAP(CDlgBR, CDialogEx)
	ON_WM_HSCROLL()
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER2, &CDlgBR::OnNMReleasedcaptureSlider2)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER3, &CDlgBR::OnNMReleasedcaptureSlider3)
	ON_BN_CLICKED(IDCANCEL, &CDlgBR::OnBnClickedCancel)
	ON_EN_KILLFOCUS(IDC_EDIT1, &CDlgBR::OnEnKillfocusEdit1)
	ON_EN_KILLFOCUS(IDC_EDIT2, &CDlgBR::OnEnKillfocusEdit2)
END_MESSAGE_MAP()


// CDlgBR 消息处理程序


BOOL CDlgBR::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置滑动条的范围和初始值
	m_slider1.SetRange(-255, 255, TRUE);// 这里TRUE参数指定滑动条重绘，使得滑块能够居中显示
	m_slider1.SetPos(0);
	m_slider2.SetRange(-255, 255, TRUE);
	m_slider2.SetPos(0);

	// 读取当前文档的原始状态并保存至old变量
	CMainFrame* pMain = (CMainFrame*)AfxGetApp()->m_pMainWnd;
	CChildFrame* pChild = (CChildFrame*)pMain->GetActiveFrame();
	COpenCVTestView* pView = (COpenCVTestView*)pChild->GetActiveView();
	if (pView == nullptr)
	{
		MessageBox(_T("发生意料之外的错误。pView为 nullptr。"));
		return FALSE;
	}
	COpenCVTestDoc* pDoc = (COpenCVTestDoc*)pView->GetDocument();
	old = pDoc->timg.clone();
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}

// 在滑动条滑动时实时更改编辑框数值
void CDlgBR::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	CSliderCtrl* pSlider1 = (CSliderCtrl*)GetDlgItem(IDC_SLIDER2);
	bright = pSlider1->GetPos();
	CSliderCtrl* pSlider2 = (CSliderCtrl*)GetDlgItem(IDC_SLIDER3);
	contr = pSlider2->GetPos();
	UpdateData(FALSE);


	CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
}

// 松开滑块时自动进行预览
void CDlgBR::OnNMReleasedcaptureSlider2(NMHDR* pNMHDR, LRESULT* pResult)
{
	CMainFrame* pMain = (CMainFrame*)AfxGetApp()->m_pMainWnd;
	CChildFrame* pChild = (CChildFrame*)pMain->GetActiveFrame();
	COpenCVTestView* pView = (COpenCVTestView*)pChild->GetActiveView();
	if (pView == nullptr)
	{
		MessageBox(_T("发生意料之外的错误。pView为 nullptr。"));
		return;
	}
	COpenCVTestDoc* pDoc = (COpenCVTestDoc*)pView->GetDocument();

	UpdateData(TRUE);
	// 调用自定义函数进行变换，然后显示
	pDoc->timg = this->ChangeBC();
	resize(pDoc->timg, pDoc->simg, cv::Size(0, 0), pDoc->GetScale() * 0.01, pDoc->GetScale() * 0.01);
	pView->Invalidate();
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
}

void CDlgBR::OnNMReleasedcaptureSlider3(NMHDR* pNMHDR, LRESULT* pResult)
{
	CMainFrame* pMain = (CMainFrame*)AfxGetApp()->m_pMainWnd;
	CChildFrame* pChild = (CChildFrame*)pMain->GetActiveFrame();
	COpenCVTestView* pView = (COpenCVTestView*)pChild->GetActiveView();
	if (pView == nullptr)
	{
		MessageBox(_T("发生意料之外的错误。pView为 nullptr。"));
		return;
	}
	COpenCVTestDoc* pDoc = (COpenCVTestDoc*)pView->GetDocument();

	UpdateData(TRUE);
	// 调用自定义函数进行变换，然后显示
	pDoc->timg = this->ChangeBC();
	resize(pDoc->timg, pDoc->simg, cv::Size(0, 0), pDoc->GetScale() * 0.01, pDoc->GetScale() * 0.01);
	pView->Invalidate();
	*pResult = 0;
}



// 根据contr的值获取alpha并进行线性变换
Mat CDlgBR::ChangeBC()
{
	// 线性变换的原理：g(i,j)=a*f(i,j)+b，注意计算后超出255或者小于0的值会被处理成255或者0

	// 将滑动条中的负值处理成1/contr，即缩小contr倍
	double alpha;
	if (contr > 0)
		alpha = contr;
	else if (contr == 0)
		alpha = 1;
	else
		alpha = 1.0 / abs(contr);
	// 考虑到实际使用效果，此处对处理出的contr开四次根号再作为alpha值进行线性变换
	// 以使图像对比度的改变较为平缓，调整更加精细
	// 如果不进行如此处理，则contr的绝对值大于10时，图像已经完全成为灰色或者几乎全白，更大的调节值没有意义
	alpha = pow(alpha, 0.25);

	Mat dst;
	old.convertTo(dst, -1, alpha, bright);
	return dst;
}




void CDlgBR::OnBnClickedCancel()
{
	// 单击取消时，将原始数据从old中还原并重绘画面。
	CMainFrame* pMain = (CMainFrame*)AfxGetApp()->m_pMainWnd;
	CChildFrame* pChild = (CChildFrame*)pMain->GetActiveFrame();
	COpenCVTestView* pView = (COpenCVTestView*)pChild->GetActiveView();
	COpenCVTestDoc* pDoc = (COpenCVTestDoc*)pView->GetDocument();

	pDoc->timg = old.clone();
	resize(pDoc->timg, pDoc->simg, cv::Size(0, 0), pDoc->GetScale() * 0.01, pDoc->GetScale() * 0.01);
	pView->Invalidate();
	CDialogEx::OnCancel();
}


void CDlgBR::OnEnKillfocusEdit1()
{
	// 编辑框失去焦点将数据同步到对应变量
	UpdateData(TRUE);
	// 如果用户输入非法数值，则会自动触发弹窗，但是bright的值仍然非法，故必须在此进行处理
	if (bright < -255)
		bright = -255;
	else if (bright > 255)
		bright = 255;
	UpdateData(FALSE);// 反向同步数据以更改编辑框内显示的值

	// 根据变量数值设置滑动条
	// 注意：bright对应滑动条ID为IDC_SLIDER2，contr对应IDC_SLIDER3，懒得改了
	CSliderCtrl* pSld = (CSliderCtrl*)GetDlgItem(IDC_SLIDER2);
	pSld->SetPos(bright);

	//失去焦点时进行预览
	CMainFrame* pMain = (CMainFrame*)AfxGetApp()->m_pMainWnd;
	CChildFrame* pChild = (CChildFrame*)pMain->GetActiveFrame();
	COpenCVTestView* pView = (COpenCVTestView*)pChild->GetActiveView();
	COpenCVTestDoc* pDoc = (COpenCVTestDoc*)pView->GetDocument();
	pDoc->timg = this->ChangeBC();
	resize(pDoc->timg, pDoc->simg, cv::Size(0, 0), pDoc->GetScale() * 0.01, pDoc->GetScale() * 0.01);
	pView->Invalidate();
}


void CDlgBR::OnEnKillfocusEdit2()
{
	// 编辑框失去焦点时将数据同步到对应变量
	UpdateData(TRUE);
	// 如果用户输入非法数值，则会自动触发弹窗，但是contr的值仍然非法，故必须在此进行处理
	if (contr < -255)
		contr = -255;
	else if (contr > 255)
		contr = 255;

	UpdateData(FALSE);// 反向同步数据以更改编辑框内显示的值

	// 根据变量数值设置滑动条
	// 注意：bright对应滑动条ID为IDC_SLIDER2，contr对应IDC_SLIDER3，懒得改了
	CSliderCtrl* pSld = (CSliderCtrl*)GetDlgItem(IDC_SLIDER3);
	pSld->SetPos(contr);

	// 失去焦点时进行预览
	CMainFrame* pMain = (CMainFrame*)AfxGetApp()->m_pMainWnd;
	CChildFrame* pChild = (CChildFrame*)pMain->GetActiveFrame();
	COpenCVTestView* pView = (COpenCVTestView*)pChild->GetActiveView();
	COpenCVTestDoc* pDoc = (COpenCVTestDoc*)pView->GetDocument();
	pDoc->timg = this->ChangeBC();
	resize(pDoc->timg, pDoc->simg, cv::Size(0, 0), pDoc->GetScale() * 0.01, pDoc->GetScale() * 0.01);
	pView->Invalidate();
}
