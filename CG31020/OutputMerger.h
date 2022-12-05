#pragma once
#include "PipelineStruct.h"
#include <vector>

using std::vector;

class OutputMerger
{
public:
	OutputMerger(int width, int height);
	OutputMerger(OutputMerger&&) = delete;
	OutputMerger(const OutputMerger&) = delete;
	OutputMerger& operator=(const OutputMerger&) = delete;
	virtual ~OutputMerger();

	void ClearZBuffer();
	void Resize(int width, int height);
	vector<PixelOut>& Merge(vector<PixelOut>& pixels);
protected:
	void InitZBuffer();

	int mWidth;
	int mHeight;

	double* mZBuffer;
	vector<PixelOut> mPixels;
};

