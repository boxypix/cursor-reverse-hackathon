#include "morphosis.h"
#include <string.h>

// Simple text rendering using OpenGL lines
// This is a basic implementation for button labels

void render_text_simple(t_gl *gl, const char *text, float x, float y, float scale) {
    // This is a placeholder for simple text rendering
    // In a real implementation, you would:
    // 1. Load a font texture
    // 2. Create a text shader
    // 3. Render each character as a textured quad
    
    // For now, we'll just draw a simple pattern to indicate text
    GLint color_loc = glGetUniformLocation(gl->ui->ui_shader_program, "color");
    glUniform3f(color_loc, 0.0f, 0.0f, 0.0f); // Black text
    
    // Draw simple lines to represent text
    // This is just a placeholder - real text rendering would be much more complex
    (void)text;
    (void)x;
    (void)y;
    (void)scale;
}

void render_button_with_text(t_gl *gl, t_ui_button *button) {
    mat4 transform;
    GLint transform_loc;
    GLint color_loc;
    float x_norm, y_norm, w_norm, h_norm;
    
    // Convert screen coordinates to normalized device coordinates
    x_norm = (2.0f * button->x / SRC_WIDTH) - 1.0f;
    y_norm = 1.0f - (2.0f * button->y / SRC_HEIGHT);
    w_norm = button->width / SRC_WIDTH;
    h_norm = button->height / SRC_HEIGHT;
    
    // Set uniforms
    transform_loc = glGetUniformLocation(gl->ui->ui_shader_program, "transform");
    color_loc = glGetUniformLocation(gl->ui->ui_shader_program, "color");
    
    // Draw black border
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
    
    // Draw simple text representation
    render_text_simple(gl, button->text, button->x + 5, button->y + 15, 1.0f);
}
