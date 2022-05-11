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

// MainFrm.h: CMainFrame 类的接口
//

#pragma once
//#include "CalendarBar.h"
#include "Resource.h"
#include "OpenCVTestDoc.h"
#include "OpenCVTestView.h"
#include "ChildFrm.h"
#include "AdjustShape.h"
#include "CDlgGauss.h"
#include "CDlgBlur.h"
#include "CDlgMidBlur.h"
#include "CDlgUSM.h"
#include "CDlgBR.h"

#define WM_UPDATE_SCALEBAR WM_USER+1 // 表示更新显示比例控件
class COutlookBar : public CMFCOutlookBar
{
	virtual BOOL AllowShowOnPaneMenu() const { return TRUE; }
	virtual void GetPaneName(CString& strName) const { BOOL bNameValid = strName.LoadString(IDS_OUTLOOKBAR); ASSERT(bNameValid); if (!bNameValid) strName.Empty(); }
};

class CMainFrame : public CMDIFrameWndEx
{
	DECLARE_DYNAMIC(CMainFrame)
public:
	CMainFrame() noexcept;

// 特性
public:

// 操作
public:

// 重写
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// 实现
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // 控件条嵌入成员
	CMFCRibbonBar     m_wndRibbonBar;
	CMFCRibbonApplicationButton m_MainButton;
	CMFCToolBarImages m_PanelImages;
	CMFCRibbonStatusBar  m_wndStatusBar;
	COutlookBar       m_wndNavigationBar;
	CMFCShellTreeCtrl m_wndTree;
	//CCalendarBar      m_wndCalendar;
	CMFCCaptionBar    m_wndCaptionBar;

// 生成的消息映射函数
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnWindowManager();
	afx_msg void OnApplicationLook(UINT id);
	afx_msg void OnUpdateApplicationLook(CCmdUI* pCmdUI);
	afx_msg void OnViewCaptionBar();
	afx_msg void OnUpdateViewCaptionBar(CCmdUI* pCmdUI);
	afx_msg void OnOptions();
	//afx_msg LRESULT OnUpdateScaleBar(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()

	BOOL CreateOutlookBar(CMFCOutlookBar& bar, UINT uiID, CMFCShellTreeCtrl& tree, int nInitialWidth);
	BOOL CreateCaptionBar();

	int FindFocusedOutlookWnd(CMFCOutlookBarTabCtrl** ppOutlookWnd);

	CMFCOutlookBarTabCtrl* FindOutlookParent(CWnd* pWnd);
	CMFCOutlookBarTabCtrl* m_pCurrOutlookWnd;
	CMFCOutlookBarPane*    m_pCurrOutlookPage;
public:
	afx_msg void ShowAt100Pcnt();
	afx_msg void OnChangeSize();
	afx_msg void ShowAt50Pcnt();
	afx_msg void OnUpdateButton5(CCmdUI* pCmdUI);
	afx_msg void OnUpdateButton2(CCmdUI* pCmdUI);
	afx_msg void ShowAt200Pcnt();
	afx_msg void OnUpdateButton3(CCmdUI* pCmdUI);
	afx_msg void OnSlider1();
	afx_msg void OnUpdateSlider1(CCmdUI* pCmdUI);
	afx_msg void OnChildActivate();
	afx_msg void OnMDIActivate(BOOL bActivate, CWnd* pActivateWnd, CWnd* pDeactivateWnd);
	afx_msg void OnSpin1();
	afx_msg void OnUpdateSpin1(CCmdUI* pCmdUI);
	afx_msg void OnUpdateAdjustSize(CCmdUI* pCmdUI);
	afx_msg void OnGauss();
	afx_msg void OnUpdateGauss(CCmdUI* pCmdUI);
	afx_msg void OnBlur();
	afx_msg void OnUpdateBlur(CCmdUI* pCmdUI);
	afx_msg void OnMidBlur();
	afx_msg void OnUpdateButton12(CCmdUI* pCmdUI);
	afx_msg void OnSharpen();
	afx_msg void OnUpdateSharpen(CCmdUI* pCmdUI);
	afx_msg void OnStdSharpen();
	afx_msg void OnUpdateStdSharpen(CCmdUI* pCmdUI);
	afx_msg void OnBr();
	afx_msg void OnUpdateBr(CCmdUI* pCmdUI);
};


