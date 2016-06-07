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
	int ID;						//�����ΰ� �����ΰ� ������
	int NumInput, NumOuput;	//����ɰ� �Է����� ����
	BOOL ConnInput[10], ConnOutput[10];
	//���̺귯���� ������� ������ִ°�?
	BOOL CanBeDivided;		//�б� ���ɻ��¸� ���׶��
	BOOL isOnFocus;			//���� ���� �𼭸��� �׸�
	std::vector <LogicObject *> currObject;
	std::vector <LogicObject *> logicInfo;
	std::vector <LineObject *> lines;

	LogicObject* temp;
	Store store;
	//method
	void CheckCircuit();

	int FindEmpty(int type);	//���̺귯�� ����� ���ڸ� ã���ֱ�
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