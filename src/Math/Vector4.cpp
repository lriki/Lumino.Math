
#include "../../include/Lumino/Math/MathUtils.h"
#include "../../include/Lumino/Math/Vector2.h"
#include "../../include/Lumino/Math/Vector3.h"
#include "../../include/Lumino/Math/Vector4.h"
#include "../../include/Lumino/Math/Matrix.h"
#include "Asm.h"

LN_NAMESPACE_BEGIN

//=============================================================================
// Vector4
//=============================================================================

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Vector4::Vector4(const Vector2& vec, float z_, float w_)
{
	X = vec.X;
	Y = vec.Y;
	Z = z_;
	W = w_;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Vector4::Vector4(const Vector3& vec, float w_)
{
	X = vec.X;
	Y = vec.Y;
	Z = vec.Z;
	W = w_;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
float Vector4::GetLength() const
{
	return Asm::sqrt(X * X + Y * Y + Z * Z + W * W);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
float Vector4::GetLengthSquared() const
{
	return X * X + Y * Y + Z * Z + W * W;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Vector4::Clamp(float min_xyzw, float max_xyzw)
{
	X = Math::Clamp(X, min_xyzw, max_xyzw);
	Y = Math::Clamp(Y, min_xyzw, max_xyzw);
	Z = Math::Clamp(Z, min_xyzw, max_xyzw);
	W = Math::Clamp(W, min_xyzw, max_xyzw);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Vector4::Clamp(const Vector4& minVec, const Vector4& maxVec)
{
	X = Math::Clamp(X, minVec.X, maxVec.X);
	Y = Math::Clamp(Y, minVec.Y, maxVec.Y);
	Z = Math::Clamp(Z, minVec.Z, maxVec.Z);
	W = Math::Clamp(W, minVec.W, maxVec.W);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
bool Vector4::IsNaNOrInf() const
{
	return Math::IsNaNOrInf(X) || Math::IsNaNOrInf(Y) || Math::IsNaNOrInf(Z) || Math::IsNaNOrInf(W);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Vector4::Print(const char* format, FILE* stream) const
{
	if (!format) {
		format = "%f, %f, %f, %f\n";
	}
	if (!stream) {
		stream = stdout;
	}
	fprintf(stream, format, X, Y, Z, W);
}

//-----------------------------------------------------------------------------
// static
//-----------------------------------------------------------------------------
Vector4 Vector4::Normalize(const Vector4& vec)
{
	float t = 1.0f / Asm::sqrt(vec.X * vec.X + vec.Y * vec.Y + vec.Z * vec.Z + vec.W * vec.W);
	return Vector4(
		vec.X * t,
		vec.Y * t,
		vec.Z * t,
		vec.W * t);
}

//-----------------------------------------------------------------------------
// static
//-----------------------------------------------------------------------------
float Vector4::Dot(const Vector4& vec1, const Vector4& vec2)
{
	return (vec1.X * vec2.X + vec1.Y * vec2.Y + vec1.Z * vec2.Z + vec1.W * vec2.W);
}

//-----------------------------------------------------------------------------
// static
//-----------------------------------------------------------------------------
Vector4 Vector4::Min(const Vector4& vec1, const Vector4& vec2)
{
	return Vector4(
		(vec1.X < vec2.X) ? vec1.X : vec2.X,
		(vec1.Y < vec2.Y) ? vec1.Y : vec2.Y,
		(vec1.Z < vec2.Z) ? vec1.Z : vec2.Z,
		(vec1.W < vec2.W) ? vec1.W : vec2.W);
}

//-----------------------------------------------------------------------------
// static
//-----------------------------------------------------------------------------
Vector4 Vector4::Max(const Vector4& vec1, const Vector4& vec2)
{
	return Vector4(
		(vec1.X > vec2.X) ? vec1.X : vec2.X,
		(vec1.Y > vec2.Y) ? vec1.Y : vec2.Y,
		(vec1.Z > vec2.Z) ? vec1.Z : vec2.Z,
		(vec1.W > vec2.W) ? vec1.W : vec2.W);
}

//-----------------------------------------------------------------------------
// static
//-----------------------------------------------------------------------------
Vector4 Vector4::Transform(const Vector4& vec, const Matrix& mat)
{
	return Vector4(
		(vec.X * mat.M11) + (vec.Y * mat.M21) + (vec.Z * mat.M31) + (vec.W * mat.M41),
		(vec.X * mat.M12) + (vec.Y * mat.M22) + (vec.Z * mat.M32) + (vec.W * mat.M42),
		(vec.X * mat.M13) + (vec.Y * mat.M23) + (vec.Z * mat.M33) + (vec.W * mat.M43),
		(vec.X * mat.M14) + (vec.Y * mat.M24) + (vec.Z * mat.M34) + (vec.W * mat.M44));
}

//-----------------------------------------------------------------------------
// static
//-----------------------------------------------------------------------------
Vector4 Vector4::Lerp(const Vector4& start, const Vector4& end, float t)
{
	return Vector4(
		Math::Lerp(start.X, end.X, t),
		Math::Lerp(start.Y, end.Y, t),
		Math::Lerp(start.Z, end.Z, t),
		Math::Lerp(start.W, end.W, t));
}

//-----------------------------------------------------------------------------
// static
//-----------------------------------------------------------------------------
Vector4 Vector4::Hermite(const Vector4& v1, const Vector4& a1, const Vector4& v2, const Vector4& a2, float t)
{
	return Vector4(
		Math::Hermite(v1.X, a1.X, v2.X, a2.X, t),
		Math::Hermite(v1.Y, a1.Y, v2.Y, a2.Y, t),
		Math::Hermite(v1.Z, a1.Z, v2.Z, a2.Z, t),
		Math::Hermite(v1.W, a1.W, v2.W, a2.W, t));
}

//-----------------------------------------------------------------------------
// static
//-----------------------------------------------------------------------------
Vector4 Vector4::CatmullRom(const Vector4& vec1, const Vector4& vec2, const Vector4& vec3, const Vector4& vec4, float t)
{
	return Vector4(
		Math::CatmullRom(vec1.X, vec2.X, vec3.X, vec4.X, t),
		Math::CatmullRom(vec1.Y, vec2.Y, vec3.Y, vec4.Y, t),
		Math::CatmullRom(vec1.Z, vec2.Z, vec3.Z, vec4.Z, t),
		Math::CatmullRom(vec1.W, vec2.W, vec3.W, vec4.W, t));
}

LN_NAMESPACE_END
