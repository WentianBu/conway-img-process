#pragma once
#include "MainFrm.h"

// CDlgBR 对话框

class CDlgBR : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgBR)

public:
	CDlgBR(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CDlgBR();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_BR };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	// 亮度调节
	int bright;
	// 对比度调节
	int contr;
	// 为了预览功能，需要保留图片最开始的状态
	Mat old;
	virtual BOOL OnInitDialog();
	CSliderCtrl m_slider1;
	CSliderCtrl m_slider2;
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnNMReleasedcaptureSlider2(NMHDR* pNMHDR, LRESULT* pResult);
//	afx_msg void OnEnChangeEdit1();
//	afx_msg void OnEnChangeEdit2();
	// 根据contr的值获取alpha并进行线性变换
	Mat ChangeBC();
	afx_msg void OnNMReleasedcaptureSlider3(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBnClickedCancel();
	afx_msg void OnEnKillfocusEdit1();
	afx_msg void OnEnKillfocusEdit2();
};
