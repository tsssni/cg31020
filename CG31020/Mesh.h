#pragma once
#include "Vec.h"
#include "PipelineStruct.h"
#include <vector>
using std::vector;

class Mesh
{
public:
	virtual ~Mesh();
	vector<Vertex>& GetVertices();
	vector<int>& GetIndices();
	void SetMeshConstants(const MeshConstants& mc);
	MeshConstants& GetMeshConstants();
protected:
	vector<Vertex> mVertices;
	vector<int> mIndices;
	MeshConstants mMeshConstants;
};

class Box : public Mesh
{
public:
	Box(double width, double height, double depth);
	Box(const Box& box);
	Box& operator=(const Box& box);
	~Box();
};



