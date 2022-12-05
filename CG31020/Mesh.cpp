#include "pch.h"
#include "Mesh.h"

Vec RandColor()
{
	int r = rand() % 256;
	int g = rand() % 256;
	int b = rand() % 256;

	return { (double)r, (double)g, (double)b };
}

Mesh::~Mesh()
{
}

vector<Vertex>& Mesh::GetVertices()
{
    return mVertices;
}

vector<int>& Mesh::GetIndices()
{
    return mIndices;
}

void Mesh::SetMeshConstants(const MeshConstants& mc)
{
    mMeshConstants = mc;
}

MeshConstants& Mesh::GetMeshConstants()
{
    return mMeshConstants;
}

Box::Box(double width, double height, double depth)
{
    // 顶点的世界坐标均由边长的一半构成
	// 故先除2来方便后续处理

	double w2 = 0.5f * width;
	double h2 = 0.5f * height;
	double d2 = 0.5f * depth;

	// 由于需要为每个面赋不同的颜色
	// 若只保存6个顶点则无法通过插值完成该工作
	// 故为每个面都保存4个顶点，共24个顶点
	Vertex v[24];

	// 前面顶点
	v[0] = { { -w2, -h2, -d2, 1.f }, {0.0,1.0} };
	v[1] = { { -w2, +h2, -d2, 1.f }, {0.0,0.0} };
	v[2] = { { +w2, +h2, -d2, 1.f }, {1.0,0.0} };
	v[3] = { { +w2, -h2, -d2, 1.f }, {1.0,1.0} };

	// 后面顶点
	v[4] = { { -w2, -h2, +d2, 1.f }, {1.0,1.0} };
	v[5] = { { +w2, -h2, +d2, 1.f }, {0.0,1.0} };
	v[6] = { { +w2, +h2, +d2, 1.f }, {0.0,0.0} };
	v[7] = { { -w2, +h2, +d2, 1.f }, {1.0,0.0} };

	// 顶面顶点
	v[8] = { { -w2, +h2, -d2, 1.f }, {0.0,1.0} };
	v[9] = { { -w2, +h2, +d2, 1.f }, {0.0,0.0} };
	v[10] = { { +w2, +h2, +d2, 1.f }, {1.0,0.0} };
	v[11] = { { +w2, +h2, -d2, 1.f }, {1.0,1.0} };

	// 底面顶点
	v[12] = { { -w2, -h2, -d2, 1.f }, {1.0,1.0} };
	v[13] = { { +w2, -h2, -d2, 1.f }, {0.0,1.0} };
	v[14] = { { +w2, -h2, +d2, 1.f }, {0.0,0.0} };
	v[15] = { { -w2, -h2, +d2, 1.f }, {1.0,0.0} };

	// 左面顶点
	v[16] = { { -w2, -h2, +d2, 1.f }, {0.0,1.0} };
	v[17] = { { -w2, +h2, +d2, 1.f }, {0.0,0.0} };
	v[18] = { { -w2, +h2, -d2, 1.f }, {1.0,0.0} };
	v[19] = { { -w2, -h2, -d2, 1.f }, {1.0,1.0} };

	// 右面顶点
	v[20] = { { +w2, -h2, -d2, 1.f }, {0.0,1.0} };
	v[21] = { { +w2, +h2, -d2, 1.f }, {0.0,0.0} };
	v[22] = { { +w2, +h2, +d2, 1.f }, {1.0,0.0} };
	v[23] = { { +w2, -h2, +d2, 1.f }, {1.0,1.0} };

	mVertices.assign(&v[0], &v[24]);

	// 创建顶点索引

	int i[36];

	// 前面索引
	i[0] = 0; i[1] = 1; i[2] = 2;
	i[3] = 0; i[4] = 2; i[5] = 3;

	// 后面索引
	i[6] = 4; i[7] = 5; i[8] = 6;
	i[9] = 4; i[10] = 6; i[11] = 7;

	// 顶面索引
	i[12] = 8; i[13] = 9; i[14] = 10;
	i[15] = 8; i[16] = 10; i[17] = 11;

	// 底面索引
	i[18] = 12; i[19] = 13; i[20] = 14;
	i[21] = 12; i[22] = 14; i[23] = 15;

	// 左面索引
	i[24] = 16; i[25] = 17; i[26] = 18;
	i[27] = 16; i[28] = 18; i[29] = 19;

	// 右面索引
	i[30] = 20; i[31] = 21; i[32] = 22;
	i[33] = 20; i[34] = 22; i[35] = 23;

	mIndices.assign(&i[0], &i[36]);
}

Box::Box(const Box& box)
{
    *this = box;
}

Box& Box::operator=(const Box& box)
{
    mVertices = box.mVertices;
    mIndices = box.mIndices;

	return *this;
}

Box::~Box()
{
}
