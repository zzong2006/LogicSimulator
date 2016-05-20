#pragma once
#include "afxpropertygridctrl.h"



// CPropertyView �� ���Դϴ�.

class CPropertyView : public CFormView
{
	DECLARE_DYNCREATE(CPropertyView)

protected:
	CPropertyView();           // ���� ����⿡ ���Ǵ� protected �������Դϴ�.
	virtual ~CPropertyView();

public:
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PROPERTYVIEW };
#endif
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();
	// PropertyControl
	CMFCPropertyGridCtrl m_ctrlGridControl;
};


