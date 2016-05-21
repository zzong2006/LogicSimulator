
// LogicSimulatorView.cpp : CLogicSimulatorView Ŭ������ ����
//

#include "stdafx.h"
// SHARED_HANDLERS�� �̸� ����, ����� �׸� �� �˻� ���� ó���⸦ �����ϴ� ATL ������Ʈ���� ������ �� ������
// �ش� ������Ʈ�� ���� �ڵ带 �����ϵ��� �� �ݴϴ�.
#ifndef SHARED_HANDLERS
#include "LogicSimulator.h"
#endif

#include "LogicSimulatorDoc.h"
#include "LogicSimulatorView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CLogicSimulatorView

IMPLEMENT_DYNCREATE(CLogicSimulatorView, CView)

BEGIN_MESSAGE_MAP(CLogicSimulatorView, CView)
	// ǥ�� �μ� ����Դϴ�.
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
END_MESSAGE_MAP()

// CLogicSimulatorView ����/�Ҹ�

CLogicSimulatorView::CLogicSimulatorView()
{
	// TODO: ���⿡ ���� �ڵ带 �߰��մϴ�.

}

CLogicSimulatorView::~CLogicSimulatorView()
{

}

BOOL CLogicSimulatorView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs�� �����Ͽ� ���⿡��
	//  Window Ŭ���� �Ǵ� ��Ÿ���� �����մϴ�.

	return CView::PreCreateWindow(cs);
}

// CLogicSimulatorView �׸���

void CLogicSimulatorView::OnDraw(CDC* /*pDC*/)
{
	CLogicSimulatorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: ���⿡ ���� �����Ϳ� ���� �׸��� �ڵ带 �߰��մϴ�.
}


// CLogicSimulatorView �μ�

BOOL CLogicSimulatorView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// �⺻���� �غ�
	return DoPreparePrinting(pInfo);
}

void CLogicSimulatorView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: �μ��ϱ� ���� �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
}

void CLogicSimulatorView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: �μ� �� ���� �۾��� �߰��մϴ�.
}


// CLogicSimulatorView ����

#ifdef _DEBUG
void CLogicSimulatorView::AssertValid() const
{
	CView::AssertValid();
}

void CLogicSimulatorView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CLogicSimulatorDoc* CLogicSimulatorView::GetDocument() const // ����׵��� ���� ������ �ζ������� �����˴ϴ�.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CLogicSimulatorDoc)));
	return (CLogicSimulatorDoc*)m_pDocument;
}
#endif //_DEBUG


// CLogicSimulatorView �޽��� ó����
