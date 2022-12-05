#pragma once
#include "PipelineStruct.h"
#include <vector>

using std::vector;

class PixelShader
{
public:
	vector<PixelOut>& PixelsProcess(const vector<VertexOut>& pixels, PassConstants& pc, MeshConstants& mc);
protected:
	vector<PixelOut> mPixels;
};

