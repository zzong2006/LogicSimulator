#pragma once
#include "LogicObject.h"

class FlipFlop : public LogicObject
{

protected:
	BOOL UpTrigger;		//초기 설정 상승엣지
public:
	BOOL CurrClock , prevOutput[2];		//초기 클럭 변수
	
	BOOL GetTrigger() { return UpTrigger; };
	void SetTrigger(CString input);
	int CheckTrigger();
	void setOutput();
	virtual void set_Coord_From_outC(int x, int y);
	FlipFlop();
	~FlipFlop();
};

