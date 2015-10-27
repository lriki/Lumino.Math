
#include <assert.h>
#include "../../include/Lumino/Math/MathUtils.h"
#include "../../include/Lumino/Math/Vector3.h"
#include "../../include/Lumino/Math/Matrix.h"
#include "../../include/Lumino/Math/Quaternion.h"
#include "Asm.h"

LN_NAMESPACE_BEGIN

//=============================================================================
// Quaternion
//=============================================================================

const Quaternion Quaternion::Identity = Quaternion(0.0f, 0.0f, 0.0f, 1.0f);

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Quaternion::Quaternion(const Vector3& axis, float r)
{
	float tx = axis.X;
	float ty = axis.Y;
	float tz = axis.Z;
	float len = tx * tx + ty * ty + tz * tz;

	// 軸が正規化されていなければ正規化
	if (len != 1.0f)
	{
		len = Asm::sqrt(len);
		tx /= len;
		ty /= len;
		tz /= len;
	}

	float s, c;
	Asm::sincos(r * 0.5f, &s, &c);
	X = s * tx,
	Y = s * ty,
	Z = s * tz,
	W = c;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
float Quaternion::GetLength() const
{
	return Asm::sqrt((X * X) + (Y * Y) + (Z * Z) + (W * W));
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
float Quaternion::GetLengthSquared() const
{
	return (X * X) + (Y * Y) + (Z * Z) + (W * W);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Quaternion::Normalize()
{
	float t = 1.0f / GetLength();
	X *= t;
	Y *= t;
	Z *= t;
	W *= t;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Quaternion::Multiply(const Quaternion& qua)
{
	float lx = X;
	float ly = Y;
	float lz = Z;
	float lw = W;
	X = (qua.X * lw + lx * qua.W + qua.Y * lz) - (qua.Z * ly);
	Y = (qua.Y * lw + ly * qua.W + qua.Z * lx) - (qua.X * lz);
	Z = (qua.Z * lw + lz * qua.W + qua.X * ly) - (qua.Y * lx);
	W = (qua.W * lw) - (qua.X * lx + qua.Y * ly + qua.Z * lz);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Quaternion::RotateX(float angle)
{
	float s, c;
	Asm::sincos(angle * 0.5f, &s, &c);

	float lx = X;
	float ly = Y;
	float lz = Z;
	float lw = W;
	X = (s * lw + lx * c);
	Y = (ly * c) - (s * lz);
	Z = (lz * c + s * ly);
	W = (c * lw) - (s * lx);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Quaternion::RotateY(float angle)
{
	float s, c;
	Asm::sincos(angle * 0.5f, &s, &c);

	float lx = X;
	float ly = Y;
	float lz = Z;
	float lw = W;
	X = (lx * c + s * lz);
	Y = (s * lw + ly * c);
	Z = (lz * c) - (s * lx);
	W = (c * lw) - (s * ly);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Quaternion::RotateZ(float angle)
{
	float s, c;
	Asm::sincos(angle * 0.5f, &s, &c);

	float lx = X;
	float ly = Y;
	float lz = Z;
	float lw = W;
	X = (lx * c) - (s * ly);
	Y = (ly * c + s * lx);
	Z = (s * lw + lz * c);
	W = (c * lw) - (s * lz);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Quaternion::RotateAxis(const Vector3& axis, float r)
{
	// TODO: もうちょい最適化
	Multiply(RotationAxis(axis, r));
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Vector3 Quaternion::ToEulerAngles(RotationOrder order, bool* locked) const
{
	// クォータニオン→行列
	Matrix rot = Matrix::RotationQuaternion(Quaternion::Normalize(*this));
	return rot.ToEulerAngles(order, locked);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Quaternion::ToAxisAngle(Vector3* axis, float* angle) const
{
	Quaternion q = Quaternion::Normalize(*this);
	if (axis)
	{
		axis->X = q.X;
		axis->Y = q.Y;
		axis->Z = q.Z;
		axis->Normalize();
	}
	if (angle)
	{
		*angle = 2.0f * acos(q.W);
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
bool Quaternion::IsNaNOrInf() const
{
	return Math::IsNaNOrInf(X) || Math::IsNaNOrInf(Y) || Math::IsNaNOrInf(Z) || Math::IsNaNOrInf(W);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Quaternion::Print(const char* format, FILE* stream) const
{
	if (!format) {
		format = "%f, %f, %f, %f\n";
	}
	if (!stream) {
		stream = stdout;
	}
	fprintf(stream, format, X, Y, Z, W);
}

//-------------------------------------------------------------------------
// static
//-------------------------------------------------------------------------
Quaternion Quaternion::Normalize(const Quaternion& qua)
{
	float length = 1.0f / qua.GetLength();
	return Quaternion(
		qua.X * length,
		qua.Y * length,
		qua.Z * length,
		qua.W * length);
}

//-------------------------------------------------------------------------
// static
//-------------------------------------------------------------------------
Quaternion Quaternion::Conjugate(const Quaternion& qua)
{
	return Quaternion(
		-qua.X,
		-qua.Y,
		-qua.Z,
		qua.W);
}

//-------------------------------------------------------------------------
// static
//-------------------------------------------------------------------------
float Quaternion::Dot(const Quaternion& qua1, const Quaternion& qua2)
{
	return (qua1.X * qua2.X) + (qua1.Y * qua2.Y) + (qua1.Z * qua2.Z) + (qua1.W * qua2.W);
}

//-------------------------------------------------------------------------
// static
//-------------------------------------------------------------------------
Quaternion Quaternion::Inverse(const Quaternion& qua)
{
	float t = 1.0f / ((qua.X * qua.X) + (qua.Y * qua.Y) + (qua.Z * qua.Z) + (qua.W * qua.W));
	return Quaternion(
		-qua.X * t,
		-qua.Y * t,
		-qua.Z * t,
		qua.W * t);
}

//-----------------------------------------------------------------------------
// static
//-----------------------------------------------------------------------------
Quaternion Quaternion::Multiply(const Quaternion& qua1, const Quaternion& qua2)
{
	return Quaternion(
		(qua2.X * qua1.W + qua1.X * qua2.W + qua2.Y * qua1.Z) - (qua2.Z * qua1.Y),
		(qua2.Y * qua1.W + qua1.Y * qua2.W + qua2.Z * qua1.X) - (qua2.X * qua1.Z),
		(qua2.Z * qua1.W + qua1.Z * qua2.W + qua2.X * qua1.Y) - (qua2.Y * qua1.X),
		(qua2.W * qua1.W) - (qua2.X * qua1.X + qua2.Y * qua1.Y + qua2.Z * qua1.Z));
}

//-----------------------------------------------------------------------------
// static
//-----------------------------------------------------------------------------
Quaternion Quaternion::RotationAxis(const Vector3& axis, float r)
{
	float tx = axis.X;
	float ty = axis.Y;
	float tz = axis.Z;
	float len = tx * tx + ty * ty + tz * tz;

	// 軸が正規化されていなければ正規化
	if (len != 1.0f)
	{
		len = Asm::sqrt(len);
		tx /= len;
		ty /= len;
		tz /= len;
	}

	float s, c;
	Asm::sincos(r * 0.5f, &s, &c);
	return Quaternion(
		s * tx,
		s * ty,
		s * tz,
		c);
}

//-----------------------------------------------------------------------------
// static
//-----------------------------------------------------------------------------
Quaternion Quaternion::RotationMatrix(const Matrix& mat)
{
	float t = mat.M11 + mat.M22 + mat.M33;

	if (t > 0.0f)
	{
		float sq = Asm::sqrt(t + 1.0f);

		float w = sq * 0.5f;
		sq = 0.5f / sq;

		return Quaternion(
			(mat.M23 - mat.M32) * sq,
			(mat.M31 - mat.M13) * sq,
			(mat.M12 - mat.M21) * sq,
			w);
	}
	else if ((mat.M11 >= mat.M22) && (mat.M11 >= mat.M33))
	{
		float sq = Asm::sqrt(1.0f + mat.M11 - mat.M22 - mat.M33);
		float half = 0.5f / sq;

		return Quaternion(
			0.5f * sq,
			(mat.M12 + mat.M21) * half,
			(mat.M13 + mat.M31) * half,
			(mat.M23 - mat.M32) * half);
	}
	else if (mat.M22 > mat.M33)
	{
		float sq = Asm::sqrt(1.0f + mat.M22 - mat.M11 - mat.M33);
		float half = 0.5f / sq;

		return Quaternion(
			(mat.M21 + mat.M12) * half,
			0.5f * sq,
			(mat.M32 + mat.M23) * half,
			(mat.M31 - mat.M13) * half);
	}
	else
	{
		float sq = Asm::sqrt(1.0f + mat.M33 - mat.M11 - mat.M22);
		float half = 0.5f / sq;

		return Quaternion(
			(mat.M31 + mat.M13) * half,
			(mat.M32 + mat.M23) * half,
			0.5f * sq,
			(mat.M12 - mat.M21) * half);
	}
}

//-----------------------------------------------------------------------------
// static
//-----------------------------------------------------------------------------
Quaternion Quaternion::RotationYawPitchRoll(float yaw, float pitch, float roll)
{
	float halfRoll = roll * 0.5f;
	float sinRoll, cosRoll;
	Asm::sincos(halfRoll, &sinRoll, &cosRoll);

	float halfPitch = pitch * 0.5f;
	float sinPitch, cosPitch;
	Asm::sincos(halfPitch, &sinPitch, &cosPitch);

	float halfYaw = yaw * 0.5f;
	float sinYaw, cosYaw;
	Asm::sincos(halfYaw, &sinYaw, &cosYaw);

	return Quaternion(
		(cosYaw * sinPitch * cosRoll) + (sinYaw * cosPitch * sinRoll),
		(sinYaw * cosPitch * cosRoll) - (cosYaw * sinPitch * sinRoll),
		(cosYaw * cosPitch * sinRoll) - (sinYaw * sinPitch * cosRoll),
		(cosYaw * cosPitch * cosRoll) + (sinYaw * sinPitch * sinRoll));
}

//-----------------------------------------------------------------------------
// static
//-----------------------------------------------------------------------------
Quaternion Quaternion::RotationEulerAngles(const Vector3& angles, RotationOrder order)
{
	Quaternion q;
	switch (order)
	{
	case RotationOrder_XYZ:
		q.RotateX(angles.X); q.RotateY(angles.Y); q.RotateZ(angles.Z);
		break;
	case RotationOrder_YZX:
		q.RotateY(angles.Y); q.RotateZ(angles.Z); q.RotateX(angles.X);
		break;
	case RotationOrder_ZXY:
		q.RotateZ(angles.Z); q.RotateX(angles.X); q.RotateY(angles.Y);
		break;
	default:
		assert(0);
		break;
	}
	return q;
}

//-----------------------------------------------------------------------------
// static
//-----------------------------------------------------------------------------
Quaternion Quaternion::Slerp(const Quaternion& qua1, const Quaternion& qua2, float t)
{
	float opposite;
	float inverse;
	float dot = (qua1.X * qua2.X) + (qua1.Y * qua2.Y) + (qua1.Z * qua2.Z) + (qua1.W * qua2.W);
	bool flag = false;

	if( dot < 0.0f )
	{
		flag = true;
		dot = -dot;
	}

	if( dot > 0.999999f )
	{
		inverse = 1.0f - t;
		opposite = flag ? -t : t;
	}
	else
	{
		float ac = acos(dot);
		float invSin = (1.0f / sin(ac));

		inverse = (sin((1.0f - t) * ac)) * invSin;
		opposite = flag ? ((-sin(t * ac)) * invSin) : ((sin(t * ac)) * invSin);
	}

	return Quaternion(
		(inverse * qua1.X) + (opposite * qua2.X),
		(inverse * qua1.Y) + (opposite * qua2.Y),
		(inverse * qua1.Z) + (opposite * qua2.Z),
		(inverse * qua1.W) + (opposite * qua2.W));
}

LN_NAMESPACE_END
