
//
//  Created by Egor Chekhov O'Leo on 24/12/2015.
//  Copyright ? 2015 Control Systems Interfaces. All rights reserved.
//

#ifndef __VL__
#define __VL__

//------------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#ifndef __TRJ_ENV__
#include <inttypes.h>
#endif

//------------------------------------------------------------------------------

#define vl_lsep "--------------------------------r\n"

//------------------------------------------------------------------------------

#ifdef __TRJ_ENV__
typedef signed char 			int8_t;
typedef unsigned char 			uint8_t;
typedef signed int 				int32_t;
typedef unsigned int  			uint32_t;
#endif

typedef double	 	float64_t;
typedef float      	float32_t;

typedef struct ufp32 { uint32_t data; } ufp32_t;
typedef struct fp32  { int32_t  data; } fp32_t ;

#ifndef __TRJ_ENV__
typedef struct ufp64 { uint64_t data; } ufp64_t;
typedef struct fp64  { int64_t  data; } fp64_t ;
#endif

typedef float64_t vlf_t;

#ifndef __TRJ_ENV__
inline fp64_t    fp64_float32  (float32_t float32_data) { return (fp64_t ) { .data = (int64_t ) (1E+12 * (float64_t) float32_data) }; }
inline fp64_t    fp64_float64  (float64_t float64_data) { return (fp64_t ) { .data = (int64_t ) (1E+12 * (float64_t) float64_data) }; }
inline ufp64_t   ufp64_float32 (float32_t float32_data) { return (ufp64_t) { .data = (uint64_t) (1E+12 * (float64_t) float32_data) }; }
inline ufp64_t   ufp64_float64 (float64_t float64_data) { return (ufp64_t) { .data = (uint64_t) (1E+12 * (float64_t) float64_data) }; }

inline float32_t float32_fp64  (fp64_t  fp64_data ) { return (float32_t) (1E-12 * (float64_t) fp64_data.data ); }
inline float64_t float64_fp64  (fp64_t  fp64_data ) { return (float64_t) (1E-12 * (float64_t) fp64_data.data ); }
inline float32_t float32_ufp64 (ufp64_t ufp64_data) { return (float32_t) (1E-12 * (float64_t) ufp64_data.data); }
inline float64_t float64_ufp64 (ufp64_t ufp64_data) { return (float64_t) (1E-12 * (float64_t) ufp64_data.data); }
#endif

//------------------------------------------------------------------------------

const vlf_t vl_pi  = 3.14159265358979323846264338327950288;
const vlf_t vl_2pi = 3.14159265358979323846264338327950288 * 2;

//------------------------------------------------------------------------------

inline vlf_t vl_rad(vlf_t deg) { return ((vlf_t) (((vlf_t) (deg)) * (vl_pi / 180))); }
inline vlf_t vl_deg(vlf_t rad) { return ((vlf_t) (((vlf_t) (rad)) * (180 / vl_pi))); }
inline vlf_t vl_atan2(vlf_t x, vlf_t y) { return atan2(x, y); }
inline vlf_t vl_sin(vlf_t x) { return sin(x); }
inline vlf_t vl_cos(vlf_t x) { return cos(x); }
inline vlf_t vl_sqrt(vlf_t x) { return sqrt(x); }
inline vlf_t vl_pow(vlf_t x, vlf_t power) { return pow(x, power); }
inline vlf_t vl_crt(vlf_t x) { return x < 0 ? -pow(-x, 1.0f/3.0f) : pow(x, 1.0f/3.0f); }
#define vl_max fmax
#define vl_min fmin

//------------------------------------------------------------------------------

inline void vl_vprint(vlf_t *vec)
{
	printf("%f %f %f", vec[0], vec[1], vec[2]);
}

inline void vl_mprint(vlf_t *mat)
{
	vl_vprint(&mat[0]);
	printf("\r\n");
	vl_vprint(&mat[3]);
	printf("\r\n");
	vl_vprint(&mat[6]);
	printf("\r\n");
}

//------------------------------------------------------------------------------

inline uint32_t vl_crc32(char *str)
{
	uint32_t i;
	uint32_t j;
	uint32_t crc = 0xFFFFFFFF;
	
	for(i=0;i<strlen(str);i++) {
		char ch=str[i];
		for(j=0;j<8;j++) {
			uint32_t b=(ch^crc)&1;
			crc>>=1;
			if(b) crc=crc^0xEDB88320;
			ch>>=1;
		}
	}
	
	return ~crc;
}

//------------------------------------------------------------------------------

inline vlf_t vl_gauss1(vlf_t x, vlf_t m, vlf_t d)
{
	return exp(-0.5*((x-m)/d)*((x-m)/d)) / (d*2.50662827463);
}

//------------------------------------------------------------------------------

inline void vl_vcopy(vlf_t *res, vlf_t *vec)
{
	res[0] = vec[0];
	res[1] = vec[1];
	res[2] = vec[2];
	
	return;
}

//------------------------------------------------------------------------------

inline void vl_mcopy(vlf_t *res, vlf_t *mat)
{
	res[0 + 0] = mat[0 + 0];
	res[0 + 1] = mat[0 + 1];
	res[0 + 2] = mat[0 + 2];
	
	res[3 + 0] = mat[3 + 0];
	res[3 + 1] = mat[3 + 1];
	res[3 + 2] = mat[3 + 2];
	
	res[6 + 0] = mat[6 + 0];
	res[6 + 1] = mat[6 + 1];
	res[6 + 2] = mat[6 + 2];
	
	return;
}

//------------------------------------------------------------------------------

inline vlf_t vl_vdist(vlf_t *vec_0, vlf_t *vec_1)
{
	uint8_t i;
	
	vlf_t vec_dist = 0;
	vlf_t temp_float;
	
	for (i = 0; i < 3; ++i)
	{
		temp_float = vec_0[i] - vec_1[i];
		vec_dist += temp_float * temp_float;
	}
	
	return vl_sqrt(vec_dist);
}

inline vlf_t vl_vdist2(vlf_t *vec_0, vlf_t *vec_1)
{
	uint8_t i;
	
	vlf_t vec_dist = 0;
	vlf_t temp_float;
	
	for (i = 0; i < 3; ++i)
	{
		temp_float = vec_0[i] - vec_1[i];
		vec_dist += temp_float * temp_float;
	}
	
	return vec_dist;
}

//------------------------------------------------------------------------------

inline vlf_t vl_vnorm(vlf_t *vec)
{
	return vl_sqrt(vec[0]*vec[0] + vec[1]*vec[1] + vec[2]*vec[2]);
}

//------------------------------------------------------------------------------

inline vlf_t vl_vdot(vlf_t *vec_0, vlf_t *vec_1)
{
	return (vec_0[0]*vec_1[0] + vec_0[1]*vec_1[1] + vec_0[2]*vec_1[2]);
}

//------------------------------------------------------------------------------

inline void vl_cross(vlf_t *res, vlf_t *vec_0, vlf_t *vec_1)
{
	res[0] = vec_0[1]*vec_1[2] - vec_0[2]*vec_1[1];
	res[1] = vec_0[2]*vec_1[0] - vec_0[0]*vec_1[2];
	res[2] = vec_0[0]*vec_1[1] - vec_0[1]*vec_1[0];
	
	return;
}

//------------------------------------------------------------------------------

inline void vl_skew(vlf_t *res, vlf_t *vec)
{
	res[0 + 0] = 0;
	res[0 + 1] = -vec[2];
	res[0 + 2] = +vec[1];
	
	res[3 + 0] = +vec[2];
	res[3 + 1] = 0;
	res[3 + 2] = -vec[0];
	
	res[6 + 0] = -vec[1];
	res[6 + 1] = +vec[0];
	res[6 + 2] = 0;
	
	return;
}

//------------------------------------------------------------------------------

// def rot_norm(rot):

//     # return rot;
//     # print(rot)

//     u, a, v = scipy.linalg.svd(rot);
//     return numpy.mat(u.dot(v));

//     if abs(1.0 - scipy.linalg.det(rot)) > 1E-6:

//         u, a, v = scipy.linalg.svd(rot);
//         return numpy.mat(u.dot(v));

//     else:

//         return numpy.mat(rot);

//void vl_rm_norm(vlf_t *mat)
//{
//	vl_
//
//	return;
//}


//------------------------------------------------------------------------------

inline void vl_rm_align(vlf_t *res, vlf_t *vec_0, vlf_t *vec_1)
{
	vlf_t v[3];
	vlf_t s;
	vlf_t c;
	vlf_t k;
	
	vl_cross(v, vec_0, vec_1);
	
	s = vl_vnorm(v);
	c = vl_vdot(vec_0, vec_1);
	
	k = (1 - c) / s;
	
	res[0 + 0] = 1		+ k*(-v[2]*v[2]-v[1]*v[1]);
	res[0 + 1] = -v[2]	+ k*( v[0]*v[1]);
	res[0 + 2] = +v[1]	+ k*( v[0]*v[2]);
	
	res[3 + 0] = +v[2]	+ k*( v[0]*v[1]);
	res[3 + 1] = 1 		+ k*(-v[2]*v[2]-v[0]*v[0]);
	res[3 + 2] = -v[0]	+ k*( v[1]*v[2]);
	
	res[6 + 0] = -v[1]	+ k*( v[0]*v[2]);
	res[6 + 1] = +v[0]	+ k*( v[1]*v[2]);
	res[6 + 2] = 1		+ k*(-v[2]*v[2]-v[0]*v[0]);
	
	return;
}

//------------------------------------------------------------------------------

//inline void vl_vprint(vlf_t *vec)
//{
//	printf("%lf %lf %lf \n", vec[0], vec[1], vec[2]);
//
//	return;
//}
//
////------------------------------------------------------------------------------
//
//inline void vl_mprint(vlf_t *vec)
//{
//	printf("%lf %lf %lf \n", vec[0 + 0], vec[0 + 1], vec[0 + 2]);
//	printf("%lf %lf %lf \n", vec[3 + 0], vec[3 + 1], vec[3 + 2]);
//	printf("%lf %lf %lf \n", vec[6 + 0], vec[6 + 1], vec[6 + 2]);
//
//	printf("\n");
//
//	return;
//}

//------------------------------------------------------------------------------

inline void vl_vzero (vlf_t *vec)
{
	vec[0] = 0; vec[1] = 0; vec[2] = 0;
	
	return;
}

//------------------------------------------------------------------------------

inline void vl_vset (vlf_t *vec, vlf_t value)
{
	vec[0] = value;
	vec[1] = value;
	vec[2] = value;
	
	return;
}

//------------------------------------------------------------------------------

inline void vl_mzero (vlf_t *mat)
{
	mat[0] = 0; mat[1] = 0; mat[2] = 0;
	mat[3] = 0; mat[4] = 0; mat[5] = 0;
	mat[6] = 0; mat[7] = 0; mat[8] = 0;
	
	return;
}

//------------------------------------------------------------------------------

inline void vl_mid (vlf_t *mat)
{
	mat[0] = 1; mat[1] = 0; mat[2] = 0;
	mat[3] = 0; mat[4] = 1; mat[5] = 0;
	mat[6] = 0; mat[7] = 0; mat[8] = 1;
	
	return;
}

//------------------------------------------------------------------------------

inline void vl_row (vlf_t *res, vlf_t *mat, uint8_t row)
{
	switch (row)
	{
		case  0: { vl_vcopy(res, &mat[0]); break; }
		case  1: { vl_vcopy(res, &mat[1]); break; }
		case  2: { vl_vcopy(res, &mat[2]); break; }
		default: { vl_vcopy(res, &mat[2]); break; }
	}
	
	return;
}

//------------------------------------------------------------------------------

inline void vl_col (vlf_t *res, vlf_t *mat, uint8_t col)
{
	switch (col)
	{
		case  0: { res[0] = mat[0]; res[1] = mat[3]; res[2] = mat[6]; break; }
		case  1: { res[0] = mat[1]; res[1] = mat[4]; res[2] = mat[7]; break; }
		case  2: { res[0] = mat[2]; res[1] = mat[5]; res[2] = mat[8]; break; }
		default: { res[0] = mat[2]; res[1] = mat[5]; res[2] = mat[8]; break; }
	}
	
	return;
}

//------------------------------------------------------------------------------

inline void vl_mmul_s(vlf_t *res, vlf_t *mat, vlf_t scalar)
{
	res[0 + 0] = mat[0 + 0] * scalar;
	res[0 + 1] = mat[0 + 1] * scalar;
	res[0 + 2] = mat[0 + 2] * scalar;
	
	res[3 + 0] = mat[3 + 0] * scalar;
	res[3 + 1] = mat[3 + 1] * scalar;
	res[3 + 2] = mat[3 + 2] * scalar;
	
	res[6 + 0] = mat[6 + 0] * scalar;
	res[6 + 1] = mat[6 + 1] * scalar;
	res[6 + 2] = mat[6 + 2] * scalar;
	
	return;
}

//------------------------------------------------------------------------------

inline void vl_mmul_m_(uint32_t n, vlf_t *res, vlf_t *mat_0, vlf_t *mat_1)
{
	uint8_t i;
	uint8_t j;
	uint8_t k;
	
	if (res != mat_0 && res != mat_1)
	{
		for (i = 0; i < n; ++i)
		{
			for (j = 0; j < n; ++j)
			{
				res[i*n + j] = 0;
				
				for (k = 0; k < n; ++k)
				{
					res[i*n + j] += mat_0[i*n + k] * mat_1[k*n + j];
				}
			}
		}
	}
	
	return;
}

//------------------------------------------------------------------------------

inline void vl_mmul_m(vlf_t *res, vlf_t *mat_0, vlf_t *mat_1)
{
	uint8_t i;
	uint8_t j;
	uint8_t k;
	
	if (res != mat_0 && res != mat_1)
	{
		for (i = 0; i < 3; ++i)
		{
			for (j = 0; j < 3; ++j)
			{
				res[i * 3 + j] = 0;
				
				for (k = 0; k < 3; ++k)
				{
					res[i*3 + j] += mat_0[i*3 + k] * mat_1[k*3 + j];
				}
			}
		}
	}
	
	else
	{
		vlf_t m0_[9] = { mat_0[0], mat_0[1], mat_0[2], mat_0[3], mat_0[4], mat_0[5], mat_0[6], mat_0[7], mat_0[8] };
		vlf_t m1_[9] = { mat_1[0], mat_1[1], mat_1[2], mat_1[3], mat_1[4], mat_1[5], mat_1[6], mat_1[7], mat_1[8] };;
		
		for (i = 0; i < 3; ++i)
		{
			for (j = 0; j < 3; ++j)
			{
				res[i * 3 + j] = 0;
				
				for (k = 0; k < 3; ++k)
				{
					res[i*3 + j] += m0_[i*3 + k] * m1_[k*3 + j];
				}
			}
		}
	}
	
	return;
}

//------------------------------------------------------------------------------

inline void vl_mmul_mt(vlf_t *res, vlf_t *mat_0, vlf_t *mat_1)
{
	uint8_t i;
	uint8_t j;
	uint8_t k;
	
	if (res != mat_0 && res != mat_1)
	{
		for (i = 0; i < 3; ++i)
		{
			for (j = 0; j < 3; ++j)
			{
				res[i * 3 + j] = 0;
				
				for (k = 0; k < 3; ++k)
				{
					res[i*3 + j] += mat_0[i*3 + k] * mat_1[j*3 + k];
				}
			}
		}
	}
	
	else
	{
		vlf_t m0_[9] = { mat_0[0], mat_0[1], mat_0[2], mat_0[3], mat_0[4], mat_0[5], mat_0[6], mat_0[7], mat_0[8] };
		vlf_t m1_[9] = { mat_1[0], mat_1[1], mat_1[2], mat_1[3], mat_1[4], mat_1[5], mat_1[6], mat_1[7], mat_1[8] };;
		
		for (i = 0; i < 3; ++i)
		{
			for (j = 0; j < 3; ++j)
			{
				res[i * 3 + j] = 0;
				
				for (k = 0; k < 3; ++k)
				{
					res[i*3 + j] += m0_[i*3 + k] * m1_[j*3 + k];
				}
			}
		}
	}
	
	return;
}

//------------------------------------------------------------------------------

inline void vl_mtmul_m(vlf_t *res, vlf_t *mat_0, vlf_t *mat_1)
{
	uint8_t i;
	uint8_t j;
	uint8_t k;
	
	if (res != mat_0 && res != mat_1)
	{
		for (i = 0; i < 3; ++i)
		{
			for (j = 0; j < 3; ++j)
			{
				res[i*3+j] = 0;
				
				for (k = 0; k < 3; ++k)
				{
					res[i*3+j] += mat_0[k*3+i] * mat_1[k*3+j];
				}
			}
		}
	}
	else
	{
		vlf_t m0_[9] = { mat_0[0], mat_0[1], mat_0[2], mat_0[3], mat_0[4], mat_0[5], mat_0[6], mat_0[7], mat_0[8] };
		vlf_t m1_[9] = { mat_1[0], mat_1[1], mat_1[2], mat_1[3], mat_1[4], mat_1[5], mat_1[6], mat_1[7], mat_1[8] };;
		
		for (i = 0; i < 3; ++i)
		{
			for (j = 0; j < 3; ++j)
			{
				res[i*3+j] = 0;
				
				for (k = 0; k < 3; ++k)
				{
					res[i*3+j] += m0_[k*3+i] * m1_[k*3+j];
				}
			}
		}
	}
		
		return;
}

//------------------------------------------------------------------------------

inline void vl_mmul_v(vlf_t *res, vlf_t *mat, vlf_t *vec)
{
	if (res != vec)
	{
		res[0] = mat[0] * vec[0] + mat[1] * vec[1] + mat[2] * vec[2];
		res[1] = mat[3] * vec[0] + mat[4] * vec[1] + mat[5] * vec[2];
		res[2] = mat[6] * vec[0] + mat[7] * vec[1] + mat[8] * vec[2];
	}
	
	else
	{
		vlf_t vec_[3] = { vec[0], vec[1], vec[2] };
		
		res[0] = mat[0] * vec_[0] + mat[1] * vec_[1] + mat[2] * vec_[2];
		res[1] = mat[3] * vec_[0] + mat[4] * vec_[1] + mat[5] * vec_[2];
		res[2] = mat[6] * vec_[0] + mat[7] * vec_[1] + mat[8] * vec_[2];
	}
	
	return;
}

//------------------------------------------------------------------------------

inline void vl_mtmul_v(vlf_t *res, vlf_t *mat, vlf_t *vec)
{
	if (res != vec)
	{
		res[0] = mat[0] * vec[0] + mat[3] * vec[1] + mat[6] * vec[2];
		res[1] = mat[1] * vec[0] + mat[4] * vec[1] + mat[7] * vec[2];
		res[2] = mat[2] * vec[0] + mat[5] * vec[1] + mat[8] * vec[2];
	}
	
	else
	{
		vlf_t vec_[3] = { vec[0], vec[1], vec[2] };
		
		res[0] = mat[0] * vec_[0] + mat[3] * vec_[1] + mat[6] * vec_[2];
		res[1] = mat[1] * vec_[0] + mat[4] * vec_[1] + mat[7] * vec_[2];
		res[2] = mat[2] * vec_[0] + mat[5] * vec_[1] + mat[8] * vec_[2];
	}
	
	return;
}

//------------------------------------------------------------------------------

inline void vl_tnp(vlf_t *res, vlf_t *mat)
{
	uint32_t i;
	uint32_t j;

	if (res != mat)
    {
        for (i = 0; i < 3; ++i)
        {
            for (j = 0; j < 3; ++j)
            {
                res[j*3 + i] = mat[i*3 + j];
            }
        }
    }

	else
    {
	    vlf_t mat_[9];
        vl_mcopy(mat_, mat);

        for (i = 0; i < 3; ++i)
        {
            for (j = 0; j < 3; ++j)
            {
                res[j*3 + i] = mat_[i*3 + j];
            }
        }
    }
	
	return;
}

//------------------------------------------------------------------------------

inline vlf_t vl_det(vlf_t *mat)
{
	vlf_t d0 = mat[1*3+1]*mat[2*3+2] - mat[1*3+2]*mat[2*3+1];
	vlf_t d1 = mat[1*3+0]*mat[2*3+2] - mat[1*3+2]*mat[2*3+0];
	vlf_t d2 = mat[1*3+0]*mat[2*3+1] - mat[1*3+1]*mat[2*3+0];
	
	return d0*mat[0] - d1*mat[1] + d2*mat[2];
}

//------------------------------------------------------------------------------

inline void vl_msum(vlf_t *res, vlf_t *mat_0, vlf_t *mat_1)
{
	uint32_t i;
	
	for (i = 0; i < 9; ++i)
	{
		res[i] = mat_0[i] + mat_1[i];
	}
	
	return;
}

inline void vl_msumm(vlf_t *res, vlf_t *mat_0, vlf_t *mat_1, vlf_t gain)
{
	uint32_t i;
	
	for (i = 0; i < 9; ++i)
	{
		res[i] = mat_0[i] + gain*mat_1[i];
	}
	
	return;
}

inline void vl_msub(vlf_t *res, vlf_t *mat_0, vlf_t *mat_1)
{
	uint32_t i;
	
	for (i = 0; i < 9; ++i)
	{
		res[i] = mat_0[i] - mat_1[i];
	}
	
	return;
}

inline void vl_msubm(vlf_t *res, vlf_t *mat_0, vlf_t *mat_1, vlf_t gain)
{
	uint32_t i;
	
	for (i = 0; i < 9; ++i)
	{
		res[i] = mat_0[i] - gain*mat_1[i];
	}
	
	return;
}

//------------------------------------------------------------------------------

inline void vl_vsum(vlf_t *res, vlf_t *vec_0, vlf_t *vec_1)
{
	res[0] = vec_0[0] + vec_1[0];
	res[1] = vec_0[1] + vec_1[1];
	res[2] = vec_0[2] + vec_1[2];
	
	return;
}

//------------------------------------------------------------------------------

inline void vl_vsumm(vlf_t *res, vlf_t *vec_0, vlf_t *vec_1, vlf_t gain)
{
	res[0] = vec_0[0] + gain * vec_1[0];
	res[1] = vec_0[1] + gain * vec_1[1];
	res[2] = vec_0[2] + gain * vec_1[2];
	
	return;
}

inline void vl_vsub(vlf_t *res, vlf_t *vec_0, vlf_t *vec_1)
{
	res[0] = vec_0[0] - vec_1[0];
	res[1] = vec_0[1] - vec_1[1];
	res[2] = vec_0[2] - vec_1[2];
	
	return;
}

//------------------------------------------------------------------------------

inline void vl_vmul_s(vlf_t *res, vlf_t *vec, vlf_t scalar)
{
	res[0] = vec[0] * scalar;
	res[1] = vec[1] * scalar;
	res[2] = vec[2] * scalar;
	
	return;
}

inline void vl_vmul_v(vlf_t *res, vlf_t *vec_0, vlf_t *vec_1)
{
	res[0] = vec_0[0] * vec_1[0];
	res[1] = vec_0[1] * vec_1[1];
	res[2] = vec_0[2] * vec_1[2];
	
	return;
}

inline void vl_vdiv_v(vlf_t *res, vlf_t *vec_0, vlf_t *vec_1)
{
	res[0] = vec_0[0] / vec_1[0];
	res[1] = vec_0[1] / vec_1[1];
	res[2] = vec_0[2] / vec_1[2];
	
	return;
}

inline void vl_inv(uint32_t n, vlf_t *inv, vlf_t *mat_)
{
	vlf_t temp;
	vlf_t *mat = (vlf_t*) malloc(sizeof(vlf_t) * n * n);
	
	uint32_t i, j, k;
	
	for (i = 0; i < n; ++i)
	{
		for (j = 0; j < n; ++j)
		{
			mat[i*n+j] = mat_[i*n+j];
			
			if (i == j)
			{ inv[n * i + j] = 1.0; }
			else
			{ inv[n * i + j] = 0.0; }
		}
	}
	
	for (k = 0; k < n; ++k)                                  //by some row operations,and the same row operations of
	{                                                       //Unit mat. I gives the inverse of matrix A
		temp = mat[n*k+k];                   //'temp'
		// stores the A[k][k] value so that A[k][k]  will not change
		for (j = 0; j < n; ++j)      //during the operation //A[i] //[j]/=A[k][k]  when i=j=k
		{
			mat[n*k+j] /= temp;
			inv[n*k+j] /= temp;
		}
		//R1=R1-R0*A[1][0] similarly for I
		for (i = 0; i < n; ++i)
		{
			temp = mat[n*i+k];
			
			for (j = 0; j < n; ++j)
			{                                   //R2=R2-R1*A[2][1]
				if(i == k) break;                      //R2=R2/A[2][2]
				mat[n*i+j] -= mat[n*k+j] * temp;          //R0=R0-R2*A[0][2]
				inv[n*i+j] -= inv[n*k+j] * temp;          //R1=R1-R2*A[1][2]
			}
		}
	}
	
	free(mat);
	
	return;
}

//------------------------------------------------------------------------------

inline void vl_unskew(vlf_t *res, vlf_t *mat)
{
	vlf_t temp_mat[9];
	vlf_t mat_tnp[9];
	
	vl_tnp(mat_tnp, mat);
	vl_msub(temp_mat, mat, mat_tnp);
	
	// mat = 0.5 * (mat - mat.T);
//
//	res[0 + 0] = 0;
//	res[0 + 1] = -vec[2];
//	res[0 + 2] = +vec[1];
//
//	res[3 + 0] = +vec[2];
//	res[3 + 1] = 0;
//	res[3 + 2] = -vec[0];
//
//	res[6 + 0] = -vec[1];
//	res[6 + 1] = +vec[0];
//	res[6 + 2] = 0;
//
	res[0] = 0.5 * (mat[2*3 + 1] - mat[1*3 + 2]);
	res[1] = 0.5 * (mat[0*3 + 2] - mat[2*3 + 0]);
	res[2] = 0.5 * (mat[1*3 + 0] - mat[0*3 + 1]);
	
	return;
}

//------------------------------------------------------------------------------

inline vlf_t vl_mtrace(vlf_t *mat)
{
	return (mat[0] + mat[4] + mat[8]);
}

//------------------------------------------------------------------------------

inline void vl_vinter(vlf_t *res, vlf_t *v0, vlf_t *v1, vlf_t dist)
{
	res[0] = v0[0] + (v1[0]-v0[0]) * dist;
	res[1] = v0[1] + (v1[1]-v0[1]) * dist;
	res[2] = v0[2] + (v1[2]-v0[2]) * dist;
	
	return;
}

//------------------------------------------------------------------------------

inline void vl_minter(vlf_t *res, vlf_t *m0, vlf_t *m1, vlf_t dist)
{
	res[0] = m0[0] + (m1[0]-m0[0]) * dist;
	res[1] = m0[1] + (m1[1]-m0[1]) * dist;
	res[2] = m0[2] + (m1[2]-m0[2]) * dist;
	res[3] = m0[3] + (m1[3]-m0[3]) * dist;
	res[4] = m0[4] + (m1[4]-m0[4]) * dist;
	res[5] = m0[5] + (m1[5]-m0[5]) * dist;
	res[6] = m0[6] + (m1[6]-m0[6]) * dist;
	res[7] = m0[7] + (m1[7]-m0[7]) * dist;
	res[8] = m0[8] + (m1[8]-m0[8]) * dist;

	return;
}

//------------------------------------------------------------------------------

inline vlf_t vl_mdist(vlf_t *m0, vlf_t *m1)
{
	// does not work due to numerical instabilities and also pretty slow
	vlf_t m0m1t[9];
	vl_mmul_mt(m0m1t, m1, m0);
	
	vlf_t cosm = (vl_mtrace(m0m1t) - 1.0) * 0.5;
	
	if (cosm < -1) cosm = -1;
	if (cosm > +1) cosm = +1;
	
	return acos(cosm);
}

//------------------------------------------------------------------------------

inline void vl_mset(vlf_t *m, vlf_t value)
{
	m[0] = value; m[1] = value; m[2] = value;
	m[3] = value; m[4] = value; m[5] = value;
	m[6] = value; m[7] = value; m[8] = value;
	
	return;
}

//------------------------------------------------------------------------------

typedef struct
{
	vlf_t a[9];
	vlf_t at[9];
	vlf_t trace_a;
	vlf_t omega;
	vlf_t v[3];
	
} 	s_vl_rd1;

// https://math.stackexchange.com/questions/668866/how-do-you-find-angular-velocity-given-a-pair-of-3x3-rotation-matrices
inline void vl_rd1f(s_vl_rd1 *self, vlf_t *res, vlf_t *r0, vlf_t *r1)
{
	vl_mmul_mt(self->a, r1, r0);
	self->trace_a = vl_mtrace(self->a);
	
	vlf_t acos_arg = 0.5*(self->trace_a - 1.0);
	
	if (acos_arg < -1.0) acos_arg = -1.0;
	if (acos_arg > +1.0) acos_arg = +1.0;
	
	self->omega = acos(acos_arg);
	
	if (fabs(self->omega) < 1E-16)
	{
		vl_mset(res, 0.0);
	}
	
	else
	{
		// https://en.wikipedia.org/wiki/Axis%E2%80%93angle_representation
		vlf_t k = self->omega * 0.5 / vl_sin(self->omega);
		self->v[0] = (self->a[2*3+1] - self->a[1*3+2]) * k;
		self->v[1] = (self->a[0*3+2] - self->a[2*3+0]) * k;
		self->v[2] = (self->a[1*3+0] - self->a[0*3+1]) * k;
		
		vl_skew(res, self->v);
	}
	
	return;
	
}

inline void vl_rd1(vlf_t *res, vlf_t *r0, vlf_t *r1)
{
	s_vl_rd1 self;
	vl_rd1f(&self, res, r0, r1);
	
	return;
}

//------------------------------------------------------------------------------

inline void vl_raxis(vlf_t *angle, vlf_t *vec, vlf_t *mat)
{
	vlf_t tr_a = vl_mtrace(mat);
	vlf_t acos_arg = 0.5*(tr_a - 1.0);
	
	if (acos_arg < -1.0) acos_arg = -1.0;
	if (acos_arg > +1.0) acos_arg = +1.0;
	
	*angle = acos(acos_arg);
	
	if (fabs(*angle) < 1E-16)
	{
		vec[0] = 1.0;
		vec[1] = 0.0;
		vec[2] = 0.0;
	}
	
	else
	{
		// https://en.wikipedia.org/wiki/Axis%E2%80%93angle_representation
		vlf_t k = *angle * 0.5 / vl_sin(*angle);
		vec[0] = (mat[2*3+1] - mat[1*3+2]) * k;
		vec[1] = (mat[0*3+2] - mat[2*3+0]) * k;
		vec[2] = (mat[1*3+0] - mat[0*3+1]) * k;
	}

	return;
}

//------------------------------------------------------------------------------

typedef struct vl_hpr
{
	vlf_t heading;
	vlf_t pitch;
	vlf_t roll;

}	s_vl_hpr;

inline void vl_hpr(s_vl_hpr *hpr, vlf_t *rot)
{
	vlf_t c_tf[9] =
			{
					1,  0, 0,
					0,  0, 1,
					0, -1, 0
			};
	
	vlf_t c_tf_tnp[9];
	vl_tnp(c_tf_tnp, c_tf);
	
	vlf_t temp_mat_0[9];
	vlf_t temp_mat_1[9];
	
	// c_tn = c_tf.T.dot(c_tn_).dot(c_tf);
	vl_mmul_m(temp_mat_0, rot, c_tf);
	vl_mmul_m(temp_mat_1, c_tf_tnp, temp_mat_0);
	
	hpr->heading = -vl_atan2( temp_mat_1[1*3+0], temp_mat_1[0*3+0]);
	hpr->pitch   = -vl_atan2(-temp_mat_1[2*3+0], vl_sqrt(temp_mat_1[2*3+1]*temp_mat_1[2*3+1] + temp_mat_1[2*3+2]*temp_mat_1[2*3+2]));
	hpr->roll    =  vl_atan2( temp_mat_1[2*3+1], temp_mat_1[2*3+2]);
	
	if (hpr->heading < 0)
	{
		hpr->heading = hpr->heading + vl_2pi;
	}
	
	if (hpr->heading >= vl_2pi)
	{
		hpr->heading = hpr->heading - vl_2pi;
	}
	
	return;
}

//------------------------------------------------------------------------------

inline void vl_rot(vlf_t *rot, s_vl_hpr hpr)
{
	vlf_t a = -hpr.heading;
	vlf_t b = -hpr.pitch;
	vlf_t g =  hpr.roll;
	
	rot[0] = vl_cos(a)*vl_cos(b);
	rot[1] = vl_cos(a)*vl_sin(b)*vl_sin(g) - vl_sin(a)*vl_cos(g);
	rot[2] = vl_cos(a)*vl_sin(b)*vl_cos(g) + vl_sin(a)*vl_sin(g);
	rot[3] = vl_sin(a)*vl_cos(b);
	rot[4] = vl_sin(a)*vl_sin(b)*vl_sin(g) + vl_cos(a)*vl_cos(g);
	rot[5] = vl_sin(a)*vl_sin(b)*vl_cos(g) - vl_cos(a)*vl_sin(g);
	rot[6] = -vl_sin(b);
	rot[7] = vl_cos(b)*vl_sin(g);
	rot[8] = vl_cos(b)*vl_cos(g);
	
	vlf_t c_tf[9] =
			{
					1,  0, 0,
					0,  0, 1,
					0, -1, 0
			};
	
	vlf_t temp_mat[9];
	
	// c_tn = c_tf * c_tn * c_tf.I;
	
	vl_mmul_mt(temp_mat, rot, c_tf);
	vl_mmul_m(rot, c_tf, temp_mat);
	
	return;
}

//------------------------------------------------------------------------------

#define _gamma 5.828427124 // FOUR_GAMMA_SQUARED = sqrt(8)+3;
#define _cstar 0.923879532 // cos(pi/8)
#define _sstar 0.3826834323 // sin(p/8)
#define EPSILON 1e-6

/* This is a novel and fast routine for the reciprocal square root of an
IEEE float (single precision).

http://www.lomont.org/Math/Papers/2003/InvSqrt.pdf
http://playstation2-linux.com/download/p2lsd/fastrsqrt.pdf
http://www.beyond3d.com/content/articles/8/
*/
inline float rsqrt(float x) {
// int ihalf = *(int *)&x - 0x00800000; // Alternative to next line,
// float xhalf = *(float *)&ihalf;      // for sufficiently large nos.
	float xhalf = 0.5f*x;
	int i = *(int *)&x;          // View x as an int.
// i = 0x5f3759df - (i >> 1);   // Initial guess (traditional).
	i = 0x5f375a82 - (i >> 1);   // Initial guess (slightly better).
	x = *(float *)&i;            // View i as float.
	x = x*(1.5f - xhalf*x*x);    // Newton step.
// x = x*(1.5008908 - xhalf*x*x);  // Newton step for a balanced error.
	return x;
}

/* This is rsqrt with an additional step of the Newton iteration, for
increased accuracy. The constant 0x5f37599e makes the relative error
range from 0 to -0.00000463.
   You can't balance the error by adjusting the constant. */
inline float rsqrt1(float x) {
	float xhalf = 0.5f*x;
	int i = *(int *)&x;          // View x as an int.
	i = 0x5f37599e - (i >> 1);   // Initial guess.
	x = *(float *)&i;            // View i as float.
	x = x*(1.5f - xhalf*x*x);    // Newton step.
	x = x*(1.5f - xhalf*x*x);    // Newton step again.
	return x;
}

inline float accurateSqrt(float x)
{
	return x * rsqrt1(x);
}

inline void condSwap(uint8_t c, float *X, float *Y)
{
	// used in step 2
	float Z = *X;
	*X = (c != 0x00) ? *Y : *X;
	*Y = (c != 0x00) ? Z : *Y;
}

inline void condNegSwap(uint8_t c, float *X, float *Y)
{
	// used in step 2 and 3
	float Z = -*X;
	*X = (c != 0x00) ? *Y : *X;
	*Y = (c != 0x00) ? Z : *Y;
}

// matrix multiplication M = A * B
inline void multAB(float a11, float a12, float a13,
				   float a21, float a22, float a23,
				   float a31, float a32, float a33,
		//
				   float b11, float b12, float b13,
				   float b21, float b22, float b23,
				   float b31, float b32, float b33,
		//
				   float *m11, float *m12, float *m13,
				   float *m21, float *m22, float *m23,
				   float *m31, float *m32, float *m33)
{
	
	*m11=a11*b11 + a12*b21 + a13*b31; *m12=a11*b12 + a12*b22 + a13*b32; *m13=a11*b13 + a12*b23 + a13*b33;
	*m21=a21*b11 + a22*b21 + a23*b31; *m22=a21*b12 + a22*b22 + a23*b32; *m23=a21*b13 + a22*b23 + a23*b33;
	*m31=a31*b11 + a32*b21 + a33*b31; *m32=a31*b12 + a32*b22 + a33*b32; *m33=a31*b13 + a32*b23 + a33*b33;
}

// matrix multiplication M = Transpose[A] * B
inline void multAtB(float a11, float a12, float a13,
					float a21, float a22, float a23,
					float a31, float a32, float a33,
		//
					float b11, float b12, float b13,
					float b21, float b22, float b23,
					float b31, float b32, float b33,
		//
					float *m11, float *m12, float *m13,
					float *m21, float *m22, float *m23,
					float *m31, float *m32, float *m33)
{
	*m11=a11*b11 + a21*b21 + a31*b31; *m12=a11*b12 + a21*b22 + a31*b32; *m13=a11*b13 + a21*b23 + a31*b33;
	*m21=a12*b11 + a22*b21 + a32*b31; *m22=a12*b12 + a22*b22 + a32*b32; *m23=a12*b13 + a22*b23 + a32*b33;
	*m31=a13*b11 + a23*b21 + a33*b31; *m32=a13*b12 + a23*b22 + a33*b32; *m33=a13*b13 + a23*b23 + a33*b33;
}

inline void quatToMat3(const float * qV,
					   float *m11, float *m12, float *m13,
					   float *m21, float *m22, float *m23,
					   float *m31, float *m32, float *m33
)
{
	float w = qV[3];
	float x = qV[0];
	float y = qV[1];
	float z = qV[2];
	
	float qxx = x*x;
	float qyy = y*y;
	float qzz = z*z;
	float qxz = x*z;
	float qxy = x*y;
	float qyz = y*z;
	float qwx = w*x;
	float qwy = w*y;
	float qwz = w*z;
	
	*m11=1 - 2*(qyy + qzz); *m12=2*(qxy - qwz); 	*m13=2*(qxz + qwy);
	*m21=2*(qxy + qwz); 	*m22=1 - 2*(qxx + qzz); *m23=2*(qyz - qwx);
	*m31=2*(qxz - qwy); 	*m32=2*(qyz + qwx); 	*m33=1 - 2*(qxx + qyy);
}

inline void approximateGivensQuaternion(float a11, float a12, float a22, float *ch, float *sh)
{
/*
     * Given givens angle computed by approximateGivensAngles,
     * compute the corresponding rotation quaternion.
     */
	*ch = 2*(a11-a22);
	*sh = a12;
	uint8_t b = _gamma**sh**sh < *ch**ch;
	// fast rsqrt function suffices
	// rsqrt2 (https://code.google.com/p/lppython/source/browse/algorithm/HDcode/newCode/rsqrt.c?r=26)
	// is even faster but results in too much error
	float w = rsqrt(*ch**ch+*sh**sh);
	*ch=(b!=0x00)?w**ch:(float)_cstar;
	*sh=(b!=0x00)?w**sh:(float)_sstar;
}

inline void jacobiConjugation( const int x, const int y, const int z,
							   float *s11,
							   float *s21, float *s22,
							   float *s31, float *s32, float *s33,
							   float * qV)
{
	float ch,sh;
	approximateGivensQuaternion(*s11,*s21,*s22,&ch,&sh);
	
	float scale = ch*ch+sh*sh;
	float a = (ch*ch-sh*sh)/scale;
	float b = (2*sh*ch)/scale;
	
	// make temp copy of S
	float _s11 = *s11;
	float _s21 = *s21; float _s22 = *s22;
	float _s31 = *s31; float _s32 = *s32; float _s33 = *s33;
	
	// perform conjugation S = Q'*S*Q
	// Q already implicitly solved from a, b
	*s11 =a*(a*_s11 + b*_s21) + b*(a*_s21 + b*_s22);
	*s21 =a*(-b*_s11 + a*_s21) + b*(-b*_s21 + a*_s22);	*s22=-b*(-b*_s11 + a*_s21) + a*(-b*_s21 + a*_s22);
	*s31 =a*_s31 + b*_s32;								*s32=-b*_s31 + a*_s32; *s33=_s33;
	
	// update cumulative rotation qV
	float tmp[3];
	tmp[0]=qV[0]*sh;
	tmp[1]=qV[1]*sh;
	tmp[2]=qV[2]*sh;
	sh *= qV[3];
	
	qV[0] *= ch;
	qV[1] *= ch;
	qV[2] *= ch;
	qV[3] *= ch;
	
	// (x,y,z) corresponds to ((0,1,2),(1,2,0),(2,0,1))
	// for (p,q) = ((0,1),(1,2),(0,2))
	qV[z] += sh;
	qV[3] -= tmp[z]; // w
	qV[x] += tmp[y];
	qV[y] -= tmp[x];
	
	// re-arrange matrix for next iteration
	_s11 = *s22;
	_s21 = *s32; _s22 = *s33;
	_s31 = *s21; _s32 = *s31; _s33 = *s11;
	*s11 = _s11;
	*s21 = _s21; *s22 = _s22;
	*s31 = _s31; *s32 = _s32; *s33 = _s33;
	
}

inline float dist2(float x, float y, float z)
{
	return x*x+y*y+z*z;
}

// finds transformation that diagonalizes a symmetric matrix
inline void jacobiEigenanlysis( // symmetric matrix
		float *s11,
		float *s21, float *s22,
		float *s31, float *s32, float *s33,
		// quaternion representation of V
		float * qV)
{
	qV[3]=1; qV[0]=0;qV[1]=0;qV[2]=0; // follow same indexing convention as GLM
	for (int i=0;i<4;i++)
	{
		// we wish to eliminate the maximum off-diagonal element
		// on every iteration, but cycling over all 3 possible rotations
		// in fixed order (p,q) = (1,2) , (2,3), (1,3) still retains
		//  asymptotic convergence
		jacobiConjugation(0,1,2,s11,s21,s22,s31,s32,s33,qV); // p,q = 0,1
		jacobiConjugation(1,2,0,s11,s21,s22,s31,s32,s33,qV); // p,q = 1,2
		jacobiConjugation(2,0,1,s11,s21,s22,s31,s32,s33,qV); // p,q = 0,2
	}
}


inline void sortSingularValues(// matrix that we want to decompose
		float *b11, float *b12, float *b13,
		float *b21, float *b22, float *b23,
		float *b31, float *b32, float *b33,
		// sort V simultaneously
		float *v11, float *v12, float *v13,
		float *v21, float *v22, float *v23,
		float *v31, float *v32, float *v33)
{
	float rho1 = dist2(*b11,*b21,*b31);
	float rho2 = dist2(*b12,*b22,*b32);
	float rho3 = dist2(*b13,*b23,*b33);
	uint8_t c;
	c = rho1 < rho2;
	condNegSwap(c,b11,b12); condNegSwap(c,v11,v12);
	condNegSwap(c,b21,b22); condNegSwap(c,v21,v22);
	condNegSwap(c,b31,b32); condNegSwap(c,v31,v32);
	condSwap(c,&rho1,&rho2);
	c = rho1 < rho3;
	condNegSwap(c,b11,b13); condNegSwap(c,v11,v13);
	condNegSwap(c,b21,b23); condNegSwap(c,v21,v23);
	condNegSwap(c,b31,b33); condNegSwap(c,v31,v33);
	condSwap(c,&rho1,&rho3);
	c = rho2 < rho3;
	condNegSwap(c,b12,b13); condNegSwap(c,v12,v13);
	condNegSwap(c,b22,b23); condNegSwap(c,v22,v23);
	condNegSwap(c,b32,b33); condNegSwap(c,v32,v33);
}


inline void QRGivensQuaternion(float a1, float a2, float *ch, float *sh)
{
	// a1 = pivot point on diagonal
	// a2 = lower triangular entry we want to annihilate
	float epsilon = (float)EPSILON;
	float rho = accurateSqrt(a1*a1 + a2*a2);
	
	*sh = rho > epsilon ? a2 : 0;
	*ch = fabsf(a1) + fmaxf(rho,epsilon);
	uint8_t b = a1 < 0;
	condSwap(b,sh,ch);
	float w = rsqrt(*ch**ch+*sh**sh);
	*ch *= w;
	*sh *= w;
}


inline void QRDecomposition(// matrix that we want to decompose
		float b11, float b12, float b13,
		float b21, float b22, float b23,
		float b31, float b32, float b33,
		// output Q
		float *q11, float *q12, float *q13,
		float *q21, float *q22, float *q23,
		float *q31, float *q32, float *q33,
		// output R
		float *r11, float *r12, float *r13,
		float *r21, float *r22, float *r23,
		float *r31, float *r32, float *r33)
{
	float ch1,sh1,ch2,sh2,ch3,sh3;
	float a,b;
	
	// first givens rotation (ch,0,0,sh)
	QRGivensQuaternion(b11,b21,&ch1,&sh1);
	a=1-2*sh1*sh1;
	b=2*ch1*sh1;
	// apply B = Q' * B
	*r11=a*b11+b*b21;  *r12=a*b12+b*b22;  *r13=a*b13+b*b23;
	*r21=-b*b11+a*b21; *r22=-b*b12+a*b22; *r23=-b*b13+a*b23;
	*r31=b31;          *r32=b32;          *r33=b33;
	
	// second givens rotation (ch,0,-sh,0)
	QRGivensQuaternion(*r11,*r31,&ch2,&sh2);
	a=1-2*sh2*sh2;
	b=2*ch2*sh2;
	// apply B = Q' * B;
	b11=a**r11+b**r31;  b12=a**r12+b**r32;  b13=a**r13+b**r33;
	b21=*r21;           b22=*r22;           b23=*r23;
	b31=-b**r11+a**r31; b32=-b**r12+a**r32; b33=-b**r13+a**r33;
	
	// third givens rotation (ch,sh,0,0)
	QRGivensQuaternion(b22,b32,&ch3,&sh3);
	a=1-2*sh3*sh3;
	b=2*ch3*sh3;
	// R is now set to desired value
	*r11=b11;             *r12=b12;           *r13=b13;
	*r21=a*b21+b*b31;     *r22=a*b22+b*b32;   *r23=a*b23+b*b33;
	*r31=-b*b21+a*b31;    *r32=-b*b22+a*b32;  *r33=-b*b23+a*b33;
	
	// construct the cumulative rotation Q=Q1 * Q2 * Q3
	// the number of floating point operations for three quaternion multiplications
	// is more or less comparable to the explicit form of the joined matrix.
	// certainly more memory-efficient!
	float sh12=sh1*sh1;
	float sh22=sh2*sh2;
	float sh32=sh3*sh3;
	
	*q11=(-1+2*sh12)*(-1+2*sh22);
	*q12=4*ch2*ch3*(-1+2*sh12)*sh2*sh3+2*ch1*sh1*(-1+2*sh32);
	*q13=4*ch1*ch3*sh1*sh3-2*ch2*(-1+2*sh12)*sh2*(-1+2*sh32);
	
	*q21=2*ch1*sh1*(1-2*sh22);
	*q22=-8*ch1*ch2*ch3*sh1*sh2*sh3+(-1+2*sh12)*(-1+2*sh32);
	*q23=-2*ch3*sh3+4*sh1*(ch3*sh1*sh3+ch1*ch2*sh2*(-1+2*sh32));
	
	*q31=2*ch2*sh2;
	*q32=2*ch3*(1-2*sh22)*sh3;
	*q33=(-1+2*sh22)*(-1+2*sh32);
}

inline void svd(// input A
		float a11, float a12, float a13,
		float a21, float a22, float a23,
		float a31, float a32, float a33,
		// output U
		float *u11, float *u12, float *u13,
		float *u21, float *u22, float *u23,
		float *u31, float *u32, float *u33,
		// output S
		float *s11, float *s12, float *s13,
		float *s21, float *s22, float *s23,
		float *s31, float *s32, float *s33,
		// output V
		float *v11, float *v12, float *v13,
		float *v21, float *v22, float *v23,
		float *v31, float *v32, float *v33)
{
	// normal equations matrix
	float ATA11, ATA12, ATA13;
	float ATA21, ATA22, ATA23;
	float ATA31, ATA32, ATA33;
	
	multAtB(a11,a12,a13,a21,a22,a23,a31,a32,a33,
			a11,a12,a13,a21,a22,a23,a31,a32,a33,
			&ATA11,&ATA12,&ATA13,&ATA21,&ATA22,&ATA23,&ATA31,&ATA32,&ATA33);
	
	// symmetric eigenalysis
	float qV[4];
	jacobiEigenanlysis(&ATA11,&ATA21,&ATA22, &ATA31,&ATA32,&ATA33,qV);
	quatToMat3(qV,v11,v12,v13,v21,v22,v23,v31,v32,v33);
	
	float b11, b12, b13;
	float b21, b22, b23;
	float b31, b32, b33;
	multAB(a11,a12,a13,a21,a22,a23,a31,a32,a33,
		   *v11,*v12,*v13,*v21,*v22,*v23,*v31,*v32,*v33,
		   &b11, &b12, &b13, &b21, &b22, &b23, &b31, &b32, &b33);
	
	// sort singular values and find V
	sortSingularValues(&b11, &b12, &b13, &b21, &b22, &b23, &b31, &b32, &b33,
					   v11,v12,v13,v21,v22,v23,v31,v32,v33);
	
	// QR decomposition
	QRDecomposition(b11, b12, b13, b21, b22, b23, b31, b32, b33,
					u11, u12, u13, u21, u22, u23, u31, u32, u33,
					s11, s12, s13, s21, s22, s23, s31, s32, s33
	);
}

//------------------------------------------------------------------------------

inline void vl_svd(float *mat, float *u, float *s, float *v)
{
	svd(
			mat[0], mat[1], mat[2],
			mat[3], mat[4], mat[5],
			mat[6], mat[7], mat[8],
			
			&u[0], &u[1], &u[2],
			&u[3], &u[4], &u[5],
			&u[6], &u[7], &u[8],
			
			&s[0], &s[1], &s[2],
			&s[3], &s[4], &s[5],
			&s[6], &s[7], &s[8],
			
			&v[0], &v[1], &v[2],
			&v[3], &v[4], &v[5],
			&v[6], &v[7], &v[8]
	);
	
	return;
}

//------------------------------------------------------------------------------

inline void vl_rnorm(vlf_t *mat)
{
	float u[9];
	float s[9];
	float v[9];
	
	vlf_t vlf_u[9];
	vlf_t vlf_v[9];
	vlf_t vlf_v_t[9];
	
	float f_mat[9] = {
			(float) mat[0], (float) mat[1], (float) mat[2],
			(float) mat[3], (float) mat[4], (float) mat[5],
			(float) mat[6], (float) mat[7], (float) mat[8],
	};
	
	// smh matrices close to det(A) == 1 cant be SVDed
	if (fabsl(1.0 - vl_det(mat)) > 0.01)
	{
		vl_svd(f_mat, u, s, v);
		
		uint32_t i;
		for (i = 0; i < 9; ++i)
		{
			mat[i]   = (vlf_t) f_mat[i];
			vlf_u[i] = (vlf_t) u[i];
			vlf_v[i] = (vlf_t) v[i];
		}
		
		vl_tnp(vlf_v_t, vlf_v);
		vl_mmul_m(mat, vlf_u, vlf_v_t);
	}

//    u, a, v = scipy.linalg.svd(rot);
//    return numpy.mat(u.dot(v));
	
	return;
}

//------------------------------------------------------------------------------

inline void vl_rinter(vlf_t *res, vlf_t *r0, vlf_t *r1, vlf_t dist)
{
	vlf_t rot[9];
	
	vl_rd1(rot, r0, r1);
	vl_mmul_s(rot, rot, dist);
	vl_mmul_m(res, rot, r0);
	vl_msum(res, res, r0);
	vl_rnorm(res);

//	vlf_t test[9];
//	vl_mmul_m(test, rot, r0);
//	vl_msub(test, test, r1);
	
	return;
}

//------------------------------------------------------------------------------

#endif /* __VL__ */
































