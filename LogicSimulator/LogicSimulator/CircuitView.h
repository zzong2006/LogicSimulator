#pragma once


// CCircuitView ���Դϴ�.

class CCircuitView : public CView
{
	DECLARE_DYNCREATE(CCircuitView)

protected:
	CCircuitView();           // ���� ����⿡ ���Ǵ� protected �������Դϴ�.
	virtual ~CCircuitView();

public:
	virtual void OnDraw(CDC* pDC);      // �� �並 �׸��� ���� �����ǵǾ����ϴ�.
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
};


