// This is a 4 dimensional Vect4D class
#ifndef Vect4D_H
#define Vect4D_H

#include "Enum.h"
#include<xmmintrin.h>
#include<smmintrin.h>

// Foward Declarations
class Matrix;

// class
class Vect4D: Align16
{
public:
	union
	{
		_declspec (align(16)) __m128 _m;

		struct
		{
			float x;
			float y;
			float z;
			float w;
		};
	};
	friend class Matrix;

	Vect4D();
	Vect4D(const Vect4D& v) = default;
	inline Vect4D& Vect4D::operator=(const Vect4D& vec)
	{
		this->_m = vec._m;
		return *this;
	}
	~Vect4D();

	Vect4D(const __m128 x);
	Vect4D(float tx, float ty, float tz, float tw=1.0f);

	const void norm(Vect4D &out) const;
	const void set(const float tx, const float ty, const float tz, const float tw=1.0f);

	inline const Vect4D Vect4D::operator+ (const Vect4D t) const
	{
		return Vect4D(_mm_add_ps(this->_m, t._m));
	}

	inline const Vect4D Vect4D::operator- (const Vect4D t) const
	{
		return Vect4D(_mm_sub_ps(this->_m, t._m));
	}

	inline const Vect4D Vect4D::operator* (const float scale) const
	{
		return Vect4D(_mm_mul_ps(_m, _mm_load1_ps(&scale)));
	}

	inline const void Vect4D::operator*= (const float tmp)
	{
		this->_m = _mm_mul_ps(this->_m, _mm_load1_ps(&tmp));
	}

	const void Cross(Vect4D& vout) const;

};

#endif  

// End of file

