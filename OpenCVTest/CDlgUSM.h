#pragma once
#include "MainFrm.h"

// CDlgUSM 对话框

class CDlgUSM : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgUSM)

public:
	CDlgUSM(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CDlgUSM();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_USM };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	// 指定锐化的强度
	int num;
	CSpinButtonCtrl m_spin1;
	// 指定锐化半径，即前期执行高斯滤波的半径
	int rad;
	CSpinButtonCtrl m_spin2;
	// 指定锐化阈值，色阶差高于此值才会进行锐化
	int threshold;
	CSpinButtonCtrl m_spin3;
	virtual BOOL OnInitDialog();
	// 执行USM锐化操作
	Mat SharpenUSM(Mat src, double num, int rad, int threshold);
	virtual void OnOK();
	// 采用默认设置对图像快速进行锐化
	static Mat StdSharp(Mat src);
};
