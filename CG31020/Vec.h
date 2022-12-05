#pragma once
class Mat;

class Vec
{
public:
	Vec(double _x = 0.0, double _y = 0.0, double _z = 0.0, double _w = 0.0);
	Vec(const Vec& v);
	Vec& operator=(const Vec& v);
	Vec operator+(const Vec& v)const;
	Vec& operator+=(const Vec& v);
	Vec operator-(const Vec& v)const;
	Vec& operator-=(const Vec& v);
	Vec operator*(const Vec& v)const;
	Vec& operator*=(const Vec& v);
	Vec operator*(double num)const;
	Vec& operator*=(double num);
	Vec operator/(double num)const;
	Vec& operator/=(double num);

	double x = 0.0, y = 0.0, z = 0.0, w = 0.0;
};

Vec operator*(double num, const Vec& vec);

double Vec2Norm(const Vec& v);
double Vec3Norm(const Vec& v);
double Vec4Norm(const Vec& v);
Vec Vec2Normalize(const Vec& v);
Vec Vec3Normalize(const Vec& v);
Vec Vec4Normalize(const Vec& v);
double Vec2Dot(const Vec& v1, const Vec& v2);
double Vec3Dot(const Vec& v1, const Vec& v2);
double Vec4Dot(const Vec& v1, const Vec& v2);
double Vec2Cross(const Vec& v1, const Vec& v2);
Vec Vec3Cross(const Vec& v1, const Vec& v2);
Vec VecMul(const Vec& v, const Mat& m);

