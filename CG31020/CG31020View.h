
// CG31020View.h: CCG31020View 类的接口
//

#pragma once
#include "Mat.h"
#include "Mesh.h"
#include "PipelineStruct.h"
#include "Pipeline.h"

class CCG31020View : public CView
{
protected: // 仅从序列化创建
	CCG31020View() noexcept;
	DECLARE_DYNCREATE(CCG31020View)

// 特性
public:
	CCG31020Doc* GetDocument() const;

// 操作
public:

// 重写
public:
	virtual void OnDraw(CDC* pDC);  // 重写以绘制该视图
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 实现
public:
	virtual ~CCG31020View();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
	void SelectProj();
	void Render();
#endif

protected:
	PassConstants mPassConstants;
	Pipeline mPipeline;
	Box mBox;
	
	COLORREF* mPixels = nullptr;
	BITMAPINFO mDispInfo;
	int mWidth;
	int mHeight;

	enum
	{
		OBLIQUE_BIMETRIC_PROJ, PERSPECTIVE_PROJ
	};
	int mProjType = PERSPECTIVE_PROJ;
	Mat mView;
	Mat mProj;

	Mat mTranslation;
	Mat mScaling;
	Mat mRotation;
	Vec mRotationAxis = { 0.f,1.f,0.f };
	double mRotationAngle = 0.f;

	bool mLButtonDown = false;
	CPoint mRotationStart;
// 生成的消息映射函数
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnClear();
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnRotationAxis();
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnObliqueBimetric();
	afx_msg void OnPerspective();
};

#ifndef _DEBUG  // CG31020View.cpp 中的调试版本
inline CCG31020Doc* CCG31020View::GetDocument() const
   { return reinterpret_cast<CCG31020Doc*>(m_pDocument); }
#endif

