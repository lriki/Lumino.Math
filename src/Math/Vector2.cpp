
#include "../../include/Lumino/Math/MathUtils.h"
#include "../../include/Lumino/Math/Matrix.h"
#include "../../include/Lumino/Math/Vector2.h"
#include "../../include/Lumino/Math/Vector4.h"
#include "Asm.h"

LN_NAMESPACE_BEGIN

//=============================================================================
// Vector2
//=============================================================================

const Vector2   Vector2::Zero	= Vector2(0, 0);
const Vector2   Vector2::UnitX	= Vector2(1, 0);
const Vector2   Vector2::UnitY	= Vector2(0, 1);
const Vector2   Vector2::One	= Vector2(1, 1);

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
float Vector2::GetLength() const
{
    return Asm::sqrt( X * X + Y * Y );
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
float Vector2::GetLengthSquared() const
{
    return ( X * X + Y * Y );
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Vector2::Normalize()
{
	float t = GetLength();
	if (t == 0.0f)
		return;
	t = 1.0f / t;
	X *= t;
	Y *= t;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Vector2::TransformCoord(const Matrix& mat)
{
	*this = TransformCoord(*this, mat);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
bool Vector2::IsNaNOrInf() const
{
	return Math::IsNaNOrInf(X) || Math::IsNaNOrInf(Y);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Vector2::Print(const char* format, FILE* stream) const
{
	if (!format) {
		format = "%f, %f\n";
	}
	if (!stream) {
		stream = stdout;
	}
	fprintf(stream, format, X, Y);
}

//-----------------------------------------------------------------------------
// static
//-----------------------------------------------------------------------------
Vector2 Vector2::Normalize(const Vector2& vec)
{
	Vector2 t(vec);
	t.Normalize();
	return t;
}

//-----------------------------------------------------------------------------
// static
//-----------------------------------------------------------------------------
float Vector2::Dot(const Vector2& vec1, const Vector2& vec2)
{
	return (vec1.X * vec2.X) + (vec1.Y * vec2.Y);
}

//-----------------------------------------------------------------------------
// static
//-----------------------------------------------------------------------------
Vector2 Vector2::Min(const Vector2& vec1, const Vector2& vec2)
{
	return Vector2(
		(vec1.X < vec2.X) ? vec1.X : vec2.X,
		(vec1.Y < vec2.Y) ? vec1.Y : vec2.Y);
}

//-----------------------------------------------------------------------------
// static
//-----------------------------------------------------------------------------
Vector2 Vector2::Max(const Vector2& vec1, const Vector2& vec2)
{
	return Vector2(
		(vec1.X > vec2.X) ? vec1.X : vec2.X,
		(vec1.Y > vec2.Y) ? vec1.Y : vec2.Y);
}

//-----------------------------------------------------------------------------
// static
//-----------------------------------------------------------------------------
Vector4 Vector2::Transform(const Vector2& vec, const Matrix& mat)
{
	Vector4 t;
	t.X = (vec.X * mat.M[0][0]) + (vec.Y * mat.M[1][0]) + mat.M[3][0];
	t.Y = (vec.X * mat.M[0][1]) + (vec.Y * mat.M[1][1]) + mat.M[3][1];
	t.Z = (vec.X * mat.M[0][2]) + (vec.Y * mat.M[1][2]) + mat.M[3][2];
	t.W = (vec.X * mat.M[0][3]) + (vec.Y * mat.M[1][3]) + mat.M[3][3];
	return t;
}

//-----------------------------------------------------------------------------
// static
//-----------------------------------------------------------------------------
Vector2 Vector2::TransformCoord(const Vector2& vec, const Matrix& mat)
{
	float tx = (vec.X * mat.M[0][0]) + (vec.Y * mat.M[1][0]) + mat.M[3][0];
	float ty = (vec.X * mat.M[0][1]) + (vec.Y * mat.M[1][1]) + mat.M[3][1];
	//float tz = (vec.X * mat.M[0][3]) + (vec.Y * mat.M[1][2]) + mat.M[3][2];
	float tw = 1.0f / ((vec.X * mat.M[0][3]) + (vec.Y * mat.M[1][3]) + mat.M[3][3]);
	return Vector2(tx * tw, ty * tw);
}

//-----------------------------------------------------------------------------
// static
//-----------------------------------------------------------------------------
Vector2 Vector2::Lerp(const Vector2& start, const Vector2& end, float t)
{
	return Vector2(
		Math::Lerp(start.X, end.X, t),
		Math::Lerp(start.Y, end.Y, t));
}

//-----------------------------------------------------------------------------
// static
//-----------------------------------------------------------------------------
Vector2 Vector2::Hermite(const Vector2& v1, const Vector2& a1, const Vector2& v2, const Vector2& a2, float t)
{
	return Vector2(
		Math::Hermite(v1.X, a1.X, v2.X, a2.X, t),
		Math::Hermite(v1.Y, a1.Y, v2.Y, a2.Y, t));
}

//-----------------------------------------------------------------------------
// static
//-----------------------------------------------------------------------------
Vector2 Vector2::CatmullRom(const Vector2& vec1, const Vector2& vec2, const Vector2& vec3, const Vector2& vec4, float t)
{
	return Vector2(
		Math::CatmullRom(vec1.X, vec2.X, vec3.X, vec4.X, t),
		Math::CatmullRom(vec1.Y, vec2.Y, vec3.Y, vec4.Y, t));
}

LN_NAMESPACE_END
