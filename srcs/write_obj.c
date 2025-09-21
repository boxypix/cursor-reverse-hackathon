#include "morphosis.h"

static void						fetch_vertex_coords(float3 vertex, float *res)
{
	res[0] = vertex.x;
	res[1] = vertex.y;
	res[2] = vertex.z;
}

void 						export_obj(t_data *data)
{
	obj 					*o;
	int						surface;

	o = obj_create(NULL);
	surface = obj_add_surf(o);
	write_mesh(data, surface, o);
	printf("SAVING-----\n");
	obj_sort(o, 32);
	obj_proc(o);
	obj_write(o, OUTPUT_FILE, NULL, OUTPUT_PRECISION);
	obj_delete(o);
}

void						write_mesh(t_data *data, int surface, obj *o)
{
	float3 					**tris;
	uint 					i;
	int						polygon;
	int 					verts[3];
	float 					*vertex;

	if (!(vertex = (float *)malloc(3 * sizeof(float))))
		error(MALLOC_FAIL_ERR, data);
	tris = data->triangles;
	i = 0;
	while (i < data->gl->num_tris)
	{
		// Show progress every 1000 triangles instead of every triangle
		if (i % 1000 == 0 || i == data->gl->num_tris - 1)
			printf("Written: %.3f %%\n", (((float)i / data->gl->num_tris) * 100));
		
		polygon = obj_add_poly(o, surface);
		for (int v = 0; v < 3; v++)
		{
			verts[v] = obj_add_vert(o);
			fetch_vertex_coords(tris[i][v], vertex);
			obj_set_vert_v(o, verts[v], vertex);
		}
		obj_set_poly(o, surface, polygon, verts);
		i++;
	}
	free(vertex);
}
