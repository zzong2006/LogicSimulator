
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
		lines.at(i)->chk = 0;
	
	for (int i = 0; i < gateInfo.size(); i++)
		gateInfo.at(i)->chk = 0;
	

	//�Է� Pin/Clock �� ���õ� ���� �ޱ�
	//��� Pin ���� ���� �����ؾ� �Ѵ�.
	for (int i = 0; i < pinInfo.size(); i++)
	{
		LineObject* temp_line = pinInfo.at(i)->output_line;
		temp_line->state = pinInfo.at(i)->getOutput();
		searchLine.push(temp_line);
	}

	for (int i = 0; i < clockInfo.size(); i++)
	{
		LineObject* temp_line = clockInfo.at(i)->output_line;
		temp_line->state = clockInfo.at(i)->getOutput();
		searchLine.push(temp_line);
	}


	//����
	while (!searchLine.empty())
	{
		//(�ʹ�) Pin�� ����� ���� �� ���鼭 �˻��Ѵ�.
		while (!searchLine.empty())
		{
			LineObject* temp_line = searchLine.front();
			searchLine.pop();

			temp_line->chk = TRUE;
			for (int i = 0; i < temp_line->connect_lines.size(); i++)
			{
				if (temp_line->connect_lines.at(i)->chk != TRUE)
				{
					temp_line->connect_lines.at(i)->state = temp_line->state;
					temp_line->connect_lines.at(i)->chk = TRUE;
					searchLine.push(temp_line->connect_lines.at(i));
				}
			}
		}

		//��ȸ �ؾߵ� LogicObject-> gate, flipflop, Library Box ������. ���� ������Ʈ�� �Է� ���� ��� �湮�Ǿ���? && �̹� �湮�Ͽ��°�?
		for (int i = 0; i < gateInfo.size(); i++)
		{
			Gate* temp_gate = gateInfo.at(i);

			if (temp_gate->isInputSet() && !temp_gate->chk)
			{
				temp_gate->chk = TRUE;
				temp_gate->setOutput();
				searchLine.push(temp_gate->output_line);
			}
		}
	}
}
