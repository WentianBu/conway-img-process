#pragma once

#include "MainFrm.h"
// CDlgBlur 对话框

class CDlgBlur : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgBlur)

public:
	CDlgBlur(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CDlgBlur();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_BLUR };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	// 滤波核宽度
	int kw;
	// 滤波核高度
	int kh;
	CSpinButtonCtrl m_spin1;
	CSpinButtonCtrl m_spin2;
	virtual BOOL OnInitDialog();
	virtual void OnOK();
};
