#pragma once

#include "MainFrm.h"
// CDlgGauss 对话框

class CDlgGauss : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgGauss)

public:
	CDlgGauss(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CDlgGauss();
// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_GAUSS };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	// 模糊半径
	int r_edit;
	CSpinButtonCtrl m_spin;
	afx_msg void OnBnClickedOk();
};
