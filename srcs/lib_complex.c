#include <lib_complex.h>

TYPE			cl_creal(const cl_complex n)
{
	return(n.x);
}

TYPE			cl_cimag(const cl_complex n)
{
	return(n.y);
}

TYPE			cl_cmod(const cl_complex n)
{
	return(sqrt((TYPE)((n.x * n.x) + (n.y * n.y))));
}

TYPE			cl_cdot(const cl_complex a, const cl_complex b)
{
	return((a.x * b.x) + (a.y * b.y));
}

cl_complex		cl_cadd(const cl_complex a, const cl_complex b)
{
	cl_complex		res;

    res.x = a.x + b.x;
    res.y = a.y + b.y;
	return(res);
}

cl_complex 		cl_cmult(const cl_complex a, const cl_complex b)
{
	cl_complex		res;

	res.x = (a.x * b.x - a.y * b.y);
	res.y = (a.x * b.y + a.y * b.x);
	return (res);
}

cl_complex 		cl_cpow(const cl_complex base , int exp)
{
	cl_complex	res;

	res.x = 1;
	res.y = 1;
	
	while(exp)
	{
		if(exp & 1)
			res=cl_cmult(res, base);
		exp>>= 1;
		res = cl_cmult(res, res);
	}
	return (res);
}

cl_complex 		cl_cdiv(const cl_complex a, const cl_complex b)
{
	TYPE		dividend;
	cl_complex	res;

	dividend = (b.x * b.x + b.y * b.y);
	res.x = ((a.x * b.x + a.y * b.y) / dividend);
	res.y = ((a.y * b.x - a.x * b.y) / dividend);
	return(res);
}

TYPE 			cl_carg(const cl_complex a)
{
	if(a.x > 0)
		return(atan((TYPE)(a.y / a.x)));
	else if(a.x < 0 && a.y >= 0)
		return(atan((TYPE)(a.y / a.x)) + Pi_cl);
	else if(a.x < 0 && a.y < 0)
		return(atan((TYPE)(a.y / a.x)) - Pi_cl);
	else if(a.x == 0 && a.y > 0)
		return((TYPE)(Pi_cl / 2));
	else if(a.x == 0 && a.y < 0)
		return((TYPE)(Pi_cl * -1) / 2);
	else
		return(0);
}

cl_complex 		cl_csqrt(const cl_complex n)
{
	const TYPE sm = sqrt((TYPE)(cl_cmod(n)));
	const TYPE a2 = cl_carg(n) / 2;
	const TYPE ca2 = cos((TYPE)a2);
	const TYPE sa2 = sin((TYPE)a2);
	cl_complex	res;

	res.x = sm * ca2;
	res.y = sm * sa2;
	return(res);
}

cl_complex 		cl_cexp(const cl_complex n)
{
	const TYPE e = exp((TYPE)n.x);
	cl_complex	res;

	res.x = (e * cos((TYPE)n.y));
	res.y = (e * sin((TYPE)n.y));
	return(res);
}

cl_complex 		cl_clog(const cl_complex z)
{
	cl_complex	res;

	res.x = log((TYPE)cl_cmod(z));
	res.y = cl_carg(z);
	return(res);
}

cl_quat			cl_quat_mult(cl_quat q1, cl_quat q2)
{
	cl_quat 	res;

	res.x = (q1.x * q2.x) - (q1.y * q2.y) - (q1.z * q2.z) - (q1.w * q2.w);
	res.y = (q1.x * q2.y) + (q1.y * q2.x) + (q1.z * q2.w) - (q1.w * q2.z);
	res.z = (q1.x * q2.z) + (q1.z * q2.x) + (q1.w * q2.y) - (q1.y * q2.w);
	res.w = (q1.x * q1.w) + (q1.w * q2.x) + (q1.y * q2.z) - (q1.z * q2.y);
	return res;
}

cl_quat 		cl_quat_sum(cl_quat q1, cl_quat q2)
{
	cl_quat 	res;

	res.x = q1.x + q2.x;
	res.y = q1.y + q2.y;
	res.z = q1.z + q2.z;
	res.w = q1.w + q2.w;
	return res;
}

cl_quat 		cl_quat_conjugate(cl_quat q)
{
	cl_quat 	res;

	res.x = q.x;
	res.y = -1 * q.y;
	res.z = -1 * q.z;
	res.w = -1 * q.w;
	return res;
}

TYPE 			cl_quat_mod(cl_quat q)
{
	cl_quat 	tmp;

	tmp = cl_quat_conjugate(q);
	tmp = cl_quat_mult(q, tmp);
	return (sqrt((TYPE)((tmp.x * tmp.x) + (tmp.y * tmp.y) + (tmp.z * tmp.z) + (tmp.w * tmp.w))));
}

// Optimized version that returns squared magnitude to avoid sqrt()
TYPE 			cl_quat_mod_squared(cl_quat q)
{
	return (q.x * q.x + q.y * q.y + q.z * q.z + q.w * q.w);
}
