#include "morphosis.h"
#include <string.h>

// UI vertex data for a rectangle (two triangles)
static float ui_vertices[] = {
    // First triangle
    -1.0f, -1.0f, 0.0f,  // bottom-left
     1.0f, -1.0f, 0.0f,  // bottom-right
     1.0f,  1.0f, 0.0f,  // top-right
    
    // Second triangle
    -1.0f, -1.0f, 0.0f,  // bottom-left
     1.0f,  1.0f, 0.0f,  // top-right
    -1.0f,  1.0f, 0.0f   // top-left
};

// Simple UI vertex shader source
static const char* ui_vertex_shader_source = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "uniform mat4 transform;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = transform * vec4(aPos, 1.0);\n"
    "}\0";

// Simple UI fragment shader source
static const char* ui_fragment_shader_source = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "uniform vec3 color;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(color, 1.0f);\n"
    "}\n\0";

t_ui						*init_ui(void)
{
	t_ui					*ui;
	
	if (!(ui = (t_ui *)malloc(sizeof(t_ui))))
		error(MALLOC_FAIL_ERR, NULL);
		
	ui->ui_vao = 0;
	ui->ui_vbo = 0;
	ui->ui_shader_program = 0;
	ui->ui_vertex_shader = 0;
	ui->ui_fragment_shader = 0;
	
	// Mouse disabled for now
	ui->mouse = NULL;
	
	setup_ui_buttons(ui);
	init_ui_shaders(ui);
	
	// Create VAO and VBO for UI rendering
	glGenVertexArrays(1, &ui->ui_vao);
	glGenBuffers(1, &ui->ui_vbo);
	
	glBindVertexArray(ui->ui_vao);
	glBindBuffer(GL_ARRAY_BUFFER, ui->ui_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(ui_vertices), ui_vertices, GL_STATIC_DRAW);
	
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	
	glBindVertexArray(0);
	
	return ui;
}

void						setup_ui_buttons(t_ui *ui)
{
	float button_width = 80.0f;
	float button_height = 35.0f;
	float button_spacing = 15.0f;
	float start_x = 30.0f;
	float bottom_y = 20.0f;
	
	// Initialize render buttons: Line, Solid, Shiny
	ui->render_buttons[0].x = start_x;
	ui->render_buttons[0].y = bottom_y;
	ui->render_buttons[0].width = button_width;
	ui->render_buttons[0].height = button_height;
	ui->render_buttons[0].text = "Line";
	ui->render_buttons[0].active = 1; // Line is default
	
	ui->render_buttons[1].x = start_x + button_width + button_spacing;
	ui->render_buttons[1].y = bottom_y;
	ui->render_buttons[1].width = button_width;
	ui->render_buttons[1].height = button_height;
	ui->render_buttons[1].text = "Solid";
	ui->render_buttons[1].active = 0;
	
	ui->render_buttons[2].x = start_x + 2 * (button_width + button_spacing);
	ui->render_buttons[2].y = bottom_y;
	ui->render_buttons[2].width = button_width;
	ui->render_buttons[2].height = button_height;
	ui->render_buttons[2].text = "Shiny";
	ui->render_buttons[2].active = 0;
	
	// Initialize background buttons: Black, Gradient, Blue
	float bg_start_x = start_x + 3 * (button_width + button_spacing) + 30.0f;
	
	ui->bg_buttons[0].x = bg_start_x;
	ui->bg_buttons[0].y = bottom_y;
	ui->bg_buttons[0].width = button_width;
	ui->bg_buttons[0].height = button_height;
	ui->bg_buttons[0].text = "Black";
	ui->bg_buttons[0].active = 1; // Black is default
	
	ui->bg_buttons[1].x = bg_start_x + button_width + button_spacing;
	ui->bg_buttons[1].y = bottom_y;
	ui->bg_buttons[1].width = button_width;
	ui->bg_buttons[1].height = button_height;
	ui->bg_buttons[1].text = "Gradient";
	ui->bg_buttons[1].active = 0;
	
	ui->bg_buttons[2].x = bg_start_x + 2 * (button_width + button_spacing);
	ui->bg_buttons[2].y = bottom_y;
	ui->bg_buttons[2].width = button_width;
	ui->bg_buttons[2].height = button_height;
	ui->bg_buttons[2].text = "Blue";
	ui->bg_buttons[2].active = 0;
	
	// Initialize input field (top-left area)
	ui->input_field.x = 30.0f;
	ui->input_field.y = SRC_HEIGHT - 50.0f;
	ui->input_field.width = 200.0f;
	ui->input_field.height = 35.0f;
	ui->input_field.text = "0.1 0.0 0.0 0.0 1.0";
	ui->input_field.active = 0;
	
	// Initialize OK button (next to input field)
	ui->ok_button.x = 250.0f;
	ui->ok_button.y = SRC_HEIGHT - 50.0f;
	ui->ok_button.width = 50.0f;
	ui->ok_button.height = 35.0f;
	ui->ok_button.text = "OK";
	ui->ok_button.active = 0;
	
	// Initialize exit button (top-right corner)
	ui->exit_button.x = SRC_WIDTH - 60.0f;
	ui->exit_button.y = SRC_HEIGHT - 50.0f;
	ui->exit_button.width = 35.0f;
	ui->exit_button.height = 35.0f;
	ui->exit_button.text = "X";
	ui->exit_button.active = 0;
}

void						init_ui_shaders(t_ui *ui)
{
	GLint					success;
	GLchar					infoLog[512];
	
	// Create vertex shader
	ui->ui_vertex_shader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(ui->ui_vertex_shader, 1, &ui_vertex_shader_source, NULL);
	glCompileShader(ui->ui_vertex_shader);
	
	glGetShaderiv(ui->ui_vertex_shader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(ui->ui_vertex_shader, 512, NULL, infoLog);
		printf("ERROR: UI Vertex shader compilation failed: %s\n", infoLog);
	}
	
	// Create fragment shader
	ui->ui_fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(ui->ui_fragment_shader, 1, &ui_fragment_shader_source, NULL);
	glCompileShader(ui->ui_fragment_shader);
	
	glGetShaderiv(ui->ui_fragment_shader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(ui->ui_fragment_shader, 512, NULL, infoLog);
		printf("ERROR: UI Fragment shader compilation failed: %s\n", infoLog);
	}
	
	// Create shader program
	ui->ui_shader_program = glCreateProgram();
	glAttachShader(ui->ui_shader_program, ui->ui_vertex_shader);
	glAttachShader(ui->ui_shader_program, ui->ui_fragment_shader);
	glLinkProgram(ui->ui_shader_program);
	
	glGetProgramiv(ui->ui_shader_program, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(ui->ui_shader_program, 512, NULL, infoLog);
		printf("ERROR: UI Shader program linking failed: %s\n", infoLog);
	}
	
	glDeleteShader(ui->ui_vertex_shader);
	glDeleteShader(ui->ui_fragment_shader);
}

void						render_button(t_gl *gl, t_ui_button *button)
{
	mat4					transform;
	GLint					transform_loc;
	GLint					color_loc;
	float					x_norm, y_norm, w_norm, h_norm;
	
	// Convert screen coordinates to normalized device coordinates
	x_norm = (2.0f * button->x / SRC_WIDTH) - 1.0f;
	y_norm = 1.0f - (2.0f * button->y / SRC_HEIGHT);
	w_norm = button->width / SRC_WIDTH;
	h_norm = button->height / SRC_HEIGHT;
	
	// Set uniforms
	transform_loc = glGetUniformLocation(gl->ui->ui_shader_program, "transform");
	color_loc = glGetUniformLocation(gl->ui->ui_shader_program, "color");
	
	// Draw black border (slightly larger rectangle)
	glm_mat4_identity(transform);
	glm_translate(transform, (vec3){x_norm + w_norm, y_norm - h_norm, 0.0f});
	glm_scale(transform, (vec3){w_norm + 0.005f, h_norm + 0.005f, 1.0f});
	glUniformMatrix4fv(transform_loc, 1, GL_FALSE, (float*)transform);
	glUniform3f(color_loc, 0.0f, 0.0f, 0.0f); // Black border
	glBindVertexArray(gl->ui->ui_vao);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	
	// Draw white button background
	glm_mat4_identity(transform);
	glm_translate(transform, (vec3){x_norm + w_norm, y_norm - h_norm, 0.0f});
	glm_scale(transform, (vec3){w_norm, h_norm, 1.0f});
	glUniformMatrix4fv(transform_loc, 1, GL_FALSE, (float*)transform);
	
	// Set button color based on active state
	if (button->active)
		glUniform3f(color_loc, 0.9f, 0.9f, 0.9f); // Slightly darker white for active
	else
		glUniform3f(color_loc, 1.0f, 1.0f, 1.0f); // Pure white for inactive
	
	glDrawArrays(GL_TRIANGLES, 0, 6);
	
	// Simple text rendering using OpenGL lines
	render_button_text(gl, button);
}

void						render_ui(t_gl *gl)
{
	int						i;
	
	if (!gl->ui)
		return;
		
	// Save current OpenGL state
	GLint current_program;
	glGetIntegerv(GL_CURRENT_PROGRAM, &current_program);
		
	// Enable blending for UI elements
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	// Disable depth testing for UI
	glDisable(GL_DEPTH_TEST);
	
	// Use UI shader
	glUseProgram(gl->ui->ui_shader_program);
	
	// Render render buttons
	for (i = 0; i < 3; i++)
	{
		render_button(gl, &gl->ui->render_buttons[i]);
	}
	
	// Render background buttons
	for (i = 0; i < 3; i++)
	{
		render_button(gl, &gl->ui->bg_buttons[i]);
	}
	
	// Render input field
	render_button(gl, &gl->ui->input_field);
	
	// Render OK button
	render_button(gl, &gl->ui->ok_button);
	
	// Render exit button
	render_button(gl, &gl->ui->exit_button);
	
	// Restore OpenGL state
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);
	glUseProgram(current_program);
}

void						handle_ui_input(GLFWwindow *window, t_ui *ui)
{
	// TODO: Implement mouse click handling for buttons
	// This will be implemented when we add interaction functionality
	(void)window;
	(void)ui;
}

void						init_mouse(t_mouse *mouse)
{
	mouse->last_x = SRC_WIDTH / 2.0;
	mouse->last_y = SRC_HEIGHT / 2.0;
	mouse->first_mouse = 1;
	mouse->yaw = 45.0f;  // Start with a better angle
	mouse->pitch = -30.0f;  // Look down slightly
}

void						mouse_callback(GLFWwindow *window, double xpos, double ypos)
{
	t_gl					*gl;
	t_mouse					*mouse;
	float					xoffset;
	float					yoffset;
	float					sensitivity;
	
	gl = (t_gl *)glfwGetWindowUserPointer(window);
	if (!gl || !gl->ui || !gl->ui->mouse)
		return;
		
	mouse = gl->ui->mouse;
	sensitivity = 0.1f;
	
	if (mouse->first_mouse)
	{
		mouse->last_x = xpos;
		mouse->last_y = ypos;
		mouse->first_mouse = 0;
	}
	
	xoffset = (xpos - mouse->last_x) * sensitivity;
	yoffset = (mouse->last_y - ypos) * sensitivity;
	
	mouse->last_x = xpos;
	mouse->last_y = ypos;
	
	mouse->yaw += xoffset;
	mouse->pitch += yoffset;
	
	// Constrain pitch
	if (mouse->pitch > 89.0f)
		mouse->pitch = 89.0f;
	if (mouse->pitch < -89.0f)
		mouse->pitch = -89.0f;
	
	update_camera_rotation(gl);
}

void						update_camera_rotation(t_gl *gl)
{
	t_mouse					*mouse;
	vec3					front;
	vec3					right;
	vec3					up;
	float					yaw_rad;
	float					pitch_rad;
	
	if (!gl || !gl->ui || !gl->ui->mouse)
		return;
		
	mouse = gl->ui->mouse;
	yaw_rad = glm_rad(mouse->yaw);
	pitch_rad = glm_rad(mouse->pitch);
	
	// Calculate new front vector
	front[0] = cos(yaw_rad) * cos(pitch_rad);
	front[1] = sin(pitch_rad);
	front[2] = sin(yaw_rad) * cos(pitch_rad);
	glm_vec3_normalize(front);
	
	// Calculate right and up vectors
	glm_vec3_cross(front, gl->matrix->up, right);
	glm_vec3_normalize(right);
	glm_vec3_cross(right, front, up);
	glm_vec3_normalize(up);
	
	// Update camera center (look at point)
	glm_vec3_add(gl->matrix->eye, front, gl->matrix->center);
	
	// Update view matrix
	glm_lookat(gl->matrix->eye, gl->matrix->center, up, gl->matrix->view_mat);
}

void						render_button_text(t_gl *gl, t_ui_button *button)
{
	// Simple text rendering using lines
	// This is a basic implementation - in a real app you'd use a font system
	GLint					color_loc;
	float					x_norm, y_norm, w_norm, h_norm;
	
	// Convert screen coordinates to normalized device coordinates
	x_norm = (2.0f * button->x / SRC_WIDTH) - 1.0f;
	y_norm = 1.0f - (2.0f * button->y / SRC_HEIGHT);
	w_norm = button->width / SRC_WIDTH;
	h_norm = button->height / SRC_HEIGHT;
	
	color_loc = glGetUniformLocation(gl->ui->ui_shader_program, "color");
	
	// Set text color to black
	glUniform3f(color_loc, 0.0f, 0.0f, 0.0f);
	
	// For now, just draw a simple pattern to indicate text
	// In a real implementation, you'd render actual characters
	// This is just a placeholder to show that text area exists
}

void						cleanup_ui(t_ui *ui)
{
	if (!ui)
		return;
		
	glDeleteVertexArrays(1, &ui->ui_vao);
	glDeleteBuffers(1, &ui->ui_vbo);
	glDeleteProgram(ui->ui_shader_program);
	
	if (ui->mouse)
		free(ui->mouse);
	
	free(ui);
}
