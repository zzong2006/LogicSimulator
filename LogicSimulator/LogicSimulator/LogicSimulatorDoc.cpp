
// LogicSimulatorDoc.cpp : CLogicSimulatorDoc Ŭ������ ����
//

#include "stdafx.h"

// SHARED_HANDLERS�� �̸� ����, ����� �׸� �� �˻� ���� ó���⸦ �����ϴ� ATL ������Ʈ���� ������ �� ������
// �ش� ������Ʈ�� ���� �ڵ带 �����ϵ��� �� �ݴϴ�.
#ifndef SHARED_HANDLERS
#include "LogicSimulator.h"
#endif

#include "LogicSimulatorDoc.h"


#include <propkey.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CLogicSimulatorDoc

IMPLEMENT_DYNCREATE(CLogicSimulatorDoc, CDocument)

BEGIN_MESSAGE_MAP(CLogicSimulatorDoc, CDocument)
END_MESSAGE_MAP()


// CLogicSimulatorDoc ����/�Ҹ�

CLogicSimulatorDoc::CLogicSimulatorDoc()
{
	temp = NULL;
	clickMode = false;
	selectMode = true;
	simulateMode = false;
	CanBeDivided = false;
	isOnFocus = false;
}

CLogicSimulatorDoc::~CLogicSimulatorDoc()
{
	for (int i = 0; i < logicInfo.size(); i++)
		delete logicInfo.at(i);
	for (int i = 0; i < lines.size(); i++)
		delete lines.at(i);
	
}

BOOL CLogicSimulatorDoc::OnNewDocument()
{
	for (int i = 0; i < logicInfo.size(); i++)
		delete logicInfo.at(i);
	
	for (int i = 0; i < lines.size(); i++)
		delete lines.at(i);
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: ���⿡ ���ʱ�ȭ �ڵ带 �߰��մϴ�.
	// SDI ������ �� ������ �ٽ� ����մϴ�.

	return TRUE;
}




// CLogicSimulatorDoc serialization

void CLogicSimulatorDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: ���⿡ ���� �ڵ带 �߰��մϴ�.
	}
	else
	{
		// TODO: ���⿡ �ε� �ڵ带 �߰��մϴ�.
	}
}

#ifdef SHARED_HANDLERS

// ����� �׸��� �����մϴ�.
void CLogicSimulatorDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// ������ �����͸� �׸����� �� �ڵ带 �����Ͻʽÿ�.
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// �˻� ó���⸦ �����մϴ�.
void CLogicSimulatorDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// ������ �����Ϳ��� �˻� �������� �����մϴ�.
	// ������ �κ��� ";"�� ���еǾ�� �մϴ�.

	// ��: strSearchContent = _T("point;rectangle;circle;ole object;");
	SetSearchContent(strSearchContent);
}

void CLogicSimulatorDoc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = NULL;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != NULL)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// CLogicSimulatorDoc ����

#ifdef _DEBUG
void CLogicSimulatorDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CLogicSimulatorDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CLogicSimulatorDoc ���


void CLogicSimulatorDoc::CheckCircuit()
{
	std::queue <LineObject *> searchLine;

	//�ʱ�ȭ
	for (int i = 0; i < lines.size(); i++)
	{
		lines.at(i)->chk = 0;
		lines.at(i)->state = OFF_SIGNAL;
	}
	
	//����Ʈ ,���̺귯�� �ڽ�, �����, �ø��÷�
	int lgn = gateInfo.size();
	for (int i = 0; i < lgn; i++)
	{
		LogicObject* curLogic = gateInfo.at(i);
		curLogic->chk = FALSE;
		int out = curLogic->outputNum, in = curLogic->inputNum;

		//����� �ʱ�ȭ
		for (int i = 0; i < 1; i++)
			curLogic->outputCoord[i].second = -1;
		//�Է��� �ʱ�ȭ
		for (int i = 0; i < in; i++)
			curLogic->inputCoord[i].second = -1;			//input chk �ʱ�ȭ

	}

	////////////////////////////////////////////////////////////////��� ����//////////////////////////////////////////////////////////
	//�Է� Pin/Clock �� ���õ� ���� �ޱ�
	//��� Pin ���� ���� �����ؾ� �Ѵ�.
	int pn = pinInfo.size();
	for (int i = 0; i < pn; i++)
	{
		CPoint pinos = pinInfo.at(i)->outputCoord[0].first;
		int lin = lines.size();

		for (int i = 0; i < lin; i++)
		{
			LineObject* curline = lines.at(i);
			if (curline->line[0] == pinos || curline->line[1] == pinos)
			{
				//curline->chk = 1;
				if (pinInfo.at(i)->getOutput() == TRUE)
				curline->state = ON_SIGNAL;//pinInfo.at(i)->getOutput();
				else curline->state = OFF_SIGNAL;
				searchLine.push(curline);
			}
		}
		pinInfo.at(i)->chk = TRUE;
	}

	for (int i = 0; i < clockInfo.size(); i++)
	{
	}
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////���� ����///////////////////////////////////////////////////////////
	while (!searchLine.empty())
	{
		//(�ʹ�) Pin�� ����� ���� �� ���鼭 �˻��Ѵ�.
		while (!searchLine.empty())
		{
			LineObject* temp_line = searchLine.front();
			searchLine.pop();
			int lin = lines.size();

			/////////////////////////////////////////////////////����� �� üũ////////////////////////////////////////////////////
			for (int i = 0; i < lin; i++)
			{
				LineObject* curline = lines.at(i);
				if (curline->chk == FALSE)
				{
					if (curline->line[0] == temp_line->line[0] || curline->line[1] == temp_line->line[0]
						|| curline->line[0] == temp_line->line[1] || curline->line[1] == temp_line->line[1])
					{
						curline->chk = TRUE;
						curline->state = temp_line->state;
						searchLine.push(curline);
					}
				}
			}
			//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

			//////////////////////////////////////////////����Ʈ �Է¼� üũ///////////////////////////////////////////////////////
			int gan = gateInfo.size();
			for (int i = 0; i < gan; i++)
			{
				Gate* curgate = gateInfo.at(i);
				if (curgate->chk == FALSE)
				{
					int ip = curgate->inputNum;
					for (int j = 0; j < ip; j++)
					{
						if (curgate->inputCoord[j].first == temp_line->line[0] || curgate->inputCoord[j].first == temp_line->line[1])
						{
							curgate->inputCoord[j].second = TRUE;
						}
					}
				}
			}
			//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			temp_line->chk = TRUE;
		}

		//////////////////////////////////////////////����Ʈ ��¼� üũ///////////////////////////////////////////////////////
		//��ȸ �ؾߵ� LogicObject-> gate, flipflop, Library Box ������. ���� ������Ʈ�� �Է� ���� ��� �湮�Ǿ���? && �̹� �湮�Ͽ��°�?
		int gan = gateInfo.size();
		for (int i = 0; i < gan; i++)
		{
			Gate* temp_gate = gateInfo.at(i);
			//Gate �湮
			if (temp_gate->isInputSet() && temp_gate->chk == FALSE)
			{
				temp_gate->chk = TRUE;
				temp_gate->setOutput();
				CPoint gatpos = temp_gate->outputCoord[0].first;

				int lin = lines.size();
				for (int i = 0; i < lin; i++)
				{
					LineObject* curline = lines.at(i);
					if (curline->line[0] == gatpos || curline->line[1] == gatpos)
					{
						curline->chk = 1;
						if (temp_gate->outputCoord[0].second == 1)
							curline->state = ON_SIGNAL;
						else curline->state = OFF_SIGNAL;
						searchLine.push(curline);
					}
				}
			}
		}
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//Outpin �湮 (���� ������)
		for (int i = 0; i < outInfo.size(); i++)
		{
			Out* temp_out = outInfo.at(i);
		
			if (temp_out->isInputSet() && !temp_out->chk)
			{
				temp_out->chk = TRUE;
				temp_out->setOutput();
			}
		}
	}
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
}
