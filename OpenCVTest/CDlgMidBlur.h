#pragma once
#include "MainFrm.h"

// CDlgMidBlur 对话框

class CDlgMidBlur : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgMidBlur)

public:
	CDlgMidBlur(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CDlgMidBlur();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MID_BLR };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	// 滤波核宽度
	int kw;
	CSpinButtonCtrl m_spin1;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
};
