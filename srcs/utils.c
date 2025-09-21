#include "morphosis.h"

// Optimized memory allocation with exponential growth to avoid O(n²) complexity
float3						**alloc_float3_arr_optimized(float3 **mem, uint2 *len, uint *capacity)
{
	uint 					required_size = len->x + len->y;
	
	if (!required_size)
		return NULL;
	
	// If we need more space than current capacity, grow exponentially
	if (required_size > *capacity)
	{
		// Grow by 1.5x or to required size, whichever is larger
		uint new_capacity = (*capacity > 0) ? *capacity : 16;
		while (new_capacity < required_size)
			new_capacity = new_capacity + (new_capacity >> 1); // 1.5x growth
		
		if (!mem)
		{
			if (!(mem = (float3 **)malloc(sizeof(float3 *) * new_capacity)))
				return NULL;
		}
		else
		{
			if (!(mem = (float3 **)realloc(mem, sizeof(float3 *) * new_capacity)))
				return NULL;
		}
		
		// Allocate memory for new triangles
		for (uint c = *capacity; c < new_capacity; c++)
		{
			if (!(mem[c] = (float3 *)malloc(3 * sizeof(float3))))
				return NULL;
		}
		*capacity = new_capacity;
	}
	
	return mem;
}

// Legacy function - simplified version without optimization for now
float3						**alloc_float3_arr(float3 **mem, uint2 *len)
{
	uint 					size;

	size = len->x + len->y;
	if (!size)
		return NULL;
	if (!mem)
	{
		if (!(mem = (float3 **)malloc(sizeof(float3 *) * size)))
			return NULL;
		for (uint c = len->x; c < size; c++)
		{
			if (!(mem[c] = (float3 *)malloc(3 * sizeof(float3))))
				return NULL;
		}
	}
	else
	{
		if (!(mem = (float3 **)realloc(mem, sizeof(float3 *) * size)))
			return NULL;
		for (uint c = len->x; c < size; c++)
		{
			if (!(mem[c] = (float3 *)malloc(3 * sizeof(float3))))
				return NULL;
		}
	}
	return mem;
}

float3						**arr_float3_cat(float3 **f_from, float3 **f_to, uint2 *len)
{
	uint					from_c;
	uint 					to_c;
	uint 					res_c;

	res_c = len->x + len->y;
	from_c = 0;
	to_c = len->x;
	if (!(f_to = alloc_float3_arr(f_to, len)))
		return NULL;

	while (to_c < res_c)
	{
		f_to[to_c][0] = f_from[from_c][0];
		f_to[to_c][1] = f_from[from_c][1];
		f_to[to_c][2] = f_from[from_c][2];
		to_c++;
		from_c++;
	}
	len->x = res_c;

	if (f_from)
	{
		for (uint i = 0; i < len->y; i++)
			free(f_from[i]);
		free(f_from);
		f_from = NULL;
	}

	len->y = 0;
	f_from = NULL;
	return f_to;
}
