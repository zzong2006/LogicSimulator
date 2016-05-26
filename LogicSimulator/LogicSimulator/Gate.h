#pragma once

#include <gdiplus.h>
#include "LogicObject.h"
#include "LineObject.h"

class Gate : public LogicObject
{

protected:
	int input_state[INPUT_SIZE], output_state;

public:
	LineObject* connect_line;
	Gate();
	virtual bool calOutput() = 0;	//결과 생성
	virtual ~Gate();

};

