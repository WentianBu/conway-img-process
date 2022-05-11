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

// MainFrm.cpp: CMainFrame 类的实现
//

#include "pch.h"
#include "framework.h"
#include "OpenCVTest.h"

#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CMDIFrameWndEx)

BEGIN_MESSAGE_MAP(CMainFrame, CMDIFrameWndEx)
	ON_WM_CREATE()
	ON_COMMAND(ID_WINDOW_MANAGER, &CMainFrame::OnWindowManager)
	ON_COMMAND_RANGE(ID_VIEW_APPLOOK_WIN_2000, ID_VIEW_APPLOOK_WINDOWS_7, &CMainFrame::OnApplicationLook)
	ON_UPDATE_COMMAND_UI_RANGE(ID_VIEW_APPLOOK_WIN_2000, ID_VIEW_APPLOOK_WINDOWS_7, &CMainFrame::OnUpdateApplicationLook)
	ON_COMMAND(ID_VIEW_CAPTION_BAR, &CMainFrame::OnViewCaptionBar)
	ON_UPDATE_COMMAND_UI(ID_VIEW_CAPTION_BAR, &CMainFrame::OnUpdateViewCaptionBar)
	ON_COMMAND(ID_TOOLS_OPTIONS, &CMainFrame::OnOptions)
	ON_COMMAND(ID_BUTTON5, &CMainFrame::ShowAt100Pcnt)
	ON_COMMAND(ID_OLE_INSERT_NEW, &CMainFrame::OnChangeSize)
	ON_COMMAND(ID_BUTTON2, &CMainFrame::ShowAt50Pcnt)
	ON_UPDATE_COMMAND_UI(ID_BUTTON5, &CMainFrame::OnUpdateButton5)
	ON_UPDATE_COMMAND_UI(ID_BUTTON2, &CMainFrame::OnUpdateButton2)
	ON_COMMAND(ID_BUTTON3, &CMainFrame::ShowAt200Pcnt)
	ON_UPDATE_COMMAND_UI(ID_BUTTON3, &CMainFrame::OnUpdateButton3)
	ON_COMMAND(ID_SLIDER1, &CMainFrame::OnSlider1)
	ON_UPDATE_COMMAND_UI(ID_SLIDER1, &CMainFrame::OnUpdateSlider1)
	//ON_REGISTERED_MESSAGE(WM_UPDATE_SCALEBAR, OnUpdateScaleBar)
	ON_WM_CHILDACTIVATE()
	ON_WM_MDIACTIVATE()
	ON_COMMAND(ID_SPIN1, &CMainFrame::OnSpin1)
	ON_UPDATE_COMMAND_UI(ID_SPIN1, &CMainFrame::OnUpdateSpin1)
	ON_UPDATE_COMMAND_UI(ID_OLE_INSERT_NEW, &CMainFrame::OnUpdateAdjustSize)
	ON_COMMAND(ID_GAUSS, &CMainFrame::OnGauss)
	ON_UPDATE_COMMAND_UI(ID_GAUSS, &CMainFrame::OnUpdateGauss)
	ON_COMMAND(ID_BLUR, &CMainFrame::OnBlur)
	ON_UPDATE_COMMAND_UI(ID_BLUR, &CMainFrame::OnUpdateBlur)
	ON_COMMAND(ID_BUTTON12, &CMainFrame::OnMidBlur)
	ON_UPDATE_COMMAND_UI(ID_BUTTON12, &CMainFrame::OnUpdateButton12)
	ON_COMMAND(ID_SHARPEN, &CMainFrame::OnSharpen)
	ON_UPDATE_COMMAND_UI(ID_SHARPEN, &CMainFrame::OnUpdateSharpen)
	ON_COMMAND(ID_STD_SHARPEN, &CMainFrame::OnStdSharpen)
	ON_UPDATE_COMMAND_UI(ID_STD_SHARPEN, &CMainFrame::OnUpdateStdSharpen)
	ON_COMMAND(ID_BR, &CMainFrame::OnBr)
	ON_UPDATE_COMMAND_UI(ID_BR, &CMainFrame::OnUpdateBr)
END_MESSAGE_MAP()

// CMainFrame 构造/析构

CMainFrame::CMainFrame() noexcept
{
	// TODO: 在此添加成员初始化代码
	theApp.m_nAppLook = theApp.GetInt(_T("ApplicationLook"), ID_VIEW_APPLOOK_OFF_2007_BLUE);
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CMDIFrameWndEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	BOOL bNameValid;

	CMDITabInfo mdiTabParams;
	mdiTabParams.m_style = CMFCTabCtrl::STYLE_3D_ONENOTE; // 其他可用样式...
	mdiTabParams.m_bActiveTabCloseButton = TRUE;      // 设置为 FALSE 会将关闭按钮放置在选项卡区域的右侧
	mdiTabParams.m_bTabIcons = FALSE;    // 设置为 TRUE 将在 MDI 选项卡上启用文档图标
	mdiTabParams.m_bAutoColor = TRUE;    // 设置为 FALSE 将禁用 MDI 选项卡的自动着色
	mdiTabParams.m_bDocumentMenu = TRUE; // 在选项卡区域的右边缘启用文档菜单
	EnableMDITabbedGroups(TRUE, mdiTabParams);

	m_wndRibbonBar.Create(this);
	m_wndRibbonBar.LoadFromResource(IDR_RIBBON);

	if (!m_wndStatusBar.Create(this))
	{
		TRACE0("未能创建状态栏\n");
		return -1;      // 未能创建
	}

	CString strTitlePane1;
	CString strTitlePane2;
	bNameValid = strTitlePane1.LoadString(IDS_STATUS_PANE1);
	ASSERT(bNameValid);
	bNameValid = strTitlePane2.LoadString(IDS_STATUS_PANE2);
	ASSERT(bNameValid);
	m_wndStatusBar.AddElement(new CMFCRibbonStatusBarPane(ID_STATUSBAR_PANE1, strTitlePane1, TRUE), strTitlePane1);
	m_wndStatusBar.AddExtendedElement(new CMFCRibbonStatusBarPane(ID_STATUSBAR_PANE2, strTitlePane2, TRUE), strTitlePane2);

	// 启用 Visual Studio 2005 样式停靠窗口行为
	CDockingManager::SetDockingMode(DT_SMART);
	// 启用 Visual Studio 2005 样式停靠窗口自动隐藏行为
	EnableAutoHidePanes(CBRS_ALIGN_ANY);
	
	// 导航窗格将创建在左侧，因此将暂时禁用左侧的停靠: 
	EnableDocking(CBRS_ALIGN_TOP | CBRS_ALIGN_BOTTOM | CBRS_ALIGN_RIGHT | CBRS_ALIGN_LEFT);
	/*
	// 创建并设置“Outlook”导航栏: 
	if (!CreateOutlookBar(m_wndNavigationBar, ID_VIEW_NAVIGATION, m_wndTree, 250))
	{
		TRACE0("未能创建导航窗格\n");
		return -1;      // 未能创建
	}
	*/
	// 创建标题栏: 
	if (!CreateCaptionBar())
	{
		TRACE0("未能创建标题栏\n");
		return -1;      // 未能创建
	}
	/*
	// 已创建 Outlook 栏，应允许在左侧停靠。
	EnableDocking(CBRS_ALIGN_LEFT);
	EnableAutoHidePanes(CBRS_ALIGN_RIGHT);
	*/
	// 基于持久值设置视觉管理器和样式
	OnApplicationLook(theApp.m_nAppLook);

	// 启用增强的窗口管理对话框
	EnableWindowsDialog(ID_WINDOW_MANAGER, ID_WINDOW_MANAGER, TRUE);

	// 将文档名和应用程序名称在窗口标题栏上的顺序进行交换。这
	// 将改进任务栏的可用性，因为显示的文档名带有缩略图。
	ModifyStyle(0, FWS_PREFIXTITLE);

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CMDIFrameWndEx::PreCreateWindow(cs) )
		return FALSE;
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return TRUE;
}

BOOL CMainFrame::CreateOutlookBar(CMFCOutlookBar& bar, UINT uiID, CMFCShellTreeCtrl& tree, int nInitialWidth)
{
	bar.SetMode2003();

	BOOL bNameValid;
	CString strTemp;
	bNameValid = strTemp.LoadString(IDS_SHORTCUTS);
	ASSERT(bNameValid);
	if (!bar.Create(strTemp, this, CRect(0, 0, nInitialWidth, 32000), uiID, WS_CHILD | WS_VISIBLE | CBRS_LEFT))
	{
		return FALSE; // 未能创建
	}
	
	CMFCOutlookBarTabCtrl* pOutlookBar = (CMFCOutlookBarTabCtrl*)bar.GetUnderlyingWindow();

	if (pOutlookBar == nullptr)
	{
		ASSERT(FALSE);
		return FALSE;
	}

	pOutlookBar->EnableInPlaceEdit(TRUE);

	static UINT uiPageID = 1;

	// 可浮动，可自动隐藏，可调整大小，但不能关闭
	DWORD dwStyle = AFX_CBRS_FLOAT | AFX_CBRS_AUTOHIDE | AFX_CBRS_RESIZE|AFX_CBRS_CLOSE;

	CRect rectDummy(0, 0, 0, 0);
	const DWORD dwTreeStyle = WS_CHILD | WS_VISIBLE | TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS;

	tree.Create(dwTreeStyle, rectDummy, &bar, 1200);
	bNameValid = strTemp.LoadString(IDS_FOLDERS);
	ASSERT(bNameValid);
	pOutlookBar->AddControl(&tree, strTemp, 2, TRUE, dwStyle);
	
	/*
	calendar.Create(rectDummy, &bar, 1201);
	bNameValid = strTemp.LoadString(IDS_CALENDAR);
	ASSERT(bNameValid);
	pOutlookBar->AddControl(&calendar, strTemp, 3, TRUE, dwStyle);
	*/
	bar.SetPaneStyle(bar.GetPaneStyle() | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC);

	pOutlookBar->SetImageList(theApp.m_bHiColorIcons ? IDB_PAGES_HC : IDB_PAGES, 24);
	pOutlookBar->SetToolbarImageList(theApp.m_bHiColorIcons ? IDB_PAGES_SMALL_HC : IDB_PAGES_SMALL, 16);
	pOutlookBar->RecalcLayout();

	BOOL bAnimation = theApp.GetInt(_T("OutlookAnimation"), TRUE);
	CMFCOutlookBarTabCtrl::EnableAnimation(bAnimation);

	bar.SetButtonsFont(&afxGlobalData.fontBold);

	return TRUE;
}

BOOL CMainFrame::CreateCaptionBar()
{
	if (!m_wndCaptionBar.Create(WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS, this, ID_VIEW_CAPTION_BAR, -1, TRUE))
	{
		TRACE0("未能创建标题栏\n");
		return FALSE;
	}

	BOOL bNameValid;

	CString strTemp, strTemp2;
	bNameValid = strTemp.LoadString(IDS_CAPTION_BUTTON);
	ASSERT(bNameValid);
	m_wndCaptionBar.SetButton(strTemp, ID_TOOLS_OPTIONS, CMFCCaptionBar::ALIGN_LEFT, FALSE);
	bNameValid = strTemp.LoadString(IDS_CAPTION_BUTTON_TIP);
	ASSERT(bNameValid);
	m_wndCaptionBar.SetButtonToolTip(strTemp);

	bNameValid = strTemp.LoadString(IDS_CAPTION_TEXT);
	ASSERT(bNameValid);
	m_wndCaptionBar.SetText(strTemp, CMFCCaptionBar::ALIGN_LEFT);

	m_wndCaptionBar.SetBitmap(IDB_INFO, RGB(255, 255, 255), FALSE, CMFCCaptionBar::ALIGN_LEFT);
	bNameValid = strTemp.LoadString(IDS_CAPTION_IMAGE_TIP);
	ASSERT(bNameValid);
	bNameValid = strTemp2.LoadString(IDS_CAPTION_IMAGE_TEXT);
	ASSERT(bNameValid);
	m_wndCaptionBar.SetImageToolTip(strTemp, strTemp2);

	return TRUE;
}

// CMainFrame 诊断

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CMDIFrameWndEx::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CMDIFrameWndEx::Dump(dc);
}
#endif //_DEBUG


// CMainFrame 消息处理程序

void CMainFrame::OnWindowManager()
{
	ShowWindowsDialog();
}

void CMainFrame::OnApplicationLook(UINT id)
{
	CWaitCursor wait;

	theApp.m_nAppLook = id;

	switch (theApp.m_nAppLook)
	{
	case ID_VIEW_APPLOOK_WIN_2000:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManager));
		m_wndRibbonBar.SetWindows7Look(FALSE);
		break;

	case ID_VIEW_APPLOOK_OFF_XP:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOfficeXP));
		m_wndRibbonBar.SetWindows7Look(FALSE);
		break;

	case ID_VIEW_APPLOOK_WIN_XP:
		CMFCVisualManagerWindows::m_b3DTabsXPTheme = TRUE;
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));
		m_wndRibbonBar.SetWindows7Look(FALSE);
		break;

	case ID_VIEW_APPLOOK_OFF_2003:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOffice2003));
		CDockingManager::SetDockingMode(DT_SMART);
		m_wndRibbonBar.SetWindows7Look(FALSE);
		break;

	case ID_VIEW_APPLOOK_VS_2005:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerVS2005));
		CDockingManager::SetDockingMode(DT_SMART);
		m_wndRibbonBar.SetWindows7Look(FALSE);
		break;

	case ID_VIEW_APPLOOK_VS_2008:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerVS2008));
		CDockingManager::SetDockingMode(DT_SMART);
		m_wndRibbonBar.SetWindows7Look(FALSE);
		break;

	case ID_VIEW_APPLOOK_WINDOWS_7:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows7));
		CDockingManager::SetDockingMode(DT_SMART);
		m_wndRibbonBar.SetWindows7Look(TRUE);
		break;

	default:
		switch (theApp.m_nAppLook)
		{
		case ID_VIEW_APPLOOK_OFF_2007_BLUE:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_LunaBlue);
			break;

		case ID_VIEW_APPLOOK_OFF_2007_BLACK:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_ObsidianBlack);
			break;

		case ID_VIEW_APPLOOK_OFF_2007_SILVER:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_Silver);
			break;

		case ID_VIEW_APPLOOK_OFF_2007_AQUA:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_Aqua);
			break;
		}

		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOffice2007));
		CDockingManager::SetDockingMode(DT_SMART);
		m_wndRibbonBar.SetWindows7Look(FALSE);
	}

	RedrawWindow(nullptr, nullptr, RDW_ALLCHILDREN | RDW_INVALIDATE | RDW_UPDATENOW | RDW_FRAME | RDW_ERASE);

	theApp.WriteInt(_T("ApplicationLook"), theApp.m_nAppLook);
}

void CMainFrame::OnUpdateApplicationLook(CCmdUI* pCmdUI)
{
	pCmdUI->SetRadio(theApp.m_nAppLook == pCmdUI->m_nID);
}

void CMainFrame::OnViewCaptionBar()
{
	m_wndCaptionBar.ShowWindow(m_wndCaptionBar.IsVisible() ? SW_HIDE : SW_SHOW);
	RecalcLayout(FALSE);
}

void CMainFrame::OnUpdateViewCaptionBar(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_wndCaptionBar.IsVisible());
}

void CMainFrame::OnOptions()
{
	CMFCRibbonCustomizeDialog *pOptionsDlg = new CMFCRibbonCustomizeDialog(this, &m_wndRibbonBar);
	ASSERT(pOptionsDlg != nullptr);

	pOptionsDlg->DoModal();
	delete pOptionsDlg;
}




void CMainFrame::ShowAt100Pcnt()
{
	// 获取当前活动文档和视图指针
	CMainFrame* pMain = (CMainFrame*)AfxGetApp()->m_pMainWnd;
	CChildFrame* pChild = (CChildFrame*)pMain->GetActiveFrame();
	CMFCRibbonSlider* pSlider = DYNAMIC_DOWNCAST(CMFCRibbonSlider, m_wndRibbonBar.FindByID(ID_SLIDER1));
	CMFCRibbonEdit* pSpin = DYNAMIC_DOWNCAST(CMFCRibbonEdit, m_wndRibbonBar.FindByID(ID_SPIN1));
	COpenCVTestView* pView = (COpenCVTestView*)pChild->GetActiveView();
	if (pView != nullptr)
	{
		COpenCVTestDoc* pDoc = (COpenCVTestDoc*)pView->GetDocument();	
		// 从timg克隆至simg以恢复100%比例
		pDoc->simg = pDoc->timg.clone();
		pDoc->SetScale(100);
		pSlider->SetPos(100);
		pSpin->SetEditText(_T("100"));
		// UpdateAllViews用于强制更新视图，以根据视图状态重绘滚动条
		pDoc->UpdateAllViews(NULL);
		pView->Invalidate();
		//UpdateWindow();
	}
	
}


void CMainFrame::ShowAt50Pcnt()
{
	// 获取当前活动文档和视图指针
	CMainFrame* pMain = (CMainFrame*)AfxGetApp()->m_pMainWnd;
	CChildFrame* pChild = (CChildFrame*)pMain->GetActiveFrame();
	CMFCRibbonSlider* pSlider = DYNAMIC_DOWNCAST(CMFCRibbonSlider, m_wndRibbonBar.FindByID(ID_SLIDER1));
	CMFCRibbonEdit* pSpin = DYNAMIC_DOWNCAST(CMFCRibbonEdit, m_wndRibbonBar.FindByID(ID_SPIN1));
	COpenCVTestView* pView = (COpenCVTestView*)pChild->GetActiveView();
	if (pView != nullptr)
	{
		COpenCVTestDoc* pDoc = (COpenCVTestDoc*)pView->GetDocument();
		// 从timg克隆至simg以恢复100%比例，然后缩小至50%
		pDoc->simg = pDoc->timg.clone();
		resize(pDoc->simg, pDoc->simg, cv::Size(0, 0), 0.5, 0.5);
		pDoc->SetScale(50);
		pSlider->SetPos(50);
		pSpin->SetEditText(_T("50"));
		pDoc->UpdateAllViews(NULL);
		pView->Invalidate();
		//UpdateWindow();
	}
}


void CMainFrame::OnUpdateButton5(CCmdUI* pCmdUI)
{
	// 用于在没有打开任何文档时禁用按钮100%
	CMainFrame* pMain = (CMainFrame*)AfxGetApp()->m_pMainWnd;
	CChildFrame* pChild = (CChildFrame*)pMain->GetActiveFrame();
	COpenCVTestView* pView = (COpenCVTestView*)pChild->GetActiveView();
	if (pView==nullptr)
		pCmdUI->Enable(FALSE);
}


void CMainFrame::OnUpdateButton2(CCmdUI* pCmdUI)
{
	// 用于在没有打开任何文档时禁用按钮50%
	CMainFrame* pMain = (CMainFrame*)AfxGetApp()->m_pMainWnd;
	CChildFrame* pChild = (CChildFrame*)pMain->GetActiveFrame();
	COpenCVTestView* pView = (COpenCVTestView*)pChild->GetActiveView();
	if (pView == nullptr)
		pCmdUI->Enable(FALSE);
}


void CMainFrame::ShowAt200Pcnt()
{
	// 获取当前活动文档和视图指针
	CMainFrame* pMain = (CMainFrame*)AfxGetApp()->m_pMainWnd;
	CChildFrame* pChild = (CChildFrame*)pMain->GetActiveFrame();
	CMFCRibbonSlider* pSlider = DYNAMIC_DOWNCAST(CMFCRibbonSlider, m_wndRibbonBar.FindByID(ID_SLIDER1));
	CMFCRibbonEdit* pSpin = DYNAMIC_DOWNCAST(CMFCRibbonEdit, m_wndRibbonBar.FindByID(ID_SPIN1));
	COpenCVTestView* pView = (COpenCVTestView*)pChild->GetActiveView();
	if (pView != nullptr)
	{
		COpenCVTestDoc* pDoc = (COpenCVTestDoc*)pView->GetDocument();
		// 从timg克隆至simg以恢复100%比例，然后放大至200%
		pDoc->simg = pDoc->timg.clone();
		resize(pDoc->simg, pDoc->simg, cv::Size(0, 0), 2, 2);
		pDoc->SetScale(200);
		pSlider->SetPos(200);
		pSpin->SetEditText(_T("200"));
		pDoc->UpdateAllViews(NULL);
		pView->Invalidate();
		
	}
}

void CMainFrame::OnUpdateButton3(CCmdUI* pCmdUI)
{
	// 用于在没有打开任何文档时禁用按钮200%
	CMainFrame* pMain = (CMainFrame*)AfxGetApp()->m_pMainWnd;
	CChildFrame* pChild = (CChildFrame*)pMain->GetActiveFrame();
	COpenCVTestView* pView = (COpenCVTestView*)pChild->GetActiveView();
	if (pView == nullptr)
		pCmdUI->Enable(FALSE);
}


void CMainFrame::OnSlider1()
{
	CMainFrame* pMain = (CMainFrame*)AfxGetApp()->m_pMainWnd;
	CMFCRibbonEdit* pSpin = DYNAMIC_DOWNCAST(CMFCRibbonEdit, m_wndRibbonBar.FindByID(ID_SPIN1));
	CMFCRibbonSlider* pSlider = DYNAMIC_DOWNCAST(CMFCRibbonSlider, m_wndRibbonBar.FindByID(ID_SLIDER1));
	CChildFrame* pChild = (CChildFrame*)pMain->GetActiveFrame();
	COpenCVTestView* pView = (COpenCVTestView*)pChild->GetActiveView();
	if (pView != nullptr)
	{
		int slider = pSlider->GetPos();
		COpenCVTestDoc* pDoc = (COpenCVTestDoc*)pView->GetDocument();
		pDoc->SetScale(slider);
		CString s;
		s.Format(_T("%d"), slider);
		pSpin->SetEditText(s);
		pDoc->simg = pDoc->timg.clone();
		resize(pDoc->simg, pDoc->simg, cv::Size(0, 0), slider*0.01, slider*0.01);
		pDoc->UpdateAllViews(NULL);
		pView->Invalidate();
	}
}


void CMainFrame::OnUpdateSlider1(CCmdUI* pCmdUI)
{
	// 用于在没有打开任何文档时禁用滑动条
	CMainFrame* pMain = (CMainFrame*)AfxGetApp()->m_pMainWnd;
	CChildFrame* pChild = (CChildFrame*)pMain->GetActiveFrame();
	CMFCRibbonSlider* pSlider = DYNAMIC_DOWNCAST(CMFCRibbonSlider, m_wndRibbonBar.FindByID(ID_SLIDER1));
	COpenCVTestView* pView = (COpenCVTestView*)pChild->GetActiveView();
	if (pView == nullptr)
		pCmdUI->Enable(FALSE);
}


void CMainFrame::OnChildActivate()
{
	//CMDIFrameWndEx::OnChildActivate();
	
	// TODO: 在此处添加消息处理程序代码
}


void CMainFrame::OnMDIActivate(BOOL bActivate, CWnd* pActivateWnd, CWnd* pDeactivateWnd)
{
	//CMDIFrameWndEx::OnMDIActivate(bActivate, pActivateWnd, pDeactivateWnd);
	CMainFrame* pMain = (CMainFrame*)AfxGetApp()->m_pMainWnd;
	CChildFrame* pChild = (CChildFrame*)pMain->GetActiveFrame();
	COpenCVTestView* pView = (COpenCVTestView*)pChild->GetActiveView();
	COpenCVTestDoc* pDoc = (COpenCVTestDoc*)pView->GetDocument();
	CMFCRibbonSlider* pSlider = DYNAMIC_DOWNCAST(CMFCRibbonSlider, m_wndRibbonBar.FindByID(ID_SLIDER1));
	CMFCRibbonEdit* pSpin = DYNAMIC_DOWNCAST(CMFCRibbonEdit, m_wndRibbonBar.FindByID(ID_SPIN1));
	pSlider->SetPos(pDoc->GetScale());
	CString s;
	s.Format(_T("%d"), pDoc->GetScale());
	pSpin->SetEditText(s);
	// TODO: 在此处添加消息处理程序代码
}


void CMainFrame::OnSpin1()
{
	// 显示比例调节框的处理程序
	CMainFrame* pMain = (CMainFrame*)AfxGetApp()->m_pMainWnd;
	CMFCRibbonSlider* pSlider = DYNAMIC_DOWNCAST(CMFCRibbonSlider, m_wndRibbonBar.FindByID(ID_SLIDER1));
	CMFCRibbonEdit* pSpin = DYNAMIC_DOWNCAST(CMFCRibbonEdit, m_wndRibbonBar.FindByID(ID_SPIN1));
	CChildFrame* pChild = (CChildFrame*)pMain->GetActiveFrame();
	COpenCVTestView* pView = (COpenCVTestView*)pChild->GetActiveView();
	if (pView != nullptr)
	{
		COpenCVTestDoc* pDoc = (COpenCVTestDoc*)pView->GetDocument();
		int spin = _ttoi(pSpin->GetEditText());
		if (spin>400||spin<10)
		{
			MessageBox(_T("数据必须在10-400之间！"));
			CString s;
			s.Format(_T("%d"), pDoc->GetScale());
			pSpin->SetEditText(s);
			return;
		}
		
		pDoc->SetScale(spin);
		pDoc->simg = pDoc->timg.clone();
		pSlider->SetPos(spin);
		resize(pDoc->simg, pDoc->simg, cv::Size(0, 0), spin * 0.01, spin * 0.01);
		pDoc->UpdateAllViews(NULL);
		pView->Invalidate();
		
	}
}


void CMainFrame::OnUpdateSpin1(CCmdUI* pCmdUI)
{
	CMainFrame* pMain = (CMainFrame*)AfxGetApp()->m_pMainWnd;
	CChildFrame* pChild = (CChildFrame*)pMain->GetActiveFrame();
	COpenCVTestView* pView = (COpenCVTestView*)pChild->GetActiveView();
	if (pView == nullptr)
		pCmdUI->Enable(FALSE);
}


void CMainFrame::OnChangeSize()
{
	// 更改形状按钮的事件处理程序
	CAdjustShape adjustShapeDialog;
	adjustShapeDialog.DoModal();
}


void CMainFrame::OnUpdateAdjustSize(CCmdUI* pCmdUI)
{
	// 当没有打开文档时禁用调整大小按钮
	CMainFrame* pMain = (CMainFrame*)AfxGetApp()->m_pMainWnd;
	CChildFrame* pChild = (CChildFrame*)pMain->GetActiveFrame();
	COpenCVTestView* pView = (COpenCVTestView*)pChild->GetActiveView();
	if (pView == nullptr)
		pCmdUI->Enable(FALSE);
}


void CMainFrame::OnGauss()
{
	// 创建高斯模糊的模态对话框
	CDlgGauss dialogGauss;
	dialogGauss.DoModal();
}


void CMainFrame::OnUpdateGauss(CCmdUI* pCmdUI)
{
	// 没有打开文档时禁用高斯模糊按钮
	CMainFrame* pMain = (CMainFrame*)AfxGetApp()->m_pMainWnd;
	CChildFrame* pChild = (CChildFrame*)pMain->GetActiveFrame();
	COpenCVTestView* pView = (COpenCVTestView*)pChild->GetActiveView();
	if (pView == nullptr)
		pCmdUI->Enable(FALSE);
}


void CMainFrame::OnBlur()
{
	CDlgBlur dialogBlur;
	dialogBlur.DoModal();
}


void CMainFrame::OnUpdateBlur(CCmdUI* pCmdUI)
{
	// 没有打开文档时禁用均值模糊按钮
	CMainFrame* pMain = (CMainFrame*)AfxGetApp()->m_pMainWnd;
	CChildFrame* pChild = (CChildFrame*)pMain->GetActiveFrame();
	COpenCVTestView* pView = (COpenCVTestView*)pChild->GetActiveView();
	if (pView == nullptr)
		pCmdUI->Enable(FALSE);
}


void CMainFrame::OnMidBlur()
{
	CDlgMidBlur dialogMidBlur;
	dialogMidBlur.DoModal();
}


void CMainFrame::OnUpdateButton12(CCmdUI* pCmdUI)
{
	// 没有打开文档时禁用均值模糊按钮
	CMainFrame* pMain = (CMainFrame*)AfxGetApp()->m_pMainWnd;
	CChildFrame* pChild = (CChildFrame*)pMain->GetActiveFrame();
	COpenCVTestView* pView = (COpenCVTestView*)pChild->GetActiveView();
	if (pView == nullptr)
		pCmdUI->Enable(FALSE);
}


void CMainFrame::OnSharpen()
{
	CDlgUSM dialogUSM;
	dialogUSM.DoModal();
}


void CMainFrame::OnUpdateSharpen(CCmdUI* pCmdUI)
{
	// 没有打开文档时禁用锐化按钮
	CMainFrame* pMain = (CMainFrame*)AfxGetApp()->m_pMainWnd;
	CChildFrame* pChild = (CChildFrame*)pMain->GetActiveFrame();
	COpenCVTestView* pView = (COpenCVTestView*)pChild->GetActiveView();
	if (pView == nullptr)
		pCmdUI->Enable(FALSE);
}


void CMainFrame::OnStdSharpen()
{
	// 调用USM类中的自定义函数进行快速锐化
	CMainFrame* pMain = (CMainFrame*)AfxGetApp()->m_pMainWnd;
	CChildFrame* pChild = (CChildFrame*)pMain->GetActiveFrame();
	COpenCVTestView* pView = (COpenCVTestView*)pChild->GetActiveView();
	if (pView == nullptr)
		return;
	COpenCVTestDoc* pDoc = (COpenCVTestDoc*)pView->GetDocument();
	pDoc->timg = CDlgUSM::StdSharp(pDoc->timg);
	resize(pDoc->timg, pDoc->simg, cv::Size(0, 0), pDoc->GetScale() * 0.01, pDoc->GetScale() * 0.01);
	pView->Invalidate();
}


void CMainFrame::OnUpdateStdSharpen(CCmdUI* pCmdUI)
{
	// 没有打开文档时禁用锐化按钮
	CMainFrame* pMain = (CMainFrame*)AfxGetApp()->m_pMainWnd;
	CChildFrame* pChild = (CChildFrame*)pMain->GetActiveFrame();
	COpenCVTestView* pView = (COpenCVTestView*)pChild->GetActiveView();
	if (pView == nullptr)
		pCmdUI->Enable(FALSE);
}


void CMainFrame::OnBr()
{
	// TODO: 在此添加命令处理程序代码
	CDlgBR dialogBRC;
	dialogBRC.DoModal();
}


void CMainFrame::OnUpdateBr(CCmdUI* pCmdUI)
{
	// 没有打开文档时禁用按钮
	CMainFrame* pMain = (CMainFrame*)AfxGetApp()->m_pMainWnd;
	CChildFrame* pChild = (CChildFrame*)pMain->GetActiveFrame();
	COpenCVTestView* pView = (COpenCVTestView*)pChild->GetActiveView();
	if (pView == nullptr)
		pCmdUI->Enable(FALSE);
}
