#include "pch.h"
#include "Mat.h"

const static double pi = 3.1415926535898f;

Mat Mat::operator+(const Mat& m)const
{
    Mat res = *this;
    res += m;

    return res;
}

Mat& Mat::operator+=(const Mat& m)
{
    _11 += m._11;
    _12 += m._12;
    _13 += m._13;
    _14 += m._14;
    _21 += m._21;
    _22 += m._22;
    _23 += m._23;
    _24 += m._24;
    _31 += m._31;
    _32 += m._32;
    _33 += m._33;
    _34 += m._34;
    _41 += m._41;
    _42 += m._42;
    _43 += m._43;
    _44 += m._44;

    return *this;
}

Mat Mat::operator-(const Mat& m)const
{
    Mat res = *this;
    res -= m;

    return res;
}

Mat& Mat::operator-=(const Mat& m)
{    
    _11 -= m._11;
    _12 -= m._12;
    _13 -= m._13;
    _14 -= m._14;
    _21 -= m._21;
    _22 -= m._22;
    _23 -= m._23;
    _24 -= m._24;
    _31 -= m._31;
    _32 -= m._32;
    _33 -= m._33;
    _34 -= m._34;
    _41 -= m._41;
    _42 -= m._42;
    _43 -= m._43;
    _44 -= m._44;

    return *this;
}

Mat Mat::operator*(const Mat& m)const
{
    Mat res = *this;
    res *= m;

    return res;
}

Mat& Mat::operator*=(const Mat& m)
{
    Vec row1 = { _11,_12,_13,_14 };
    Vec row2 = { _21,_22,_23,_24 };
    Vec row3 = { _31,_32,_33,_34 };
    Vec row4 = { _41,_42,_43,_44 };

    Vec col1 = { m._11,m._21,m._31,m._41 };
    Vec col2 = { m._12,m._22,m._32,m._42 };
    Vec col3 = { m._13,m._23,m._33,m._43 };
    Vec col4 = { m._14,m._24,m._34,m._44 };

    *this=
    {
        Vec4Dot(row1,col1),Vec4Dot(row1,col2),Vec4Dot(row1,col3),Vec4Dot(row1,col4),
        Vec4Dot(row2,col1),Vec4Dot(row2,col2),Vec4Dot(row2,col3),Vec4Dot(row2,col4),
        Vec4Dot(row3,col1),Vec4Dot(row3,col2),Vec4Dot(row3,col3),Vec4Dot(row3,col4),
        Vec4Dot(row4,col1),Vec4Dot(row4,col2),Vec4Dot(row4,col3),Vec4Dot(row4,col4)
    };

    return *this;
}

Mat MatDot(const Mat& m1, const Mat& m2)
{
    Mat res = m1;

    res._11 *= m2._11;
    res._12 *= m2._12;
    res._13 *= m2._13;
    res._14 *= m2._14;
    res._21 *= m2._21;
    res._22 *= m2._22;
    res._23 *= m2._23;
    res._24 *= m2._24;
    res._31 *= m2._31;
    res._32 *= m2._32;
    res._33 *= m2._33;
    res._34 *= m2._34;
    res._41 *= m2._41;
    res._42 *= m2._42;
    res._43 *= m2._43;
    res._44 *= m2._44;

    return res;
}

Mat MatIdentity()
{
    return {};
}

Mat MatTranspose(const Mat& m)
{
    return
    {
        m._11,m._21,m._31,m._41,
        m._12,m._22,m._32,m._42,
        m._13,m._23,m._33,m._43,
        m._14,m._24,m._34,m._44,
    };
}

Mat MatInverse(const Mat& m)
{
    static auto mat3Determinant = [](double m[3][3])
    {
        return m[0][0] * m[1][1] * m[2][2]
             + m[0][1] * m[1][2] * m[2][0]
             + m[1][0] * m[2][1] * m[0][2]
             - m[2][0] * m[1][1] * m[0][2]
             - m[2][1] * m[1][2] * m[0][0]
             - m[1][0] * m[0][1] * m[2][2];
    };
     
    double det;
    double mat[4][4] =
    {
        m._11,m._12,m._13,m._14,
        m._21,m._22,m._23,m._24,
        m._31,m._32,m._33,m._34,
        m._41,m._42,m._43,m._44
    };
    double adjM[4][4];

    // 求代数余子式
    for (int i = 0; i < 4; ++i)
    {
        for (int j = 0; j < 4; ++j)
        {
            double mat3[3][3];
            int row = 0;

            for (int i1 = 0; i1 < 4; ++i1)
            {
                if (i1 == i)
                {
                    continue;
                }

                int col = 0;
                for (int j1 = 0; j1 < 4; ++j1)
                {
                    if (j1 == j)
                    {
                        continue;
                    }

                    mat3[row][col] = mat[i1][j1];
                    ++col;
                }

                ++row;
            }

            adjM[j][i] = pow(-1, j + i) * mat3Determinant(mat3);
        }

        // 用第一行的计算结果计算行列式
        if (i == 0)
        {
            det = mat[0][0] * adjM[0][0] + mat[0][1] * adjM[1][0] + mat[0][2] * adjM[2][0] + mat[0][3] * adjM[3][0];
            
            if (det == 0)
            {
                return {};
            }
        }
    }

    for (int i = 0; i < 4; ++i)
    {
        for (int j = 0; j < 4; ++j)
        {
            mat[i][j] = adjM[i][j] / det;
        }
    }

    return
    {
        mat[0][0],mat[0][1],mat[0][2],mat[0][3],
        mat[1][0],mat[1][1],mat[1][2],mat[1][3],
        mat[2][0],mat[2][1],mat[2][2],mat[2][3],
        mat[3][0],mat[3][1],mat[3][2],mat[3][3]
    };
}

Mat MatScaling(double x, double y, double z)
{
    return
    {
        x,0.0,0.0,0.0,
        0.0,y,0.0,0.0,
        0.0,0.0,z,0.0,
        0.0,0.0,0.0,1.0
    };
}

Mat MatScaling(Vec v)
{
    return MatScaling(v.x, v.y, v.z);
}

Mat MatTranslation(double x, double y, double z)
{
    return
    {
        1.0,0.0,0.0,0.0,
        0.0,1.0,0.0,0.0,
        0.0,0.0,1.0,0.0,
        x,y,z,1.0
    };
}

Mat MatTranslation(Vec v)
{
    return MatTranslation(v.x, v.y, v.z);
}

Mat MatRotation(double x, double y, double z, double angle)
{
    // 左手系，先绕y轴旋转，再绕x轴旋转，将旋转轴转换为z轴

    double normY = Vec2Norm({ x,z });
    double cosY = normY == 0.0 ? 1.0 : z / normY;
    double sinY = normY == 0.0 ? 0.0 : x / normY;

    Mat rotY =
    {
        cosY,0.0,sinY,0.0,
        0.0,1.0,0.0,0.0,
        -sinY,0.0,cosY,0.0,
        0.0,0.0,0.0,1.0
    };
    Mat invRotY = MatInverse(rotY);

    double norm = Vec3Norm({ x,y,z });
    double cosX = normY / norm;
    double sinX = sqrt(1 - cosX * cosX);

    Mat rotX =
    {
        1.0,0.0,0.0,0.0,
        0.0,cosX,sinX,0.0,
        0.0,-sinX,cosX,0.0,
        0.0,0.0,0.0,1.0
    };
    Mat invRotX = MatInverse(rotX);

    double rad = angle * pi / 180.0;
    double cosZ = cos(rad);
    double sinZ = sin(rad);

    Mat rotZ =
    {
        cosZ,-sinZ,0.0,0.0,
        sinZ,cosZ,0.0,0.0,
        0.0,0.0,1.0,0.0,
        0.0,0.0,0.0,1.0
    };

    return rotY * rotX * rotZ * invRotX * invRotY;
}

Mat MatRotation(Vec axis, double angle)
{
    return MatRotation(axis.x, axis.y, axis.z, angle);
}

Mat MatView(Vec eyePos, Vec target, Vec worldUp)
{
    Vec look = Vec3Normalize(target - eyePos);
    Vec right = Vec3Normalize(Vec3Cross(worldUp, look));
    Vec up = Vec3Normalize(Vec3Cross(look, right));

    Mat translation = MatTranslation(-eyePos.x, -eyePos.y, -eyePos.z);
    Mat axis =
    {
        right.x, up.x, look.x, 0.0,
        right.y, up.y, look.y, 0.0,
        right.z, up.z, look.z, 0.0,
        0.0, 0.0, 0.0, 1.0
    };

    return translation * axis;
}

Mat MatPerspectiveProj(double fovAngle, double aspectRatio, double nearZ, double farZ)
{
    // 构建视锥体，
    // 投影结果的x、y值为归一化设备坐标(NDC),
    // z值最小为nearZ, 最大为farZ，投影结果在[0,1]的区间中，用于深度比较

    double halfFov = fovAngle * pi * 0.5 / 180.0;
    double cotHalfFov = 1.0 / tanf(halfFov);

    return
    {
        cotHalfFov / aspectRatio,0.0,0.0,0.0,
        0.0,cotHalfFov,0.0,0.0,
        0.0,0.0,farZ / (farZ - nearZ),1.0,
        0.0,0.0,-nearZ * farZ / (farZ - nearZ),0.0
    };
}

Mat MatObliqueBimetricProj(double width, double height, double l, double alpha, double nearZ, double farZ)
{
    double halfWidth = width * 0.5;
    double halfHeight = height * 0.5;
    double alphaRad = alpha * pi / 180.0;
    double lCosAlpha = l * cos(alphaRad);
    double lSinAlpha = l * sin(alphaRad);

    return Mat
    {
        1.0,0.0,0.0,0.0,
        0.0,1.0,0.0,0.0,
        lCosAlpha,lSinAlpha,1 / (farZ - nearZ),0.0,
        0.0,0.0,-nearZ / (farZ - nearZ),1.0
    } *MatScaling(1 / halfWidth, 1 / halfHeight, 1.0f);

}
