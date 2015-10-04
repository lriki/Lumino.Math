
#ifndef LUMINO_MATH_DXLIB_H
#define LUMINO_MATH_DXLIB_H

#ifndef __DXLIB
#error please include after "DxLib.h"
#endif

// DxLib::VECTOR ‚Æ‚Ì‘ŠŒÝ•ÏŠ·
#define LN_MATH_VECTOR3_EXTENSION \
	Vector3(const DxLib::VECTOR& v) { X = v.x; Y = v.y; Z = v.z; } \
	Vector3& operator = (const DxLib::VECTOR& v) { X = v.x; Y = v.y; Z = v.z; return *this; } \
	operator const DxLib::VECTOR& () { return *(reinterpret_cast<const DxLib::VECTOR*>(this)); }

// DxLib::MATRIX ‚Æ‚Ì‘ŠŒÝ•ÏŠ·
#define LN_MATH_MATRIX_EXTENSION \
	Matrix(const DxLib::MATRIX& m) { memcpy_s(this, sizeof(Matrix), &m, sizeof(DxLib::MATRIX)); } \
	Matrix& operator = (const DxLib::MATRIX& m) { memcpy_s(this, sizeof(Matrix), &m, sizeof(DxLib::MATRIX)); return *this; } \
	operator const DxLib::MATRIX& () { return *(reinterpret_cast<const DxLib::MATRIX*>(this)); }

#include "LuminoMath.h"

#ifdef _MSC_VER
	#if _MSC_VER >= 1900	// Visual C++ 14.0 (Visual Studio 2015)
		#define LN_MSVC_LIBVERNAME	"msvc140"
	#elif _MSC_VER >= 1800	// Visual C++ 12.0 (Visual Studio 2013)
		#define LN_MSVC_LIBVERNAME	"msvc120"
	#elif _MSC_VER >= 1700	// Visual C++ 11.0 (Visual Studio 2012)
		#define LN_MSVC_LIBVERNAME	"msvc110"
	#elif _MSC_VER >= 1600	// Visual C++ 10.0 (Visual Studio 2010)
		#define LN_MSVC_LIBVERNAME	"msvc100"
	#elif _MSC_VER >= 1500	// Visual C++ 9.0 (Visual Studio 2008)
		#define LN_MSVC_LIBVERNAME	"msvc90"
	#elif _MSC_VER >= 1400	// Visual C++ 8.0 (Visual Studio 2005)
		#define LN_MSVC_LIBVERNAME	"msvc80"
	#else
	#endif
#endif

#ifdef _DEBUG
	#pragma comment(lib, "LuminoMath_" LN_MSVC_LIBVERNAME "x86_static_MTd")
#else
	#pragma comment(lib, "LuminoMath_" LN_MSVC_LIBVERNAME "x86_static_MT")
#endif

#endif // LUMINO_MATH_DXLIB_H
