#include "pch.h"
#include "OutputMerger.h"

OutputMerger::OutputMerger(int width, int height)
    :mWidth(width), mHeight(height)
{
    InitZBuffer();
}

OutputMerger::~OutputMerger()
{
    delete[] mZBuffer;
}

void OutputMerger::ClearZBuffer()
{
    for (int i = 0; i < mWidth * mHeight; ++i)
    {
        mZBuffer[i] = 1.0;
    }
}

void OutputMerger::Resize(int width, int height)
{
    mWidth = width;
    mHeight = height;
    delete[] mZBuffer;
    InitZBuffer();
}

vector<PixelOut>& OutputMerger::Merge(vector<PixelOut>& pixels)
{
    auto inScreen = [&](Vec& pos)
    {
        return (pos.x >= 0 && pos.x < mWidth) && (pos.y >= 0 && pos.y < mHeight);
    };

    auto getPos = [&](Vec& pos)
    {
        int i = pos.x;
        int j = pos.y;
        return i + j * mWidth;
    };

    mPixels.clear();

    for (auto& pixel : pixels)
    {
        if (!inScreen(pixel.pos))
        {
            continue;
        }

        int pos = getPos(pixel.pos);

        if (pixel.pos.z < mZBuffer[pos])
        {
            mZBuffer[pos] = pixel.pos.z;
        }
    }

    for (auto& pixel : pixels)
    {
        if (!inScreen(pixel.pos))
        {
            continue;
        }

        int pos = getPos(pixel.pos);

		if (pixel.pos.z == mZBuffer[pos])
        {
            mPixels.push_back(pixel);
        }
    }

    return mPixels;
}

void OutputMerger::InitZBuffer()
{
    mZBuffer = new double[mWidth * mHeight];
    ClearZBuffer();
}
