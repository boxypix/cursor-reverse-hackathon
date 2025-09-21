#ifndef _LIB_COMPLEX_H
# define _LIB_COMPLEX_H

# include <OpenCL/opencl.h>
# include "opencl-c-base.h"
#include "math.h"

# if TEST_DOUBLE_SUPPORT
#  if defined(cl_khr_fp64)  // Khronos extension available?
#   pragma OPENCL EXTENSION cl_khr_fp64 : enable
#   define DOUBLE_SUPPORT_AVAILABLE
#  elif defined(cl_amd_fp64)  // AMD extension available?
#   pragma OPENCL EXTENSION cl_amd_fp64 : enable
#   define DOUBLE_SUPPORT_AVAILABLE
#  endif
# endif

# if defined(DOUBLE_SUPPORT_AVAILABLE)
 typedef double2		cl_complex;
 typedef cl_double4 	cl_quat;
 typedef double			TYPE;
# else
 typedef float2			cl_complex;
 typedef float4			cl_quat;
 typedef float			TYPE;
# endif

# define I_cl ((cl_complex)(0.0, 1.0))
# define Pi_cl 3.14159265358979323846
# define E_cl 2.718281828459045235360

TYPE			cl_creal(cl_complex n);
TYPE			cl_cimag(cl_complex n);
TYPE			cl_cmod(cl_complex n);
cl_complex		cl_cadd(cl_complex a, cl_complex b);
cl_complex 		cl_cmult(cl_complex a, cl_complex b);
cl_complex 		cl_cpow(cl_complex base , int exp);
cl_complex 		cl_cdiv(cl_complex a, cl_complex b);
TYPE 			cl_carg(cl_complex a);
cl_complex 		cl_csqrt(cl_complex n);
cl_complex 		cl_cexp(cl_complex n);
cl_complex 		cl_clog(cl_complex z);
TYPE			cl_cdot(cl_complex a, cl_complex b);
cl_quat			cl_quat_mult(cl_quat q1, cl_quat q2);
cl_quat 		cl_quat_sum(cl_quat q1, cl_quat q2);
cl_quat 		cl_quat_conjugate(cl_quat q);
TYPE 			cl_quat_mod(cl_quat q);
TYPE 			cl_quat_mod_squared(cl_quat q);

#endif
