
#include "../../include/Lumino/Math/Matrix.h"
#include "../../include/Lumino/Math/ViewFrustum.h"

LN_NAMESPACE_BEGIN

//=============================================================================
// ViewFrustum
//=============================================================================

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
ViewFrustum::ViewFrustum()
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
ViewFrustum::ViewFrustum(const Matrix& viewProjMatrix)
{
	SetViewProjMatrix(viewProjMatrix);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void ViewFrustum::SetViewProjMatrix(const Matrix& mat)
{
	mPlanes[FrustumPlane_Near] = Plane(-mat.M13, -mat.M23, -mat.M33, -mat.M43);
	mPlanes[FrustumPlane_Far] = Plane(-mat.M14 + mat.M13, -mat.M24 + mat.M23, -mat.M34 + mat.M33, -mat.M44 + mat.M43);
	mPlanes[FrustumPlane_Left] = Plane(-mat.M14 - mat.M11, -mat.M24 - mat.M21, -mat.M34 - mat.M31, -mat.M44 - mat.M41);
	mPlanes[FrustumPlane_Right] = Plane(-mat.M14 + mat.M11, -mat.M24 + mat.M21, -mat.M34 + mat.M31, -mat.M44 + mat.M41);
	mPlanes[FrustumPlane_Top] = Plane(-mat.M14 + mat.M12, -mat.M24 + mat.M22, -mat.M34 + mat.M32, -mat.M44 + mat.M42);
	mPlanes[FrustumPlane_Bottom] = Plane(-mat.M14 - mat.M12, -mat.M24 - mat.M22, -mat.M34 - mat.M32, -mat.M44 - mat.M42);

	for (int i = 0; i < FrustumPlane_Max; ++i)
	{
		mPlanes[i].Normalize();
	}

	mViewProjMatrix = mat;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
bool ViewFrustum::Intersects(const Vector3& point) const
{
	for (int i = 0; i < FrustumPlane_Max; ++i)
	{
		if (!mPlanes[i].CheckInsideLower(point)) {
			return false;
		}
	}
	return true;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
bool ViewFrustum::Intersects(const Vector3& center, float radius) const
{
	for (int i = 0; i < FrustumPlane_Max; ++i)
	{
		if (!mPlanes[i].CheckInside(center, radius)) {
			return false;
		}
	}
	return true;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void ViewFrustum::GetCornerPoints(Vector3* points) const
{
	// Near
	points[0].Set(-1, 1, 0);
	points[1].Set(1, 1, 0);
	points[2].Set(1, -1, 0);
	points[3].Set(-1, -1, 0);

	// Far
	points[4].Set(-1, 1, 1);
	points[5].Set(1, 1, 1);
	points[6].Set(1, -1, 1);
	points[7].Set(-1, -1, 1);

	// Transfrom
	Matrix inv = Matrix::Inverse(mViewProjMatrix);
	for (int i = 0; i < 8; ++i)
	{
		points[i].TransformCoord(inv);
	}
}

LN_NAMESPACE_END

