// PropertyView.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "LogicSimulator.h"
#include "PropertyView.h"
#include "LogicSimulatorDoc.h"
#include "CircuitView.h"
#include "MainFrm.h"
// CPropertyView

IMPLEMENT_DYNCREATE(CPropertyView, CFormView)

CPropertyView::CPropertyView()
	: CFormView(IDD_PROPERTYVIEW)
{

}

CPropertyView::~CPropertyView()
{
}

void CPropertyView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CPropertyView, CFormView)
	ON_REGISTERED_MESSAGE(AFX_WM_PROPERTY_CHANGED,OnPropertyChanged)
END_MESSAGE_MAP()


// CPropertyView �����Դϴ�.

#ifdef _DEBUG
void CPropertyView::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CPropertyView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CPropertyView �޽��� ó�����Դϴ�.


void CPropertyView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	//��ũ�ѹ� ��ȿȭ 
	CSize scrollSize(0, 0);
	SetScrollSizes(MM_TEXT, scrollSize);

	CRect rectDummy, rc;

	rectDummy.SetRectEmpty();

	m_pGridInfo.Create(WS_VISIBLE | WS_CHILD, rectDummy, this, 2);
	
	//PropertyGrid �⺻ ����
	CPropertyView::GetClientRect(&rc);
	CPropertyView::MapWindowPoints(this, &rc);

	m_pGridInfo.MoveWindow(&rc);

	m_pGridInfo.EnableHeaderCtrl(FALSE);
	m_pGridInfo.SetVSDotNetLook();
	m_pGridInfo.EnableDescriptionArea();

	//���� �߰�
	CMFCPropertyGridProperty* pGroupInfo = new CMFCPropertyGridProperty(_T("Information"));
	
	m_pGridInfo.AddProperty(pGroupInfo);
	m_pGridInfo.UpdateData(FALSE);
}

void CPropertyView::InitializePropGrid(LogicObject *tempLO)
{
	m_pGridInfo.RemoveAll();
	m_pGridInfo.EnableHeaderCtrl(FALSE);
	m_pGridInfo.SetVSDotNetLook();
	m_pGridInfo.EnableDescriptionArea(FALSE);

	CString labelName, trigger ,cycleHz;

	switch (tempLO->facing) {
	case EAST:
		labelName = _T("East");
		break;
	case WEST:
		labelName = _T("West");
		break;
	case SOUTH:
		labelName = _T("South");
		break;
	case NORTH:
		labelName = _T("North");
		break;
	}

	CMFCPropertyGridProperty* pGroupInfo = new CMFCPropertyGridProperty(_T("Information"));

	CMFCPropertyGridProperty* Label = new CMFCPropertyGridProperty(_T("Facing"), labelName,_T("����"), 1);

	pGroupInfo->AddSubItem(new CMFCPropertyGridProperty(_T("Label"), tempLO->label, 0));

	Label->AddOption(_T("East"));
	Label->AddOption(_T("West"));
	Label->AddOption(_T("North"));
	Label->AddOption(_T("South"));
	Label->AllowEdit(FALSE);
	pGroupInfo->AddSubItem(Label);

	if (tempLO->objectType == FLIPFLOP_TYPE)
	{
		FlipFlop* FFtemp = (FlipFlop *) tempLO;
		if (FFtemp->GetTrigger())
			trigger = _T("Rising Edge");
		else trigger = _T("Falling Edge");

		CMFCPropertyGridProperty* triggerEdge = new CMFCPropertyGridProperty(_T("Trigger"), trigger, _T("����"), 2);
	
		triggerEdge->AddOption(_T("Rising Edge"));
		triggerEdge->AddOption(_T("Falling Edge"));

		pGroupInfo->AddSubItem(triggerEdge);
	}

	if (tempLO->objectName == CLOCK)
	{
		Clock* Ctemp = (Clock *)tempLO;
		cycleHz.Format(_T("%.0f"), (double)(1000/Ctemp->getOricycle()));

		pGroupInfo->AddSubItem(new CMFCPropertyGridProperty(_T("Clock(Hz)"), cycleHz, _T("����"),3));

	}

	m_pGridInfo.AddProperty(pGroupInfo);
	
	m_pGridInfo.UpdateData(FALSE);
}

LRESULT CPropertyView::OnPropertyChanged(WPARAM wParam, LPARAM lParam)
{
	// wParam - control id, lParam - a pointer to property that changed
	CMFCPropertyGridProperty* pProperty = (CMFCPropertyGridProperty*)lParam;

	CCircuitView *CVCtrl = (CCircuitView *)((CMainFrame*)AfxGetMainWnd())->m_wndSplitterMain.GetPane(0, 1);

	CLogicSimulatorDoc *pDoc = (CLogicSimulatorDoc *)GetDocument();

	if (pDoc->currBox->currObject.size() == 1) {
		switch (pProperty->GetData())
		{
		case 0:			//Label
			pDoc->currBox->currObject.at(0)->setLabel(pProperty->GetValue());
			break;
		case 1:			//Facing
			pDoc->currBox->currObject.at(0)->setFacing(pProperty->GetValue());
			pDoc->currBox->currObject.at(0)->set_Coord_ByFacing(pProperty->GetValue());
			break;
		case 2:
		{
			FlipFlop *FFtemp = (FlipFlop*)pDoc->currBox->currObject.at(0);
			FFtemp->SetTrigger(pProperty->GetValue());

		}
			break;
		case 3:
		{
			Clock *Ctemp = (Clock *)pDoc->currBox->currObject.at(0);
			CString cycle = pProperty->GetValue();
			Ctemp->setCycle(_ttoi(cycle));
		}
			break;
		}
	}
	pDoc->currBox->CheckCircuit();
	CVCtrl->Invalidate();

	return 0L;
}


