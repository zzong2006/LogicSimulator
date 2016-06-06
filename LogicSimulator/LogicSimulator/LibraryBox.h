#pragma once

#include "LogicObject.h"
#include "Clock.h"
#include "FlipFlop.h"
#include "andGate.h"
#include "orGate.h"
#include "xorGate.h"
#include "norGate.h"
#include "nandGate.h"
#include "notGate.h"
#include "Gate.h"
#include "Pin.h"
#include "Out.h"
#include "name_repo.h"
#include "Sevenseg.h"

class Action
{
public:
	Action(int type, int act)
	{
		Type = type;
		Act = act;
		if (type == LINE)
			line_num = 2;

	}
	Action()
	{

	}
	enum {
		line_add
	};
	int Act, Type;
	int line_num;


	std::vector <LineObject *> lineked_line;

	std::vector <LogicObject *> logicInfo;
	std::vector <Sevenseg *> segInfo;
	std::vector <LineObject *> lines;
	std::vector <int> lineIndex;
	std::vector <int> logicIndex;
};

class LibraryBox : public LogicObject
{
public:
	CString selectedType;
	BOOL isSelected;
	BOOL CanBeDivided;		//분기 가능상태면 동그라미
	BOOL clickMode;
	BOOL selectMode;
	BOOL simulateMode;
	BOOL isOnFocus;			//선택 상태 모서리에 네모
	std::vector <LogicObject *> currObject;
	std::vector <LogicObject *> logicInfo;
	std::vector <LineObject *> lines;

	int objectType;
	int objectName;

	LogicObject* temp;

	// undo/redo
	CList <Action> mUndo;
	CList <Action> mRedo;


	//method
	void CheckCircuit();
	void LineCheck();
	void draw_shadow(Gdiplus::Graphics* gp, Gdiplus::Pen * p);
	void draw_main(Gdiplus::Graphics* gp);
	void set_Coord_From_outC(int x, int y);

	BOOL IsInput(LogicObject* lo);
	BOOL IsGate(LogicObject* lo);
	BOOL IsOutput(LogicObject* lo);

	BOOL IsConflict(LineObject* st, LineObject* ed, CPoint& pos);
	BOOL IsConnect(CPoint p, LineObject* ed);


	BOOL CanUndo();
	BOOL CanRedo();
	void Undo();
	void Redo();

	LibraryBox();
	~LibraryBox();
};