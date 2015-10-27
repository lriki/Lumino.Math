
#include "../../include/Lumino/Math/Vector3.h"
#include "../../include/Lumino/Math/Vector4.h"
#include "../../include/Lumino/Math/Matrix.h"
#include "../../include/Lumino/Math/Plane.h"
#include "Asm.h"

LN_NAMESPACE_BEGIN

//=============================================================================
// Plane
//=============================================================================

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Plane::Plane()
	: Normal(0.0f, 0.0f, 0.0f)
	, D(0.0f)
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Plane::Plane(float a, float b, float c, float d)
	: Normal(a, b, c)
	, D(d)
{
}
	
//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Plane::Plane(const Vector3& p1, const Vector3& normal)
	: Normal(normal)
	, D(-Vector3::Dot(normal, p1))
{
}
	
//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Plane::Plane(const Vector3& point1, const Vector3& point2, const Vector3& point3)
{
	Normal = Vector3::Cross((point2 - point1), (point3 - point1));
	Normal.Normalize();

	D = -Vector3::Dot(point1, Normal);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Plane::Normalize()
{
	float t = 1.0f / Asm::sqrt((Normal.X * Normal.X) + (Normal.Y * Normal.Y) + (Normal.Z * Normal.Z));
	Normal.X *= t;
	Normal.Y *= t;
	Normal.Z *= t;
	D *= t;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
bool Plane::Intersects(const Vector3& start, const Vector3& end, Vector3* point) const
{
	Vector3 direction = end - start;
	float dot = Vector3::Dot(Normal, direction);
	if ( dot == 0.0f ) return false;

	if (point != NULL)
	{
		float t = (D + Vector3::Dot(Normal, start)) / dot;
		(*point) = start - (t * direction);
	}
	return true;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Plane::Transform(const Matrix& mat)
{
	float x = Normal.X;
	float y = Normal.Y;
	float z = Normal.Z;
	float d = D;
	Matrix t = Matrix::Inverse(mat);

	Normal.X = (((x * t.M11) + (y * t.M12)) + (z * t.M13)) + (d * t.M14);
	Normal.Y = (((x * t.M21) + (y * t.M22)) + (z * t.M23)) + (d * t.M24);
	Normal.Z = (((x * t.M31) + (y * t.M32)) + (z * t.M33)) + (d * t.M34);
	D = (((x * t.M41) + (y * t.M42)) + (z * t.M43)) + (d * t.M44);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Plane::Print(const char* format, FILE* stream) const
{
	if (!format) {
		format = "%f, %f, %f, %f\n";
	}
	if (!stream) {
		stream = stdout;
	}
	fprintf(stream, format, Normal.X, Normal.Y, Normal.Z, D);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Plane Plane::Normalize(const Plane& plane)
{
	float t = 1.0f / Asm::sqrt((plane.Normal.X * plane.Normal.X) + (plane.Normal.Y * plane.Normal.Y) + (plane.Normal.Z * plane.Normal.Z));
	return Plane(
		plane.Normal.X * t,
		plane.Normal.Y * t,
		plane.Normal.Z * t,
		plane.D * t);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
float Plane::Dot(const Plane& plane, const Vector4& vec)
{
	return (plane.Normal.X * vec.X) + (plane.Normal.Y * vec.Y) + (plane.Normal.Z * vec.Z) + (plane.D * vec.W);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
float Plane::DotCoord(const Plane& plane, const Vector3& vec)
{
	return (plane.Normal.X * vec.X) + (plane.Normal.Y * vec.Y) + (plane.Normal.Z * vec.Z) + plane.D;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
float Plane::DotNormal(const Plane& plane, const Vector3& vec)
{
	return (plane.Normal.X * vec.X) + (plane.Normal.Y * vec.Y) + (plane.Normal.Z * vec.Z);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Plane Plane::Transform(const Plane& plane, const Matrix& mat)
{
	float x = plane.Normal.X;
	float y = plane.Normal.Y;
	float z = plane.Normal.Z;
	float d = plane.D;
	Matrix t = Matrix::Inverse(mat);

	return Plane(
		(((x * t.M11) + (y * t.M12)) + (z * t.M13)) + (d * t.M14),
		(((x * t.M21) + (y * t.M22)) + (z * t.M23)) + (d * t.M24),
		(((x * t.M31) + (y * t.M32)) + (z * t.M33)) + (d * t.M34),
		(((x * t.M41) + (y * t.M42)) + (z * t.M43)) + (d * t.M44));
}

LN_NAMESPACE_END
