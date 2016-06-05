#pragma once
#include "LogicObject.h"

class FlipFlop : public LogicObject
{

protected:
	BOOL UpTrigger;		//�ʱ� ���� ��¿���
public:
	BOOL CurrClock , prevOutput[2];		//�ʱ� Ŭ�� ����
	
	BOOL GetTrigger() { return UpTrigger; };
	void SetTrigger(CString input);
	int CheckTrigger();
	void setOutput();
	virtual void set_Coord_From_outC(int x, int y);
	FlipFlop();
	~FlipFlop();
};

