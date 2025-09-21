#include "morphosis.h"

float 						sample_4D_Julia(t_julia *julia, float3 pos)
{
	cl_quat 				z;
	uint 					iter;
	float					temp_mod_squared;
	const float				threshold_squared = 4.0f; // 2.0² = 4.0

	iter = 0;
	z.x = pos.x;
	z.y = pos.y;
	z.z = pos.z;
	z.w = julia->w;

	while (iter < julia->max_iter)
	{
		z = cl_quat_mult(z, z);
		z = cl_quat_sum(z, julia->c);
		// Use optimized squared magnitude to avoid expensive sqrt()
		temp_mod_squared = cl_quat_mod_squared(z);
		if (temp_mod_squared > threshold_squared)
			return 0.0f;
		iter++;
	}
	return 1.0f;
}
