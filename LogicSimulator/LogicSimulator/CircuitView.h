#pragma once

#define UNIT 10
// CCircuitView ���Դϴ�.

class CCircuitView : public CView
{
	DECLARE_DYNCREATE(CCircuitView)
protected:
	int dec_x, dec_y;

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
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	virtual void OnInitialUpdate();
	afx_msg void OnPaint();
	afx_msg void OnClickMode();
	afx_msg void OnSelectMode();
	afx_msg void OnUpdateClickMode(CCmdUI *pCmdUI);
	afx_msg void OnUpdateSelectMode(CCmdUI *pCmdUI);
};


