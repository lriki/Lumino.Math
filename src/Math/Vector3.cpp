
#include "../../include/Lumino/Math/MathUtils.h"
#include "../../include/Lumino/Math/Vector2.h"
#include "../../include/Lumino/Math/Vector3.h"
#include "../../include/Lumino/Math/Vector4.h"
#include "../../include/Lumino/Math/Quaternion.h"
#include "../../include/Lumino/Math/Matrix.h"
#include "Asm.h"

LN_NAMESPACE_BEGIN

//=============================================================================
// Vector3
//=============================================================================
	
const Vector3   Vector3::Zero	= Vector3(0, 0, 0);
const Vector3   Vector3::UnitX	= Vector3(1, 0, 0);
const Vector3   Vector3::UnitY	= Vector3(0, 1, 0);
const Vector3   Vector3::UnitZ	= Vector3(0, 0, 1);
const Vector3   Vector3::One	= Vector3(1, 1, 1);

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Vector3::Vector3(const Vector2& vec, float z)
{
	X = vec.X;
	Y = vec.Y;
	Z = z;
}

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
float Vector3::GetLength() const
{
	return Asm::sqrt(X * X + Y * Y + Z * Z);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Vector3::Normalize()
{
	float t = 1.0f / Asm::sqrt(X * X + Y * Y + Z * Z);
	X *= t;
	Y *= t;
	Z *= t;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Vector3::Clamp(const Vector3& minVec, const Vector3& maxVec)
{
	X = Math::Clamp(X, minVec.X, maxVec.X);
	Y = Math::Clamp(Y, minVec.Y, maxVec.Y);
	Z = Math::Clamp(Z, minVec.Z, maxVec.Z);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Vector3::TransformCoord(const Matrix& mat)
{
	float tx = X;
	float ty = Y;
	float tz = Z;
	float w = 1.0f / (tx * mat.M[0][3] + ty * mat.M[1][3] + tz * mat.M[2][3] + mat.M[3][3]);
	X = (tx * mat.M[0][0] + ty * mat.M[1][0] + tz * mat.M[2][0] + mat.M[3][0]) * w;
	Y = (tx * mat.M[0][1] + ty * mat.M[1][1] + tz * mat.M[2][1] + mat.M[3][1]) * w;
	Z = (tx * mat.M[0][2] + ty * mat.M[1][2] + tz * mat.M[2][2] + mat.M[3][2]) * w;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
bool Vector3::IsNaNOrInf() const
{
	return Math::IsNaNOrInf(X) || Math::IsNaNOrInf(Y) || Math::IsNaNOrInf(Z);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Vector3::Print(const char* format, FILE* stream) const
{
	if (!format) {
		format = "%f, %f, %f\n";
	}
	if (!stream) {
		stream = stdout;
	}
	fprintf(stream, format, X, Y, Z);
}

//-----------------------------------------------------------------------------
// static
//-----------------------------------------------------------------------------
Vector3 Vector3::Normalize(float x, float y, float z)
{
	float t = 1.0f / Asm::sqrt(x * x + y * y + z * z);
	return Vector3(
		x * t,
		y * t,
		z * t);
}

//-----------------------------------------------------------------------------
// static
//-----------------------------------------------------------------------------
Vector3 Vector3::Normalize(const Vector3& vec)
{
	float t = 1.0f / Asm::sqrt(vec.X * vec.X + vec.Y * vec.Y + vec.Z * vec.Z);
	return Vector3(
		vec.X * t,
		vec.Y * t,
		vec.Z * t);
}

//-----------------------------------------------------------------------------
// static
//-----------------------------------------------------------------------------
float Vector3::Dot(const Vector3& vec1, const Vector3& vec2)
{
	return (vec1.X * vec2.X + vec1.Y * vec2.Y + vec1.Z * vec2.Z);
}

//-----------------------------------------------------------------------------
// static
//-----------------------------------------------------------------------------
Vector3 Vector3::Cross(const Vector3& vec1, const Vector3& vec2)
{
	return Vector3(
		(vec1.Y * vec2.Z) - (vec1.Z * vec2.Y),
		(vec1.Z * vec2.X) - (vec1.X * vec2.Z),
		(vec1.X * vec2.Y) - (vec1.Y * vec2.X));
}

//-----------------------------------------------------------------------------
// static
//-----------------------------------------------------------------------------
Vector3 Vector3::Min(const Vector3& vec1, const Vector3& vec2)
{
	return Vector3(
		(vec1.X < vec2.X) ? vec1.X : vec2.X,
		(vec1.Y < vec2.Y) ? vec1.Y : vec2.Y,
		(vec1.Z < vec2.Z) ? vec1.Z : vec2.Z);
}

//-----------------------------------------------------------------------------
// static
//-----------------------------------------------------------------------------
Vector3 Vector3::Max(const Vector3& vec1, const Vector3& vec2)
{
	return Vector3(
		(vec1.X > vec2.X) ? vec1.X : vec2.X,
		(vec1.Y > vec2.Y) ? vec1.Y : vec2.Y,
		(vec1.Z > vec2.Z) ? vec1.Z : vec2.Z);
}

//-----------------------------------------------------------------------------
// static
//-----------------------------------------------------------------------------
Vector3 Vector3::Reflect(const Vector3& vec, const Vector3& normal)
{
	float t = -( normal.X * vec.X + normal.Y * vec.Y + normal.Z * vec.Z ) / ( normal.X * normal.X + normal.Y * normal.Y + normal.Z * normal.Z );
	return Vector3(
		vec.X + t * normal.X * 2.0f,
		vec.Y + t * normal.Y * 2.0f,
		vec.Z + t * normal.Z * 2.0f );
}

//-----------------------------------------------------------------------------
// static
//-----------------------------------------------------------------------------
Vector3 Vector3::Slide(const Vector3& vec, const Vector3& normal)
{
	float t = -(normal.X * vec.X + normal.Y * vec.Y + normal.Z * vec.Z) / (normal.X * normal.X + normal.Y * normal.Y + normal.Z * normal.Z);
	return Vector3(
		vec.X + t * normal.X,
		vec.Y + t * normal.Y,
		vec.Z + t * normal.Z );
}

//-----------------------------------------------------------------------------
// static
//-----------------------------------------------------------------------------
Vector3 Vector3::Transform(const Vector3& vec, const Quaternion& qua)
{
	float x = qua.X + qua.X;
	float y = qua.Y + qua.Y;
	float z = qua.Z + qua.Z;
	float wx = qua.W * x;
	float wy = qua.W * y;
	float wz = qua.W * z;
	float xx = qua.X * x;
	float xy = qua.X * y;
	float xz = qua.X * z;
	float yy = qua.Y * y;
	float yz = qua.Y * z;
	float zz = qua.Z * z;

	return Vector3(
		((vec.X * ((1.0f - yy) - zz)) + (vec.Y * (xy - wz))) + (vec.Z * (xz + wy)),
		((vec.X * (xy + wz)) + (vec.Y * ((1.0f - xx) - zz))) + (vec.Z * (yz - wx)),
		((vec.X * (xz - wy)) + (vec.Y * (yz + wx))) + (vec.Z * ((1.0f - xx) - yy)));
}

//-----------------------------------------------------------------------------
// static
//-----------------------------------------------------------------------------
Vector4 Vector3::Transform(const Vector3& vec, const Matrix& mat)
{
	return Vector4(
		(((vec.X * mat.M11) + (vec.Y * mat.M21)) + (vec.Z * mat.M31)) + mat.M41,
		(((vec.X * mat.M12) + (vec.Y * mat.M22)) + (vec.Z * mat.M32)) + mat.M42,
		(((vec.X * mat.M13) + (vec.Y * mat.M23)) + (vec.Z * mat.M33)) + mat.M43,
		(((vec.X * mat.M14) + (vec.Y * mat.M24)) + (vec.Z * mat.M34)) + mat.M44);
}

//-----------------------------------------------------------------------------
// static
//-----------------------------------------------------------------------------
Vector3 Vector3::TransformCoord(const Vector3& vec, const Matrix& mat)
{
	float w = 1.0f / ((((vec.X * mat.M14) + (vec.Y * mat.M24)) + (vec.Z * mat.M34)) + mat.M44);
	return Vector3(
		((vec.X * mat.M11) + (vec.Y * mat.M21) + (vec.Z * mat.M31) + mat.M41) * w,
		((vec.X * mat.M12) + (vec.Y * mat.M22) + (vec.Z * mat.M32) + mat.M42) * w,
		((vec.X * mat.M13) + (vec.Y * mat.M23) + (vec.Z * mat.M33) + mat.M43) * w);
}

//-----------------------------------------------------------------------------
// static
//-----------------------------------------------------------------------------
Vector3 Vector3::Lerp(const Vector3& start, const Vector3& end, float t)
{
	return Vector3(
		Math::Lerp(start.X, end.X, t),
		Math::Lerp(start.Y, end.Y, t),
		Math::Lerp(start.Z, end.Z, t));
}

//-----------------------------------------------------------------------------
// static
//-----------------------------------------------------------------------------
Vector3 Vector3::Hermite(const Vector3& v1, const Vector3& a1, const Vector3& v2, const Vector3& a2, float t)
{
	return Vector3(
		Math::Hermite(v1.X, a1.X, v2.X, a2.X, t),
		Math::Hermite(v1.Y, a1.Y, v2.Y, a2.Y, t),
		Math::Hermite(v1.Z, a1.Z, v2.Z, a2.Z, t));
}

//-----------------------------------------------------------------------------
// static
//-----------------------------------------------------------------------------
Vector3 Vector3::CatmullRom(const Vector3& vec1, const Vector3& vec2, const Vector3& vec3, const Vector3& vec4, float t)
{
	return Vector3(
		Math::CatmullRom(vec1.X, vec2.X, vec3.X, vec4.X, t),
		Math::CatmullRom(vec1.Y, vec2.Y, vec3.Y, vec4.Y, t),
		Math::CatmullRom(vec1.Z, vec2.Z, vec3.Z, vec4.Z, t));
}

//-----------------------------------------------------------------------------
// static
//-----------------------------------------------------------------------------
Vector3 Vector3::Project(const Vector3& point, const Matrix& worldViewProj, float x, float y, float width, float height, float minZ, float maxZ)
{
	Vector3 v = Vector3::TransformCoord(point, worldViewProj);
	return Vector3(
		((1.0f + v.X) * 0.5f * width) + x, 
		((1.0f - v.Y) * 0.5f * height) + y, 
		(v.Z * (maxZ - minZ)) + minZ);
}

//-----------------------------------------------------------------------------
// static
//-----------------------------------------------------------------------------
Vector3 Vector3::Unproject(const Vector3& point, const Matrix& worldViewProj, float x, float y, float width, float height, float minZ, float maxZ)
{
	Vector3 v;
	Matrix inv = Matrix::Inverse(worldViewProj);

	v.X = (((point.X - x) / width) * 2.0f) - 1.0f;
	v.Y = -((((point.Y - y) / height) * 2.0f) - 1.0f);
	v.Z = (point.Z - minZ) / (maxZ - minZ);

	return Vector3::TransformCoord(v, inv);
}

LN_NAMESPACE_END
