#include <morphosis.h>

int					***read_poem(FILE *stream)
{
	// int				i; // Unused variables removed
	// int				j;
	char			c;
	int 			nbr;
	int 			***matrix;

	int				c_t = 0;
	int				dim = 0;

	matrix = alloc_matrix1();
	while (fscanf(stream, "%c1", &c) == 1)
	{
		nbr = c;
		if (c_t == 36)
		{
			c_t = 0;
			dim++;
		}
		fill_matrix(matrix[dim], nbr, c_t);
		c_t++;
	}
	return matrix;
}
