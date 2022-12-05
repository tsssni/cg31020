#include "pch.h"
#include "Rasterizer.h"

Rasterizer::Rasterizer(int width, int height)
	:mWidth(width), mHeight(height)
{
	SetScreenTransformation();
}

void Rasterizer::Resize(int width, int height)
{
	mWidth = width;
	mHeight = height;
	SetScreenTransformation();
}

vector<VertexOut>& Rasterizer::Rasterize(vector<VertexOut>& vertices, const vector<int>& indices)
{
	mPixels.clear();

	for (int i = 0; i < indices.size(); i += 3)
	{
		VertexOut& v1 = vertices[indices[i]];
		VertexOut& v2 = vertices[indices[i + 1]];
		VertexOut& v3 = vertices[indices[i + 2]];

		Triangle triangle(v1, v2, v3);
		triangle.posScreen1 = VecMul(v1.posH / v1.posH.w, mScreenTransformation);
		triangle.posScreen2 = VecMul(v2.posH / v2.posH.w, mScreenTransformation);
		triangle.posScreen3 = VecMul(v3.posH / v3.posH.w, mScreenTransformation);

		if (!triangle.IsFrontFace())
		{
			continue;
		}

		TriangleTraversal(triangle);
	}

	return mPixels;
}

void Rasterizer::SetScreenTransformation()
{
	mScreenTransformation =
	{
		0.5f,0.f,0.f,0.f,
		0.f,-0.5f,0.f,0.f,
		0.f,0.f,1.f,0.f,
		0.5f,0.5f,0.f,1.f
	};

	mScreenTransformation *= MatScaling(mWidth, mHeight, 1.f);
	mInvScreenTransformation = MatInverse(mScreenTransformation);
}

void Rasterizer::TriangleTraversal(const Triangle& triangle)
{
	// 以 4x4 的像素块为单位进行遍历来提高遍历效率
	int minX = (int)min(triangle.posScreen1.x, min(triangle.posScreen2.x, triangle.posScreen3.x)) >> 2;
	int maxX = (int)max(triangle.posScreen1.x, max(triangle.posScreen2.x, triangle.posScreen3.x)) >> 2;
	int minY = (int)min(triangle.posScreen1.y, min(triangle.posScreen2.y, triangle.posScreen3.y)) >> 2;
	int maxY = (int)max(triangle.posScreen1.y, max(triangle.posScreen2.y, triangle.posScreen3.y)) >> 2;
	
	// 构建像素块的AABB包围盒
	// 与三角形的边求交来判断是三角形外、三角形内或相交

	for (int j = max(minY,0); j <= min(maxY,mHeight >> 2); ++j)
	{
		for (int i = max(minX, 0); i <= min(maxX, mWidth >> 2); ++i)
		{
			Vec center = { (double)i * 4 + 2, (double)j * 4 + 2, 0.f, 1.f };
			static Vec extent = { 2.f, -2.f };

			int test = triangle.AabbIntersectionTest(VecMul(center, mInvScreenTransformation), VecMul(extent, mInvScreenTransformation));

			if (test == Triangle::OUTSIDE)
			{
				continue;
			}

			for (int pi = 0; pi < 4; ++pi)
			{
				for (int pj = 0; pj < 4; ++pj)
				{
					Vec posH = { i * 4 + pi + 0.5f,j * 4 + pj + 0.5f, 0.f, 1.f };
					posH = VecMul(posH, mInvScreenTransformation);

					if ((test==Triangle::INSIDE) || triangle.InTriangle(posH))
					{
						Interpolate(triangle, posH);
					}
				}
			}	
		}
	}
}

void Rasterizer::Interpolate(const Triangle& triangle, const Vec& posH)
{
	Vec uv = triangle.GetPerspectiveCorrectUV(posH);
	double u = uv.x;
	double v = uv.y;
	double w = 1.f - u - v;

	if (!(u > 0.0 && v > 0.0 && w > 0.0))
	{
		return;
	}

	VertexOut vout;
	vout.posH = VecMul(posH, mScreenTransformation);
	vout.posH.w = 1.0;
	vout.posH.z = u * (triangle.v1.posH.z / triangle.v1.posH.w) + v * (triangle.v2.posH.z / triangle.v2.posH.w) + w * (triangle.v3.posH.z / triangle.v3.posH.w);
	vout.posW = u * triangle.v1.posW + v * triangle.v2.posW + w * triangle.v3.posW;
	vout.texC = u * triangle.v1.texC + v * triangle.v2.texC + w * triangle.v3.texC;

	if (vout.texC.x < 0.0 || vout.texC.x>1.0 || vout.texC.y < 0.0 || vout.texC.y>1.0)
	{
		int a = 1;
	}

	mPixels.push_back(vout);
}

Triangle::Triangle(const VertexOut& _v1, const VertexOut& _v2, const VertexOut& _v3)
	:v1(_v1), v2(_v2), v3(_v3)
{
	static auto geta = [](Vec v1, Vec v2)
	{
		return -(v1.y - v2.y);
	};

	static auto getb = [](Vec v1, Vec v2)
	{
		return v1.x - v2.x;
	};

	static auto getc = [](Vec v1, Vec v2)
	{
		return (v1.y - v2.y) * v1.x - (v1.x - v2.x) * v1.y;
	};

	// 计算边表达式，用于计算像素是否在三角形内部
	Vec posNdc1 = v1.posH / v1.posH.w;
	Vec posNdc2 = v2.posH / v2.posH.w;
	Vec posNdc3 = v3.posH / v3.posH.w;

	e1 = { geta(posNdc1,posNdc2),getb(posNdc1,posNdc2),getc(posNdc1,posNdc2) };
	e2 = { geta(posNdc2,posNdc3),getb(posNdc2,posNdc3),getc(posNdc2,posNdc3) };
	e3 = { geta(posNdc3,posNdc1),getb(posNdc3,posNdc1),getc(posNdc3,posNdc1) };
}

int Triangle::AabbIntersectionTest(const Vec& center, const Vec& extent, const Edge& edge)const
{
	// 计算方法见Real-Time Rendering 4th edtion P970

	Vec normal = { edge.a,edge.b };
	double e = extent.x * fabs(edge.a) + extent.y * fabs(edge.b);
	double s = edge.a * center.x + edge.b * center.y + edge.c;

	if (s - e > 0)
	{
		return INSIDE;
	}
	else if (s + e < 0)
	{
		return OUTSIDE;
	}
	else
	{
		return INTERSECTING;
	}

}

bool Triangle::IsFrontFace()
{
	// 以顺时针为正
	// 通过行列式来计算带符号的三角形面积的两倍
	// 若为正则为顺时针

	static auto det2 = [](Vec v1, Vec v2)
	{
		return v1.x * v2.y - v1.y * v2.x;
	};

	return det2(posScreen1, posScreen2) + det2(posScreen2, posScreen3) + det2(posScreen3, posScreen1) > 0;
}

bool Triangle::InTriangle(const Vec& posH)const
{
	static auto leftTopTest = [](double& test, const Edge& e)
	{
		if (test == 0.f && ((e.a > 0.f) || (e.a == 0.f && e.b < 0.f)))
		{
			test += 0.1f;
		}
	};

	double test1 = (e1.a * posH.x + e1.b * posH.y + e1.c);
	double test2 = (e2.a * posH.x + e2.b * posH.y + e2.c);
	double test3 = (e3.a * posH.x + e3.b * posH.y + e3.c);

	leftTopTest(test1, e1);
	leftTopTest(test2, e2);
	leftTopTest(test3, e3);

	return (test1 > 0.f) && (test2 > 0.f) && (test3 > 0.f);
}

int Triangle::AabbIntersectionTest(const Vec& center, const Vec& extent)const
{
	int test1 = AabbIntersectionTest(center, extent, e1);
	int test2 = AabbIntersectionTest(center, extent, e2);
	int test3 = AabbIntersectionTest(center, extent, e3);

	if (test1 == OUTSIDE || test2 == OUTSIDE || test3 == OUTSIDE)
	{
		return OUTSIDE;
	}
	else if (test1 == INSIDE && test2 == INSIDE && test3 == INSIDE)
	{
		return INSIDE;
	}
	else
	{
		return INTERSECTING;
	}
}

Vec Triangle::GetPerspectiveCorrectUV(const Vec& posH)const
{
	// 采用透视矫正插值

	// 首先根据面积计算重心坐标
	// 计算方法见Real-Time Rendering 4th edition P999

	// p点与各顶点形成的向量
	Vec p1 = posH - v1.posH / v1.posH.w;
	Vec p2 = posH - v2.posH / v2.posH.w;
	Vec p3 = posH - v3.posH / v3.posH.w;

	// 各边的法线
	Vec n1 = { e1.a,e1.b };
	Vec n2 = { e2.a,e2.b };
	Vec n3 = { e3.a,e3.b };

	// 可以证明在目前所使用的边的表示方式下，
	// 二者的点积就是对应三角形的面积的两倍
	double f1 = Vec2Dot(p2, n2) / v1.posH.w;
	double f2 = Vec2Dot(p3, n3) / v2.posH.w;
	double f3 = Vec2Dot(p1, n1) / v3.posH.w;
	double f = f1 + f2 + f3;
	
	return { f1 / f,f2 / f };
}
