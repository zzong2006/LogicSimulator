
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
		curLogic->chk = 0;
		int out = curLogic->outputNum, in = curLogic->inputNum;

		//����� �ʱ�ȭ
		for (int i = 0; i < 1; i++)
			curLogic->outputCoord[i].second = -1;
		//�Է��� �ʱ�ȭ
		for (int i = 0; i < in; i++)
			curLogic->inputCoord[i].second = -1;			//input chk �ʱ�ȭ

	}

	////////////////////////////////////////////////////////////////��� ����////////////////////////////////////////////////////////////
	//�Է� Pin/Clock �� ���õ� ���� �ޱ�
	//��� Pin ���� ���� �����ؾ� �Ѵ�.
	int pn = pinInfo.size();
	for (int i = 0; i < pn; i++)
	{
		CPoint pinos = pinInfo.at(i)->outputCoord[0].first;
		int lin = lines.size();

		for (int j = 0; j < lin; j++)
		{
			LineObject* curline = lines.at(j);
			if (curline->line[0] == pinos || curline->line[1] == pinos)
			{
				curline->chk = 1;
				curline->state = pinInfo.at(i)->getOutput();
				searchLine.push(curline);
			}
		}
		pinInfo.at(i)->chk = 1;
	}

	for (int i = 0; i < clockInfo.size(); i++)
	{
		CPoint pinos = clockInfo.at(i)->outputCoord[0].first;
		int lin = lines.size();

		for (int j = 0; j < lin; j++)
		{
			LineObject* curline = lines.at(j);
			if (curline->line[0] == pinos || curline->line[1] == pinos)
			{
				curline->chk = 1;
				curline->state = clockInfo.at(i)->getOutput();
				searchLine.push(curline);
			}
		}
		clockInfo.at(i)->chk = 1;
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
				if (curline->chk == 0)
				{
					if (curline->line[0] == temp_line->line[0] || curline->line[1] == temp_line->line[0]
						|| curline->line[0] == temp_line->line[1] || curline->line[1] == temp_line->line[1])
					{
						curline->chk = 1;
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
				if (curgate->chk == 0)
				{
					int ip = curgate->inputNum;
					for (int j = 0; j < ip; j++)
					{
						if (curgate->inputCoord[j].first == temp_line->line[0] || curgate->inputCoord[j].first == temp_line->line[1])
						{
							curgate->inputCoord[j].second = temp_line->state;
						}
					}
				}
			}
			//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			temp_line->chk = 1;
		}

		//////////////////////////////////////////////����Ʈ ��¼� üũ///////////////////////////////////////////////////////
		//��ȸ �ؾߵ� LogicObject-> gate, flipflop, Library Box ������. ���� ������Ʈ�� �Է� ���� ��� �湮�Ǿ���? && �̹� �湮�Ͽ��°�?
		int gan = gateInfo.size();
		for (int i = 0; i < gan; i++)
		{
			Gate* temp_gate = gateInfo.at(i);
			//Gate �湮
			if (temp_gate->isInputSet() && temp_gate->chk == 0)
			{
				temp_gate->chk = 1;
				temp_gate->setOutput();
				CPoint gatpos = temp_gate->outputCoord[0].first;

				int lin = lines.size();
				for (int i = 0; i < lin; i++)
				{
					LineObject* curline = lines.at(i);
					if (curline->line[0] == gatpos || curline->line[1] == gatpos)
					{
							curline->state = temp_gate->outputCoord[0].second;

						if (curline->chk != 1)
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
				temp_out->chk = 1;
				temp_out->setOutput();
			}
		}
	}
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
}

BOOL CLogicSimulatorDoc::CanUndo()
{
	while (mUndo.GetCount() > MAX_UNDO)
	{
		mUndo.RemoveTail();
	}
	return (mUndo.GetCount() > 0);
}

void CLogicSimulatorDoc::Undo()
{
	Action temp;
	CPoint stp, edp;

	temp = mUndo.RemoveHead();
	int lkedn = temp.lineked_line.size();

	switch (temp.Type)
	{
	case LINE :

		for (int i = lkedn - 1; i > 0; i -= 2)
		{
			temp.lineked_line.at(i - 1)->line[1] = temp.lineked_line.at(i)->line[1];
			lines.pop_back();
		}

		for (int i = 0; i < 2; i++)
		{
			temp.lines.push_back(lines.at(lines.size() - 1));
			lines.pop_back();
		}

		break;
	case OBJECT :
		logicInfo.pop_back();
		break;
	}
	mRedo.AddHead(temp);
}

BOOL CLogicSimulatorDoc::CanRedo()
{
	while (mRedo.GetCount() > MAX_UNDO)
	{
		mRedo.RemoveTail();
	}
	return (mRedo.GetCount() > 0);
}

void CLogicSimulatorDoc::Redo()
{
	Action temp;
	CPoint stp, edp;

	temp = mRedo.RemoveHead();
	int lkedn = temp.lineked_line.size();

	switch (temp.Type)
	{
	case LINE : 
		for (int i = 1; i > -1; i--)
		{
			lines.push_back(temp.lines.at(i));
			temp.lines.pop_back();
		}

		for (int i = 0; i < lkedn; i += 2)
		{
			temp.lineked_line.at(i)->line[1] = temp.lineked_line.at(i + 1)->line[0];
			lines.push_back(temp.lineked_line.at(i + 1));
		}
		break;
	case OBJECT:
		logicInfo.push_back(temp.logicInfo.at(0));
		break;
	}
	mUndo.AddHead(temp);
}