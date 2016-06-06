#include "stdafx.h"
#include "LibraryBox.h"
#include <queue>
#include <math.h>

LibraryBox::LibraryBox()
{
	temp = NULL;
	clickMode = false;
	selectMode = true;
	simulateMode = false;
	CanBeDivided = false;
	isOnFocus = false;
}


LibraryBox::~LibraryBox()
{
	for (int i = 0; i < logicInfo.size(); i++)
		delete logicInfo.at(i);
	for (int i = 0; i < lines.size(); i++)
		delete lines.at(i);
}

void LibraryBox::CheckCircuit()
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
		else {
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
			//Gate & FlipFlop �湮
			if (IsGate(curLogic))
			{
				if (curLogic->chk == 0 && curLogic->isInputSet())
				{
					curLogic->chk = 1;
					if (curLogic->objectType == FLIPFLOP_TYPE) {
						FlipFlop* fftemp = (FlipFlop *)curLogic;
						fftemp->setOutput();
					}
					else {
						curLogic->setOutput();
					}

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
					if (curLogic->objectName == SEG7) {
						Sevenseg* curSeg = (Sevenseg *)curLogic;
						curSeg->setOutput();
					}
					else {
						curLogic->setOutput();
					}

				}
			}
		}
	}
}

void LibraryBox::draw_shadow(Gdiplus::Graphics * gp, Gdiplus::Pen * p)
{
	
}

void LibraryBox::draw_main(Gdiplus::Graphics * gp)
{

}

void LibraryBox::set_Coord_From_outC(int x, int y)
{

}

BOOL LibraryBox::IsInput(LogicObject * lo)
{
	return lo->objectName == PIN || lo->objectName == CLOCK;
}

BOOL LibraryBox::IsGate(LogicObject * lo)
{
	return lo->objectType == GATE_TYPE || lo->objectType == FLIPFLOP_TYPE;
}

BOOL LibraryBox::IsOutput(LogicObject * lo)
{
	return lo->objectName == OUTPIN || lo->objectName == SEG7;
}

BOOL LibraryBox::CanUndo()
{
	while (mUndo.GetCount() > MAX_UNDO)
	{
		mUndo.RemoveTail();
	}
	return (mUndo.GetCount() > 0);
}

BOOL LibraryBox::CanRedo()
{
	while (mRedo.GetCount() > MAX_UNDO)
	{
		mRedo.RemoveTail();
	}
	return (mRedo.GetCount() > 0);
}

void LibraryBox::Undo()
{
	Action temp;
	CPoint stp, edp;

	temp = mUndo.RemoveHead();
	int lkedn = temp.lineked_line.size();
	int line_num = temp.lineIndex.size(), object_num = temp.logicIndex.size();

	switch (temp.Act)
	{
	case NEW:
			temp.logicInfo.push_back(logicInfo.at(logicInfo.size() - 1));
			logicInfo.pop_back();
			LineCheck();
		break;
	case REMOVE:

		for (int i = 0; i < line_num; i++)
		{
			lines.insert(lines.begin() + temp.lineIndex.at(i), temp.lines.at(i));
		}
		for (int i = 0; i < object_num; i++)
		{
			logicInfo.insert(logicInfo.begin() + temp.logicIndex.at(i), temp.logicInfo.at(i));
		}

		break;
	case MOVE:
		break;
	case COPY:
		break;
	case PASTE:
		break;
	}
	mRedo.AddHead(temp);
}

void LibraryBox::Redo()
{
	Action temp;
	CPoint stp, edp;

	temp = mRedo.RemoveHead();
	int lkedn = temp.lineked_line.size();
	int line_num = temp.lineIndex.size(), object_num = temp.logicIndex.size();

	switch (temp.Act)
	{
	case NEW:
		logicInfo.push_back(temp.logicInfo.at(0));
		LineCheck();
		temp.logicInfo.pop_back();
		break;
	case REMOVE:
		for (int i = 0; i < line_num; i++)
		{
			lines.erase(lines.begin() + temp.lineIndex.at(i));
		}
		for (int i = 0; i < object_num; i++)
		{
			logicInfo.erase(logicInfo.begin() + temp.logicIndex.at(i));
		}
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


void  LibraryBox::LineCheck()
{
	for (int i = 0; i < lines.size(); i++)
	{
		LineObject* stline = lines.at(i);
		vector<int> sp, ep, erase;
		CPoint temp, edge;
		int ed1 = -1, ed2;
		int cur_shape = stline->shape;

		for (int j = 0; j < lines.size(); j++)
		{
			if (i == j) continue;

			LineObject* edline = lines.at(j);
			CPoint conpos;
			if (IsConflict(stline, edline, conpos))
			{
				LineObject* newline = new LineObject(conpos.x, conpos.y);
				newline->line[1] = stline->line[1];
				if (newline->line[0].x == newline->line[1].x) newline->shape = VERTICAL;
				else newline->shape = HORIZONTAL;
				stline->line[1] = conpos;
				lines.insert(lines.begin() + j, newline);
			}
			if (IsConnect(stline->line[0], edline)) 
				sp.push_back(j);
			if (IsConnect(stline->line[1], edline)) 
				ep.push_back(j);
		}

		BOOL connect = FALSE;
		edge = stline->line[0];
		for (int j = 0; j < sp.size(); j++)
		{
			if (stline->shape != lines.at(sp.at(j))->shape)
			{
				connect = FALSE;
				break;
			}
			else{
				connect = TRUE;
				if (lines.at(sp.at(j))->line[0] == stline->line[0])
					temp = lines.at(sp.at(j))->line[1];
				else temp = lines.at(sp.at(j))->line[0];

				if (cur_shape == VERTICAL)
				{
					if (abs(temp.y - stline->line[0].y) > abs(edge.y - stline->line[0].y))
					{
						ed1 = sp.at(j);
						edge = temp;
					}
				}
				else
				{
					if (abs(temp.x - stline->line[0].x) > abs(edge.x - stline->line[0].x))
					{
						ed1 = sp.at(j);
						edge = temp;
					}
				}
			}
		}

		if (connect)
		{
			stline->line[0] = edge;
			lines.erase(lines.begin() + ed1);
		}

		connect = FALSE;
		edge = stline->line[1];
		for (int j = 0; j < ep.size(); j++)
		{
			if (stline->shape != lines.at(ep.at(j))->shape)
			{
				connect = FALSE;
				break;
			}
			else
			{
				connect = TRUE;
				if (lines.at(ep.at(j))->line[0] == stline->line[1])
					temp = lines.at(ep.at(j))->line[1];
				else temp = lines.at(ep.at(j))->line[0];

				if (cur_shape == VERTICAL)
				{
					if (abs(temp.y - stline->line[1].y) > abs(edge.y - stline->line[1].y))
					{
						ed2 = ep.at(j);
						edge = temp;
					}
				}
				else
				{
					if (abs(temp.x - stline->line[1].x) > abs(edge.x - stline->line[1].x))
					{
						ed2 = ep.at(j);
						edge = temp;
					}
				}
			}
		}

		if (connect)
		{
			stline->line[1] = edge;
			if (ed1 != ed2)
			{
				lines.erase(lines.begin() + ed2);
			}
		}
	}
}

BOOL LibraryBox::IsConflict(LineObject* st, LineObject* ed , CPoint& pos)
{
	if (st->shape != ed->shape)
	if (st->Is_match_IineCoord(ed->line[0]))
	{
		pos = ed->line[0];
		return (st->line[0] != ed->line[0] && st->line[0] != ed->line[1] && st->line[1] != ed->line[0] && st->line[1] != ed->line[1]);
	}
	else if (st->Is_match_IineCoord(ed->line[1]))
	{
		pos = ed->line[1];
		return (st->line[0] != ed->line[0] && st->line[0] != ed->line[1] && st->line[1] != ed->line[0] && st->line[1] != ed->line[1]);
	}

	return FALSE;
}

BOOL LibraryBox::IsConnect(CPoint p, LineObject* ed)
{
	if (p == ed->line[0] || p == ed->line[1])
		return TRUE;
	return FALSE;
}