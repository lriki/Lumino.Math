
#ifndef LUMINO_MATH_ASM_H
#define LUMINO_MATH_ASM_H

#include <math.h>

namespace Lumino
{
class Asm
{
public:

	/// sqrt
	static inline float sqrt( float r )
	{
	#ifdef _MSC_VER
		__asm
		{
			fld		r
			fsqrt
			fstp	r
		}

		return r;

	#else
		return ::sqrt( r );
	#endif
	}

	/// sincos
	static inline void sincos(float r_, float* sin_, float* cos_)
	{
	#ifdef _MSC_VER
		float ts, tc;

		__asm
		{
			fld		r_
			fsincos
			fstp	tc
			fstp	ts
		}

		*sin_ = ts;
		*cos_ = tc;
	#else
		*sin_ = sin( r_ );
		*cos_ = cos( r_ );
	#endif
	}
};

} // namespace Lumino

#endif // LUMINO_MATH_ASM_H
