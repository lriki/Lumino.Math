
#ifndef LUMINO_MATH_COMMON_H
#define LUMINO_MATH_COMMON_H

#if defined(LUMINO_DLL) && defined(LUMINO_BUILD_DLL)
	/* 
	 * LUMINO_DLL と LUMINO_BUILD_DLL の同時定義は禁止。
	 * LUMINO_DLL はライブラリをDLLとして使うとき、
	 * LUMINO_BUILD_DLL はライブラリ本体をビルドするときにのみ使用する。
	 */
	#error "You must not have both LUMINO_DLL and LUMINO_BUILD_DLL defined"
#endif

#if defined(_WIN32) && defined(LUMINO_BUILD_DLL)
	#define LUMINO_EXPORT	__declspec(dllexport)
#elif defined(_WIN32) && defined(LUMINO_DLL)
	#define LUMINO_EXPORT	__declspec(dllimport)
#elif defined(__GNUC__) && defined(LUMINO_BUILD_DLL)
	#define LUMINO_EXPORT	__attribute__((visibility("default")))
#else
	#define LUMINO_EXPORT	
#endif

#define LN_NAMESPACE_BEGIN	namespace ln {
#define LN_NAMESPACE_END	}
	
LN_NAMESPACE_BEGIN

/// 回転順序
enum RotationOrder
{
	RotationOrder_XYZ = 0,		///< X → Y → Z
	//RotationOrder_XZY,		///< X → Z → Y 
	//RotationOrder_YXZ,		///< Y → X → Z
	RotationOrder_YZX,			///< Y → Z → X
	RotationOrder_ZXY,			///< Z → X → Y (RotationYawPitchRoll)
	//RotationOrder_ZYX,		///< Z → Y → X
};

/// 錐台の各面
enum FrustumPlane
{
	FrustumPlane_Near = 0,
	FrustumPlane_Far,
	FrustumPlane_Left,
	FrustumPlane_Right,
	FrustumPlane_Top,
	FrustumPlane_Bottom,

	FrustumPlane_Max,
};

LN_NAMESPACE_END

#endif // __LUMINO_MATH_COMMON_H__