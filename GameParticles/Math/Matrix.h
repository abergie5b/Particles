#ifndef MATRIX_H
#define MATRIX_H

// includes
#include "Enum.h"
#include "Vect4D.h"


// class
class Matrix: Align16
{
public:

	// local enumerations
	enum MatrixRowEnum
	{
		MATRIX_ROW_0,
		MATRIX_ROW_1,
		MATRIX_ROW_2,
		MATRIX_ROW_3
	};
	friend struct MmultM;

	Matrix();
	Matrix(const Matrix& t);
	const void operator= (const Matrix& rhs);
	~Matrix();
	Matrix(const __m128& v0, const __m128& v1, const __m128& v2, const __m128& v3);

	const void setIdentMatrix();
	const void setTransMatrix(const Vect4D *t);
	const void setScaleMatrix(const Vect4D *s);
	const void setRotZMatrix(const float Z_Radians);

	//const Matrix operator*(const Matrix& t);

	const float Determinant();

	const Matrix GetAdjugate();
	const Matrix& Matrix::operator/=(const float t);

	const void Matrix::Inverse(Matrix &out);

	union
	{

		struct
		{
			Vect4D v0;
			Vect4D v1;
			Vect4D v2;
			Vect4D v3;
		};

		struct
		{
			__m128 mm0;
			__m128 mm1;
			__m128 mm2;
			__m128 mm3;
		};

		struct
		{
			// ROW 0
			float m0;
			float m1;
			float m2;
			float m3;

			// ROW 1
			float m4;
			float m5;
			float m6;
			float m7;

			// ROW 2
			float m8;
			float m9;
			float m10;
			float m11;

			// ROW 3
			float m12;
			float m13;
			float m14;
			float m15;
		};
	};
};

struct MmultM
{
	const Matrix& _m1;
	const Matrix& _m2;

	MmultM(const Matrix& _m1, const Matrix& _m2)
		: _m1(_m1), _m2(_m2)
	{
	};

	operator Matrix()
	{
		Matrix tmp;
		__m128 multCol0 = _mm_mul_ps(_mm_load1_ps(&_m1.m0), _m2.v0._m);
		__m128 multCol1 = _mm_mul_ps(_mm_load1_ps(&_m1.m1), _m2.v1._m);
		__m128 multCol2 = _mm_mul_ps(_mm_load1_ps(&_m1.m2), _m2.v2._m);
		__m128 multCol3 = _mm_mul_ps(_mm_load1_ps(&_m1.m3), _m2.v3._m);
		tmp.v0._m = _mm_add_ps(_mm_add_ps(multCol0, multCol1), _mm_add_ps(multCol2, multCol3));

		__m128 multCol4 = _mm_mul_ps(_mm_load1_ps(&_m1.m4), _m2.v0._m);
		__m128 multCol5 = _mm_mul_ps(_mm_load1_ps(&_m1.m5), _m2.v1._m);
		__m128 multCol6 = _mm_mul_ps(_mm_load1_ps(&_m1.m6), _m2.v2._m);
		__m128 multCol7 = _mm_mul_ps(_mm_load1_ps(&_m1.m7), _m2.v3._m);
		tmp.v1._m = _mm_add_ps(_mm_add_ps(multCol4, multCol5), _mm_add_ps(multCol6, multCol7));

		__m128 multCol8 = _mm_mul_ps(_mm_load1_ps(&_m1.m8), _m2.v0._m);
		__m128 multCol9 = _mm_mul_ps(_mm_load1_ps(&_m1.m9), _m2.v1._m);
		__m128 multCol10 = _mm_mul_ps(_mm_load1_ps(&_m1.m10), _m2.v2._m);
		__m128 multCol11 = _mm_mul_ps(_mm_load1_ps(&_m1.m11), _m2.v3._m);
		tmp.v2._m = _mm_add_ps(_mm_add_ps(multCol8, multCol9), _mm_add_ps(multCol10, multCol11));

		__m128 multCol12 = _mm_mul_ps(_mm_load1_ps(&_m1.m12), _m2.v0._m);
		__m128 multCol13 = _mm_mul_ps(_mm_load1_ps(&_m1.m13), _m2.v1._m);
		__m128 multCol14 = _mm_mul_ps(_mm_load1_ps(&_m1.m14), _m2.v2._m);
		__m128 multCol15 = _mm_mul_ps(_mm_load1_ps(&_m1.m15), _m2.v3._m);
		tmp.v3._m = _mm_add_ps(_mm_add_ps(multCol12, multCol13), _mm_add_ps(multCol14, multCol15));

		return tmp;
	}
};

inline MmultM operator*(const Matrix& _m1, const Matrix& _m2)
{
	return MmultM(_m1, _m2);
}

struct MmultMmultM
{
	const Matrix& _m1;
	const Matrix& _m2;
	const Matrix& _m3;

	MmultMmultM(const MmultM& _m1, const Matrix& _m2)
		: _m1(_m1._m1), _m2(_m1._m2), _m3(_m2)
	{
	};

	operator Matrix()
	{
		Matrix tmp;
		__m128 multCol0 = _mm_mul_ps(_mm_load1_ps(&_m1.m0), _m2.v0._m);
		__m128 multCol1 = _mm_mul_ps(_mm_load1_ps(&_m1.m1), _m2.v1._m);
		__m128 multCol2 = _mm_mul_ps(_mm_load1_ps(&_m1.m2), _m2.v2._m);
		__m128 multCol3 = _mm_mul_ps(_mm_load1_ps(&_m1.m3), _m2.v3._m);
		tmp.v0._m = _mm_add_ps(_mm_add_ps(multCol0, multCol1), _mm_add_ps(multCol2, multCol3));

		__m128 multCol4 = _mm_mul_ps(_mm_load1_ps(&_m1.m4), _m2.v0._m);
		__m128 multCol5 = _mm_mul_ps(_mm_load1_ps(&_m1.m5), _m2.v1._m);
		__m128 multCol6 = _mm_mul_ps(_mm_load1_ps(&_m1.m6), _m2.v2._m);
		__m128 multCol7 = _mm_mul_ps(_mm_load1_ps(&_m1.m7), _m2.v3._m);
		tmp.v1._m = _mm_add_ps(_mm_add_ps(multCol4, multCol5), _mm_add_ps(multCol6, multCol7));

		__m128 multCol8 = _mm_mul_ps(_mm_load1_ps(&_m1.m8), _m2.v0._m);
		__m128 multCol9 = _mm_mul_ps(_mm_load1_ps(&_m1.m9), _m2.v1._m);
		__m128 multCol10 = _mm_mul_ps(_mm_load1_ps(&_m1.m10), _m2.v2._m);
		__m128 multCol11 = _mm_mul_ps(_mm_load1_ps(&_m1.m11), _m2.v3._m);
		tmp.v2._m = _mm_add_ps(_mm_add_ps(multCol8, multCol9), _mm_add_ps(multCol10, multCol11));

		__m128 multCol12 = _mm_mul_ps(_mm_load1_ps(&_m1.m12), _m2.v0._m);
		__m128 multCol13 = _mm_mul_ps(_mm_load1_ps(&_m1.m13), _m2.v1._m);
		__m128 multCol14 = _mm_mul_ps(_mm_load1_ps(&_m1.m14), _m2.v2._m);
		__m128 multCol15 = _mm_mul_ps(_mm_load1_ps(&_m1.m15), _m2.v3._m);
		tmp.v3._m = _mm_add_ps(_mm_add_ps(multCol12, multCol13), _mm_add_ps(multCol14, multCol15));
		//

		multCol0 = _mm_mul_ps(_mm_load1_ps(&tmp.m0), _m3.v0._m);
		multCol1 = _mm_mul_ps(_mm_load1_ps(&tmp.m1), _m3.v1._m);
		multCol2 = _mm_mul_ps(_mm_load1_ps(&tmp.m2), _m3.v2._m);
		multCol3 = _mm_mul_ps(_mm_load1_ps(&tmp.m3), _m3.v3._m);
		tmp.v0._m = _mm_add_ps(_mm_add_ps(multCol0, multCol1), _mm_add_ps(multCol2, multCol3));

		multCol4 = _mm_mul_ps(_mm_load1_ps(&tmp.m4), _m3.v0._m);
		multCol5 = _mm_mul_ps(_mm_load1_ps(&tmp.m5), _m3.v1._m);
		multCol6 = _mm_mul_ps(_mm_load1_ps(&tmp.m6), _m3.v2._m);
		multCol7 = _mm_mul_ps(_mm_load1_ps(&tmp.m7), _m3.v3._m);
		tmp.v1._m = _mm_add_ps(_mm_add_ps(multCol4, multCol5), _mm_add_ps(multCol6, multCol7));

		multCol8 = _mm_mul_ps(_mm_load1_ps(&tmp.m8), _m3.v0._m);
		multCol9 = _mm_mul_ps(_mm_load1_ps(&tmp.m9), _m3.v1._m);
		multCol10 = _mm_mul_ps(_mm_load1_ps(&tmp.m10), _m3.v2._m);
		multCol11 = _mm_mul_ps(_mm_load1_ps(&tmp.m11), _m3.v3._m);
		tmp.v2._m = _mm_add_ps(_mm_add_ps(multCol8, multCol9), _mm_add_ps(multCol10, multCol11));

		multCol12 = _mm_mul_ps(_mm_load1_ps(&tmp.m12), _m3.v0._m);
		multCol13 = _mm_mul_ps(_mm_load1_ps(&tmp.m13), _m3.v1._m);
		multCol14 = _mm_mul_ps(_mm_load1_ps(&tmp.m14), _m3.v2._m);
		multCol15 = _mm_mul_ps(_mm_load1_ps(&tmp.m15), _m3.v3._m);
		tmp.v3._m = _mm_add_ps(_mm_add_ps(multCol12, multCol13), _mm_add_ps(multCol14, multCol15));

		return tmp;
	}
};

inline MmultMmultM operator*(const MmultM& _m1, const Matrix& _m2)
{
	return MmultMmultM(_m1, _m2);
}

struct MmultMmultMmultM
{
	const Matrix& _m1;
	const Matrix& _m2;
	const Matrix& _m3;
	const Matrix& _m4;

	MmultMmultMmultM(const MmultMmultM& _m1, const Matrix& _m2)
		: _m1(_m1._m1), _m2(_m1._m2), _m3(_m1._m3), _m4(_m2)
	{
	};

	operator Matrix()
	{
		Matrix tmp;
		__m128 multCol0 = _mm_mul_ps(_mm_load1_ps(&_m1.m0), _m2.v0._m);
		__m128 multCol1 = _mm_mul_ps(_mm_load1_ps(&_m1.m1), _m2.v1._m);
		__m128 multCol2 = _mm_mul_ps(_mm_load1_ps(&_m1.m2), _m2.v2._m);
		__m128 multCol3 = _mm_mul_ps(_mm_load1_ps(&_m1.m3), _m2.v3._m);
		tmp.v0._m = _mm_add_ps(_mm_add_ps(multCol0, multCol1), _mm_add_ps(multCol2, multCol3));

		__m128 multCol4 = _mm_mul_ps(_mm_load1_ps(&_m1.m4), _m2.v0._m);
		__m128 multCol5 = _mm_mul_ps(_mm_load1_ps(&_m1.m5), _m2.v1._m);
		__m128 multCol6 = _mm_mul_ps(_mm_load1_ps(&_m1.m6), _m2.v2._m);
		__m128 multCol7 = _mm_mul_ps(_mm_load1_ps(&_m1.m7), _m2.v3._m);
		tmp.v1._m = _mm_add_ps(_mm_add_ps(multCol4, multCol5), _mm_add_ps(multCol6, multCol7));

		__m128 multCol8 = _mm_mul_ps(_mm_load1_ps(&_m1.m8), _m2.v0._m);
		__m128 multCol9 = _mm_mul_ps(_mm_load1_ps(&_m1.m9), _m2.v1._m);
		__m128 multCol10 = _mm_mul_ps(_mm_load1_ps(&_m1.m10), _m2.v2._m);
		__m128 multCol11 = _mm_mul_ps(_mm_load1_ps(&_m1.m11), _m2.v3._m);
		tmp.v2._m = _mm_add_ps(_mm_add_ps(multCol8, multCol9), _mm_add_ps(multCol10, multCol11));

		__m128 multCol12 = _mm_mul_ps(_mm_load1_ps(&_m1.m12), _m2.v0._m);
		__m128 multCol13 = _mm_mul_ps(_mm_load1_ps(&_m1.m13), _m2.v1._m);
		__m128 multCol14 = _mm_mul_ps(_mm_load1_ps(&_m1.m14), _m2.v2._m);
		__m128 multCol15 = _mm_mul_ps(_mm_load1_ps(&_m1.m15), _m2.v3._m);
		tmp.v3._m = _mm_add_ps(_mm_add_ps(multCol12, multCol13), _mm_add_ps(multCol14, multCol15));
		//

		multCol0 = _mm_mul_ps(_mm_load1_ps(&tmp.m0), _m3.v0._m);
		multCol1 = _mm_mul_ps(_mm_load1_ps(&tmp.m1), _m3.v1._m);
		multCol2 = _mm_mul_ps(_mm_load1_ps(&tmp.m2), _m3.v2._m);
		multCol3 = _mm_mul_ps(_mm_load1_ps(&tmp.m3), _m3.v3._m);
		tmp.v0._m = _mm_add_ps(_mm_add_ps(multCol0, multCol1), _mm_add_ps(multCol2, multCol3));

		multCol4 = _mm_mul_ps(_mm_load1_ps(&tmp.m4), _m3.v0._m);
		multCol5 = _mm_mul_ps(_mm_load1_ps(&tmp.m5), _m3.v1._m);
		multCol6 = _mm_mul_ps(_mm_load1_ps(&tmp.m6), _m3.v2._m);
		multCol7 = _mm_mul_ps(_mm_load1_ps(&tmp.m7), _m3.v3._m);
		tmp.v1._m = _mm_add_ps(_mm_add_ps(multCol4, multCol5), _mm_add_ps(multCol6, multCol7));

		multCol8 = _mm_mul_ps(_mm_load1_ps(&tmp.m8), _m3.v0._m);
		multCol9 = _mm_mul_ps(_mm_load1_ps(&tmp.m9), _m3.v1._m);
		multCol10 = _mm_mul_ps(_mm_load1_ps(&tmp.m10), _m3.v2._m);
		multCol11 = _mm_mul_ps(_mm_load1_ps(&tmp.m11), _m3.v3._m);
		tmp.v2._m = _mm_add_ps(_mm_add_ps(multCol8, multCol9), _mm_add_ps(multCol10, multCol11));

		multCol12 = _mm_mul_ps(_mm_load1_ps(&tmp.m12), _m3.v0._m);
		multCol13 = _mm_mul_ps(_mm_load1_ps(&tmp.m13), _m3.v1._m);
		multCol14 = _mm_mul_ps(_mm_load1_ps(&tmp.m14), _m3.v2._m);
		multCol15 = _mm_mul_ps(_mm_load1_ps(&tmp.m15), _m3.v3._m);
		tmp.v3._m = _mm_add_ps(_mm_add_ps(multCol12, multCol13), _mm_add_ps(multCol14, multCol15));
		//

		multCol0 = _mm_mul_ps(_mm_load1_ps(&tmp.m0), _m4.v0._m);
		multCol1 = _mm_mul_ps(_mm_load1_ps(&tmp.m1), _m4.v1._m);
		multCol2 = _mm_mul_ps(_mm_load1_ps(&tmp.m2), _m4.v2._m);
		multCol3 = _mm_mul_ps(_mm_load1_ps(&tmp.m3), _m4.v3._m);
		tmp.v0._m = _mm_add_ps(_mm_add_ps(multCol0, multCol1), _mm_add_ps(multCol2, multCol3));

		multCol4 = _mm_mul_ps(_mm_load1_ps(&tmp.m4), _m4.v0._m);
		multCol5 = _mm_mul_ps(_mm_load1_ps(&tmp.m5), _m4.v1._m);
		multCol6 = _mm_mul_ps(_mm_load1_ps(&tmp.m6), _m4.v2._m);
		multCol7 = _mm_mul_ps(_mm_load1_ps(&tmp.m7), _m4.v3._m);
		tmp.v1._m = _mm_add_ps(_mm_add_ps(multCol4, multCol5), _mm_add_ps(multCol6, multCol7));

		multCol8 = _mm_mul_ps(_mm_load1_ps(&tmp.m8), _m4.v0._m);
		multCol9 = _mm_mul_ps(_mm_load1_ps(&tmp.m9), _m4.v1._m);
		multCol10 = _mm_mul_ps(_mm_load1_ps(&tmp.m10), _m4.v2._m);
		multCol11 = _mm_mul_ps(_mm_load1_ps(&tmp.m11), _m4.v3._m);
		tmp.v2._m = _mm_add_ps(_mm_add_ps(multCol8, multCol9), _mm_add_ps(multCol10, multCol11));

		multCol12 = _mm_mul_ps(_mm_load1_ps(&tmp.m12), _m4.v0._m);
		multCol13 = _mm_mul_ps(_mm_load1_ps(&tmp.m13), _m4.v1._m);
		multCol14 = _mm_mul_ps(_mm_load1_ps(&tmp.m14), _m4.v2._m);
		multCol15 = _mm_mul_ps(_mm_load1_ps(&tmp.m15), _m4.v3._m);
		tmp.v3._m = _mm_add_ps(_mm_add_ps(multCol12, multCol13), _mm_add_ps(multCol14, multCol15));
		//
		return tmp;
	}
};

inline MmultMmultMmultM operator*(const MmultMmultM& _m1, const Matrix& _m2)
{
	return MmultMmultMmultM(_m1, _m2);
}

struct MmultMmultMmultMmultM
{
	const Matrix& _m1;
	const Matrix& _m2;
	const Matrix& _m3;
	const Matrix& _m4;
	const Matrix& _m5;

	MmultMmultMmultMmultM(const MmultMmultMmultM& _m1, const Matrix& _m2)
		: _m1(_m1._m1), _m2(_m1._m2), _m3(_m1._m3), _m4(_m1._m4), _m5(_m2)
	{
	};

	operator Matrix()
	{
		Matrix tmp;
		__m128 multCol0 = _mm_mul_ps(_mm_load1_ps(&_m1.m0), _m2.v0._m);
		__m128 multCol1 = _mm_mul_ps(_mm_load1_ps(&_m1.m1), _m2.v1._m);
		__m128 multCol2 = _mm_mul_ps(_mm_load1_ps(&_m1.m2), _m2.v2._m);
		__m128 multCol3 = _mm_mul_ps(_mm_load1_ps(&_m1.m3), _m2.v3._m);
		tmp.v0._m = _mm_add_ps(_mm_add_ps(multCol0, multCol1), _mm_add_ps(multCol2, multCol3));

		__m128 multCol4 = _mm_mul_ps(_mm_load1_ps(&_m1.m4), _m2.v0._m);
		__m128 multCol5 = _mm_mul_ps(_mm_load1_ps(&_m1.m5), _m2.v1._m);
		__m128 multCol6 = _mm_mul_ps(_mm_load1_ps(&_m1.m6), _m2.v2._m);
		__m128 multCol7 = _mm_mul_ps(_mm_load1_ps(&_m1.m7), _m2.v3._m);
		tmp.v1._m = _mm_add_ps(_mm_add_ps(multCol4, multCol5), _mm_add_ps(multCol6, multCol7));

		__m128 multCol8 = _mm_mul_ps(_mm_load1_ps(&_m1.m8), _m2.v0._m);
		__m128 multCol9 = _mm_mul_ps(_mm_load1_ps(&_m1.m9), _m2.v1._m);
		__m128 multCol10 = _mm_mul_ps(_mm_load1_ps(&_m1.m10), _m2.v2._m);
		__m128 multCol11 = _mm_mul_ps(_mm_load1_ps(&_m1.m11), _m2.v3._m);
		tmp.v2._m = _mm_add_ps(_mm_add_ps(multCol8, multCol9), _mm_add_ps(multCol10, multCol11));

		__m128 multCol12 = _mm_mul_ps(_mm_load1_ps(&_m1.m12), _m2.v0._m);
		__m128 multCol13 = _mm_mul_ps(_mm_load1_ps(&_m1.m13), _m2.v1._m);
		__m128 multCol14 = _mm_mul_ps(_mm_load1_ps(&_m1.m14), _m2.v2._m);
		__m128 multCol15 = _mm_mul_ps(_mm_load1_ps(&_m1.m15), _m2.v3._m);
		tmp.v3._m = _mm_add_ps(_mm_add_ps(multCol12, multCol13), _mm_add_ps(multCol14, multCol15));
		//

		multCol0 = _mm_mul_ps(_mm_load1_ps(&tmp.m0), _m3.v0._m);
		multCol1 = _mm_mul_ps(_mm_load1_ps(&tmp.m1), _m3.v1._m);
		multCol2 = _mm_mul_ps(_mm_load1_ps(&tmp.m2), _m3.v2._m);
		multCol3 = _mm_mul_ps(_mm_load1_ps(&tmp.m3), _m3.v3._m);
		tmp.v0._m = _mm_add_ps(_mm_add_ps(multCol0, multCol1), _mm_add_ps(multCol2, multCol3));

		multCol4 = _mm_mul_ps(_mm_load1_ps(&tmp.m4), _m3.v0._m);
		multCol5 = _mm_mul_ps(_mm_load1_ps(&tmp.m5), _m3.v1._m);
		multCol6 = _mm_mul_ps(_mm_load1_ps(&tmp.m6), _m3.v2._m);
		multCol7 = _mm_mul_ps(_mm_load1_ps(&tmp.m7), _m3.v3._m);
		tmp.v1._m = _mm_add_ps(_mm_add_ps(multCol4, multCol5), _mm_add_ps(multCol6, multCol7));

		multCol8 = _mm_mul_ps(_mm_load1_ps(&tmp.m8), _m3.v0._m);
		multCol9 = _mm_mul_ps(_mm_load1_ps(&tmp.m9), _m3.v1._m);
		multCol10 = _mm_mul_ps(_mm_load1_ps(&tmp.m10), _m3.v2._m);
		multCol11 = _mm_mul_ps(_mm_load1_ps(&tmp.m11), _m3.v3._m);
		tmp.v2._m = _mm_add_ps(_mm_add_ps(multCol8, multCol9), _mm_add_ps(multCol10, multCol11));

		multCol12 = _mm_mul_ps(_mm_load1_ps(&tmp.m12), _m3.v0._m);
		multCol13 = _mm_mul_ps(_mm_load1_ps(&tmp.m13), _m3.v1._m);
		multCol14 = _mm_mul_ps(_mm_load1_ps(&tmp.m14), _m3.v2._m);
		multCol15 = _mm_mul_ps(_mm_load1_ps(&tmp.m15), _m3.v3._m);
		tmp.v3._m = _mm_add_ps(_mm_add_ps(multCol12, multCol13), _mm_add_ps(multCol14, multCol15));
		//

		multCol0 = _mm_mul_ps(_mm_load1_ps(&tmp.m0), _m4.v0._m);
		multCol1 = _mm_mul_ps(_mm_load1_ps(&tmp.m1), _m4.v1._m);
		multCol2 = _mm_mul_ps(_mm_load1_ps(&tmp.m2), _m4.v2._m);
		multCol3 = _mm_mul_ps(_mm_load1_ps(&tmp.m3), _m4.v3._m);
		tmp.v0._m = _mm_add_ps(_mm_add_ps(multCol0, multCol1), _mm_add_ps(multCol2, multCol3));

		multCol4 = _mm_mul_ps(_mm_load1_ps(&tmp.m4), _m4.v0._m);
		multCol5 = _mm_mul_ps(_mm_load1_ps(&tmp.m5), _m4.v1._m);
		multCol6 = _mm_mul_ps(_mm_load1_ps(&tmp.m6), _m4.v2._m);
		multCol7 = _mm_mul_ps(_mm_load1_ps(&tmp.m7), _m4.v3._m);
		tmp.v1._m = _mm_add_ps(_mm_add_ps(multCol4, multCol5), _mm_add_ps(multCol6, multCol7));

		multCol8 = _mm_mul_ps(_mm_load1_ps(&tmp.m8), _m4.v0._m);
		multCol9 = _mm_mul_ps(_mm_load1_ps(&tmp.m9), _m4.v1._m);
		multCol10 = _mm_mul_ps(_mm_load1_ps(&tmp.m10), _m4.v2._m);
		multCol11 = _mm_mul_ps(_mm_load1_ps(&tmp.m11), _m4.v3._m);
		tmp.v2._m = _mm_add_ps(_mm_add_ps(multCol8, multCol9), _mm_add_ps(multCol10, multCol11));

		multCol12 = _mm_mul_ps(_mm_load1_ps(&tmp.m12), _m4.v0._m);
		multCol13 = _mm_mul_ps(_mm_load1_ps(&tmp.m13), _m4.v1._m);
		multCol14 = _mm_mul_ps(_mm_load1_ps(&tmp.m14), _m4.v2._m);
		multCol15 = _mm_mul_ps(_mm_load1_ps(&tmp.m15), _m4.v3._m);
		tmp.v3._m = _mm_add_ps(_mm_add_ps(multCol12, multCol13), _mm_add_ps(multCol14, multCol15));
		//

		multCol0 = _mm_mul_ps(_mm_load1_ps(&tmp.m0), _m5.v0._m);
		multCol1 = _mm_mul_ps(_mm_load1_ps(&tmp.m1), _m5.v1._m);
		multCol2 = _mm_mul_ps(_mm_load1_ps(&tmp.m2), _m5.v2._m);
		multCol3 = _mm_mul_ps(_mm_load1_ps(&tmp.m3), _m5.v3._m);
		tmp.v0._m = _mm_add_ps(_mm_add_ps(multCol0, multCol1), _mm_add_ps(multCol2, multCol3));

		multCol4 = _mm_mul_ps(_mm_load1_ps(&tmp.m4), _m5.v0._m);
		multCol5 = _mm_mul_ps(_mm_load1_ps(&tmp.m5), _m5.v1._m);
		multCol6 = _mm_mul_ps(_mm_load1_ps(&tmp.m6), _m5.v2._m);
		multCol7 = _mm_mul_ps(_mm_load1_ps(&tmp.m7), _m5.v3._m);
		tmp.v1._m = _mm_add_ps(_mm_add_ps(multCol4, multCol5), _mm_add_ps(multCol6, multCol7));

		multCol8 = _mm_mul_ps(_mm_load1_ps(&tmp.m8), _m5.v0._m);
		multCol9 = _mm_mul_ps(_mm_load1_ps(&tmp.m9), _m5.v1._m);
		multCol10 = _mm_mul_ps(_mm_load1_ps(&tmp.m10), _m5.v2._m);
		multCol11 = _mm_mul_ps(_mm_load1_ps(&tmp.m11), _m5.v3._m);
		tmp.v2._m = _mm_add_ps(_mm_add_ps(multCol8, multCol9), _mm_add_ps(multCol10, multCol11));

		multCol12 = _mm_mul_ps(_mm_load1_ps(&tmp.m12), _m5.v0._m);
		multCol13 = _mm_mul_ps(_mm_load1_ps(&tmp.m13), _m5.v1._m);
		multCol14 = _mm_mul_ps(_mm_load1_ps(&tmp.m14), _m5.v2._m);
		multCol15 = _mm_mul_ps(_mm_load1_ps(&tmp.m15), _m5.v3._m);
		tmp.v3._m = _mm_add_ps(_mm_add_ps(multCol12, multCol13), _mm_add_ps(multCol14, multCol15));
		//

		return tmp;
	}
};

inline MmultMmultMmultMmultM operator*(const MmultMmultMmultM& _m1, const Matrix& _m2)
{
	return MmultMmultMmultMmultM(_m1, _m2);
}


#endif 

// End of File

