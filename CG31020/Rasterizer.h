#pragma once
#include "Vec.h"
#include "Mat.h"
#include "PipelineStruct.h"
#include <vector>

using std::vector;

class Edge
{
public:
	double a, b, c;
};

class Triangle
{
public:
	enum
	{
		INSIDE,OUTSIDE,INTERSECTING
	};

	Triangle(const VertexOut& _v1, const VertexOut& _v2, const VertexOut& _v3);
	Triangle(Triangle&&) = delete;
	Triangle(const Triangle&) = delete;
	Triangle& operator=(const Triangle&) = delete;

	bool IsFrontFace();
	bool InTriangle(const Vec& pos)const;
	int AabbIntersectionTest(const Vec& center, const Vec& extent)const;
	Vec GetPerspectiveCorrectUV(const Vec& pos)const;

protected:
	int AabbIntersectionTest(const Vec& center, const Vec& extent, const Edge& edge)const;

public:
	const VertexOut& v1;
	const VertexOut& v2;
	const VertexOut& v3;

	Vec posScreen1;
	Vec posScreen2;
	Vec posScreen3;

	Edge e1;
	Edge e2;
	Edge e3;
};

class Rasterizer
{
public:
	Rasterizer(int width, int height);
	Rasterizer(Rasterizer&&) = delete;
	Rasterizer(const Rasterizer&) = delete;
	Rasterizer& operator=(const Rasterizer&) = delete;

	void Resize(int width, int height);
	vector<VertexOut>& Rasterize(vector<VertexOut>& vertices, const vector<int>& indices);
protected:
	void SetScreenTransformation();
	void TriangleTraversal(const Triangle& triangle);
	void Interpolate(const Triangle& triangle, const Vec& pos);

	int mWidth;
	int mHeight;

	Mat mScreenTransformation;
	Mat mInvScreenTransformation;

	vector<VertexOut> mPixels;
};



