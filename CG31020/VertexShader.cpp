#include "pch.h"
#include "VertexShader.h"

vector<VertexOut>& VertexShader::VerticesProcess(vector<Vertex>& vertices, PassConstants& pc, MeshConstants& mc)
{
	mVertices.clear();

	for (auto& vertex : vertices)
	{
		VertexOut vout;
		vout.posW = VecMul(vertex.pos, mc.world);
		vout.posH = VecMul(vout.posW, pc.viewProj);
		vout.texC = vertex.texC;

		mVertices.push_back(vout);
	}

	return mVertices;
}
