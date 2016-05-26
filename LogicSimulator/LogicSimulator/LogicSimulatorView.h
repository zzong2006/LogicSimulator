
// LogicSimulatorView.h : CLogicSimulatorView Ŭ������ �������̽�
//

#pragma once


class CLogicSimulatorView : public CView
{
protected: // serialization������ ��������ϴ�.
	CLogicSimulatorView();
	DECLARE_DYNCREATE(CLogicSimulatorView)

	// Ư���Դϴ�.
public:
	CLogicSimulatorDoc* GetDocument() const;

	// �۾��Դϴ�.
public:

	// �������Դϴ�.
public:
	virtual void OnDraw(CDC* pDC);  // �� �並 �׸��� ���� �����ǵǾ����ϴ�.
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

	// �����Դϴ�.
public:
	virtual ~CLogicSimulatorView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

	// ������ �޽��� �� �Լ�
protected:
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // LogicSimulatorView.cpp�� ����� ����
inline CLogicSimulatorDoc* CLogicSimulatorView::GetDocument() const
{
	return reinterpret_cast<CLogicSimulatorDoc*>(m_pDocument);
}
#endif

