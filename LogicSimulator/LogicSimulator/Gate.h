#pragma once

#include <gdiplus.h>
#include "LogicObject.h"
#include "LineObject.h"

class Gate : public LogicObject
{

protected:
	int input_state[INPUT_SIZE], output_state;

public:
	//LineObject* connect_line;
	Gate();
	Gate(int dec_x, int dec_y);
	virtual bool calOutput() = 0;	//결과 생성
	virtual ~Gate();
	virtual void draw_shadow(Gdiplus::Graphics* gp, Gdiplus::Pen * p) = 0;
	virtual void draw_main(Gdiplus::Graphics* gp) = 0;
	virtual void turn() = 0;
	virtual void set_Coord_From_outC(int x, int y) = 0;
	int getOutput();

};

