
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
	ON_COMMAND(ID_FILE_SAVE, &CLogicSimulatorDoc::OnFileSave)
	ON_COMMAND(ID_FILE_OPEN, &CLogicSimulatorDoc::OnFileOpen)
	ON_COMMAND(ID_FILE_NEW, &CLogicSimulatorDoc::OnFileNew)
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
	int line_num, logic_num;

	if (ar.IsStoring())
	{
		// TODO: ���⿡ ���� �ڵ带 �߰��մϴ�.
		/*/////////////////////////////////////////////////////////���� ����//////////////////////////////////////////////////////////
		1. �� ( ���� -> ����)
		2. ������Ʈ (���� -> ����)
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////*/
		line_num = lines.size();
		logic_num = logicInfo.size();

		ar << line_num;
		ar << logic_num;

		//�� ��ġ 1�� -> 2��
		for (int i = 0; i < line_num; i++)
		{
			ar << lines.at(i)->line[0] << lines.at(i)->line[1];
		}
		//Ÿ�� -> �̸� -> ��ġ
		for (int i = 0; i < logic_num; i++)
		{
			LogicObject* tempLogic = logicInfo.at(i);
			CPoint find_pos;
			find_pos.x = tempLogic->get_bottm().x;
			find_pos.y = (tempLogic->get_top().y + tempLogic->get_bottm().y) / 2;
			ar << tempLogic->objectType << tempLogic->objectName << find_pos;
		}

	}
	else
	{
		// TODO: ���⿡ �ε� �ڵ带 �߰��մϴ�.
		/*/////////////////////////////////////////////////////////�ε� ����//////////////////////////////////////////////////////////
		1. �� ( ���� -> ����)
		2. ������Ʈ (���� -> ����)
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////*/
		ar >> line_num;
		ar >> logic_num;

		for (int i = 0; i < line_num; i++)
		{
			CPoint pos1, pos2;
			ar >> pos1 >> pos2;
			LineObject* templine = new LineObject(pos1, pos2);
			lines.push_back(templine);
		}

		for (int i = 0; i < logic_num; i++)
		{
			int objectType, objectName;
			CPoint find_pos;

			ar >> objectType >> objectName >> find_pos;

			Gate *Gtemp;
			FlipFlop *Ftemp;
			Pin *Ptemp;
			Clock *Ctemp;
			Out *Otemp;

			switch (objectType)
			{
			case GATE_TYPE:
				Gtemp = NULL;
				switch (objectName)
				{
				case AND_GATE:
					Gtemp = new andGate(find_pos.x, find_pos.y);
					break;
				case OR_GATE:
					Gtemp = new orGate(find_pos.x, find_pos.y);
					break;
				case NAND_GATE:
					Gtemp = new nandGate(find_pos.x, find_pos.y);
					break;
				case NOR_GATE:
					Gtemp = new norGate(find_pos.x, find_pos.y);
					break;
				case XOR_GATE:
					Gtemp = new xorGate(find_pos.x, find_pos.y);
					break;
				case NOT_GATE:
					Gtemp = new notGate(find_pos.x, find_pos.y);
					break;
				}

				if (Gtemp != NULL) {
					Gtemp->set_Coord_From_outC(find_pos.x, find_pos.y);
					logicInfo.push_back(Gtemp);
				}
				break;
			case FLIPFLOP_TYPE :
				Ftemp = NULL;
				switch (objectName)
				{
				case D_FF:
					Ftemp = new DFlipFlop(find_pos.x, find_pos.y);
					break;
				case JK_FF:
					break;
				case T_FF:
					break;
				}

				if (Ftemp != NULL) {
					Ftemp->set_Coord_From_outC(find_pos.x, find_pos.y);

					logicInfo.push_back(Ftemp);
				}
				break;
			case WIRING_TYPE :
				switch (objectName)
				{
				case PIN :
					Ptemp = NULL;
					Ptemp = new Pin(find_pos.x, find_pos.y);
					logicInfo.push_back(Ptemp);
					Ptemp->set_outputCoord(find_pos.x, find_pos.y);
					Ptemp->set_Coord_From_outC(find_pos.x, find_pos.y);
					break;
				case CLOCK :
					Ctemp = NULL;
					Ctemp = new Clock(find_pos.x, find_pos.y);
					Ctemp->set_outputCoord(find_pos.x, find_pos.y);
					Ctemp->set_Coord_From_outC(find_pos.x, find_pos.y);
					logicInfo.push_back(Ctemp);
					break;
				case OUTPIN :
					Otemp = NULL;
					Otemp = new Out(find_pos.x, find_pos.y);
					Otemp->set_outputCoord(find_pos.x, find_pos.y);
					Otemp->set_Coord_From_outC(find_pos.x, find_pos.y);
					logicInfo.push_back(Otemp);
					break;
				}
				break;
			}
		}
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
BOOL CLogicSimulatorDoc::IsInput(LogicObject* lo)
{
	return lo->objectName == PIN || lo->objectName == CLOCK;
}
BOOL CLogicSimulatorDoc::IsGate(LogicObject* lo)
{
	return lo->objectType == GATE_TYPE || lo->objectType == FLIPFLOP_TYPE;
}

BOOL CLogicSimulatorDoc::IsOutput(LogicObject* lo)
{
	return lo->objectType == OUTPIN;
}

void CLogicSimulatorDoc::CheckCircuit()
{
	std::queue <LineObject *> searchLine;
	LogicObject* curLogic;

	//�ʱ�ȭ
	for (int i = 0; i < lines.size(); i++)
	{
		lines.at(i)->chk = 0;
		lines.at(i)->state = OFF_SIGNAL;
	}
	
	//����Ʈ ,���̺귯�� �ڽ�, �����, �ø��÷�
	for (int i = 0; i < logicInfo.size(); i++)
	{
		curLogic = logicInfo.at(i);
		if (IsInput(curLogic))
		{
			CPoint pos = curLogic->outputCoord[0].first;
			int lin = lines.size();

			for (int j = 0; j < lin; j++)
			{
				LineObject* curline = lines.at(j);
				if (curline->line[0] == pos || curline->line[1] == pos)
				{
					curline->chk = 1;
					curline->state = curLogic->getOutput();
					searchLine.push(curline);
				}
			}
			curLogic->chk = 1;
		}
		else{
			curLogic->chk = 0;
			int out = curLogic->outputNum, in = curLogic->inputNum;

			//�Է��� �ʱ�ȭ
			for (int i = 0; i < in; i++)
				curLogic->inputCoord[i].second = -1;			//input chk �ʱ�ȭ
		}
	}

	////////////////////��� ����/////////////////////
	//�Է� Pin/Clock �� ���õ� ���� �ޱ�
	//��� Pin ���� ���� �����ؾ� �Ѵ�.

	////////////////////���� ����/////////////////////////
	while (!searchLine.empty())
	{
		//(�ʹ�) Pin�� ����� ���� �� ���鼭 �˻��Ѵ�.
		while (!searchLine.empty())
		{
			LineObject* temp_line = searchLine.front();
			searchLine.pop();
			
			/////////����� �� üũ////////////////////////
			int lin = (int)lines.size();
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


			//����Ʈ ,���̺귯�� �ڽ�, �����, �ø��÷�
			for (int i = 0; i < logicInfo.size(); i++)
			{
				LogicObject* curLogic = logicInfo.at(i);
				if (!IsInput(curLogic))
				{
					if (curLogic->chk == 0)
					{
						int ip = curLogic->inputNum;
						for (int j = 0; j < ip; j++)
						{
							if (curLogic->inputCoord[j].first == temp_line->line[0] || curLogic->inputCoord[j].first == temp_line->line[1])
							{
								curLogic->inputCoord[j].second = temp_line->state;
							}
						}

					}
				}
			}
			temp_line->chk = 1;
		}

		/////////////////����Ʈ ��¼� üũ//////////////////////////
		//��ȸ �ؾߵ� LogicObject-> gate, flipflop, Library Box ������. ���� ������Ʈ�� �Է� ���� ��� �湮�Ǿ���? && �̹� �湮�Ͽ��°�?

		for (int i = 0; i < logicInfo.size(); i++)
		{
			LogicObject* curLogic = logicInfo.at(i);
			//Gate �湮
			if (IsGate(curLogic))
			{
				if (curLogic->chk == 0 && curLogic->isInputSet())
				{
					curLogic->chk = 1;
					curLogic->setOutput();

					//�ø��÷� ��¼��� �ΰ�
					for (int j = 0; j < curLogic->outputNum; j++)
					{
						CPoint gatpos = curLogic->outputCoord[j].first;

						int lin = (int)lines.size();
						for (int i = 0; i < lin; i++)
						{
							LineObject* curline = lines.at(i);
							if (curline->line[0] == gatpos || curline->line[1] == gatpos)
							{
								curline->state = curLogic->outputCoord[j].second;

								if (curline->chk != 1)
									searchLine.push(curline);
							}
						}
					}
				}
			}
		}
		
		//Outpin �湮 (���� ������)
		for (int i = 0; i < logicInfo.size(); i++)
		{
			LogicObject* curLogic = logicInfo.at(i);

			if (IsOutput(curLogic))
			{
				//Gate �湮
				if (curLogic->isInputSet() && curLogic->chk == 0)
				{
					curLogic->chk = 1;
					//curLogic->setOutput();
				}
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

	switch (temp.Act)
	{
	case NEW :
		if (temp.Type == LINE)
		{

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
		}
		else{
			temp.logicInfo.push_back(logicInfo.at(logicInfo.size() - 1));
			logicInfo.pop_back();
		}
		break;
	case DELETE :
		
		break;
	case MOVE :
		break;
	case COPY :
		break;
	case PASTE :
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

	switch (temp.Act)
	{
	case NEW:
		if (temp.Type == LINE)
		{
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
		}
		else{
			logicInfo.push_back(temp.logicInfo.at(0));
			temp.logicInfo.pop_back();
		}
		break;
	case DELETE:
		break;
	case MOVE:
		break;
	case COPY:
		break;
	case PASTE:
		break;
	}
	mUndo.AddHead(temp);
}

void CLogicSimulatorDoc::OnFileSave()
{
	CFileDialog dlg(FALSE, L"circ", L"default.circ", OFN_HIDEREADONLY| OFN_OVERWRITEPROMPT, L"LogicSimulator Files (*.circ)|*.circ|All Files (*.*)|*.*|");

	dlg.m_ofn.lpstrTitle = L"������ �����ϼ���.";
	dlg.m_ofn.lStructSize = sizeof(OPENFILENAME) + 12;

	if (dlg.DoModal() == IDOK) {
		CString Temp;
		Temp.Format(L"%s", dlg.GetPathName());

		HANDLE hFile;
		DWORD dwRead;
		DWORD dwWritten;

		//���� ���� ����
		CString con;

		hFile = CreateFile(Temp, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

		WriteFile(hFile, L"FWEFEW", sizeof(int), &dwRead, NULL);

		CloseHandle(hFile);

		//CString Temp;
		//Temp.Format(_T("%s"), dlg.GetPathName());

		CFile  file;
		// ������ ������� ����.
		if (file.Open(Temp, CFile::modeCreate | CFile::modeWrite)) {
			// ���������� ���� ������ CArchive �� �����ϱ� ���� �����Ѵ�.
			CArchive ar(&file, CArchive::store);

			try {
				Serialize(ar);
			}
			catch (CFileException *fe) {
				// ���ܰ� �߻��ϸ� �޼����ڽ��� ���Ͽ� ����ڿ��� �˸���.
				fe->ReportError();
			}
			catch (CArchiveException *ae) {
				// ���ܰ� �߻��ϸ� �޼����ڽ��� ���Ͽ� ����ڿ��� �˸���.
				ae->ReportError();
			}
			// CArchive �� �ݴ´�.
			ar.Close();
			// ������ �ݴ´�.
			file.Close();
		}
	}

}


void CLogicSimulatorDoc::OnFileOpen()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
	CFileDialog dlg(FALSE, L"circ", L"default.circ", OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, L"LogicSimulator Files (*.circ)|*.circ|All Files (*.*)|*.*|");
	dlg.m_ofn.lpstrTitle = L"������ �ҷ�������.";
	dlg.m_ofn.lStructSize = sizeof(OPENFILENAME) + 12;

	if (dlg.DoModal() == IDOK)
	{
		CString Temp;
		Temp.Format(L"%s", dlg.GetPathName());
		clearAll();

		CFile  file;
		if (file.Open(Temp, CFile::modeRead | CFile::shareExclusive)) {
			// ���������� ���� ������ CArchive �� �����ϱ� ���� �����Ѵ�.
			CArchive ar(&file, CArchive::load | CArchive::bNoFlushOnDelete);

			try {
				Serialize(ar);
			}
			catch (CFileException *fe) {
				// ���ܰ� �߻��ϸ� �޼����ڽ��� ���Ͽ� ����ڿ��� �˸���.
				fe->ReportError();
			}
			catch (CArchiveException *ae) {
				// ���ܰ� �߻��ϸ� �޼����ڽ��� ���Ͽ� ����ڿ��� �˸���.
				ae->ReportError();
			}
			// CArchive �� �ݴ´�.
			ar.Close();
			// ������ �ݴ´�.
			file.Close();
		}
	}

}


void CLogicSimulatorDoc::OnFileNew()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
	int Answer;
	Answer = AfxMessageBox(_T("���� �Ͻðڽ��ϱ�?"), MB_YESNOCANCEL);
	switch (Answer)
	{
	case IDYES:
		OnFileSave();
		clearAll();
		break;
	case IDNO :
		clearAll();
		break;
	case IDCANCEL :
		break;
	}
}

void CLogicSimulatorDoc::clearAll()
{
	lines.clear();
	logicInfo.clear();
	mUndo.RemoveAll();
	mRedo.RemoveAll();
}