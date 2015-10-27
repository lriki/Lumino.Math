#pragma once

#include <gtest/gtest.h>

#include <limits.h>
#include <math.h>
#include <LuminoMath.h>
using namespace ln;

// float の演算方法がコンパイラによって異なり、結果が微妙に違くなる。
// その差を吸収するため、比較する桁数を指定する。
// (具体的にどう異なるのかは調査中。VS2010 と VS2013 では Vector3::Project() の誤差が一番大きかった)
#define LN_FLOAT_THRESHOLD 0.0001/*0.000001*/

#define ASSERT_VEC2_NEAR(x_, y_, v) \
	ASSERT_NEAR(x_, v.X, LN_FLOAT_THRESHOLD); \
	ASSERT_NEAR(y_, v.Y, LN_FLOAT_THRESHOLD);

#define ASSERT_VEC3_NEAR(x_, y_, z_, v) \
	ASSERT_NEAR(x_, v.X, LN_FLOAT_THRESHOLD); \
	ASSERT_NEAR(y_, v.Y, LN_FLOAT_THRESHOLD); \
	ASSERT_NEAR(z_, v.Z, LN_FLOAT_THRESHOLD);

#define ASSERT_VEC4_NEAR(x_, y_, z_, w_, v) \
	ASSERT_NEAR(x_, v.X, LN_FLOAT_THRESHOLD); \
	ASSERT_NEAR(y_, v.Y, LN_FLOAT_THRESHOLD); \
	ASSERT_NEAR(z_, v.Z, LN_FLOAT_THRESHOLD); \
	ASSERT_NEAR(w_, v.W, LN_FLOAT_THRESHOLD);

#define ASSERT_QUA_NEAR(x_, y_, z_, w_, q) \
	ASSERT_NEAR(x_, q.X, LN_FLOAT_THRESHOLD); \
	ASSERT_NEAR(y_, q.Y, LN_FLOAT_THRESHOLD); \
	ASSERT_NEAR(z_, q.Z, LN_FLOAT_THRESHOLD); \
	ASSERT_NEAR(w_, q.W, LN_FLOAT_THRESHOLD);

#define ASSERT_MAT_NEAR(M11_,M12_,M13_,M14_, M21_,M22_,M23_,M24_, M31_,M32_,M33_,M34_, M41_,M42_,M43_,M44_, M) \
	ASSERT_NEAR(M11_, M.M11, LN_FLOAT_THRESHOLD); \
	ASSERT_NEAR(M12_, M.M12, LN_FLOAT_THRESHOLD); \
	ASSERT_NEAR(M13_, M.M13, LN_FLOAT_THRESHOLD); \
	ASSERT_NEAR(M14_, M.M14, LN_FLOAT_THRESHOLD); \
	ASSERT_NEAR(M21_, M.M21, LN_FLOAT_THRESHOLD); \
	ASSERT_NEAR(M22_, M.M22, LN_FLOAT_THRESHOLD); \
	ASSERT_NEAR(M23_, M.M23, LN_FLOAT_THRESHOLD); \
	ASSERT_NEAR(M24_, M.M24, LN_FLOAT_THRESHOLD); \
	ASSERT_NEAR(M31_, M.M31, LN_FLOAT_THRESHOLD); \
	ASSERT_NEAR(M32_, M.M32, LN_FLOAT_THRESHOLD); \
	ASSERT_NEAR(M33_, M.M33, LN_FLOAT_THRESHOLD); \
	ASSERT_NEAR(M34_, M.M34, LN_FLOAT_THRESHOLD); \
	ASSERT_NEAR(M41_, M.M41, LN_FLOAT_THRESHOLD); \
	ASSERT_NEAR(M42_, M.M42, LN_FLOAT_THRESHOLD); \
	ASSERT_NEAR(M43_, M.M43, LN_FLOAT_THRESHOLD); \
	ASSERT_NEAR(M44_, M.M44, LN_FLOAT_THRESHOLD);

#define ASSERT_PLANE_NEAR(x_, y_, z_, w_, plane) \
	ASSERT_NEAR(x_, plane.Normal.X, LN_FLOAT_THRESHOLD); \
	ASSERT_NEAR(y_, plane.Normal.Y, LN_FLOAT_THRESHOLD); \
	ASSERT_NEAR(z_, plane.Normal.Z, LN_FLOAT_THRESHOLD); \
	ASSERT_NEAR(w_, plane.D, LN_FLOAT_THRESHOLD);

//#define D3DX9_TEST

#ifdef D3DX9_TEST
#include <d3dx9math.h>
#pragma comment(lib, "d3dx9.lib")

inline void dumpFLOAT(const char* header, FLOAT v)
{
	printf("%s : FLOAT(%f)\n", header, v);
}
inline void dumpD3DXVECTOR2(const char* header, const D3DXVECTOR2& v)
{
	printf("%s : D3DXVECTOR2(%f, %f)\n", header, v.x, v.y);
}
inline void dumpD3DXVECTOR3(const char* header, const D3DXVECTOR3& v)
{
	printf("%s : D3DXVECTOR3(%f, %f, %f)\n", header, v.x, v.y, v.z);
}
inline void dumpD3DXVECTOR4(const char* header, const D3DXVECTOR4& v)
{
	printf("%s : D3DXVECTOR4(%f, %f, %f, %f)\n", header, v.x, v.y, v.z, v.w);
}
inline void dumpD3DXQUATERNION(const char* header, const D3DXQUATERNION& v)
{
	printf("%s : D3DXQUATERNION(%f, %f, %f, %f)\n", header, v.x, v.y, v.z, v.w);
}
inline void dumpD3DXMATRIX(const char* header, const D3DXMATRIX& v)
{
	printf("%s : D3DXMATRIX(\n", header);
	printf("    %f, %f, %f, %f,\n", v.m[0][0],v.m[0][1],v.m[0][2],v.m[0][3]);
	printf("    %f, %f, %f, %f,\n", v.m[1][0],v.m[1][1],v.m[1][2],v.m[1][3]);
	printf("    %f, %f, %f, %f,\n", v.m[2][0],v.m[2][1],v.m[2][2],v.m[2][3]);
	printf("    %f, %f, %f, %f);\n",v.m[3][0],v.m[3][1],v.m[3][2],v.m[3][3]);
}
inline void dumpD3DXPLANE(const char* header, const D3DXPLANE& v)
{
	printf("%s : D3DXPLANE(%f, %f, %f, %f)\n", header, v.a, v.b, v.c, v.d);
}
#endif
