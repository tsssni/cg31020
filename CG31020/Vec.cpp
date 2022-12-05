#include "pch.h"
#include "Vec.h"
#include "Mat.h"


Vec::Vec(double _x, double _y, double _z, double _w)
    :x(_x), y(_y), z(_z), w(_w)
{
}

Vec::Vec(const Vec& v)
{
    *this = v;
}

Vec& Vec::operator=(const Vec& v)
{
    x = v.x;
    y = v.y;
    z = v.z;
    w = v.w;

    return *this;
}

Vec Vec::operator*(const Vec& v)const
{
    Vec res = *this;
    res *= v;
    return v;
}

Vec& Vec::operator*=(const Vec& v)
{
    x *= v.x;
    y *= v.y;
    z *= v.z;
    w *= v.w;

    return *this;
}

Vec Vec::operator*(double num)const
{
    Vec res = *this;
    res *= num;

    return res;
}

Vec& Vec::operator*=(double num)
{
    x *= num;
    y *= num;
    z *= num;
    w *= num;

    return *this;
}

Vec Vec::operator/(double num)const
{
    Vec res = *this;
    res /= num;

    return res;
}

Vec& Vec::operator/=(double num)
{
    x /= num;
    y /= num;
    z /= num;
    w /= num;

    return *this;
}

Vec Vec::operator+(const Vec& v)const
{
    Vec res = *this;
    res += v;

    return res;
}

Vec& Vec::operator+=(const Vec& v)
{
    x += v.x;
    y += v.y;
    z += v.z;
    w += v.w;

    return *this;
}

Vec Vec::operator-(const Vec& v)const
{
    Vec res = *this;
    res -= v;

    return res;
}

Vec& Vec::operator-=(const Vec& v)
{
    x -= v.x;
    y -= v.y;
    z -= v.z;
    w -= v.w;

    return *this;
}

Vec operator*(double num, const Vec& vec)
{
    Vec res = vec;
    return res * num;
}

double Vec2Dot(const Vec& v1, const Vec& v2)
{
    return v1.x * v2.x + v1.y * v2.y;
}

double Vec3Dot(const Vec& v1, const Vec& v2)
{
    return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

double Vec4Dot(const Vec& v1, const Vec& v2)
{
    return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z + v1.w * v2.w;
}

Vec Vec3Cross(const Vec& v1, const Vec& v2)
{
    return 
    { 
        v1.y * v2.z - v1.z * v2.y, 
        v2.x * v1.z - v2.z * v1.x,
        v1.x * v2.y - v1.y * v2.x
    };
}

Vec VecMul(const Vec& v, const Mat& m)
{
    Vec col1 = { m._11,m._21,m._31,m._41 };
    Vec col2 = { m._12,m._22,m._32,m._42 };
    Vec col3 = { m._13,m._23,m._33,m._43 };
    Vec col4 = { m._14,m._24,m._34,m._44 };

    return
    {
        Vec4Dot(v,col1),
        Vec4Dot(v,col2),
        Vec4Dot(v,col3),
        Vec4Dot(v,col4)
    };
}

Vec VecCross(const Vec& v1, const Vec& v2)
{
    return Vec();
}

double Vec2Norm(const Vec& v)
{
    return sqrt(Vec2Dot(v, v));
}

double Vec3Norm(const Vec& v)
{
    return sqrt(Vec3Dot(v, v));
}

double Vec4Norm(const Vec& v)
{
    return sqrt(Vec4Dot(v, v));
}

Vec Vec2Normalize(const Vec& v)
{
    return v / Vec2Norm(v);
}

double Vec2Cross(const Vec& v1, const Vec& v2)
{
    return v1.x * v2.y - v1.y * v2.x;
}

Vec Vec3Normalize(const Vec& v)
{
    return v / Vec3Norm(v);
}

Vec Vec4Normalize(const Vec& v)
{
    return v / Vec4Norm(v);
}
