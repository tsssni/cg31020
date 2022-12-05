#pragma once
#include "PipelineStruct.h"
#include <vector>

using std::vector;

class VertexShader
{
public:
	vector<VertexOut>& VerticesProcess(vector<Vertex>& vertices, PassConstants& pc, MeshConstants& mc);
protected:
	vector<VertexOut> mVertices;
};

