// CDlgUSM.cpp: 实现文件
//

#include "pch.h"
#include "OpenCVTest.h"
#include "CDlgUSM.h"
#include "afxdialogex.h"


// CDlgUSM 对话框

IMPLEMENT_DYNAMIC(CDlgUSM, CDialogEx)

CDlgUSM::CDlgUSM(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_USM, pParent)
	, num(50)
	, rad(1)
	, threshold(0)
{

}

CDlgUSM::~CDlgUSM()
{
}

void CDlgUSM::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, num);
	DDV_MinMaxInt(pDX, num, 1, 500);
	DDX_Control(pDX, IDC_SPIN1, m_spin1);
	DDX_Text(pDX, IDC_EDIT2, rad);
	DDV_MinMaxInt(pDX, rad, 1, 200);
	DDX_Control(pDX, IDC_SPIN2, m_spin2);
	DDX_Text(pDX, IDC_EDIT3, threshold);
	DDV_MinMaxInt(pDX, threshold, 0, 255);
	DDX_Control(pDX, IDC_SPIN3, m_spin3);
}


BEGIN_MESSAGE_MAP(CDlgUSM, CDialogEx)
END_MESSAGE_MAP()


// CDlgUSM 消息处理程序


BOOL CDlgUSM::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_spin1.SetRange(1, 500);
	m_spin1.SetBuddy(this->GetDlgItem(IDC_EDIT1));
	m_spin2.SetRange(1, 200);
	m_spin2.SetBuddy(this->GetDlgItem(IDC_EDIT2));
	m_spin3.SetRange(0, 255);
	m_spin3.SetBuddy(this->GetDlgItem(IDC_EDIT3));


	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


// 执行USM锐化操作
Mat CDlgUSM::SharpenUSM(Mat src, double amt, int rad, int thr)
{
	// 先对原图像进行高斯滤波。
	Mat blrd;
	int isz = 2 * rad + 1; // 计算高斯滤波核边长
	GaussianBlur(src, blrd, cv::Size(isz, isz), 0, 0);

	// 高斯滤波是低通滤波器，原图减低通可得高通部分，并根据阈值进行过滤
	Mat lowContrastMask = abs(src - blrd) < thr;
	Mat dst = src * (1 + amt) + blrd * (-amt);      //original + (original - blurred) * amount
	src.copyTo(dst, lowContrastMask);                  //将imageSrc中lowContrastMask对应的非0部分复制到dst中

	return dst;
}

// 单击确定时进行合法性检查并执行锐化
void CDlgUSM::OnOK()
{
	UpdateData(TRUE);
	if (num > 500 || num < 1 || rad>200 || rad < 1 || threshold>255 || threshold < 0)
	{
		return;
	}
	// 执行USM锐化
	CMainFrame* pMain = (CMainFrame*)AfxGetApp()->m_pMainWnd;
	CChildFrame* pChild = (CChildFrame*)pMain->GetActiveFrame();
	COpenCVTestView* pView = (COpenCVTestView*)pChild->GetActiveView();
	if (pView == nullptr)
	{
		MessageBox(_T("发生意料之外的错误。pView为 nullptr。"));
		return;
	}
	COpenCVTestDoc* pDoc = (COpenCVTestDoc*)pView->GetDocument();

	pDoc->timg = SharpenUSM(pDoc->timg, num * 0.01, rad, threshold);

	resize(pDoc->timg, pDoc->simg, cv::Size(0, 0), pDoc->GetScale() * 0.01, pDoc->GetScale() * 0.01);
	pView->Invalidate();
	CDialogEx::OnOK();
}


// 采用默认设置对图像快速进行锐化
Mat CDlgUSM::StdSharp(Mat src)
{
	// 构造八连通3*3滤波算子
	Mat kernel(3, 3, CV_32F);
	kernel.at<float>(1, 1) = 9.0;
	kernel.at<float>(0, 0) = -1.0;
	kernel.at<float>(0, 1) = -1.0;
	kernel.at<float>(0, 2) = -1.0;
	kernel.at<float>(1, 0) = -1.0;
	kernel.at<float>(1, 2) = -1.0;
	kernel.at<float>(2, 0) = -1.0;
	kernel.at<float>(2, 1) = -1.0;
	kernel.at<float>(2, 2) = -1.0;

	Mat dst;
	filter2D(src, dst, src.depth(), kernel);
	return dst;
}
