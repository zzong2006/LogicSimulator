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

void LibraryBox::CheckCircuit()
{
	std::queue <LineObject *> searchLine;
	LogicObject* curLogic;

	//초기화
	for (int i = 0; i < lines.size(); i++)
	{
		lines.at(i)->chk = 0;
		lines.at(i)->state = OFF_SIGNAL;
	}

	//게이트 ,라이브러리 박스, 출력핀, 플립플롭
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

			//입력핀 초기화
			for (int i = 0; i < in; i++)
				curLogic->inputCoord[i].second = -1;			//input chk 초기화
		}
	}


	////////////////////출력 시작/////////////////////
	//입력 Pin/Clock 과 관련된 값만 받기
	//출력 Pin 같은 경우는 제외해야 한다.

	////////////////////돌고 돌아/////////////////////////
	while (!searchLine.empty())
	{
		//(초반) Pin에 연결된 선을 다 돌면서 검사한다.
		while (!searchLine.empty())
		{
			LineObject* temp_line = searchLine.front();
			searchLine.pop();

			/////////연결된 선 체크////////////////////////
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


			//게이트 ,라이브러리 박스, 출력핀, 플립플롭
			
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

		/////////////////게이트 출력선 체크//////////////////////////
		//순회 해야될 LogicObject-> gate, flipflop, Library Box 세가지. 로직 오브젝트의 입력 선이 모두 방문되었나? && 이미 방문하였는가?

		for (int i = 0; i < logicInfo.size(); i++)
		{
			LogicObject* curLogic = logicInfo.at(i);
			//Gate & FlipFlop 방문
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

					//플립플롭 출력선은 두개
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

		//Outpin 방문 (제일 마지막)
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
	int ln = temp.lines.size();

	switch (temp.Act)
	 {
		case NEW:
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
			else {
				temp.logicInfo.push_back(logicInfo.at(logicInfo.size() - 1));
				logicInfo.pop_back();
			}
			break;
		case REMOVE:
			for (int i = 0; i < temp.lineIndex.size(); i++)
			{
				lines.insert(lines.begin() + temp.lineIndex.at(i), temp.lines.at(i));
			}
			for (int i = 0; i < temp.logicIndex.size(); i++)
			{
				logicInfo.insert(logicInfo.begin() + temp.logicIndex.at(i), temp.logicInfo.at(i));
			}
			break;
		case MOVE:
			break;
		case COPY:
			for (int i = 0; i < temp.lines.size(); i++)
				lines.pop_back();

			for (int i = 0; i < temp.logicInfo.size(); i++)
				logicInfo.pop_back();
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
	int ln = temp.lines.size();
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
			else {
				logicInfo.push_back(temp.logicInfo.at(0));
				temp.logicInfo.pop_back();
				
			}
		break;
		case REMOVE:
			for (int i = 0; i < temp.lineIndex.size(); i++)
			{
				lines.erase(lines.begin() + temp.lineIndex.at(i));
			}
			for (int i = 0; i < temp.logicIndex.size(); i++)
			{
				logicInfo.erase(logicInfo.begin() + temp.logicIndex.at(i));
			}
		break;
		case MOVE:

		break;

		case COPY:
			for (int i = 0; i < temp.lines.size(); i++)
				lines.push_back(temp.lines.at(i));

			for (int i = 0; i < temp.logicInfo.size(); i++)
				logicInfo.push_back(temp.logicInfo.at(i));
		break;
		}
	mUndo.AddHead(temp);
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