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

class Store
{
public:
	std::vector <int> type, name;
	int logicNum;
	CPoint dec;
	std::vector <LineObject > lines;
};


class LibraryBox 
{
public:
	int ID;						//서브인가 메인인가 구분함
	int NumInput, NumOuput;	//출력핀과 입력핀의 개수
	BOOL ConnInput[10], ConnOutput[10];
	//라이브러리의 입출력이 연결되있는가?
	BOOL CanBeDivided;		//분기 가능상태면 동그라미
	BOOL isOnFocus;			//선택 상태 모서리에 네모
	std::vector <LogicObject *> currObject;
	std::vector <LogicObject *> logicInfo;
	std::vector <LineObject *> lines;

	LogicObject* temp;
	Store store;
	//method
	void CheckCircuit();

	int FindEmpty(int type);	//라이브러리 입출력 빈자리 찾아주기
	void LineCheck();

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