#pragma once


// CMenuView ∫‰¿‘¥œ¥Ÿ.

class CMenuView : public CTreeView
{
	DECLARE_DYNCREATE(CMenuView)

protected:
	CMenuView();   
	virtual ~CMenuView();

public:
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif
	 
protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();
	afx_msg void OnTvnSelchanged(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMClick(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnClickMode();
	afx_msg void OnUpdateClickMode(CCmdUI *pCmdUI);
	afx_msg void OnSelectMode();
	afx_msg void OnUpdateSelectMode(CCmdUI *pCmdUI);
	
	afx_msg void OnNMDblclk(NMHDR *pNMHDR, LRESULT *pResult);
};


