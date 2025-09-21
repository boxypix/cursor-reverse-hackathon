#pragma once

# include <lib_complex.h>

typedef struct 				s_matrix
{
	mat4 					model_mat;
	mat4 					projection_mat;
	mat4 					view_mat;

	vec3 					eye;
	vec3 					center;
	vec3 					up;

	GLuint 					model;
	GLuint 					view;
}							t_matrix;

typedef struct 				s_ui_button
{
	float					x;
	float					y;
	float					width;
	float					height;
	char					*text;
	int						active;
}							t_ui_button;

typedef struct 				s_mouse
{
	double					last_x;
	double					last_y;
	int						first_mouse;
	float					yaw;
	float					pitch;
}							t_mouse;

typedef struct 				s_ui
{
	t_ui_button				render_buttons[3];  // Line, Solid, Shiny
	t_ui_button				bg_buttons[3];      // Black, Gradient, Blue
	t_ui_button				exit_button;        // X button
	t_ui_button				input_field;        // Parameter input field
	t_ui_button				ok_button;          // OK button for input
	
	GLuint					ui_vao;
	GLuint					ui_vbo;
	GLuint					ui_shader_program;
	GLuint					ui_vertex_shader;
	GLuint					ui_fragment_shader;
	
	t_mouse					*mouse;
}							t_ui;

typedef struct 				s_gl
{
	GLFWwindow 				*window;

	int						export;

	GLuint 					vertexShader;
	GLuint 					fragmentShader;
	GLuint					shaderProgram;

	GLuint 					vbo;
	GLuint 					vao;

	float					*tris;
	uint 					num_pts;
	uint					num_tris;
	t_matrix 				*matrix;
	t_ui					*ui;
}							t_gl;

typedef struct 				s_julia
{
	uint					max_iter;
	float 					threshold;
	float 					w;
	cl_quat 				c;
}							t_julia;

typedef struct 				s_grid
{
	float 					*x;
	float 					*y;
	float 					*z;
}							t_grid;

typedef struct 				s_voxel
{
	float 					dx;
	float 					dy;
	float 					dz;
}							t_voxel;

typedef struct				s_fract
{
	float3 					p0;
	float3 					p1;
	float 					step_size;
	float 					grid_length;
	float 					grid_size;

	t_julia 				*julia;
	t_grid 					grid;
	t_voxel 				voxel[8];
}							t_fract;

typedef struct 				s_data
{
	t_gl					*gl;
	t_fract 				*fract;
	float3 					*vertexpos;
	float					*vertexval;
	float3					**triangles;

	uint2 					len;
}							t_data;
