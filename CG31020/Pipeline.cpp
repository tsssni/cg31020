#include "pch.h"
#include "Pipeline.h"

Pipeline::Pipeline(int width, int height)
    :mRasterizer(width, height), mOutputMerger(width, height)
{
}

vector<PixelOut>& Pipeline::Render(vector<Vertex>& vertices, vector<int>& indices, PassConstants& pc, MeshConstants& mc)
{
    auto& iaOut = mInputAssembler.Assemble(vertices);
    auto& vsOut = mVertexShader.VerticesProcess(iaOut, pc, mc);
    auto& rsOut = mRasterizer.Rasterize(vsOut, indices);
    auto& psOut = mPixelShader.PixelsProcess(rsOut, pc, mc);
    auto& omOut = mOutputMerger.Merge(psOut);

    return omOut;
}

void Pipeline::Resize(int width, int height)
{
    mRasterizer.Resize(width, height);
    mOutputMerger.Resize(width, height);
}

void Pipeline::ClearZBuffer()
{
    mOutputMerger.ClearZBuffer();
}
