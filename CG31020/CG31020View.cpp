
// CG31020View.cpp: CCG31020View 类的实现
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "CG31020.h"
#endif

#include "CG31020Doc.h"
#include "CG31020View.h"
#include "CRotationAxisDlg.h"
#include "Vec.h"
#include "Pipeline.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CCG31020View

IMPLEMENT_DYNCREATE(CCG31020View, CView)

BEGIN_MESSAGE_MAP(CCG31020View, CView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_SIZE()
	ON_WM_MOUSEMOVE()
	ON_WM_SIZE()
	ON_COMMAND(ID_CLEAR, &CCG31020View::OnClear)
	ON_WM_MOUSEWHEEL()
	ON_COMMAND(ID_ROTATION_AXIS, &CCG31020View::OnRotationAxis)
	ON_WM_KEYDOWN()
	ON_COMMAND(ID_OBLIQUE_BIMETRIC, &CCG31020View::OnObliqueBimetric)
	ON_COMMAND(ID_PERSPECTIVE, &CCG31020View::OnPerspective)
END_MESSAGE_MAP()

// CCG31020View 构造/析构

CCG31020View::CCG31020View() noexcept
	:mBox(2.0,2.0,2.0),mPipeline(1422,622)
{
	CImage image;
	image.Load(L"test.bmp");
	mPassConstants.textureWidth = image.GetWidth();
	mPassConstants.textureHeight = image.GetHeight();
	mPassConstants.texture = new COLORREF[mPassConstants.textureWidth * mPassConstants.textureHeight];

	for (int i = 0; i < mPassConstants.textureWidth; ++i)
	{
		for (int j = 0; j < mPassConstants.textureHeight; ++j)
		{
			mPassConstants.texture[i + j * mPassConstants.textureWidth] = image.GetPixel(i, j);
		}
	}

	Mat b = MatObliqueBimetricProj(2.0, 2.0, 0.5, 45, 0.0, 1000.0);
	Vec v = { 0,0,1,1 };
	v = VecMul(v, b);

	ZeroMemory(&mDispInfo,sizeof(BITMAPINFO));
	mDispInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	mDispInfo.bmiHeader.biBitCount = 32;
	mDispInfo.bmiHeader.biPlanes = 1;
	mDispInfo.bmiHeader.biCompression = BI_RGB;
	mDispInfo.bmiHeader.biClrUsed = 0;
}

CCG31020View::~CCG31020View()
{
	delete[] mPassConstants.texture;
}

BOOL CCG31020View::PreCreateWindow(CREATESTRUCT& cs)
{
	cs.style &= ~WS_THICKFRAME;

	return CView::PreCreateWindow(cs);
}

// CCG31020View 绘图

void CCG31020View::OnDraw(CDC* /*pDC*/)
{
	CCG31020Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	
	Render();
}


// CCG31020View 打印

BOOL CCG31020View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void CCG31020View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加额外的打印前进行的初始化过程
}

void CCG31020View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加打印后进行的清理过程
}


// CCG31020View 诊断

#ifdef _DEBUG
void CCG31020View::AssertValid() const
{
	CView::AssertValid();
}

void CCG31020View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

void CCG31020View::SelectProj()
{
	switch (mProjType)
	{
	case OBLIQUE_BIMETRIC_PROJ:
		mProj = MatObliqueBimetricProj(5.0 * mWidth / mHeight, 5.0, 0.5, 45.0, 0.0, 1000.0);
		break;

	case PERSPECTIVE_PROJ:
		mProj = MatPerspectiveProj(45.0, 1.0*mWidth/mHeight, 1.0, 1000.0);
		break;

	default:
		mProj = Mat();
		break;
	}
}

void CCG31020View::Render()
{
	CDC* pDC = GetDC();
	
	mBox.SetMeshConstants({ mScaling * mRotation * mTranslation });
	mPipeline.ClearZBuffer();
	auto& pipelineOut = mPipeline.Render(mBox.GetVertices(), mBox.GetIndices(), mPassConstants, mBox.GetMeshConstants());

	CDC memDC;
	CBitmap dispBmp;
	memDC.CreateCompatibleDC(pDC);
	dispBmp.CreateCompatibleBitmap(pDC, mWidth, mHeight);
	memDC.SelectObject(&dispBmp);
	
	for (int i = 0; i < mWidth * mHeight; ++i)
	{
		mPixels[i] = RGB(255, 255, 255);
	}
	
	for (auto& pixel : pipelineOut)
	{
		int pos = int(pixel.pos.x) + int(pixel.pos.y) * mWidth;
		mPixels[pos] = RGB(pixel.color.x, pixel.color.y, pixel.color.z);
	}

	dispBmp.SetBitmapBits(mWidth * mHeight * sizeof(COLORREF), mPixels);
	pDC->BitBlt(0, 0, mWidth, mHeight, &memDC, 0, 0, SRCCOPY);

	dispBmp.DeleteObject();
	memDC.DeleteDC();
	ReleaseDC(pDC);
}

CCG31020Doc* CCG31020View::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CCG31020Doc)));
	return (CCG31020Doc*)m_pDocument;
}
#endif //_DEBUG


// CCG31020View 消息处理程序
void CCG31020View::OnLButtonDown(UINT nFlags, CPoint point)
{
	mLButtonDown = true;
	mRotationStart = point;

	CView::OnLButtonDown(nFlags, point);
}


void CCG31020View::OnLButtonUp(UINT nFlags, CPoint point)
{
	mLButtonDown = false;

	CView::OnLButtonUp(nFlags, point);
}

void CCG31020View::OnMouseMove(UINT nFlags, CPoint point)
{
	if (mLButtonDown)
	{
		mRotationAngle = (point.x - mRotationStart.x) * 0.2;
		mRotationStart = point;

		mRotation *= MatRotation(mRotationAxis, mRotationAngle);
		Render();
	}

	CView::OnMouseMove(nFlags, point);
}


void CCG31020View::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	CRect rect;
	GetWindowRect(&rect);
	mWidth = rect.right - rect.left;
	mHeight = rect.bottom - rect.top;

	if (mPixels)
	{
		delete[] mPixels;
	}

	if (mWidth * mHeight > 0)
	{
		mPixels = new COLORREF[mWidth * mHeight];
	}

	mPassConstants.width = mWidth;
	mPassConstants.height = mHeight;
	mPipeline.Resize(mWidth, mHeight);

	mDispInfo.bmiHeader.biWidth = mWidth;
	mDispInfo.bmiHeader.biHeight = mHeight;

	mView = MatView({ 0.0,0.0,-10.0 }, { 0.0,0.0,0.0 }, { 0.0,1.0,0.0 });
	SelectProj();
	mPassConstants.viewProj = mView * mProj;
}

void CCG31020View::OnClear()
{
	mTranslation = Mat();
	mScaling = Mat();
	mRotation = Mat();
	Render();
}


BOOL CCG31020View::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	if (zDelta > 0)
	{
		mScaling *= MatScaling(1.1, 1.1, 1.1);
	}
	else
	{
		mScaling *= MatScaling(0.9, 0.9, 0.9);
	}

	Render();
	return CView::OnMouseWheel(nFlags, zDelta, pt);
}


void CCG31020View::OnRotationAxis()
{
	CRotationAxisDlg dlg;
	if (dlg.DoModal() == IDOK)
	{
		mRotationAxis = { dlg.x,dlg.y,dlg.z };
	}
}

void CCG31020View::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	switch (nChar)
	{
	case VK_LEFT:
		mTranslation *= MatTranslation(-0.1, 0.0, 0.0);
		break;

	case VK_RIGHT:
		mTranslation *= MatTranslation(0.1, 0.0, 0.0);
		break;

	case VK_UP:
		mTranslation *= MatTranslation(0.0, 0.1, 0.0);
		break;

	case VK_DOWN:
		mTranslation *= MatTranslation(0.0, -0.1, 0.0);
		break;

	case 'W':
		mTranslation *= MatTranslation(0.0, 0.0, 0.1);
		break;

	case 'S':
		mTranslation *= MatTranslation(0.0, 0.0, -0.1);
		break;

	default:
		CView::OnKeyUp(nChar, nRepCnt, nFlags);
		return;
	}

	Render();
	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}


void CCG31020View::OnObliqueBimetric()
{
	mProjType = OBLIQUE_BIMETRIC_PROJ;
	SelectProj();
	mPassConstants.viewProj = mView * mProj;
	Render();
}


void CCG31020View::OnPerspective()
{
	mProjType = PERSPECTIVE_PROJ;
	SelectProj();
	mPassConstants.viewProj = mView * mProj;
	Render();
}
