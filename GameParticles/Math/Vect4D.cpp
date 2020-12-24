#include <math.h>

#include "Vect4D.h"

Vect4D::Vect4D()
{
	this->_m = _mm_setr_ps(0.0f, 0.0f, 0.0f, 1.0f);
}

Vect4D::Vect4D(const __m128 x)
{
	this->_m = x;
}

Vect4D::Vect4D(const float tx, const float ty, const float tz, const float tw)
{
	this->_m = _mm_set_ps(tw, tz, ty, tx);
}

Vect4D::~Vect4D()
{
}

const void Vect4D::norm(Vect4D& out) const
{
	__m128 mag = _mm_set_ps1(
		_mm_sqrt_ps(
			_mm_dp_ps(_mm_setr_ps(this->x, this->y, this->z, 0.0f), _mm_setr_ps(this->x, this->y, this->z, 0.0f), 0xF1)
		).m128_f32[0]
	);

	if (0.0f < mag.m128_f32[0])
	{
		mag.m128_f32[3] = 1.0f;
		out._m = _mm_div_ps(_mm_setr_ps(this->x, this->y, this->z, 1.0f), mag);
	}
}

const void Vect4D::Cross(Vect4D& vout) const
{
	vout._m = _mm_sub_ps(_mm_mul_ps(_mm_setr_ps(this->y, this->z, this->x, 0.0f),
									_mm_setr_ps(0.0f, 1.0f, 0.0f, 0.0f)),
						 _mm_mul_ps(_mm_setr_ps(this->z, this->x, this->y, 1.0f),
									_mm_setr_ps(0.0f, 0.0f, 1.0f, 1.0f))
	);
}

const void Vect4D::set(const float tx, const float ty, const float tz, const float tw)
{
	this->_m = _mm_set_ps(tw, tz, ty, tx);
}

// End of file
