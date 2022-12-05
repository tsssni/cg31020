#pragma once
#include "Vec.h"
#include "Mat.h"

class PassConstants
{
public:
	Mat viewProj;
	COLORREF* texture;
	int textureWidth;
	int textureHeight;
	int width;
	int height;
};

class MeshConstants
{
public:
	Mat world;
};

class Vertex
{
public:
	Vec pos;
	Vec texC;
};

class VertexOut
{
public:
	Vec posH;
	Vec posW;
	Vec texC;
};

class PixelOut
{
public:
	Vec pos;
	Vec color;
};
