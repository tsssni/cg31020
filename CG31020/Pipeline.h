#pragma once
#include "InputAssembler.h"
#include "VertexShader.h"
#include "Rasterizer.h"
#include "PixelShader.h"
#include "OutputMerger.h"

class Pipeline
{
public:
	Pipeline(int width, int height);
	Pipeline(Pipeline&&) = delete;
	Pipeline(const Pipeline&) = delete;
	Pipeline& operator=(const Pipeline&) = delete;

	vector<PixelOut>& Render(vector<Vertex>& vertices, vector<int>& indices, PassConstants& pc, MeshConstants& mc);
	void Resize(int width, int height);
	void ClearZBuffer();
protected:
	InputAssembler mInputAssembler;
	VertexShader mVertexShader;
	Rasterizer mRasterizer;
	PixelShader mPixelShader;
	OutputMerger mOutputMerger;

};

