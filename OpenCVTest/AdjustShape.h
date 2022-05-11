#pragma once
#include "MainFrm.h"

// AdjustShape 对话框

class CAdjustShape : public CDialogEx
{
	DECLARE_DYNAMIC(CAdjustShape)

public:
	CAdjustShape(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CAdjustShape();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ADJ_SHAPE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	// 目标宽度
	CString width;
	// 目标高度
	CString height;
	virtual BOOL OnInitDialog();
	// 是否保持图像宽高比
	BOOL bKeepShape;
	afx_msg void OnBnClickedKeepShape();
	// 单击确定后根据这两个值进行调整大小
	// 用于保存当前设定的宽度和高度
	int iwid; 
	int ihei;
	afx_msg void OnEnKillfocusEditWidth();
	afx_msg void OnBnClickedOk();
	afx_msg void OnEnKillfocusEditHeight();
	
};
