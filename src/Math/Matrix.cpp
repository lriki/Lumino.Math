
#include <math.h>
#include <assert.h>
#include "../../include/Lumino/Math/MathUtils.h"
#include "../../include/Lumino/Math/Vector3.h"
#include "../../include/Lumino/Math/Vector4.h"
#include "../../include/Lumino/Math/Quaternion.h"
#include "../../include/Lumino/Math/SQTTransform.h"
#include "../../include/Lumino/Math/Plane.h"
#include "../../include/Lumino/Math/Matrix.h"
#include "Asm.h"

LN_NAMESPACE_BEGIN

//=============================================================================
// Matrix
//=============================================================================
	
const Matrix Matrix::Identity = Matrix();

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Matrix::Matrix()
{
	M[0][0] = M[1][1] = M[2][2] = M[3][3] = 1.0f;
	M[0][1] = M[0][2] = M[0][3] = 0.0f;
	M[1][0] = M[1][2] = M[1][3] = 0.0f;
	M[2][0] = M[2][1] = M[2][3] = 0.0f;
	M[3][0] = M[3][1] = M[3][2] = 0.0f;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Matrix::Matrix(
	float m11, float m12, float m13, float m14,
	float m21, float m22, float m23, float m24,
	float m31, float m32, float m33, float m34,
	float m41, float m42, float m43, float m44)
{
	M[0][0] = m11; M[0][1] = m12; M[0][2] = m13; M[0][3] = m14;
	M[1][0] = m21; M[1][1] = m22; M[1][2] = m23; M[1][3] = m24;
	M[2][0] = m31; M[2][1] = m32; M[2][2] = m33; M[2][3] = m34;
	M[3][0] = m41; M[3][1] = m42; M[3][2] = m43; M[3][3] = m44;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Matrix::Matrix(const Vector4& row1, const Vector4& row2, const Vector4& row3, const Vector4& row4)
{
	*((Vector4*)this->M[0]) = row1;
	*((Vector4*)this->M[1]) = row2;
	*((Vector4*)this->M[2]) = row3;
	*((Vector4*)this->M[3]) = row4;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
//Matrix::Matrix(const Quaternion& q)
//{
//	float xx = q.X * q.X;
//	float yy = q.Y * q.Y;
//	float zz = q.Z * q.Z;
//	float xy = q.X * q.Y;
//	float zw = q.Z * q.W;
//	float zx = q.Z * q.X;
//	float yw = q.Y * q.W;
//	float yz = q.Y * q.Z;
//	float xw = q.X * q.W;
//	M11 = 1.0f - (2.0f * (yy + zz));
//	M12 = 2.0f * (xy + zw);
//	M13 = 2.0f * (zx - yw);
//	M14 = 0.0f;
//	M21 = 2.0f * (xy - zw);
//	M22 = 1.0f - (2.0f * (zz + xx));
//	M23 = 2.0f * (yz + xw);
//	M24 = 0.0f;
//	M31 = 2.0f * (zx + yw);
//	M32 = 2.0f * (yz - xw);
//	M33 = 1.0f - (2.0f * (yy + xx));
//	M34 = 0.0f;
//	M41 = 0.0f;
//	M42 = 0.0f;
//	M43 = 0.0f;
//	M44 = 1.0f;
//}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Matrix::Matrix(const SQTTransform& transform)
{
	M[0][0] = M[1][1] = M[2][2] = M[3][3] = 1.0f;
	M[0][1] = M[0][2] = M[0][3] = 0.0f;
	M[1][0] = M[1][2] = M[1][3] = 0.0f;
	M[2][0] = M[2][1] = M[2][3] = 0.0f;
	M[3][0] = M[3][1] = M[3][2] = 0.0f;

	Scale(transform.Scale);
	RotateQuaternion(transform.Rotation);
	Translate(transform.Translation);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Matrix::Set(
	float m11, float m12, float m13, float m14,
	float m21, float m22, float m23, float m24,
	float m31, float m32, float m33, float m34,
	float m41, float m42, float m43, float m44)
{
	M[0][0] = m11; M[0][1] = m12; M[0][2] = m13; M[0][3] = m14;
	M[1][0] = m21; M[1][1] = m22; M[1][2] = m23; M[1][3] = m24;
	M[2][0] = m31; M[2][1] = m32; M[2][2] = m33; M[2][3] = m34;
	M[3][0] = m41; M[3][1] = m42; M[3][2] = m43; M[3][3] = m44;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Matrix::Translate(float x, float y, float z)
{
	M[3][0] += x;
	M[3][1] += y;
	M[3][2] += z;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Matrix::Translate(const Vector3& vec)
{
	M[3][0] += vec.X;
	M[3][1] += vec.Y;
	M[3][2] += vec.Z;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Matrix::RotateX(float r)
{
	float c, s;
	Asm::sincos(r, &s, &c);

	/* 普通の行列計算
	M[0][0], M[0][1], M[0][2], M[0][3]   1, 0, 0, 0
	M[1][0], M[1][1], M[1][2], M[1][3]   0, c,-s, 0
	M[2][0], M[2][1], M[2][2], M[2][3] * 0, s, c, 0
	M[3][0], M[3][1], M[3][2], M[3][3]   0, 0, 0, 1
	*/
	/* 計算イメージ
	M[0][0] = M[0][0] * 1 + M[0][1] * 0 + M[0][2] * 0 + M[0][3] * 0;
	M[0][1] = M[0][0] * 0 + M[0][1] * c + M[0][2] * s + M[0][3] * 0;
	M[0][2] = M[0][0] * 0 + M[0][1] *-s + M[0][2] * c + M[0][3] * 0;
	M[0][3] = M[0][0] * 0 + M[0][1] * 0 + M[0][2] * 0 + M[0][3] * 1;

	M[1][0] = M[1][0] * 1 + M[1][1] * 0 + M[1][2] * 0 + M[1][3] * 0;
	M[1][1] = M[1][0] * 0 + M[1][1] * c + M[1][2] * s + M[1][3] * 0;
	M[1][2] = M[1][0] * 0 + M[1][1] *-s + M[1][2] * c + M[1][3] * 0;
	M[1][3] = M[1][0] * 0 + M[1][1] * 0 + M[1][2] * 0 + M[1][3] * 1;

	M[2][0] = M[2][0] * 1 + M[2][1] * 0 + M[2][2] * 0 + M[2][3] * 0;
	M[2][1] = M[2][0] * 0 + M[2][1] * c + M[2][2] * s + M[2][3] * 0;
	M[2][2] = M[2][0] * 0 + M[2][1] *-s + M[2][2] * c + M[2][3] * 0;
	M[2][3] = M[2][0] * 0 + M[2][1] * 0 + M[2][2] * 0 + M[2][3] * 1;

	M[3][0] = M[3][0] * 1 + M[3][1] * 0 + M[3][2] * 0 + M[3][3] * 0;
	M[3][1] = M[3][0] * 0 + M[3][1] * c + M[3][2] * s + M[3][3] * 0;
	M[3][2] = M[3][0] * 0 + M[3][1] *-s + M[3][2] * c + M[3][3] * 0;
	M[3][3] = M[3][0] * 0 + M[3][1] * 0 + M[3][2] * 0 + M[3][3] * 1;
	*/
	/* 正しく計算できるようにしたもの
	lfloat mx0 = M[0][0];
	lfloat mx1 = M[0][1];
	lfloat mx2 = M[0][2];
	M[0][0] = mx0 * 1 + mx1 * 0 + mx2 * 0 + M[0][3] * 0;
	M[0][1] = mx0 * 0 + mx1 * c + mx2 * s + M[0][3] * 0;
	M[0][2] = mx0 * 0 + mx1 *-s + mx2 * c + M[0][3] * 0;
	M[0][3] = mx0 * 0 + mx1 * 0 + mx2 * 0 + M[0][3] * 1;

	mx0 = M[1][0];
	mx1 = M[1][1];
	mx2 = M[1][2];
	M[1][0] = mx0 * 1 + mx1 * 0 + mx2 * 0 + M[1][3] * 0;
	M[1][1] = mx0 * 0 + mx1 * c + mx2 * s + M[1][3] * 0;
	M[1][2] = mx0 * 0 + mx1 *-s + mx2 * c + M[1][3] * 0;
	M[1][3] = mx0 * 0 + mx1 * 0 + mx2 * 0 + M[1][3] * 1;

	mx0 = M[2][0];
	mx1 = M[2][1];
	mx2 = M[2][2];
	M[2][0] = mx0 * 1 + mx1 * 0 + mx2 * 0 + M[2][3] * 0;
	M[2][1] = mx0 * 0 + mx1 * c + mx2 * s + M[2][3] * 0;
	M[2][2] = mx0 * 0 + mx1 *-s + mx2 * c + M[2][3] * 0;
	M[2][3] = mx0 * 0 + mx1 * 0 + mx2 * 0 + M[2][3] * 1;

	mx0 = M[3][0];
	mx1 = M[3][1];
	mx2 = M[3][2];
	M[3][0] = mx0 * 1 + mx1 * 0 + mx2 * 0 + M[3][3] * 0;
	M[3][1] = mx0 * 0 + mx1 * c + mx2 * s + M[3][3] * 0;
	M[3][2] = mx0 * 0 + mx1 *-s + mx2 * c + M[3][3] * 0;
	M[3][3] = mx0 * 0 + mx1 * 0 + mx2 * 0 + M[3][3] * 1;
	*/
	/* 単純に * 0 とかの無駄なところを切る
	lfloat mx0 = M[0][0];
	lfloat mx1 = M[0][1];
	lfloat mx2 = M[0][2];
	M[0][0] = mx0;
	M[0][1] = mx1 * c + mx2 * s;
	M[0][2] = mx1 *-s + mx2 * c;
	M[0][3] = M[0][3];

	mx0 = M[1][0];
	mx1 = M[1][1];
	mx2 = M[1][2];
	M[1][0] = mx0;
	M[1][1] = mx1 * c + mx2 * s;
	M[1][2] = mx1 *-s + mx2 * c;
	M[1][3] = M[1][3];

	mx0 = M[2][0];
	mx1 = M[2][1];
	mx2 = M[2][2];
	M[2][0] = mx0;
	M[2][1] = mx1 * c + mx2 * s;
	M[2][2] = mx1 *-s + mx2 * c;
	M[2][3] = M[2][3];

	mx0 = M[3][0];
	mx1 = M[3][1];
	mx2 = M[3][2];
	M[3][0] = mx0;
	M[3][1] = mx1 * c + mx2 * s;
	M[3][2] = mx1 *-s + mx2 * c;
	M[3][3] = M[3][3];
	*/

	// 自分自身を代入しているところを切る
	float mx1 = M[0][1];
	M[0][1] = mx1 * c + M[0][2] * -s;
	M[0][2] = mx1 * s + M[0][2] * c;

	mx1 = M[1][1];
	M[1][1] = mx1 * c + M[1][2] * -s;
	M[1][2] = mx1 * s + M[1][2] * c;

	mx1 = M[2][1];
	M[2][1] = mx1 * c + M[2][2] * -s;
	M[2][2] = mx1 * s + M[2][2] * c;

	mx1 = M[3][1];
	M[3][1] = mx1 * c + M[3][2] * -s;
	M[3][2] = mx1 * s + M[3][2] * c;

	/* OpenGL
	lnFloat mx1 = M[0][1];
	M[0][1] = mx1 * c + M[0][2] * s;
	M[0][2] = mx1 *-s + M[0][2] * c;

	mx1 = M[1][1];
	M[1][1] = mx1 * c + M[1][2] * s;
	M[1][2] = mx1 *-s + M[1][2] * c;

	mx1 = M[2][1];
	M[2][1] = mx1 * c + M[2][2] * s;
	M[2][2] = mx1 *-s + M[2][2] * c;

	mx1 = M[3][1];
	M[3][1] = mx1 * c + M[3][2] * s;
	M[3][2] = mx1 *-s + M[3][2] * c;
	*/
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Matrix::RotateY(float r)
{
	float c, s;
	Asm::sincos(r, &s, &c);

	/* 普通の行列計算
	M[0][0], M[0][1], M[0][2], M[0][3]    c, 0, s, 0
	M[1][0], M[1][1], M[1][2], M[1][3]    0, 1, 0, 0
	M[2][0], M[2][1], M[2][2], M[2][3] * -s, 0, c, 0
	M[3][0], M[3][1], M[3][2], M[3][3]    0, 0, 0, 1
	*/
	/* 計算イメージ
	M[0][0] = M[0][0] * c + M[0][1] * 0 + M[0][2] *-s + M[0][3] * 0;
	M[0][1] = M[0][0] * 0 + M[0][1] * 1 + M[0][2] * 0 + M[0][3] * 0;
	M[0][2] = M[0][0] * s + M[0][1] * 0 + M[0][2] * c + M[0][3] * 0;
	M[0][3] = M[0][0] * 0 + M[0][1] * 0 + M[0][2] * 0 + M[0][3] * 1;

	M[1][0] = M[1][0] * c + M[1][1] * 0 + M[1][2] *-s + M[1][3] * 0;
	M[1][1] = M[1][0] * 0 + M[1][1] * 1 + M[1][2] * 0 + M[1][3] * 0;
	M[1][2] = M[1][0] * s + M[1][1] * 0 + M[1][2] * c + M[1][3] * 0;
	M[1][3] = M[1][0] * 0 + M[1][1] * 0 + M[1][2] * 0 + M[1][3] * 1;

	M[2][0] = M[2][0] * c + M[2][1] * 0 + M[2][2] *-s + M[2][3] * 0;
	M[2][1] = M[2][0] * 0 + M[2][1] * 1 + M[2][2] * 0 + M[2][3] * 0;
	M[2][2] = M[2][0] * s + M[2][1] * 0 + M[2][2] * c + M[2][3] * 0;
	M[2][3] = M[2][0] * 0 + M[2][1] * 0 + M[2][2] * 0 + M[2][3] * 1;

	M[3][0] = M[3][0] * c + M[3][1] * 0 + M[3][2] *-s + M[3][3] * 0;
	M[3][1] = M[3][0] * 0 + M[3][1] * 1 + M[3][2] * 0 + M[3][3] * 0;
	M[3][2] = M[3][0] * s + M[3][1] * 0 + M[3][2] * c + M[3][3] * 0;
	M[3][3] = M[3][0] * 0 + M[3][1] * 0 + M[3][2] * 0 + M[3][3] * 1;
	*/
	/* 正しく計算できるようにしたもの
	lfloat mx0 = M[0][0];
	lfloat mx1 = M[0][1];
	lfloat mx2 = M[0][2];
	M[0][0] = mx0 * c + mx1 * 0 + mx2 *-s + M[0][3] * 0;
	M[0][1] = mx0 * 0 + mx1 * 1 + mx2 * 0 + M[0][3] * 0;
	M[0][2] = mx0 * s + mx1 * 0 + mx2 * c + M[0][3] * 0;
	M[0][3] = mx0 * 0 + mx1 * 0 + mx2 * 0 + M[0][3] * 1;

	mx0 = M[1][0];
	mx1 = M[1][1];
	mx2 = M[1][2];
	M[1][0] = mx0 * c + mx1 * 0 + mx2 *-s + M[1][3] * 0;
	M[1][1] = mx0 * 0 + mx1 * 1 + mx2 * 0 + M[1][3] * 0;
	M[1][2] = mx0 * s + mx1 * 0 + mx2 * c + M[1][3] * 0;
	M[1][3] = mx0 * 0 + mx1 * 0 + mx2 * 0 + M[1][3] * 1;

	mx0 = M[2][0];
	mx1 = M[2][1];
	mx2 = M[2][2];
	M[2][0] = mx0 * c + mx1 * 0 + mx2 *-s + M[2][3] * 0;
	M[2][1] = mx0 * 0 + mx1 * 1 + mx2 * 0 + M[2][3] * 0;
	M[2][2] = mx0 * s + mx1 * 0 + mx2 * c + M[2][3] * 0;
	M[2][3] = mx0 * 0 + mx1 * 0 + mx2 * 0 + M[2][3] * 1;

	mx0 = M[3][0];
	mx1 = M[3][1];
	mx2 = M[3][2];
	M[3][0] = mx0 * c + mx1 * 0 + mx2 *-s + M[3][3] * 0;
	M[3][1] = mx0 * 0 + mx1 * 1 + mx2 * 0 + M[3][3] * 0;
	M[3][2] = mx0 * s + mx1 * 0 + mx2 * c + M[3][3] * 0;
	M[3][3] = mx0 * 0 + mx1 * 0 + mx2 * 0 + M[3][3] * 1;
	*/
	/* 単純に * 0 とかの無駄なところを切る
	lfloat mx0 = M[0][0];
	lfloat mx1 = M[0][1];
	lfloat mx2 = M[0][2];
	M[0][0] = mx0 * c + mx2 *-s;
	M[0][1] = mx1;
	M[0][2] = mx0 * s + mx2 * c;
	M[0][3] = M[0][3];

	mx0 = M[1][0];
	mx1 = M[1][1];
	mx2 = M[1][2];
	M[1][0] = mx0 * c + mx2 *-s;
	M[1][1] = mx1;
	M[1][2] = mx0 * s + mx2 * c;
	M[1][3] = M[1][3];

	mx0 = M[2][0];
	mx1 = M[2][1];
	mx2 = M[2][2];
	M[2][0] = mx0 * c + mx2 *-s;
	M[2][1] = mx1;
	M[2][2] = mx0 * s + mx2 * c;
	M[2][3] = mx3;

	mx0 = M[3][0];
	mx1 = M[3][1];
	mx2 = M[3][2];
	M[3][0] = mx0 * c + mx2 *-s;
	M[3][1] = mx1;
	M[3][2] = mx0 * s + mx2 * c;
	M[3][3] = M[3][3];
	*/

	// 自分自身を代入しているところを切る
	float mx0 = M[0][0];
	M[0][0] = mx0 * c + M[0][2] * s;
	M[0][2] = mx0 *-s + M[0][2] * c;

	mx0 = M[1][0];
	M[1][0] = mx0 * c + M[1][2] * s;
	M[1][2] = mx0 *-s + M[1][2] * c;

	mx0 = M[2][0];
	M[2][0] = mx0 * c + M[2][2] * s;
	M[2][2] = mx0 *-s + M[2][2] * c;

	mx0 = M[3][0];
	M[3][0] = mx0 * c + M[3][2] * s;
	M[3][2] = mx0 *-s + M[3][2] * c;

	/* OpenGL
	lnFloat mx0 = M[0][0];
	M[0][0] = mx0 * c + M[0][2] *-s;
	M[0][2] = mx0 * s + M[0][2] * c;

	mx0 = M[1][0];
	M[1][0] = mx0 * c + M[1][2] *-s;
	M[1][2] = mx0 * s + M[1][2] * c;

	mx0 = M[2][0];
	M[2][0] = mx0 * c + M[2][2] *-s;
	M[2][2] = mx0 * s + M[2][2] * c;

	mx0 = M[3][0];
	M[3][0] = mx0 * c + M[3][2] *-s;
	M[3][2] = mx0 * s + M[3][2] * c;
	*/
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Matrix::RotateZ(float r)
{
	float c, s;
	Asm::sincos(r, &s, &c);

	/* 基本の計算イメージ
	M[0][0] = M[0][0] * c + M[0][1] * s + M[0][2] * 0 + M[0][3] * 0;
	M[0][1] = M[0][0] *-s + M[0][1] * c + M[0][2] * 0 + M[0][3] * 0;
	M[0][2] = M[0][0] * 0 + M[0][1] * 0 + M[0][2] * 1 + M[0][3] * 0;
	M[0][3] = M[0][0] * 0 + M[0][1] * 0 + M[0][2] * 0 + M[0][3] * 1;

	M[1][0] = M[1][0] * c + M[1][1] * s + M[1][2] * 0 + M[1][3] * 0;
	M[1][1] = M[1][0] *-s + M[1][1] * c + M[1][2] * 0 + M[1][3] * 0;
	M[1][2] = M[1][0] * 0 + M[1][1] * 0 + M[1][2] * 1 + M[1][3] * 0;
	M[1][3] = M[1][0] * 0 + M[1][1] * 0 + M[1][2] * 0 + M[1][3] * 1;

	M[2][0] = M[2][0] * c + M[2][1] * s + M[2][2] * 0 + M[2][3] * 0;
	M[2][1] = M[2][0] *-s + M[2][1] * c + M[2][2] * 0 + M[2][3] * 0;
	M[2][2] = M[2][0] * 0 + M[2][1] * 0 + M[2][2] * 1 + M[2][3] * 0;
	M[2][3] = M[2][0] * 0 + M[2][1] * 0 + M[2][2] * 0 + M[2][3] * 1;

	M[3][0] = M[3][0] * c + M[3][1] * s + M[3][2] * 0 + M[3][3] * 0;
	M[3][1] = M[3][0] *-s + M[3][1] * c + M[3][2] * 0 + M[3][3] * 0;
	M[3][2] = M[3][0] * 0 + M[3][1] * 0 + M[3][2] * 1 + M[3][3] * 0;
	M[3][3] = M[3][0] * 0 + M[3][1] * 0 + M[3][2] * 0 + M[3][3] * 1;
	*/
	/* 正しく計算できるようにしたもの
	lfloat mx0 = M[0][0];
	lfloat mx1 = M[0][1];
	lfloat mx2 = M[0][2];
	M[0][0] = mx0 * c + mx1 * s + mx2 * 0 + M[0][3] * 0;
	M[0][1] = mx0 *-s + mx1 * c + mx2 * 0 + M[0][3] * 0;
	M[0][2] = mx0 * 0 + mx1 * 0 + mx2 * 1 + M[0][3] * 0;
	M[0][3] = mx0 * 0 + mx1 * 0 + mx2 * 0 + M[0][3] * 1;

	mx0 = M[1][0];
	mx1 = M[1][1];
	mx2 = M[1][2];
	M[1][0] = mx0 * c + mx1 * s + mx2 * 0 + M[1][3] * 0;
	M[1][1] = mx0 *-s + mx1 * c + mx2 * 0 + M[1][3] * 0;
	M[1][2] = mx0 * 0 + mx1 * 0 + mx2 * 1 + M[1][3] * 0;
	M[1][3] = mx0 * 0 + mx1 * 0 + mx2 * 0 + M[1][3] * 1;

	mx0 = M[2][0];
	mx1 = M[2][1];
	mx2 = M[2][2];
	M[2][0] = mx0 * c + mx1 * s + mx2 * 0 + M[2][3] * 0;
	M[2][1] = mx0 *-s + mx1 * c + mx2 * 0 + M[2][3] * 0;
	M[2][2] = mx0 * 0 + mx1 * 0 + mx2 * 1 + M[2][3] * 0;
	M[2][3] = mx0 * 0 + mx1 * 0 + mx2 * 0 + M[2][3] * 1;

	mx0 = M[3][0];
	mx1 = M[3][1];
	mx2 = M[3][2];
	M[3][0] = mx0 * c + mx1 * s + mx2 * 0 + M[3][3] * 0;
	M[3][1] = mx0 *-s + mx1 * c + mx2 * 0 + M[3][3] * 0;
	M[3][2] = mx0 * 0 + mx1 * 0 + mx2 * 1 + M[3][3] * 0;
	M[3][3] = mx0 * 0 + mx1 * 0 + mx2 * 0 + M[3][3] * 1;
	*/
	/* 単純に * 0 とかの無駄なところを切る
	lfloat mx0 = M[0][0];
	lfloat mx1 = M[0][1];
	lfloat mx2 = M[0][2];
	M[0][0] = mx0 * c + mx1 * s;
	M[0][1] = mx0 *-s + mx1 * c;
	M[0][2] = mx2;
	M[0][3] = M[0][3];

	mx0 = M[1][0];
	mx1 = M[1][1];
	mx2 = M[1][2];
	M[1][0] = mx0 * c + mx1 * s;
	M[1][1] = mx0 *-s + mx1 * c;
	M[1][2] = mx2;
	M[1][3] = M[1][3];

	mx0 = M[2][0];
	mx1 = M[2][1];
	mx2 = M[2][2];
	M[2][0] = mx0 * c + mx1 * s;
	M[2][1] = mx0 *-s + mx1 * c;
	M[2][2] = mx2;
	M[2][3] = M[2][3];

	mx0 = M[3][0];
	mx1 = M[3][1];
	mx2 = M[3][2];
	M[3][0] = mx0 * c + mx1 * s;
	M[3][1] = mx0 *-s + mx1 * c;
	M[3][2] = mx2;
	M[3][3] = M[3][3];
	*/
	// 自分自身を代入しているところを切る
	float mx0 = M[0][0];
	M[0][0] = mx0 * c + M[0][1] * -s;
	M[0][1] = mx0 * s + M[0][1] * c;

	mx0 = M[1][0];
	M[1][0] = mx0 * c + M[1][1] * -s;
	M[1][1] = mx0 * s + M[1][1] * c;

	mx0 = M[2][0];
	M[2][0] = mx0 * c + M[2][1] * -s;
	M[2][1] = mx0 * s + M[2][1] * c;

	mx0 = M[3][0];
	M[3][0] = mx0 * c + M[3][1] * -s;
	M[3][1] = mx0 * s + M[3][1] * c;

	// OpenGL
	/*
	lnFloat mx0 = M[0][0];
	M[0][0] = mx0 * c + M[0][1] * s;
	M[0][1] = mx0 *-s + M[0][1] * c;

	mx0 = M[1][0];
	M[1][0] = mx0 * c + M[1][1] * s;
	M[1][1] = mx0 *-s + M[1][1] * c;

	mx0 = M[2][0];
	M[2][0] = mx0 * c + M[2][1] * s;
	M[2][1] = mx0 *-s + M[2][1] * c;

	mx0 = M[3][0];
	M[3][0] = mx0 * c + M[3][1] * s;
	M[3][1] = mx0 *-s + M[3][1] * c;
	*/
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Matrix::RotateEulerAngles(float x, float y, float z, RotationOrder order)
{
	RotateEulerAngles(Vector3(x, y, z), order);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Matrix::RotateEulerAngles(const Vector3& angles, RotationOrder order)
{
	switch (order)
	{
	case RotationOrder_XYZ:
		RotateX(angles.X); RotateY(angles.Y); RotateZ(angles.Z); break;
	case RotationOrder_YZX:
		RotateY(angles.Y); RotateZ(angles.Z); RotateX(angles.X); break;
	case RotationOrder_ZXY:
		RotateZ(angles.Z); RotateX(angles.X); RotateY(angles.Y); break;	// RotationYawPitchRoll
	default:
		assert(0);
		break;
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Matrix::RotateAxis(const Vector3& axis_, float r)
{
	Vector3 axis = axis_;
	if (axis.GetLengthSquared() != 1.0f) {
		axis.Normalize();
	}

	float s, c;
	Asm::sincos(r, &s, &c);
	float mc = 1.0f - c;

	/* 計算イメージ
	_00 = ( axis_.x * axis_.x ) * mc + c;
	_01 = ( axis_.x * axis_.y ) * mc + ( axis_.z * s );
	_02 = ( axis_.x * axis_.z ) * mc - ( axis_.y * s );
	_03 = 0;

	_10 = ( axis_.x * axis_.y ) * mc - ( axis_.z * s );
	_11 = ( axis_.y * axis_.y ) * mc + c;
	_12 = ( axis_.y * axis_.z ) * mc + ( axis_.x * s );
	_13 = 0;

	_20 = ( axis_.x * axis_.z ) * mc + ( axis_.y * s );
	_21 = ( axis_.y * axis_.z ) * mc - ( axis_.x * s );
	_22 = ( axis_.z * axis_.z ) * mc + c;
	_23 = 0;

	_30 = _31 = _32 = 0;
	_33 = 1;

	M[0][0] = M[0][0] * _00 + M[0][1] * _10 + M[0][2] * _20 + M[0][3] * _30;
	M[0][1] = M[0][0] * _01 + M[0][1] * _11 + M[0][2] * _21 + M[0][3] * _31;
	M[0][2] = M[0][0] * _02 + M[0][1] * _12 + M[0][2] * _22 + M[0][3] * _32;
	M[0][3] = M[0][0] * _03 + M[0][1] * _13 + M[0][2] * _23 + M[0][3] * _33;

	M[1][0] = M[1][0] * _00 + M[1][1] * _10 + M[1][2] * _20 + M[1][3] * _30;
	M[1][1] = M[1][0] * _01 + M[1][1] * _11 + M[1][2] * _21 + M[1][3] * _31;
	M[1][2] = M[1][0] * _02 + M[1][1] * _12 + M[1][2] * _22 + M[1][3] * _32;
	M[1][3] = M[1][0] * _03 + M[1][1] * _13 + M[1][2] * _23 + M[1][3] * _33;

	M[2][0] = M[2][0] * _00 + M[2][1] * _10 + M[2][2] * _20 + M[2][3] * _30;
	M[2][1] = M[2][0] * _01 + M[2][1] * _11 + M[2][2] * _21 + M[2][3] * _31;
	M[2][2] = M[2][0] * _02 + M[2][1] * _12 + M[2][2] * _22 + M[2][3] * _32;
	M[2][3] = M[2][0] * _03 + M[2][1] * _13 + M[2][2] * _23 + M[2][3] * _33;

	M[3][0] = M[3][0] * _00 + M[3][1] * _10 + M[3][2] * _20 + M[3][3] * _30;
	M[3][1] = M[3][0] * _01 + M[3][1] * _11 + M[3][2] * _21 + M[3][3] * _31;
	M[3][2] = M[3][0] * _02 + M[3][1] * _12 + M[3][2] * _22 + M[3][3] * _32;
	M[3][3] = M[3][0] * _03 + M[3][1] * _13 + M[3][2] * _23 + M[3][3] * _33;
	*/

	/* 正しく計算できるようにしたもの
	lfloat mx0 = M[0][0];
	lfloat mx1 = M[0][1];
	lfloat mx2 = M[0][2];
	M[0][0] = mx0 * _00 + mx1 * _10 + mx2 * _20 + M[0][3] * _30;
	M[0][1] = mx0 * _01 + mx1 * _11 + mx2 * _21 + M[0][3] * _31;
	M[0][2] = mx0 * _02 + mx1 * _12 + mx2 * _22 + M[0][3] * _32;
	M[0][3] = mx0 * _03 + mx1 * _13 + mx2 * _23 + M[0][3] * _33;

	mx0 = M[1][0];
	mx1 = M[1][1];
	mx2 = M[1][2];
	M[1][0] = mx0 * _00 + mx1 * _10 + mx2 * _20 + M[1][3] * _30;
	M[1][1] = mx0 * _01 + mx1 * _11 + mx2 * _21 + M[1][3] * _31;
	M[1][2] = mx0 * _02 + mx1 * _12 + mx2 * _22 + M[1][3] * _32;
	M[1][3] = mx0 * _03 + mx1 * _13 + mx2 * _23 + M[1][3] * _33;

	mx0 = M[2][0];
	mx1 = M[2][1];
	mx2 = M[2][2];
	M[2][0] = mx0 * _00 + mx1 * _10 + mx2 * _20 + M[2][3] * _30;
	M[2][1] = mx0 * _01 + mx1 * _11 + mx2 * _21 + M[2][3] * _31;
	M[2][2] = mx0 * _02 + mx1 * _12 + mx2 * _22 + M[2][3] * _32;
	M[2][3] = mx0 * _03 + mx1 * _13 + mx2 * _23 + M[2][3] * _33;

	mx0 = M[3][0];
	mx1 = M[3][1];
	mx2 = M[3][2];
	M[3][0] = mx0 * _00 + mx1 * _10 + mx2 * _20 + M[3][3] * _30;
	M[3][1] = mx0 * _01 + mx1 * _11 + mx2 * _21 + M[3][3] * _31;
	M[3][2] = mx0 * _02 + mx1 * _12 + mx2 * _22 + M[3][3] * _32;
	M[3][3] = mx0 * _03 + mx1 * _13 + mx2 * _23 + M[3][3] * _33;
	*/

	/* 0 を乗算してるところと、*1 で自分自身代入しているところを切る
	lfloat mx0 = M[0][0];
	lfloat mx1 = M[0][1];
	lfloat mx2 = M[0][2];
	M[0][0] = mx0 * _00 + mx1 * _10 + mx2 * _20;
	M[0][1] = mx0 * _01 + mx1 * _11 + mx2 * _21;
	M[0][2] = mx0 * _02 + mx1 * _12 + mx2 * _22;

	mx0 = M[1][0];
	mx1 = M[1][1];
	mx2 = M[1][2];
	M[1][0] = mx0 * _00 + mx1 * _10 + mx2 * _20;
	M[1][1] = mx0 * _01 + mx1 * _11 + mx2 * _21;
	M[1][2] = mx0 * _02 + mx1 * _12 + mx2 * _22;

	mx0 = M[2][0];
	mx1 = M[2][1];
	mx2 = M[2][2];
	M[2][0] = mx0 * _00 + mx1 * _10 + mx2 * _20;
	M[2][1] = mx0 * _01 + mx1 * _11 + mx2 * _21;
	M[2][2] = mx0 * _02 + mx1 * _12 + mx2 * _22;

	mx0 = M[3][0];
	mx1 = M[3][1];
	mx2 = M[3][2];
	M[3][0] = mx0 * _00 + mx1 * _10 + mx2 * _20;
	M[3][1] = mx0 * _01 + mx1 * _11 + mx2 * _21;
	M[3][2] = mx0 * _02 + mx1 * _12 + mx2 * _22;
	*/

	// ※古いコードのなので変数名が 0 スタートになっている点に注意
	float _00 = (axis.X * axis.X) * mc + c;
	float _01 = (axis.X * axis.Y) * mc + (axis.Z * s);
	float _02 = (axis.X * axis.Z) * mc - (axis.Y * s);

	float _10 = (axis.X * axis.Y) * mc - (axis.Z * s);
	float _11 = (axis.Y * axis.Y) * mc + c;
	float _12 = (axis.Y * axis.Z) * mc + (axis.X * s);

	float _20 = (axis.X * axis.Z) * mc + (axis.Y * s);
	float _21 = (axis.Y * axis.Z) * mc - (axis.X * s);
	float _22 = (axis.Z * axis.Z) * mc + c;

	float mx0 = M[0][0];
	float mx1 = M[0][1];
	M[0][0] = mx0 * _00 + mx1 * _10 + M[0][2] * _20;
	M[0][1] = mx0 * _01 + mx1 * _11 + M[0][2] * _21;
	M[0][2] = mx0 * _02 + mx1 * _12 + M[0][2] * _22;

	mx0 = M[1][0];
	mx1 = M[1][1];
	M[1][0] = mx0 * _00 + mx1 * _10 + M[1][2] * _20;
	M[1][1] = mx0 * _01 + mx1 * _11 + M[1][2] * _21;
	M[1][2] = mx0 * _02 + mx1 * _12 + M[1][2] * _22;

	mx0 = M[2][0];
	mx1 = M[2][1];
	M[2][0] = mx0 * _00 + mx1 * _10 + M[2][2] * _20;
	M[2][1] = mx0 * _01 + mx1 * _11 + M[2][2] * _21;
	M[2][2] = mx0 * _02 + mx1 * _12 + M[2][2] * _22;

	mx0 = M[3][0];
	mx1 = M[3][1];
	M[3][0] = mx0 * _00 + mx1 * _10 + M[3][2] * _20;
	M[3][1] = mx0 * _01 + mx1 * _11 + M[3][2] * _21;
	M[3][2] = mx0 * _02 + mx1 * _12 + M[3][2] * _22;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Matrix::RotateQuaternion(const Quaternion& q)
{
	float xx = q.X * q.X;
	float yy = q.Y * q.Y;
	float zz = q.Z * q.Z;
	float xy = q.X * q.Y;
	float zw = q.Z * q.W;
	float zx = q.Z * q.X;
	float yw = q.Y * q.W;
	float yz = q.Y * q.Z;
	float xw = q.X * q.W;

	// ※古いコードのなので変数名が 0 スタートになっている点に注意
	float _00 = 1.0f - (2.0f * (yy + zz));
	float _01 = 2.0f * (xy + zw);
	float _02 = 2.0f * (zx - yw);

	float _10 = 2.0f * (xy - zw);
	float _11 = 1.0f - (2.0f * (zz + xx));
	float _12 = 2.0f * (yz + xw);

	float _20 = 2.0f * (zx + yw);
	float _21 = 2.0f * (yz - xw);
	float _22 = 1.0f - (2.0f * (yy + xx));

	float mx0 = M[0][0];
	float mx1 = M[0][1];
	M[0][0] = mx0 * _00 + mx1 * _10 + M[0][2] * _20;
	M[0][1] = mx0 * _01 + mx1 * _11 + M[0][2] * _21;
	M[0][2] = mx0 * _02 + mx1 * _12 + M[0][2] * _22;

	mx0 = M[1][0];
	mx1 = M[1][1];
	M[1][0] = mx0 * _00 + mx1 * _10 + M[1][2] * _20;
	M[1][1] = mx0 * _01 + mx1 * _11 + M[1][2] * _21;
	M[1][2] = mx0 * _02 + mx1 * _12 + M[1][2] * _22;

	mx0 = M[2][0];
	mx1 = M[2][1];
	M[2][0] = mx0 * _00 + mx1 * _10 + M[2][2] * _20;
	M[2][1] = mx0 * _01 + mx1 * _11 + M[2][2] * _21;
	M[2][2] = mx0 * _02 + mx1 * _12 + M[2][2] * _22;

	mx0 = M[3][0];
	mx1 = M[3][1];
	M[3][0] = mx0 * _00 + mx1 * _10 + M[3][2] * _20;
	M[3][1] = mx0 * _01 + mx1 * _11 + M[3][2] * _21;
	M[3][2] = mx0 * _02 + mx1 * _12 + M[3][2] * _22;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Matrix::Scale(float x, float y, float z)
{
	M[0][0] *= x;
	M[0][1] *= y;
	M[0][2] *= z;
	M[1][0] *= x;
	M[1][1] *= y;
	M[1][2] *= z;
	M[2][0] *= x;
	M[2][1] *= y;
	M[2][2] *= z;
	M[3][0] *= x;
	M[3][1] *= y;
	M[3][2] *= z;
}

void Matrix::Scale(const Vector3& vec)
{
	Scale(vec.X, vec.Y, vec.Z);
}

void Matrix::Scale(float xyz)
{
	Scale(xyz, xyz, xyz);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Matrix::Inverse()
{
	(*this) = Matrix::Inverse(*this);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void  Matrix::Transpose()
{
	(*this) = Matrix::Transpose(*this);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Matrix::Decompose(Vector3* scale, Quaternion* rot, Vector3* trans) const
{
	Matrix scaleMat;
	Matrix rotMat;
	Matrix transMat;
	DecomposeMatrices(&scaleMat, &rotMat, &transMat);
	if (scale) {
		scale->Set(scaleMat.M[0][0], scaleMat.M[1][1], scaleMat.M[2][2]);
	}
	if (rot) {
		*rot = Quaternion::RotationMatrix(rotMat);
	}
	if (trans) {
		trans->Set(transMat.M[3][0], transMat.M[3][1], transMat.M[3][2]);
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Matrix::DecomposeMatrices(Matrix* scale, Matrix* rot, Matrix* trans) const
{
	if (trans)
	{
		*trans = Matrix::Translation(GetPosition());
	}

	Vector3 sc(
		sqrtf(M[0][0] * M[0][0] + M[0][1] * M[0][1] + M[0][2] * M[0][2]),
		sqrtf(M[1][0] * M[1][0] + M[1][1] * M[1][1] + M[1][2] * M[1][2]),
		sqrtf(M[2][0] * M[2][0] + M[2][1] * M[2][1] + M[2][2] * M[2][2]));
	if (scale)
	{
		*scale = Matrix::Scaling(sc);
	}

	if (rot)
	{
		*rot = Identity;
		rot->M[0][0] = (sc.X != 0.0f) ? M[0][0] / sc.X : 0.0f;
		rot->M[0][1] = (sc.X != 0.0f) ? M[0][1] / sc.X : 0.0f;
		rot->M[0][2] = (sc.X != 0.0f) ? M[0][2] / sc.X : 0.0f;
		rot->M[0][3] = 0.0f;
		rot->M[1][0] = (sc.Y != 0.0f) ? M[1][0] / sc.Y : 0.0f;
		rot->M[1][1] = (sc.Y != 0.0f) ? M[1][1] / sc.Y : 0.0f;
		rot->M[1][2] = (sc.Y != 0.0f) ? M[1][2] / sc.Y : 0.0f;
		rot->M[1][3] = 0.0f;
		rot->M[2][0] = (sc.Z != 0.0f) ? M[2][0] / sc.Z : 0.0f;
		rot->M[2][1] = (sc.Z != 0.0f) ? M[2][1] / sc.Z : 0.0f;
		rot->M[2][2] = (sc.Z != 0.0f) ? M[2][2] / sc.Z : 0.0f;
		rot->M[2][3] = 0.0f;
		rot->M[3][0] = 0.0f;
		rot->M[3][1] = 0.0f;
		rot->M[3][2] = 0.0f;
		rot->M[3][3] = 1.0f;
	}
}
#if 0
void Matrix::Decompose(Vector3* scale, Matrix* rot, Vector3* trans)
{
	if (trans)
	{
		trans->Set(M[3][0], M[3][1], M[3][2]);
	}

	Vector3 sc(
		sqrtf(M[0][0] * M[0][0] + M[0][1] * M[0][1] + M[0][2] * M[0][2]),
		sqrtf(M[1][0] * M[1][0] + M[1][1] * M[1][1] + M[1][2] * M[1][2]),
		sqrtf(M[2][0] * M[2][0] + M[2][1] * M[2][1] + M[2][2] * M[2][2]));
	if (scale)
	{
		*scale = sc;
	}

	if (rot)
	{
		rot->M[0][0] = (sc.X != 0.0f) ? M[0][0] / sc.X : 0.0f;
		rot->M[0][1] = (sc.X != 0.0f) ? M[0][1] / sc.X : 0.0f;
		rot->M[0][2] = (sc.X != 0.0f) ? M[0][2] / sc.X : 0.0f;
		rot->M[0][3] = 0.0f;
		rot->M[1][0] = (sc.Y != 0.0f) ? M[1][0] / sc.Y : 0.0f;
		rot->M[1][1] = (sc.Y != 0.0f) ? M[1][1] / sc.Y : 0.0f;
		rot->M[1][2] = (sc.Y != 0.0f) ? M[1][2] / sc.Y : 0.0f;
		rot->M[1][3] = 0.0f;
		rot->M[2][0] = (sc.Z != 0.0f) ? M[2][0] / sc.Z : 0.0f;
		rot->M[2][1] = (sc.Z != 0.0f) ? M[2][1] / sc.Z : 0.0f;
		rot->M[2][2] = (sc.Z != 0.0f) ? M[2][2] / sc.Z : 0.0f;
		rot->M[2][3] = 0.0f;
		rot->M[3][0] = 0.0f;
		rot->M[3][1] = 0.0f;
		rot->M[3][2] = 0.0f;
		rot->M[3][3] = 1.0f;
	}
}

void Matrix::Decompose(Vector3* scale, Quaternion* rot, Vector3* trans) const
{
	if (rot)
	{
		Matrix rotMat;
		Decompose(scale, &rotMat, trans);
		*rot =
	}
	else
	{
		Decompose(scale, NULL, trans);
	}
}
#endif

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Matrix::Print(const char* format, FILE* stream) const
{
	if (!format) {
		format = "%f, %f, %f, %f,\n%f, %f, %f, %f,\n%f, %f, %f, %f,\n%f, %f, %f, %f\n";
	}
	if (!stream) {
		stream = stdout;
	}

	fprintf(stream, format,
		M[0][0], M[0][1], M[0][2], M[0][3],
		M[1][0], M[1][1], M[1][2], M[1][3],
		M[2][0], M[2][1], M[2][2], M[2][3],
		M[3][0], M[3][1], M[3][2], M[3][3]);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
static bool EulerAnglesXYZ(const Matrix& mat, float* xRot, float* yRot, float* zRot)
{
	static const float Threshold = 0.0001f;
	
	if (mat.M[0][2] > 1.0f - Threshold || mat.M[0][2] < -1.0f + Threshold)	// ジンバルロック判定
	{
		*xRot = 0.0f;
		*yRot = (mat.M[0][2] < 0 ? Math::PIOver2 : -Math::PIOver2);
		*zRot = -atan2(-mat.M[1][0], mat.M[1][1]);
		return false;
	}

	*yRot = -asin(mat.M[0][2]);
	*xRot = asin(mat.M[1][2] / cos(*yRot));

	if (Math::IsNaN(*xRot))	// ジンバルロック判定
	{
		*xRot = 0.0f;
		*yRot = (mat.M[0][2] < 0 ? Math::PIOver2 : -Math::PIOver2);
		*zRot = -atan2(-mat.M[1][0], mat.M[1][1]);
		return false;
	}

	if (mat.M[2][2] < 0.0f) {
		*xRot = Math::PI - (*xRot);
	}

	*zRot = atan2(mat.M[0][1], mat.M[0][0]);
	return true;
}

static bool EulerAnglesYZX(const Matrix& mat, float* xRot, float* yRot, float* zRot)
{
	static const float Threshold = 0.0001f;

	if (mat.M[1][0] > 1.0f - Threshold || mat.M[1][0] < -1.0f + Threshold)	// ジンバルロック判定
	{
		*xRot = -atan2(-mat.M[2][1], mat.M[2][2]);
		*yRot = 0.0f;
		*zRot = (mat.M[1][0] < 0 ? Math::PIOver2 : -Math::PIOver2);
		return false;
	}

	*zRot = -asin(mat.M[1][0]);
	*yRot = asin(mat.M[2][0] / cos(*zRot));

	if (Math::IsNaN(*yRot))	// ジンバルロック判定
	{
		*xRot = -atan2(-mat.M[2][1], mat.M[2][2]);
		*yRot = 0.0f;
		*zRot = (mat.M[1][0] < 0 ? Math::PIOver2 : -Math::PIOver2);
		return false;
	}

	if (mat.M[0][0] < 0.0f) {
		*yRot = Math::PI - (*yRot);
	}

	*xRot = atan2(mat.M[1][2], mat.M[1][1]);
	return true;
}

static bool EulerAnglesZXY(const Matrix& mat, float* xRot, float* yRot, float* zRot)
{
	static const float Threshold = 0.0001f;
	//if (locked) { *locked = true; }		// 最初にジンバルロック発生状態にしておく

	if (mat.M[2][1] > 1.0f - Threshold || mat.M[2][1]  < -1.0f + Threshold)	// ジンバルロック判定
	{
		*xRot = (mat.M[2][1] < 0 ? Math::PIOver2 : -Math::PIOver2);
		*yRot = (float)atan2f(-mat.M[0][2], mat.M[0][0]);
		*zRot = 0.0f;
		return false;
	}

	*xRot = -(float)asinf(mat.M[2][1]);
	*zRot = (float)asinf(mat.M[0][1] / cosf(*xRot));

	if (Math::IsNaN(*zRot))	// ジンバルロック判定
	{
		*xRot = (mat.M[2][1] < 0 ? Math::PIOver2 : -Math::PIOver2);
		*yRot = (float)atan2f(-mat.M[0][2], mat.M[0][0]);
		*zRot = 0.0f;
		return false;
	}
	if (mat.M[1][1] < 0.0f) {
		*zRot = Math::PI - (*zRot);
	}

	*yRot = (float)atan2f(mat.M[2][0], mat.M[2][2]);

	//if (locked) { *locked = false; }	// ジンバルロックは発生しなかった
	//return Vector3(xRot, yRot, zRot);
	return true;
}

Vector3 Matrix::ToEulerAngles(RotationOrder order, bool* locked_) const
{
	bool locked;
	float xRot, yRot, zRot;
	switch (order)
	{
	case RotationOrder_XYZ:
		locked = EulerAnglesXYZ(*this, &xRot, &yRot, &zRot);
		break;
	//case RotationOrder_XZY: break;
	//case RotationOrder_YXZ: break;
	case RotationOrder_YZX:
		locked = EulerAnglesYZX(*this, &xRot, &yRot, &zRot);
		break;
	case RotationOrder_ZXY:
		locked = EulerAnglesZXY(*this, &xRot, &yRot, &zRot);	// RotationYawPitchRoll
		break;
	//case RotationOrder_ZYX: break;
	default:
		assert(0);
		return Vector3();
	}

	if (locked_ != NULL) { *locked_ = locked; }
	return Vector3(xRot, yRot, zRot);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Matrix Matrix::GetRotationMatrix() const
{
	return Matrix(
		M[0][0], M[0][1], M[0][2], 0.0f,
		M[1][0], M[1][1], M[1][2], 0.0f,
		M[2][0], M[2][1], M[2][2], 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
bool Matrix::IsNaNOrInf() const
{
	return
		Math::IsNaNOrInf(M[0][0]) || Math::IsNaNOrInf(M[0][1]) || Math::IsNaNOrInf(M[0][2]) || Math::IsNaNOrInf(M[0][3]) ||
		Math::IsNaNOrInf(M[1][0]) || Math::IsNaNOrInf(M[1][1]) || Math::IsNaNOrInf(M[1][2]) || Math::IsNaNOrInf(M[1][3]) ||
		Math::IsNaNOrInf(M[2][0]) || Math::IsNaNOrInf(M[2][1]) || Math::IsNaNOrInf(M[2][2]) || Math::IsNaNOrInf(M[2][3]) ||
		Math::IsNaNOrInf(M[3][0]) || Math::IsNaNOrInf(M[3][1]) || Math::IsNaNOrInf(M[3][2]) || Math::IsNaNOrInf(M[3][3]);
}

//-----------------------------------------------------------------------------
// static
//-----------------------------------------------------------------------------
Matrix Matrix::Multiply(const Matrix& mat1, const Matrix& mat2)
{
	Matrix out;

	out.M[0][0] = mat1.M[0][0] * mat2.M[0][0] + mat1.M[0][1] * mat2.M[1][0] + mat1.M[0][2] * mat2.M[2][0] + mat1.M[0][3] * mat2.M[3][0];
	out.M[0][1] = mat1.M[0][0] * mat2.M[0][1] + mat1.M[0][1] * mat2.M[1][1] + mat1.M[0][2] * mat2.M[2][1] + mat1.M[0][3] * mat2.M[3][1];
	out.M[0][2] = mat1.M[0][0] * mat2.M[0][2] + mat1.M[0][1] * mat2.M[1][2] + mat1.M[0][2] * mat2.M[2][2] + mat1.M[0][3] * mat2.M[3][2];
	out.M[0][3] = mat1.M[0][0] * mat2.M[0][3] + mat1.M[0][1] * mat2.M[1][3] + mat1.M[0][2] * mat2.M[2][3] + mat1.M[0][3] * mat2.M[3][3];

	out.M[1][0] = mat1.M[1][0] * mat2.M[0][0] + mat1.M[1][1] * mat2.M[1][0] + mat1.M[1][2] * mat2.M[2][0] + mat1.M[1][3] * mat2.M[3][0];
	out.M[1][1] = mat1.M[1][0] * mat2.M[0][1] + mat1.M[1][1] * mat2.M[1][1] + mat1.M[1][2] * mat2.M[2][1] + mat1.M[1][3] * mat2.M[3][1];
	out.M[1][2] = mat1.M[1][0] * mat2.M[0][2] + mat1.M[1][1] * mat2.M[1][2] + mat1.M[1][2] * mat2.M[2][2] + mat1.M[1][3] * mat2.M[3][2];
	out.M[1][3] = mat1.M[1][0] * mat2.M[0][3] + mat1.M[1][1] * mat2.M[1][3] + mat1.M[1][2] * mat2.M[2][3] + mat1.M[1][3] * mat2.M[3][3];

	out.M[2][0] = mat1.M[2][0] * mat2.M[0][0] + mat1.M[2][1] * mat2.M[1][0] + mat1.M[2][2] * mat2.M[2][0] + mat1.M[2][3] * mat2.M[3][0];
	out.M[2][1] = mat1.M[2][0] * mat2.M[0][1] + mat1.M[2][1] * mat2.M[1][1] + mat1.M[2][2] * mat2.M[2][1] + mat1.M[2][3] * mat2.M[3][1];
	out.M[2][2] = mat1.M[2][0] * mat2.M[0][2] + mat1.M[2][1] * mat2.M[1][2] + mat1.M[2][2] * mat2.M[2][2] + mat1.M[2][3] * mat2.M[3][2];
	out.M[2][3] = mat1.M[2][0] * mat2.M[0][3] + mat1.M[2][1] * mat2.M[1][3] + mat1.M[2][2] * mat2.M[2][3] + mat1.M[2][3] * mat2.M[3][3];

	out.M[3][0] = mat1.M[3][0] * mat2.M[0][0] + mat1.M[3][1] * mat2.M[1][0] + mat1.M[3][2] * mat2.M[2][0] + mat1.M[3][3] * mat2.M[3][0];
	out.M[3][1] = mat1.M[3][0] * mat2.M[0][1] + mat1.M[3][1] * mat2.M[1][1] + mat1.M[3][2] * mat2.M[2][1] + mat1.M[3][3] * mat2.M[3][1];
	out.M[3][2] = mat1.M[3][0] * mat2.M[0][2] + mat1.M[3][1] * mat2.M[1][2] + mat1.M[3][2] * mat2.M[2][2] + mat1.M[3][3] * mat2.M[3][2];
	out.M[3][3] = mat1.M[3][0] * mat2.M[0][3] + mat1.M[3][1] * mat2.M[1][3] + mat1.M[3][2] * mat2.M[2][3] + mat1.M[3][3] * mat2.M[3][3];

	return out;
}

//-----------------------------------------------------------------------------
// static
//-----------------------------------------------------------------------------
Matrix Matrix::Translation(float x, float y, float z)
{
	return Matrix(
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		x, y, z, 1.0f);
}

//-----------------------------------------------------------------------------
// static
//-----------------------------------------------------------------------------
Matrix Matrix::Translation(const Vector3& vec)
{
	return Matrix(
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		vec.X, vec.Y, vec.Z, 1.0f);
}

//-----------------------------------------------------------------------------
// static
//-----------------------------------------------------------------------------
Matrix Matrix::RotationX(float r)
{
	float c, s;
	Asm::sincos(r, &s, &c);
	return Matrix(
		1.0f,	0.0f,	0.0f,	0.0f,
		0.0f,	c,		s,		0.0f,
		0.0f,	-s,		c,		0.0f,
		0.0f,	0.0f,	0.0f,	1.0f);
}

//-----------------------------------------------------------------------------
// static
//-----------------------------------------------------------------------------
Matrix Matrix::RotationY(float r)
{
	float c, s;
	Asm::sincos(r, &s, &c);
	return Matrix(
		c,		0.0f,	-s,		0.0f,
		0.0f,	1.0f,	0.0f,	0.0f,
		s,		0.0f,	c,		0.0f,
		0.0f,	0.0f,	0.0f,	1.0f);
}

//-----------------------------------------------------------------------------
// static
//-----------------------------------------------------------------------------
Matrix Matrix::RotationZ(float r)
{
	float c, s;
	Asm::sincos(r, &s, &c);
	return Matrix(
		c,		s,		0.0f,	0.0f,
		-s,		c,		0.0f,	0.0f,
		0.0f,	0.0f,	1.0f,	0.0f,
		0.0f,	0.0f,	0.0f,	1.0f);
}

//-----------------------------------------------------------------------------
// static
//-----------------------------------------------------------------------------
Matrix Matrix::RotationAxis(const Vector3& axis_, float r)
{
	Vector3 axis = axis_;
	if (axis.GetLengthSquared() != 1.0f) {
		axis.Normalize();
	}

	float s, c;
	Asm::sincos(r, &s, &c);
	float mc = 1.0f - c;

	return Matrix(
		(axis.X * axis.X) * mc + c,				(axis.X * axis.Y) * mc + (axis.Z * s),	(axis.X * axis.Z) * mc - (axis.Y * s),	0.0f,
		(axis.X * axis.Y) * mc - (axis.Z * s),	(axis.Y * axis.Y) * mc + c,				(axis.Y * axis.Z) * mc + (axis.X * s),	0.0f,
		(axis.X * axis.Z) * mc + (axis.Y * s),	(axis.Y * axis.Z) * mc - (axis.X * s),	(axis.Z * axis.Z) * mc + c,				0.0f,
		0.0f, 0.0f, 0.0f, 1.0f);
}

//-----------------------------------------------------------------------------
// static
//-----------------------------------------------------------------------------
Matrix Matrix::RotationQuaternion(const Quaternion& qua)
{
	float xx = qua.X * qua.X;
	float yy = qua.Y * qua.Y;
	float zz = qua.Z * qua.Z;
	float xy = qua.X * qua.Y;
	float zw = qua.Z * qua.W;
	float zx = qua.Z * qua.X;
	float yw = qua.Y * qua.W;
	float yz = qua.Y * qua.Z;
	float xw = qua.X * qua.W;

	return Matrix(
		1.0f - (2.0f * (yy + zz)),	2.0f * (xy + zw),			2.0f * (zx - yw),			0.0f,
		2.0f * (xy - zw),			1.0f - (2.0f * (zz + xx)),	2.0f * (yz + xw),			0.0f,
		2.0f * (zx + yw),			2.0f * (yz - xw),			1.0f - (2.0f * (yy + xx)),	0.0f,
		0.0f, 0.0f, 0.0f, 1.0f);
}

//-----------------------------------------------------------------------------
// static
//-----------------------------------------------------------------------------
Matrix Matrix::RotationEulerAngles(const Vector3& angles, RotationOrder order)
{
	Matrix m;
	switch (order)
	{
	case RotationOrder_XYZ:
		m.RotateX(angles.X); m.RotateY(angles.Y); m.RotateZ(angles.Z); break;
	//case RotationOrder_XZY:
	//	m.RotateX(angles.X); m.RotateZ(angles.Z); m.RotateY(angles.Y); break;
	//case RotationOrder_YXZ:
	//	m.RotateY(angles.Y); m.RotateX(angles.X); m.RotateZ(angles.Z); break;
	case RotationOrder_YZX:
		m.RotateY(angles.Y); m.RotateZ(angles.Z); m.RotateX(angles.X); break;
	case RotationOrder_ZXY:
		m.RotateZ(angles.Z); m.RotateX(angles.X); m.RotateY(angles.Y); break;	// RotationYawPitchRoll
	//case RotationOrder_ZYX:
	//	m.RotateZ(angles.Z); m.RotateY(angles.Y); m.RotateX(angles.X); break;
	default:
		assert(0);
		return m;
	}
	return m;
}

//-----------------------------------------------------------------------------
// static
//-----------------------------------------------------------------------------
Matrix Matrix::RotationEulerAngles(float x, float y, float z, RotationOrder order)
{
	Matrix m;
	switch (order)
	{
	case RotationOrder_XYZ:
		m.RotateX(x); m.RotateY(y); m.RotateZ(z); break;
	case RotationOrder_YZX:
		m.RotateY(y); m.RotateZ(z); m.RotateX(y); break;
	case RotationOrder_ZXY:
		m.RotateZ(z); m.RotateX(x); m.RotateY(y); break;	// RotationYawPitchRoll
	default:
		assert(0);
		return m;
	}
	return m;
}

//-----------------------------------------------------------------------------
// static
//-----------------------------------------------------------------------------
Matrix Matrix::RotationYawPitchRoll(float yaw, float pitch, float roll)
{
	Quaternion q = Quaternion::RotationYawPitchRoll(yaw, pitch, roll);
	return Matrix::RotationQuaternion(q);
}

//-----------------------------------------------------------------------------
// static
//-----------------------------------------------------------------------------
Matrix Matrix::Scaling(float x, float y, float z)
{
	return Matrix(
		x, 0.0f, 0.0f, 0.0f,
		0.0f, y, 0.0f, 0.0f,
		0.0f, 0.0f, z, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f);
}

//-----------------------------------------------------------------------------
// static
//-----------------------------------------------------------------------------
Matrix Matrix::Scaling(const Vector3& vec)
{
	return Matrix(
		vec.X, 0.0f, 0.0f, 0.0f,
		0.0f, vec.Y, 0.0f, 0.0f,
		0.0f, 0.0f, vec.Z, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f);
}

//-----------------------------------------------------------------------------
// static
//-----------------------------------------------------------------------------
Matrix Matrix::Scaling(float xyz)
{
	return Matrix(
		xyz, 0.0f, 0.0f, 0.0f,
		0.0f, xyz, 0.0f, 0.0f,
		0.0f, 0.0f, xyz, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f);
}

//-----------------------------------------------------------------------------
// static
//-----------------------------------------------------------------------------
Matrix Matrix::Inverse(const Matrix& mat)
{
#if 1	// 速度は D3DXMatrixInverse の 2～3倍 (100000 回で 3000us。コンストラクタとかが足引っ張ってそうな気がする…)
	float coef00 = mat.M[2][2] * mat.M[3][3] - mat.M[3][2] * mat.M[2][3];
	float coef02 = mat.M[1][2] * mat.M[3][3] - mat.M[3][2] * mat.M[1][3];
	float coef03 = mat.M[1][2] * mat.M[2][3] - mat.M[2][2] * mat.M[1][3];

	float coef04 = mat.M[2][1] * mat.M[3][3] - mat.M[3][1] * mat.M[2][3];
	float coef06 = mat.M[1][1] * mat.M[3][3] - mat.M[3][1] * mat.M[1][3];
	float coef07 = mat.M[1][1] * mat.M[2][3] - mat.M[2][1] * mat.M[1][3];

	float coef08 = mat.M[2][1] * mat.M[3][2] - mat.M[3][1] * mat.M[2][2];
	float coef10 = mat.M[1][1] * mat.M[3][2] - mat.M[3][1] * mat.M[1][2];
	float coef11 = mat.M[1][1] * mat.M[2][2] - mat.M[2][1] * mat.M[1][2];

	float coef12 = mat.M[2][0] * mat.M[3][3] - mat.M[3][0] * mat.M[2][3];
	float coef14 = mat.M[1][0] * mat.M[3][3] - mat.M[3][0] * mat.M[1][3];
	float coef15 = mat.M[1][0] * mat.M[2][3] - mat.M[2][0] * mat.M[1][3];

	float coef16 = mat.M[2][0] * mat.M[3][2] - mat.M[3][0] * mat.M[2][2];
	float coef18 = mat.M[1][0] * mat.M[3][2] - mat.M[3][0] * mat.M[1][2];
	float coef19 = mat.M[1][0] * mat.M[2][2] - mat.M[2][0] * mat.M[1][2];

	float coef20 = mat.M[2][0] * mat.M[3][1] - mat.M[3][0] * mat.M[2][1];
	float coef22 = mat.M[1][0] * mat.M[3][1] - mat.M[3][0] * mat.M[1][1];
	float coef23 = mat.M[1][0] * mat.M[2][1] - mat.M[2][0] * mat.M[1][1];

	Vector4 fac0(coef00, coef00, coef02, coef03);
	Vector4 fac1(coef04, coef04, coef06, coef07);
	Vector4 fac2(coef08, coef08, coef10, coef11);
	Vector4 fac3(coef12, coef12, coef14, coef15);
	Vector4 fac4(coef16, coef16, coef18, coef19);
	Vector4 fac5(coef20, coef20, coef22, coef23);

	Vector4 vec0(mat.M[1][0], mat.M[0][0], mat.M[0][0], mat.M[0][0]);
	Vector4 vec1(mat.M[1][1], mat.M[0][1], mat.M[0][1], mat.M[0][1]);
	Vector4 vec2(mat.M[1][2], mat.M[0][2], mat.M[0][2], mat.M[0][2]);
	Vector4 vec3(mat.M[1][3], mat.M[0][3], mat.M[0][3], mat.M[0][3]);

	Vector4 inv0(vec1 * fac0 - vec2 * fac1 + vec3 * fac2);
	Vector4 inv1(vec0 * fac0 - vec2 * fac3 + vec3 * fac4);
	Vector4 inv2(vec0 * fac1 - vec1 * fac3 + vec3 * fac5);
	Vector4 inv3(vec0 * fac2 - vec1 * fac4 + vec2 * fac5);

	Vector4 signA(+1, -1, +1, -1);
	Vector4 signB(-1, +1, -1, +1);
	Matrix inverse(inv0 * signA, inv1 * signB, inv2 * signA, inv3 * signB);

	Vector4 dot0(mat.M[0][0] * inverse.M[0][0], mat.M[0][1] * inverse.M[1][0], mat.M[0][2] * inverse.M[2][0], mat.M[0][3] * inverse.M[3][0]);
	float dot1 = (dot0.X + dot0.Y) + (dot0.Z + dot0.W);

	if (dot1 == 0.0)
	{
		// 計算できない。D3D に合わせて、単位行列で返す
		return Identity;
	}

	float oneOverDeterminant = 1.0f / dot1;

	return inverse * oneOverDeterminant;
#endif
#if 0	// 速度は D3DXMatrixInverse の 10倍
	int i, j;
	float buf;

	Matrix	tm(mat_);
	identity(out_);

	// 掃き出し法
	for (i = 0; i < 4; ++i)
	{
		//if (tm.M[ i ][ i ] != 0)
		buf = 1.0f / tm.M[i][i];	// 0除算の可能性がある
		//else
		//	buf = 0.0;
		for (j = 0; j < 4; ++j)
		{
			tm.M[i][j] *= buf;
			out_->M[i][j] *= buf;
		}
		for (j = 0; j < 4; ++j)
		{
			if (i != j)
			{
				buf = tm.M[j][i];
				for (int k = 0; k < 4; ++k)
				{
					tm.M[j][k] -= tm.M[i][k] * buf;
					out_->M[j][k] -= out_->M[i][k] * buf;
				}
			}
		}
	}
#endif
#if 0
	//D3DXMATRIX dxmout;
	D3DXMatrixInverse((D3DXMATRIX*)out_, NULL, (D3DXMATRIX*)&mat_);
	//return Matrix( *((Matrix*)&dxmout) );
	//return Matrix();
#endif
#if 0
	// http://www.cg.info.hiroshima-cu.ac.jp/~miyazaki/knowledge/tech23.html

	float m0011 = mat_.M[0][0] * mat_.M[1][1];
	float m0012 = mat_.M[0][0] * mat_.M[1][2];
	float m0021 = mat_.M[0][0] * mat_.M[2][1];
	float m0022 = mat_.M[0][0] * mat_.M[2][2];

	float m0110 = mat_.M[0][1] * mat_.M[1][0];
	float m0112 = mat_.M[0][1] * mat_.M[1][2];
	float m0120 = mat_.M[0][1] * mat_.M[2][0];
	float m0122 = mat_.M[0][1] * mat_.M[2][2];

	float m0210 = mat_.M[0][2] * mat_.M[1][0];
	float m0211 = mat_.M[0][2] * mat_.M[1][1];
	float m0220 = mat_.M[0][2] * mat_.M[2][0];
	float m0221 = mat_.M[0][2] * mat_.M[2][1];

	float m1021 = mat_.M[1][0] * mat_.M[2][1];
	float m1022 = mat_.M[1][0] * mat_.M[2][2];

	float m1120 = mat_.M[1][1] * mat_.M[2][0];
	float m1122 = mat_.M[1][1] * mat_.M[2][2];

	float m1220 = mat_.M[1][2] * mat_.M[2][0];
	float m1221 = mat_.M[1][2] * mat_.M[2][1];


	float m1122_m1221 = m1122 - m1221;
	float m1220_m1022 = m1220 - m1022;
	float m1021_m1120 = m1021 - m1120;


	float delta = mat_.M[0][0] * (m1122_m1221)+mat_.M[0][1] * (m1220_m1022)+mat_.M[0][2] * (m1021_m1120);
	float rcpDelta = 1.f / delta;

	Matrix mat;

	mat.M[0][0] = (m1122_m1221)* rcpDelta;
	mat.M[1][0] = (m1220_m1022)* rcpDelta;
	mat.M[2][0] = (m1021_m1120)* rcpDelta;
	mat.M[0][1] = (m0221 - m0122) * rcpDelta;
	mat.M[1][1] = (m0022 - m0220) * rcpDelta;
	mat.M[2][1] = (m0120 - m0021) * rcpDelta;
	mat.M[0][2] = (m0112 - m0211) * rcpDelta;
	mat.M[1][2] = (m0210 - m0012) * rcpDelta;
	mat.M[2][2] = (m0011 - m0110) * rcpDelta;

	float t03 = mat_.M[0][3];
	float t13 = mat_.M[1][3];
	mat.M[0][3] = -(mat.M[0][0] * t03 + mat.M[0][1] * t13 + mat.M[0][2] * mat_.M[2][3]);
	mat.M[1][3] = -(mat.M[1][0] * t03 + mat.M[1][1] * t13 + mat.M[1][2] * mat_.M[2][3]);
	mat.M[2][3] = -(mat.M[2][0] * t03 + mat.M[2][1] * t13 + mat.M[2][2] * mat_.M[2][3]);

	return mat;
#endif
}

//-----------------------------------------------------------------------------
// static
//-----------------------------------------------------------------------------
Matrix Matrix::Transpose(const Matrix& mat)
{
	return Matrix(
		mat.M[0][0], mat.M[1][0], mat.M[2][0], mat.M[3][0],
		mat.M[0][1], mat.M[1][1], mat.M[2][1], mat.M[3][1],
		mat.M[0][2], mat.M[1][2], mat.M[2][2], mat.M[3][2],
		mat.M[0][3], mat.M[1][3], mat.M[2][3], mat.M[3][3]);
}

//-----------------------------------------------------------------------------
// static
//-----------------------------------------------------------------------------
Matrix Matrix::Reflection(const Plane& plane_)
{
	Plane plane = Plane::Normalize(plane_);
	float x = plane.Normal.X;
	float y = plane.Normal.Y;
	float z = plane.Normal.Z;
	float x2 = -2.0f * x;
	float y2 = -2.0f * y;
	float z2 = -2.0f * z;
	
	return Matrix(
		(x2 * x) + 1.0f, y2 * x, z2 * x, 0.0f,
		x2 * y, (y2 * y) + 1.0f, z2 * y, 0.0f,
		x2 * z, y2 * z, (z2 * z) + 1.0f, 0.0f,
		x2 * plane.D, y2 * plane.D, z2 * plane.D, 1.0f);
}

//-----------------------------------------------------------------------------
// static
//-----------------------------------------------------------------------------
Matrix Matrix::LookAtLH(const Vector3& position, const Vector3& lookAt, const Vector3& up)
{
	Vector3 xaxis, yaxis, zaxis;
	// 注視点からカメラ位置までのベクトルをZ軸とする
	zaxis = lookAt;
	zaxis -= position;
	zaxis.Normalize();
	// Z軸と上方向のベクトルの外積をとるとX軸が分かる
	xaxis = Vector3::Cross(up, zaxis);
	xaxis.Normalize();
	// 2つの軸がわかったので、その2つの外積は残りの軸(Y軸)になる
	yaxis = Vector3::Cross(zaxis, xaxis);

	return Matrix(
		xaxis.X, yaxis.X, zaxis.X, 0.0f,
		xaxis.Y, yaxis.Y, zaxis.Y, 0.0f,
		xaxis.Z, yaxis.Z, zaxis.Z, 0.0f,
		-(xaxis.X * position.X + xaxis.Y * position.Y + xaxis.Z * position.Z),
		-(yaxis.X * position.X + yaxis.Y * position.Y + yaxis.Z * position.Z),
		-(zaxis.X * position.X + zaxis.Y * position.Y + zaxis.Z * position.Z),
		1.0f);
}

//-----------------------------------------------------------------------------
// static
//-----------------------------------------------------------------------------
Matrix Matrix::LookAtRH(const Vector3& position, const Vector3& lookAt, const Vector3& up)
{
	Vector3 xaxis, yaxis, zaxis;
	// 注視点からカメラ位置までのベクトルをZ軸とする
	zaxis = position;
	zaxis -= lookAt;
	zaxis.Normalize();
	// Z軸と上方向のベクトルの外積をとるとX軸が分かる
	xaxis = Vector3::Cross(up, zaxis);
	xaxis.Normalize();
	// 2つの軸がわかったので、その2つの外積は残りの軸(Y軸)になる
	yaxis = Vector3::Cross(zaxis, xaxis);

	return Matrix(
		xaxis.X, yaxis.X, zaxis.X, 0.0f,
		xaxis.Y, yaxis.Y, zaxis.Y, 0.0f,
		xaxis.Z, yaxis.Z, zaxis.Z, 0.0f,
		-(xaxis.X * position.X + xaxis.Y * position.Y + xaxis.Z * position.Z),
		-(yaxis.X * position.X + yaxis.Y * position.Y + yaxis.Z * position.Z),
		-(zaxis.X * position.X + zaxis.Y * position.Y + zaxis.Z * position.Z),
		1.0f);
}

//-----------------------------------------------------------------------------
// static
//-----------------------------------------------------------------------------
Matrix Matrix::PerspectiveFovLH(float fovY, float aspect, float near, float far)
{
	float h = 1.f / tan(fovY * 0.5f);	// cot(fovY/2)
	return Matrix(
		h / aspect, 0.0f, 0.0f, 0.0f,
		0.0f, h, 0.0f, 0.0f,
		0.0f, 0.0f, far / (far - near), 1.0f,
		0.0f, 0.0f, (-near * far) / (far - near), 0.0f);
}

//-----------------------------------------------------------------------------
// static
//-----------------------------------------------------------------------------
Matrix Matrix::PerspectiveFovRH(float fovY, float aspect, float near, float far)
{
	float h = 1.f / tan(fovY * 0.5f);	// cot(fovY/2)
	return Matrix(
		h / aspect, 0.0f, 0.0f, 0.0f,
		0.0f, h, 0.0f, 0.0f,
		0.0f, 0.0f, far / (near - far), -1.0f,
		0.0f, 0.0f, (near * far) / (near - far), 0.0f);
}

//-----------------------------------------------------------------------------
// static
//-----------------------------------------------------------------------------
Matrix Matrix::OrthoLH(float width, float height, float near, float far)
{
	return Matrix(
		2.0f / width, 0.0f, 0.0f, 0.0f,
		0.0f, 2.0f / height, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f / (far - near), 0.0f,
		0.0f, 0.0f, near / (near - far), 1.0f);
}

//-----------------------------------------------------------------------------
// static
//-----------------------------------------------------------------------------
Matrix Matrix::OrthoRH(float width, float height, float near, float far)
{
	return Matrix(
		2.0f / width, 0.0f, 0.0f, 0.0f,
		0.0f, 2.0f / height, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f / (near - far), 0.0f,
		0.0f, 0.0f, near / (near - far), 1.0f);
}

//-----------------------------------------------------------------------------
// static
//-----------------------------------------------------------------------------
Matrix Matrix::Perspective2DLH(float width, float height, float near, float far)
{
	return Matrix(
		2.0f / width, 0.0f, 0.0f, 0.0f,
		0.0f, -2.0f / height, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f / (far - near), 0.0f,
		-1.0f, 1.0f, near / (near - far), 1.0f);
}

//-----------------------------------------------------------------------------
// static
//-----------------------------------------------------------------------------
Matrix Matrix::Perspective2DRH(float width, float height, float near, float far)
{
	return Matrix(
		2.0f / width, 0.0f, 0.0f, 0.0f,
		0.0f, -2.0f / height, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f / (near - far), 0.0f,
		-1.0f, 1.0f, near / (near - far), 1.0f);
}

//-----------------------------------------------------------------------------
// static
//-----------------------------------------------------------------------------
Matrix Matrix::AffineTransformation(const Vector3& scaling, const Vector3& rotationCenter, const Quaternion& rotation, const Vector3& translation)
{
	Matrix m = Matrix::Scaling(scaling);
	m.Translate(-rotationCenter);
	m.RotateQuaternion(rotation);
	m.Translate(rotationCenter);
	m.Translate(translation);
	return m;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Matrix& Matrix::operator *= (const Matrix& matrix)
{
	float mx0 = M[0][0];
	float mx1 = M[0][1];
	float mx2 = M[0][2];
	M[0][0] = mx0 * matrix.M[0][0] + mx1 * matrix.M[1][0] + mx2 * matrix.M[2][0] + M[0][3] * matrix.M[3][0];
	M[0][1] = mx0 * matrix.M[0][1] + mx1 * matrix.M[1][1] + mx2 * matrix.M[2][1] + M[0][3] * matrix.M[3][1];
	M[0][2] = mx0 * matrix.M[0][2] + mx1 * matrix.M[1][2] + mx2 * matrix.M[2][2] + M[0][3] * matrix.M[3][2];
	M[0][3] = mx0 * matrix.M[0][3] + mx1 * matrix.M[1][3] + mx2 * matrix.M[2][3] + M[0][3] * matrix.M[3][3];
	mx0 = M[1][0];
	mx1 = M[1][1];
	mx2 = M[1][2];
	M[1][0] = mx0 * matrix.M[0][0] + mx1 * matrix.M[1][0] + mx2 * matrix.M[2][0] + M[1][3] * matrix.M[3][0];
	M[1][1] = mx0 * matrix.M[0][1] + mx1 * matrix.M[1][1] + mx2 * matrix.M[2][1] + M[1][3] * matrix.M[3][1];
	M[1][2] = mx0 * matrix.M[0][2] + mx1 * matrix.M[1][2] + mx2 * matrix.M[2][2] + M[1][3] * matrix.M[3][2];
	M[1][3] = mx0 * matrix.M[0][3] + mx1 * matrix.M[1][3] + mx2 * matrix.M[2][3] + M[1][3] * matrix.M[3][3];
	mx0 = M[2][0];
	mx1 = M[2][1];
	mx2 = M[2][2];
	M[2][0] = mx0 * matrix.M[0][0] + mx1 * matrix.M[1][0] + mx2 * matrix.M[2][0] + M[2][3] * matrix.M[3][0];
	M[2][1] = mx0 * matrix.M[0][1] + mx1 * matrix.M[1][1] + mx2 * matrix.M[2][1] + M[2][3] * matrix.M[3][1];
	M[2][2] = mx0 * matrix.M[0][2] + mx1 * matrix.M[1][2] + mx2 * matrix.M[2][2] + M[2][3] * matrix.M[3][2];
	M[2][3] = mx0 * matrix.M[0][3] + mx1 * matrix.M[1][3] + mx2 * matrix.M[2][3] + M[2][3] * matrix.M[3][3];
	mx0 = M[3][0];
	mx1 = M[3][1];
	mx2 = M[3][2];
	M[3][0] = mx0 * matrix.M[0][0] + mx1 * matrix.M[1][0] + mx2 * matrix.M[2][0] + M[3][3] * matrix.M[3][0];
	M[3][1] = mx0 * matrix.M[0][1] + mx1 * matrix.M[1][1] + mx2 * matrix.M[2][1] + M[3][3] * matrix.M[3][1];
	M[3][2] = mx0 * matrix.M[0][2] + mx1 * matrix.M[1][2] + mx2 * matrix.M[2][2] + M[3][3] * matrix.M[3][2];
	M[3][3] = mx0 * matrix.M[0][3] + mx1 * matrix.M[1][3] + mx2 * matrix.M[2][3] + M[3][3] * matrix.M[3][3];
	return (*this);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Matrix operator * (const Matrix& mat1, const Matrix& mat2)
{
	return Matrix::Multiply(mat1, mat2);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Matrix operator * (const Matrix& mat1, float v)
{
	return Matrix(
		mat1.M[0][0] * v, mat1.M[0][1] * v, mat1.M[0][2] * v, mat1.M[0][3] * v,
		mat1.M[1][0] * v, mat1.M[1][1] * v, mat1.M[1][2] * v, mat1.M[1][3] * v,
		mat1.M[2][0] * v, mat1.M[2][1] * v, mat1.M[2][2] * v, mat1.M[2][3] * v,
		mat1.M[3][0] * v, mat1.M[3][1] * v, mat1.M[3][2] * v, mat1.M[3][3] * v);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
static bool Equals(const Matrix& value1, const Matrix& value2)
{
	return (
		value1.M11 == value2.M11 && value1.M12 == value2.M12 && value1.M13 == value2.M13 && value1.M14 == value2.M14 &&
		value1.M21 == value2.M21 && value1.M22 == value2.M22 && value1.M23 == value2.M23 && value1.M24 == value2.M24 &&
		value1.M31 == value2.M31 && value1.M32 == value2.M32 && value1.M33 == value2.M33 && value1.M34 == value2.M34 &&
		value1.M41 == value2.M41 && value1.M42 == value2.M42 && value1.M43 == value2.M43 && value1.M44 == value2.M44);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
bool Matrix::operator == (const Matrix& mat) const
{
	return Equals(*this, mat);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
bool Matrix::operator != (const Matrix& mat) const
{
	return !Equals(*this, mat);
}

LN_NAMESPACE_END
