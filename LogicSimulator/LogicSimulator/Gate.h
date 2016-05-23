#pragma once

#include <gdiplus.h>
#include "LogicObject.h"

class Gate : public LogicObject
{

protected:
	

public:
	Gate();
	virtual bool calOutput() = 0;	//결과 생성
	virtual ~Gate();

};

