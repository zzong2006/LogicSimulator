#pragma once
#include <gdiplus.h>

class Gate
{

protected:
	POINT inputCoord[2],outputCoord;	
	bool input[2], output;
	int inputNum;			//입력선 개수
	int direct;				//게이트 방향
	CString label;

public:
	Gate();
	virtual void draw(Graphics* gp, Pen* p) = 0;
	virtual void turn() = 0;
	virtual bool output() = 0;	//결과 생성

	void addLabel (CString input);				//라벨 이름
	void set_outputCoord(int x, int y);
	virtual ~Gate();

};

