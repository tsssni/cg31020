#pragma once
#include "PipelineStruct.h"
#include <vector>

using std::vector;

class InputAssembler
{
public:
	vector<Vertex>& Assemble(vector<Vertex>& vertices);
};

