#pragma once
#include "Vec.h"

class Mat
{
public:
	Mat operator+(const Mat& m)const;
	Mat& operator+=(const Mat& m);
	Mat operator-(const Mat& m)const;
	Mat& operator-=(const Mat& m);
	Mat operator*(const Mat& m)const;
	Mat& operator*=(const Mat& m);

	double _11 = 1.0, _12 = 0.0, _13 = 0.0, _14 = 0.0,
		  _21 = 0.0, _22 = 1.0, _23 = 0.0, _24 = 0.0,
		  _31 = 0.0, _32 = 0.0, _33 = 1.0, _34 = 0.0,
		  _41 = 0.0, _42 = 0.0, _43 = 0.0, _44 = 1.0;
};

Mat MatDot(const Mat& m1, const Mat& m2);
Mat MatIdentity();
Mat MatTranspose(const Mat& m);
Mat MatInverse(const Mat& m);
Mat MatScaling(double x, double y, double z);
Mat MatScaling(Vec v);
Mat MatTranslation(double x, double y, double z);
Mat MatTranslation(Vec v);
Mat MatRotation(double x, double y, double z, double angle);
Mat MatRotation(Vec axis, double angle);
Mat MatView(Vec eyePos, Vec target, Vec worldUp);
Mat MatPerspectiveProj(
	double fovAngle, // 从上到下的视野角度
	double aspectRatio, // 视图空间纵横比
	double nearZ,
	double farZ);
Mat MatObliqueBimetricProj(
	double width,
	double height,
	double l,
	double alpha,
	double nearZ,
	double farZ
);

