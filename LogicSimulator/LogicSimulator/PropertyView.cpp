// PropertyView.cpp : 구현 파일입니다.
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


// CPropertyView 진단입니다.

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


// CPropertyView 메시지 처리기입니다.


void CPropertyView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	//스크롤바 무효화 
	CSize scrollSize(0, 0);
	SetScrollSizes(MM_TEXT, scrollSize);

	CRect rectDummy, rc;

	rectDummy.SetRectEmpty();

	m_pGridInfo.Create(WS_VISIBLE | WS_CHILD, rectDummy, this, 2);
	
	//PropertyGrid 기본 셋팅
	CPropertyView::GetClientRect(&rc);
	CPropertyView::MapWindowPoints(this, &rc);

	m_pGridInfo.MoveWindow(&rc);

	m_pGridInfo.EnableHeaderCtrl(FALSE);
	m_pGridInfo.SetVSDotNetLook();
	m_pGridInfo.EnableDescriptionArea();

	//정보 추가
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

	CString labelName, trigger ,cycleHz, CoN;

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

	CMFCPropertyGridProperty* Label = new CMFCPropertyGridProperty(_T("Facing"), labelName,_T("설명"), 1);

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

		CMFCPropertyGridProperty* triggerEdge = new CMFCPropertyGridProperty(_T("Trigger"), trigger, _T("설명"), 2);
	
		triggerEdge->AddOption(_T("Rising Edge"));
		triggerEdge->AddOption(_T("Falling Edge"));

		pGroupInfo->AddSubItem(triggerEdge);
	}

	if (tempLO->objectName == CLOCK)
	{
		Clock* Ctemp = (Clock *)tempLO;
		cycleHz.Format(_T("%.0f"), (double)(1000/Ctemp->getOricycle()));

		pGroupInfo->AddSubItem(new CMFCPropertyGridProperty(_T("Clock(Hz)"), cycleHz, _T("설명"),3));

	}

	if (tempLO->objectName == OUTPIN || tempLO->objectName == PIN)
	{
		if (tempLO->objectName == OUTPIN) {
			Out * Ptemp = (Out *)tempLO;
			CoN.Format(_T("%d"), Ptemp->getConNum());
		}
			
		else {
			Pin * Ptemp = (Pin *)tempLO;
			CoN.Format(_T("%d"), Ptemp->getConNum());
		}

		CMFCPropertyGridProperty* ConnectedNum = new CMFCPropertyGridProperty(_T("ConnectedNumber"), CoN, _T("설명"), 4);

		CString tempNum;
		for (int i = -1; i < 10; i++)
		{
			tempNum.Format(_T("%d"), i);
			ConnectedNum->AddOption(tempNum);
		}
		ConnectedNum->AllowEdit(FALSE);
		pGroupInfo->AddSubItem(ConnectedNum);
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

		case 4:
			CString changedValue = pProperty->GetValue();
			//출력핀
			if (pDoc->currBox->currObject.at(0)->objectName == OUTPIN) {
				if (_ttoi(changedValue) >= pDoc->currBox->NumOuput) {
					AfxMessageBox(L"Cannot be modified by changing upper output number");
				}
				else {
					Out * Otemp = (Out *)pDoc->currBox->currObject.at(0);
					//1가지
					if (_ttoi(changedValue) < 0) {
						pDoc->currBox->ConnOutput[Otemp->getConNum()] = FALSE;
						Otemp->setConNum(-1);
					}
					
					//2가지
					BOOL Swap = FALSE;
					for (int i = 0; i < pDoc->currBox->logicInfo.size(); i++)
					{
						if (pDoc->currBox->logicInfo.at(i)->objectName == OUTPIN) {
							Out * OtherTemp = (Out *)pDoc->currBox->logicInfo.at(i);

							if (OtherTemp->getConNum() == _ttoi(changedValue)) {
								int hoi;

								hoi = OtherTemp->getConNum();
								OtherTemp->setConNum(Otemp->getConNum());
								Otemp->setConNum(_ttoi(changedValue));

								Swap = TRUE;
								break;
							}
						}
					}

					//3가지 : 없을 경우
					if (!Swap)
					{
						pDoc->currBox->ConnOutput[Otemp->getConNum()] = FALSE;
						Otemp->setConNum(_ttoi(changedValue));
						pDoc->currBox->ConnOutput[Otemp->getConNum()] = TRUE;
					}
					//총 3가지 경우, -1로 바꿀때, 다른 걸로 바꾸는데 이미 있을때, 다른걸로 바꾸는데 없을때
				}
			}
			//입력핀
			else {
				if (_ttoi(changedValue) >= pDoc->currBox->NumInput) {
					AfxMessageBox(L"Cannot be modified by changing upper input number");
				}
				else {
					Pin * Ptemp = (Pin *)pDoc->currBox->currObject.at(0);
					//1가지
					if (_ttoi(changedValue) < 0) {
						pDoc->currBox->ConnInput[Ptemp->getConNum()] = FALSE;
						Ptemp->setConNum(-1);
					}

					//2가지
					BOOL Swap = FALSE;
					for (int i = 0; i < pDoc->currBox->logicInfo.size(); i++)
					{
						if (pDoc->currBox->logicInfo.at(i)->objectName == PIN) {
							Pin * OtherTemp = (Pin *)pDoc->currBox->logicInfo.at(i);

							if (OtherTemp->getConNum() == _ttoi(changedValue)) {
								int hoi;

								hoi = OtherTemp->getConNum();
								OtherTemp->setConNum(Ptemp->getConNum());
								Ptemp->setConNum(_ttoi(changedValue));

								Swap = TRUE;
								break;
							}
						}
					}

					//3가지 : 없을 경우
					if (!Swap)
					{
						
						pDoc->currBox->ConnInput[Ptemp->getConNum()] = FALSE;
						Ptemp->setConNum(_ttoi(changedValue));
						pDoc->currBox->ConnInput[Ptemp->getConNum()] = TRUE;

					}
					//총 3가지 경우, -1로 바꿀때, 다른 걸로 바꾸는데 이미 있을때, 다른걸로 바꾸는데 없을때
				}
			}
			
		break;
		}
	}
	pDoc->currBox->CheckCircuit();
	CVCtrl->Invalidate();

	return 0L;
}


