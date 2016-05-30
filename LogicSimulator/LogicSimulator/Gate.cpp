#include "stdafx.h"
#include "Gate.h"
#include "LogicSimulator.h"

Gate::Gate()
{
	outputCoord.x = 0;
	outputCoord.y = 0;

	//test input_size
	for (int i = 0; i < INPUT_SIZE; i++)
	{
		input_state[i] = INPUT_SIGNAL;
	}

	output_state = OUTPUT_SIGNAL;
	type.SetString(_T("Gate"));
}

Gate::~Gate()
{

}
