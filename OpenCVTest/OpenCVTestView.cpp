// 这段 MFC 示例源代码演示如何使用 MFC Microsoft Office Fluent 用户界面
// (“Fluent UI”)。该示例仅供参考，
// 用以补充《Microsoft 基础类参考》和
// MFC C++ 库软件随附的相关电子文档。
// 复制、使用或分发 Fluent UI 的许可条款是单独提供的。
// 若要了解有关 Fluent UI 许可计划的详细信息，请访问
// https://go.microsoft.com/fwlink/?LinkId=238214.
//
// 版权所有(C) Microsoft Corporation
// 保留所有权利。

// OpenCVTestView.cpp: COpenCVTestView 类的实现
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "OpenCVTest.h"
#endif

#include "OpenCVTestDoc.h"
#include "OpenCVTestView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// COpenCVTestView

IMPLEMENT_DYNCREATE(COpenCVTestView, CScrollView)

BEGIN_MESSAGE_MAP(COpenCVTestView, CScrollView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &COpenCVTestView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_ERASEBKGND()
//	ON_COMMAND(ID_BUTTON5, &COpenCVTestView::ShowAt100Percnt)
END_MESSAGE_MAP()

// COpenCVTestView 构造/析构

COpenCVTestView::COpenCVTestView() noexcept
{
	// TODO: 在此处添加构造代码
	
}

COpenCVTestView::~COpenCVTestView()
{
}

BOOL COpenCVTestView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式
	
	return CScrollView::PreCreateWindow(cs);
}

// COpenCVTestView 绘图

void COpenCVTestView::OnDraw(CDC* pDC)
{
	COpenCVTestDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	// 双缓冲技术避免闪烁
	/*
	CDC memDC;
	CBitmap memBmp;
	memDC.CreateCompatibleDC(pDC);//创建兼容DC
	CRect rc;
	pDC->GetClipBox(&rc);//获得刷新区域
	// 创建兼容位图
	memBmp.CreateCompatibleBitmap(pDC, rc.Width(), rc.Height());
	memDC.SelectObject(&memBmp);//导入位图 
	//红色背景 
	memDC.FillSolidRect(0, 0, rc.Width(), rc.Height(), RGB(255, 0, 0));
	memDC.SetWindowOrg(rc.left, rc.top);
	*/
	Bitmap bitmap(pDoc->simg.cols, pDoc->simg.rows, pDoc->simg.step1(), PixelFormat32bppARGB, pDoc->simg.data);
	Graphics graphics(pDC->GetSafeHdc());
	graphics.DrawImage(&bitmap, 0, 0);
	/*
	//导入
	pDC->BitBlt(rc.left, rc.top, rc.Width(), rc.Height(), &memDC, rc.left, rc.top, SRCCOPY);
	memBmp.DeleteObject();//销毁位图
	memDC.DeleteDC();//销毁DC 
	*/
	
}


// COpenCVTestView 打印


void COpenCVTestView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL COpenCVTestView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void COpenCVTestView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加额外的打印前进行的初始化过程
}

void COpenCVTestView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加打印后进行的清理过程
}

void COpenCVTestView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void COpenCVTestView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// COpenCVTestView 诊断

#ifdef _DEBUG
void COpenCVTestView::AssertValid() const
{
	CScrollView::AssertValid();
}

void COpenCVTestView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

COpenCVTestDoc* COpenCVTestView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(COpenCVTestDoc)));
	return (COpenCVTestDoc*)m_pDocument;
}
#endif //_DEBUG


// COpenCVTestView 消息处理程序


BOOL COpenCVTestView::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	
	// 修改文档默认背景色
	CBrush backBrush(RGB(128,128,128));
	CBrush* pOldBrush = pDC->SelectObject(&backBrush);
	CRect rect;
	pDC->GetClipBox(&rect);
	pDC->PatBlt(rect.left, rect.top, rect.Width(), rect.Height(), PATCOPY);
	pDC->SelectObject(pOldBrush);
	
	//return CScrollView::OnEraseBkgnd(pDC);
	return FALSE;
}


void COpenCVTestView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();
	SetScrollSizes(MM_TEXT, GetDocument()->GetDocSize());
	// TODO: 在此添加专用代码和/或调用基类
}


void COpenCVTestView::OnUpdate(CView* /*pSender*/, LPARAM /*lHint*/, CObject* /*pHint*/)
{
	// TODO: 在此添加专用代码和/或调用基类
	SetScrollSizes(MM_TEXT, GetDocument()->GetDocSize());
	//ResizeParentToFit();
}


void COpenCVTestView::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView)
{
	// TODO: 在此添加专用代码和/或调用基类

	CScrollView::OnActivateView(bActivate, pActivateView, pDeactiveView);
}
