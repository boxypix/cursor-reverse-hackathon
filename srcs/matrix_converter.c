#include <morphosis.h>
#include <libft.h>

void							free_matrix1(int ***m)
{
	int						i;
	int						j;

	i = 0;
	while (i < 6)
	{
		j = 0;
		while (j < 6)
		{
			free(m[i][j]);
			j++;
		}
		free(m[i]);
		i++;
	}
	free(m);
	m = NULL;
}

static void					free_matrix2(int **m)
{
	int						i = 0;

	while (i < 6)
	{
		free(m[i]);
		i++;
	}
	free(m);
	m = NULL;
}

int							***alloc_matrix1(void)
{
	int						***matrix;
	int						i = 0;
	int						j = 0;

	matrix = (int ***)malloc(sizeof(int **) * 6);
	while (i < 6)
	{
		matrix[i] = (int **)malloc(sizeof(int *) * 6);
		j = 0;
		while (j < 6)
		{
			matrix[i][j] = (int *)malloc(sizeof(int) * 6);
			for (int n = 0; n < 6; n++)
				matrix[i][j][n] = 0;
			j++;
		}
		i++;
	}
	return(matrix);
}

static int					**alloc_matrix2(void)
{
	int						**matrix;
	int						i = 0;

	matrix = (int **)malloc(sizeof(int *) * 6);
	while (i < 6)
	{
		matrix[i] = (int *)malloc(sizeof(int) * 6);
		for (int n = 0; n < 6; n++)
			matrix[i][n] = 0;
		i++;
	}
	return(matrix);
}

static int					bin_to_dec(char *bin)
{
	int						binary[6];
	int						pwr = 5;
	int						res = 0;

	for (int i = 0; i < 6; i++)
	{
		if (bin[i] != '1' && bin[i] != '0')
			error(BAD_FILE_ERR, NULL);
		binary[i] = bin[i] - 48;
		res = res + (int)(binary[i] * pow(2, pwr));
		pwr--;
	}
	return(res);
}

void						fill_matrix(int **matrix, int number, int reset)
{
	static int				row = 0;
	static int				col = 0;

	if (row < 6 && col < 6)
	{
		matrix[col][row] = number;
		row++;
	}
	else if (row == 6 && col < 6)
	{
		row = 0;
		col++;
		matrix[col][row] = number;
		row++;
	}
	if (reset == 35)
	{
		row = 0;
		col = 0;
	}
}

static int					***parse_data(int fd, char *line)
{
	int						line_c;
	int						i;
	int						j;
	char					*tmp;
	char					*bin;
	int						***matrix;
	int						nbr;
	int						line_num_valid;

	int 					c_t = 0;
	int						dim = 0;

	matrix = alloc_matrix1();
	line_num_valid = 0;
	while ((get_next_line(fd, &line)))
	{
		line_num_valid++;
		line_c = 0;
		while (line[line_c])
		{
			bin = (char *)malloc(sizeof(char) * 6);
			tmp = (char *)malloc(sizeof(char) * 12);
			ft_bzero(tmp, 12);
			ft_bzero(bin, 6);
			tmp = ft_strncpy(tmp, &line[line_c], 12);
			i = 0;
			j = 0;
			while (tmp[i])
			{
				if (tmp[i] == 48 || tmp[i] == 49)
				{
					bin[j] = tmp[i];
					j++;
				}
				i++;
			}
			nbr = bin_to_dec(bin);
			if (c_t == 36)
			{
				c_t = 0;
				dim++;
			}
			fill_matrix(matrix[dim], nbr, c_t);
			free(bin);
			free(tmp);
			line_c += 12;
			c_t++;
		}
		free(line);
	}
	free(line);
	line = NULL;
	if (line_num_valid != 36)
		error(BAD_FILE_ERR, NULL);
	return(matrix);
}

static int					**find_mean(int	***matrix)
{
	int						**mean;
	int						dim = 0;
	int						row = 0;
	int						col = 0;
	int						i = 0;
	int						j = 0;
	int						res = 0;

	mean = alloc_matrix2();
	while (col < 6)
	{
		dim = 0;
		row = 0;
		res = 0;
		j = 0;
		while (row < 6)
		{
			dim = 0;
			res = 0;
			while (dim < 6)
			{
				res += matrix[dim][col][row];
				dim++;
			}
			res /= 6;
			mean[i][j] = res;
			row++;
			j++;
		}
		col++;
		i++;
	}
	return(mean);
}

void							process_matrix(char *file, t_mat_conv_data *data, int mode)
{
	int						fd;
	// int						fd_result_mean; // Unused variable removed
	char					*line = NULL; // Initialize to prevent uninitialized usage
	int						***decimals;
	int						**mean;

	char					*matrix;
	FILE 					*stream;

	int						processing_mode;

	processing_mode = MODE;
	decimals = NULL;
	mean = NULL;

	if (mode == POEM)
		processing_mode = 1;
	if (processing_mode == 1)
	{
		if (mode == MATRIX)
		{
			if ((fd = open(file, O_RDONLY)) < 0)
				error(OPEN_FILE_ERR, NULL);
			decimals = parse_data(fd, line);
			close(fd);
		}
		else if (mode == POEM)
		{
			stream = fopen(file, "r");
			decimals = read_poem(stream);
			fclose(stream);
		}
		mean = find_mean(decimals);
		free_matrix1(decimals);
		matrix_hash(mean, data);
		free_matrix2(mean);
	}
	else if (processing_mode == 2)
	{
		stream = fopen(file, "r");
		matrix = read_matrix(stream);
		fclose(stream);
		matrix_hash2(matrix, data);
	}

	printf("Matrix processed\n");
}
