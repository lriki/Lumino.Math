
#include "../../include/Lumino/Math/SQTTransform.h"

namespace Lumino
{

//=============================================================================
// SQTTransform
//=============================================================================

const SQTTransform SQTTransform::Identity = SQTTransform();

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
SQTTransform::SQTTransform()
	: Scale(Vector3::One)
	, Rotation(Quaternion::Identity)
	, Translation(Vector3::Zero)
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
SQTTransform::SQTTransform(const Vector3& scale, const Quaternion& rotation, const Vector3& translation)
	: Scale(scale)
	, Rotation(rotation)
	, Translation(translation)
{
}
	
} // namespace Lumino
