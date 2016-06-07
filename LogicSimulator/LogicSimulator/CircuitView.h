#pragma once

#define UNIT 10
// CCircuitView 뷰입니다.

class CCircuitView : public CView
{
	DECLARE_DYNCREATE(CCircuitView)
protected:
	int dec_x, dec_y;
	int object;
	int cur_line;
	BOOL CUTED = FALSE , PASTED =FALSE;
protected:
	CCircuitView();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~CCircuitView();

public:
	virtual void OnDraw(CDC* pDC);      // 이 뷰를 그리기 위해 재정의되었습니다.
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	DECLARE_MESSAGE_MAP()
public:
	void DrawImage(CDC *pDC);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	virtual void OnInitialUpdate();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnMouseLeave();
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnClickMode();
	afx_msg void OnSelectMode();
	afx_msg void OnUpdateClickMode(CCmdUI *pCmdUI);
	afx_msg void OnUpdateSelectMode(CCmdUI *pCmdUI);
	afx_msg void OnOnSimulate();
	afx_msg void OnUpdateOnSimulate(CCmdUI *pCmdUI);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnEditUndo();
	afx_msg void OnEditRedo();
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg BOOL OnCopyData(CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct);
	afx_msg void OnEditCopy();
	afx_msg void OnEditPaste();
	afx_msg void OnEditCut();
};


