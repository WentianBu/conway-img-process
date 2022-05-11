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

// OpenCVTestDoc.cpp: COpenCVTestDoc 类的实现
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "OpenCVTest.h"
#endif

#include "OpenCVTestDoc.h"

#include <propkey.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// COpenCVTestDoc

IMPLEMENT_DYNCREATE(COpenCVTestDoc, CDocument)

BEGIN_MESSAGE_MAP(COpenCVTestDoc, CDocument)
END_MESSAGE_MAP()


// COpenCVTestDoc 构造/析构

COpenCVTestDoc::COpenCVTestDoc() noexcept
{
	// TODO: 在此添加一次性构造代码

}

COpenCVTestDoc::~COpenCVTestDoc()
{
}

CSize COpenCVTestDoc::GetDocSize() const
{
	return m_docSize;
}

int COpenCVTestDoc::GetScale() const
{
	return scale;
}

BOOL COpenCVTestDoc::SetScale(int scl)
{
	if (scl>400||scl<10)
		return FALSE;
	scale = scl;
	m_docSize = CSize(simg.cols, simg.rows);
	return TRUE;
}

BOOL COpenCVTestDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: 在此添加重新初始化代码
	// (SDI 文档将重用该文档)
	img = Mat(1024, 768, CV_8UC3, Scalar(255, 255, 255));
	scale = 100;
	// 图像颜色转换
	if (img.channels() == 1)
		cvtColor(img, timg, COLOR_GRAY2BGRA);
	else if (img.channels() == 3)
		cvtColor(img, timg, COLOR_BGR2BGRA);
	else
		timg = img;
	simg = timg.clone();
	m_docSize = CSize(simg.cols, simg.rows);
	return TRUE;
}




// COpenCVTestDoc 序列化

void COpenCVTestDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO:  在此添加存储代码
		
	}
	else
	{
		// TODO:  在此添加加载代码
	}
}

#ifdef SHARED_HANDLERS

// 缩略图的支持
void COpenCVTestDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// 修改此代码以绘制文档数据
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// 搜索处理程序的支持
void COpenCVTestDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// 从文档数据设置搜索内容。
	// 内容部分应由“;”分隔

	// 例如:     strSearchContent = _T("point;rectangle;circle;ole object;")；
	SetSearchContent(strSearchContent);
}

void COpenCVTestDoc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = nullptr;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != nullptr)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// COpenCVTestDoc 诊断

#ifdef _DEBUG
void COpenCVTestDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void COpenCVTestDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// COpenCVTestDoc 命令


BOOL COpenCVTestDoc::OnOpenDocument(LPCTSTR lpszPathName)
{
	if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;
	std::string path = CT2A(lpszPathName);
	TRACE("File path: %s\n", path.c_str());
	img = imread(path, -1);
	TRACE("Width: %d  Height: %d\n", img.cols, img.rows);
	if (!img.data)
		MessageBox(NULL, _T("无法打开文件！"),lpszPathName,MB_OK);

	// 图像颜色转换
	if (img.channels() == 1)
		cvtColor(img, timg, COLOR_GRAY2BGRA);
	else if (img.channels() == 3)
		cvtColor(img, timg, COLOR_BGR2BGRA);
	else
		timg = img;
	simg = timg.clone(); // 采用深拷贝使得img_show能够改变缩放而不影响img
	scale = 100;
	m_docSize = CSize(simg.cols, simg.rows);
	// TODO:  在此添加您专用的创建代码

	return TRUE;
}


void COpenCVTestDoc::OnCloseDocument()
{
	// TODO: 在此添加专用代码和/或调用基类
	CDocument::OnCloseDocument();
}


BOOL COpenCVTestDoc::OnSaveDocument(LPCTSTR lpszPathName)
{
	// TODO: 在此添加专用代码和/或调用基类
	std::string path = CT2A(lpszPathName);
	if (imwrite(path, timg) == FALSE)
		return FALSE;
	return TRUE;
}


BOOL COpenCVTestDoc::SaveModified()
{
	// TODO: 在此添加专用代码和/或调用基类

	return CDocument::SaveModified();
}


// 更新文档大小m_docSize并返回该值
CSize COpenCVTestDoc::UpdateDocSize()
{
	m_docSize = CSize(simg.cols, simg.rows);
	return m_docSize;
}
