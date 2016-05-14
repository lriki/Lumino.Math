
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

/** 回転順序 */
enum class RotationOrder
{
	XYZ,			/**< X → Y → Z */
	YZX,			/**< Y → Z → X */
	ZXY,			/**< Z → X → Y (RotationYawPitchRoll) */
};

/** 錐台の各面 */
enum class FrustumPlane
{
	Near	= 0,
	Far		= 1,
	Left	= 2,
	Right	= 3,
	Top		= 4,
	Bottom	= 5,
};

LN_NAMESPACE_END

#endif // LUMINO_MATH_COMMON_H
