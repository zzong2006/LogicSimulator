#pragma once
#include "stdafx.h"
#include "LibraryBox.h"
#include "Box.h"
#include <queue>
#include <math.h>
#include "LogicSimulatorDoc.h"
LibraryBox::LibraryBox()
{
	temp = NULL;
	CanBeDivided = false;
	isOnFocus = false;
	NumOuput = NumInput = 0;

	for (int i = 0; i < 10; i++)
	{
		ConnInput[i] = ConnOutput[i] = FALSE;
	}
}


LibraryBox::~LibraryBox()
{
	for (int i = 0; i < logicInfo.size(); i++)
		delete logicInfo.at(i);
	for (int i = 0; i < lines.size(); i++)
		delete lines.at(i);
}

void  LibraryBox::LineCheck()
{
	bool merge_flag;
	for (int i = 0; i < lines.size(); i++)
	{
		LineObject* stline = lines.at(i);
		vector<int> sp, ep, erase;
		CPoint temp, edge;
		int ed1 = -1, ed2;
		int cur_shape = stline->shape;

		merge_flag = TRUE;

		for (int j = 0; j < lines.size(); j++)
		{
			if (i != j)
			{
				LineObject* edline = lines.at(j);
				CPoint conpos;
				if (IsConflict(stline, edline, conpos))
				{
					merge_flag = FALSE;
					LineObject* newline = new LineObject(conpos.x, conpos.y);
					newline->line[0] = stline->line[0];

					if (newline->line[0].x == newline->line[1].x) newline->shape = VERTICAL;
					else newline->shape = HORIZONTAL;

					stline->line[0] = conpos;
					lines.insert(lines.begin() + j, newline);
				}
				if (IsConnect(stline->line[0], edline))
					sp.push_back(j);
				if (IsConnect(stline->line[1], edline))
					ep.push_back(j);
			}
		}
	}

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
		if (IsInput(curLogic))					//isinput : �Է��� �Ǵ� Ŭ���ΰ�?
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
	int level = 0;
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
						curline->chk = level;
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
					if (curLogic->chk == level)
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
					else if (curLogic->objectType == LIB) {
						Box* Bxtemp = (Box *)curLogic;
						Bxtemp->setOutput();
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

int LibraryBox::FindEmpty(int type)
{
	if (!type)			//input
	{
		for (int i = 0; i < (NumInput > 10? 10 : NumInput); i++)
		{
			if (!ConnInput[i]) {
				ConnInput[i] = TRUE;
				return i;
			}
		}
	}
	else {
		for (int i = 0; i < (NumOuput > 10 ? 10 : NumOuput); i++)
		{
			if (!ConnOutput[i]) {
				ConnOutput[i] = TRUE;
				return i;
			}
		}
	}
	return -1;
}


BOOL LibraryBox::IsInput(LogicObject * lo)
{
	return lo->objectName == PIN || lo->objectName == CLOCK;
}

BOOL LibraryBox::IsGate(LogicObject * lo)
{
	return lo->objectType == GATE_TYPE || lo->objectType == FLIPFLOP_TYPE || lo->objectType == LIB;
}

BOOL LibraryBox::IsOutput(LogicObject * lo)
{
	return lo->objectName == OUTPIN || lo->objectName == SEG7;
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