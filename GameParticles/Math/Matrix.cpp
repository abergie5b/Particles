#include <Math.h>

#include "Vect4D.h"
#include "Matrix.h"

Matrix::Matrix()
	:v0(0.0f, 0.0f, 0.0f, 0.0f), 
	 v1(0.0f, 0.0f, 0.0f, 0.0f), 
	 v2(0.0f, 0.0f, 0.0f, 0.0f), 
	 v3(0.0f, 0.0f, 0.0f, 0.0f)
{
}

Matrix::Matrix(const Matrix& t)
	:v0(t.v0), v1(t.v1), v2(t.v2), v3(t.v3)
{ 
}

const void Matrix::operator= (const Matrix& rhs)
{
	mm0 = rhs.mm0;
	mm1 = rhs.mm1;
	mm2 = rhs.mm2;
	mm3 = rhs.mm3;
}

Matrix::~Matrix()
{
	// nothign to delete
}

const void Matrix::setIdentMatrix()
{
	mm0 = _mm_set_ps(0.0f, 0.0f, 0.0f, 1.0f);
	mm1 = _mm_set_ps(0.0f, 0.0f, 1.0f, 0.0f);
	mm2 = _mm_set_ps(0.0f, 1.0f, 0.0f, 0.0f);
	mm3 = _mm_set_ps(1.0f, 0.0f, 0.0f, 0.0f);
}

const void Matrix::setTransMatrix(const Vect4D *t)
{
	mm0 = _mm_set_ps(0.0f, 0.0f, 0.0f, 1.0f);
	mm1 = _mm_set_ps(0.0f, 0.0f, 1.0f, 0.0f);
	mm2 = _mm_set_ps(0.0f, 1.0f, 0.0f, 0.0f);
	mm3 = t->_m;
}

Matrix::Matrix(const __m128& mm0, const __m128& mm1, const __m128& mm2, const __m128& mm3)
	:mm0(mm0), mm1(mm1), mm2(mm2), mm3(mm3)
{

}

const Matrix& Matrix::operator/=(const float rhs)
{
	float inv_rhs = 1.0f / rhs;
	__m128 invReg = _mm_load1_ps(&inv_rhs);
	mm0 = _mm_mul_ps(mm0, invReg);
	mm1 = _mm_mul_ps(mm1, invReg);
	mm2 = _mm_mul_ps(mm2, invReg);
	mm3 = _mm_mul_ps(mm3, invReg);
	return *this;
}

const float Matrix::Determinant()
{

	// ta = (lq - mp)
	float ta = (m10 * m15) - (m11 * m14);
	// tb = (kq - mo)
	float tb = (m9 * m15) - (m11 * m13);
	// tc = (kp - lo)
	float tc = (m9 * m14) - (m10 * m13);
	// td = (jq - mn)
	float td = (m8 * m15) - (m11 * m12);
	// te = (jo - kn)
	float te = (m8 * m13) - (m9 *  m12);
	// tf = (jp - ln)
	float tf = (m8 * m14) - (m10 * m12);
	
	return ((m0 * ((m5 * ta) - (m6 * tb) + (m7 * tc)))
			- (m1 * ((m4 * ta) - (m6 * td) + (m7 * tf)))
			+ (m2 * ((m4 * tb) - (m5 * td) + (m7 * te)))
			- (m3 * ((m4 * tc) - (m5 * tf) + (m6 * te))));

}

const Matrix Matrix::GetAdjugate()
{

	Matrix tmp;

	// load		ABC		(3)		ABC--
	float t1 = (m10*m15) - (m11*m14);
	float t2 = (m9*m15) - (m11*m13);
	float t3 = (m9*m14) - (m10*m13);

	// a = f(ta) - g(tb) + h(tc)
	tmp.m0 = (m5*t1) - (m6*t2) + (m7*t3);
	// b = -( b(ta) - c(tb) + d(tc)) 
	tmp.m1 = -((m1*t1) - (m2*t2) + (m3*t3));

	// load		JK		(5)		ABCJK
	float t4 = (m8*m15) - (m11*m12);
	float t5 = (m8*m14) - (m10*m12);
	// e = - ( e(ta) - g(tj) + h(tk))
	tmp.m4 = -((m4*t1) - (m6*t4) + (m7*t5));
	// f = a(ta) - c(tj) + d(tk)
	tmp.m5 = (m0*t1) - (m2*t4) + (m3*t5);

	// unload	AJ		(3)		-BC-K
	// load		P		(4)		PBC-K
	t1 = (m8*m13) - (m9*m12);
	// n = -( e(tc) - f(tk) + g(tp))
	tmp.m12 = -((m4*t3) - (m5*t5) + (m6*t1));
	// o = a(tc) - b(tk) + c(tp)
	tmp.m13 = (m0*t3) - (m1*t5) + (m2*t1);

	// unload	KC		(2)		PB---
	// load		J		(3)		PBJ--
	t3 = (m8*m15) - (m11*m12);

	// j = e(tb) - f(tj) + h(tp)
	tmp.m8 = (m4*t2) - (m5*t3) + (m7*t1);
	// k = - ( a(tb) - b(tj) + d(tp))
	tmp.m9 = -((m0*t2) - (m1*t3) + (m3*t1));

	// unload	BPJ		(0)		-----
	// load		DLM		(3)		DLM--
	t1 = (m6*m15) - (m7*m14);
	t2 = (m4*m15) - (m7*m12);
	t3 = (m4*m14) - (m6*m12);

	// g = - ( a(td) - c(tl) + d(tm))
	tmp.m6 = -((m0*t1) - (m2*t2) + (m3*t3));

	// load		FR		(5)		DLMFR
	t4 = (m5*m14) - (m6*m13);
	t5 = (m4*m13) - (m5*m12);

	// p = - ( a(tf) - b(tm) + c(tr))
	tmp.m14 = -((m0*t4) - (m1*t3) + (m3*t5));

	// unload	M		(4)		DL-FR 
	// load		E		(5)		DLEFR 
	t3 = (m5*m15) - (m7*m13);

	// l = a(te) - b(tl) + d(tr)
	tmp.m10 = (m0*t3) - (m1*t2) + (m3*t5);

	// unload	LR		(3)		D-EF-
	// c = b(td) - c(te) + d(tf)
	tmp.m2 = (m1*t1) - (m2*t3) + (m3*t4);

	// unload	DEF		(0)		-----
	// load		GHI		(3)		GHI--
	t1 = (m6*m11) - (m7*m10);
	t2 = (m5*m11) - (m7*m9);
	t3 = (m5*m10) - (m6*m9);
	
	// d = -( b(tg) - c(th) + d(ti) )
	tmp.m3 = -( (m1*t1) - (m2*t2) + (m3*t3));
	
	// load		NO		(5)		GHINO
	t4 = (m4*m11) - (m7*m8);
	t5 = (m4*m10) - (m6*m8);

	// h = a(tg) - c(tn) + d(to)
	tmp.m7 = (m0*t1) - (m2*t4) + (m3*t5);

	// unload	G		(4)		-HINO
	// load		Q		(5)		QHINO
	t1 = (m4*m9) - (m5*m8);

	// m = -( a(th) - b(tn) + d(tq))
	tmp.m11 = -((m0*t2) - (m1*t4) + (m3*t1));

	// unload	HN		(3)		Q-I-O
	// q = a(ti) - b(to) + c(tq)
	tmp.m15 = (m0*t3) - (m1*t5) + (m2*t1);

	return tmp;
}

const void Matrix::Inverse(Matrix &out)
{
	float det = Determinant();
	if (fabs(det) < 0.0001f)
	{
		// do nothing, Matrix is not invertable
	}
	else
	{
		out = GetAdjugate();
		out /= det;
	}

}

const void Matrix::setScaleMatrix(const Vect4D *scale)
{
	//	{	sx		0		0		0	}
	//	{	0		sy		0		0	}
	//	{	0		0		sz		0	}
	//	{	0		0		0		1	}
	
	this->v0._m = _mm_set_ps(0.0f, 0.0f, 0.0f, scale->x);
	this->v1._m = _mm_set_ps(0.0f, 0.0f, scale->y, 0.0f);
	this->v2._m = _mm_set_ps(0.0f, scale->z, 0.0f, 0.0f);
	this->v3._m = _mm_set_ps(1.0f, 0.0f, 0.0f, 0.0f);
}

const void Matrix::setRotZMatrix(const float az)
{
	//	{	cos		-sin	0		0	}
	//	{	sin		cos		0		0	}
	//	{	0		0		1		0	}
	//	{	0		0		0		1	}
	
	float a = sinf(az);
	float b = cosf(az);

	this->v0._m = _mm_set_ps(0.0f, 0.0f, -a, b);
	this->v1._m = _mm_set_ps(0.0f, 0.0f, b, a);
	this->v2._m = _mm_set_ps(0.0f, 1.0f, 0.0f, 0.0f);
	this->v3._m = _mm_set_ps(1.0f, 0.0f, 0.0f, 0.0f);
}

// End of file