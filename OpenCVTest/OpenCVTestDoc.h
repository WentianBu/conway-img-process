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

// OpenCVTestDoc.h: COpenCVTestDoc 类的接口
//


#pragma once
using namespace cv;
using namespace Gdiplus;


class COpenCVTestDoc : public CDocument
{
protected: // 仅从序列化创建
	COpenCVTestDoc() noexcept;
	DECLARE_DYNCREATE(COpenCVTestDoc)

// 特性
public:
	Mat img; // 图像数据
	Mat timg; // 经过转换的图像数据
	Mat simg; // 要显示的图像数据（可能经过缩放）
	
// 操作
public:
	CSize GetDocSize() const;
	int GetScale() const;
	BOOL SetScale(int scl);
// 重写
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// 实现
public:
	virtual ~COpenCVTestDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	CSize m_docSize;
	int scale; // 图像当前的显示比例(%)
// 生成的消息映射函数
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// 用于为搜索处理程序设置搜索内容的 Helper 函数
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
public:
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	virtual void OnCloseDocument();
	virtual BOOL OnSaveDocument(LPCTSTR lpszPathName);
	virtual BOOL SaveModified();
	
	// 更新文档大小m_DocSize并返回该值
	CSize UpdateDocSize();
};
