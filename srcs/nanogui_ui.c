#include "morphosis.h"
#include <iostream>
#include <string>

// Simplified NanoGUI-like UI system
// This creates a professional-looking UI without full NanoGUI dependency

class SimpleButton {
public:
    float x, y, width, height;
    std::string text;
    bool active;
    bool hovered;
    
    SimpleButton(float x, float y, float w, float h, const std::string& t) 
        : x(x), y(y), width(w), height(h), text(t), active(false), hovered(false) {}
    
    void render(t_gl *gl) {
        // Convert to normalized coordinates
        float x_norm = (2.0f * x / SRC_WIDTH) - 1.0f;
        float y_norm = 1.0f - (2.0f * y / SRC_HEIGHT);
        float w_norm = width / SRC_WIDTH;
        float h_norm = height / SRC_HEIGHT;
        
        mat4 transform;
        GLint transform_loc = glGetUniformLocation(gl->ui->ui_shader_program, "transform");
        GLint color_loc = glGetUniformLocation(gl->ui->ui_shader_program, "color");
        
        // Draw shadow
        glm_mat4_identity(transform);
        glm_translate(transform, (vec3){x_norm + w_norm + 0.002f, y_norm - h_norm - 0.002f, 0.0f});
        glm_scale(transform, (vec3){w_norm, h_norm, 1.0f});
        glUniformMatrix4fv(transform_loc, 1, GL_FALSE, (float*)transform);
        glUniform3f(color_loc, 0.2f, 0.2f, 0.2f); // Dark shadow
        glBindVertexArray(gl->ui->ui_vao);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        
        // Draw button background
        glm_mat4_identity(transform);
        glm_translate(transform, (vec3){x_norm + w_norm, y_norm - h_norm, 0.0f});
        glm_scale(transform, (vec3){w_norm, h_norm, 1.0f});
        glUniformMatrix4fv(transform_loc, 1, GL_FALSE, (float*)transform);
        
        if (active) {
            glUniform3f(color_loc, 0.3f, 0.6f, 1.0f); // Blue for active
        } else if (hovered) {
            glUniform3f(color_loc, 0.9f, 0.9f, 0.9f); // Light gray for hover
        } else {
            glUniform3f(color_loc, 0.8f, 0.8f, 0.8f); // Light gray for normal
        }
        
        glDrawArrays(GL_TRIANGLES, 0, 6);
        
        // Draw border
        glm_mat4_identity(transform);
        glm_translate(transform, (vec3){x_norm + w_norm, y_norm - h_norm, 0.0f});
        glm_scale(transform, (vec3){w_norm + 0.002f, h_norm + 0.002f, 1.0f});
        glUniformMatrix4fv(transform_loc, 1, GL_FALSE, (float*)transform);
        glUniform3f(color_loc, 0.4f, 0.4f, 0.4f); // Dark border
        glDrawArrays(GL_TRIANGLES, 0, 6);
        
        // Draw inner border
        glm_mat4_identity(transform);
        glm_translate(transform, (vec3){x_norm + w_norm - 0.001f, y_norm - h_norm + 0.001f, 0.0f});
        glm_scale(transform, (vec3){w_norm - 0.002f, h_norm - 0.002f, 1.0f});
        glUniformMatrix4fv(transform_loc, 1, GL_FALSE, (float*)transform);
        glUniform3f(color_loc, 0.9f, 0.9f, 0.9f); // Light inner border
        glDrawArrays(GL_TRIANGLES, 0, 6);
    }
    
    bool isPointInside(float px, float py) {
        return px >= x && px <= x + width && py >= y && py <= y + height;
    }
};

class SimpleTextBox {
public:
    float x, y, width, height;
    std::string text;
    bool focused;
    
    SimpleTextBox(float x, float y, float w, float h, const std::string& t) 
        : x(x), y(y), width(w), height(h), text(t), focused(false) {}
    
    void render(t_gl *gl) {
        float x_norm = (2.0f * x / SRC_WIDTH) - 1.0f;
        float y_norm = 1.0f - (2.0f * y / SRC_HEIGHT);
        float w_norm = width / SRC_WIDTH;
        float h_norm = height / SRC_HEIGHT;
        
        mat4 transform;
        GLint transform_loc = glGetUniformLocation(gl->ui->ui_shader_program, "transform");
        GLint color_loc = glGetUniformLocation(gl->ui->ui_shader_program, "color");
        
        // Draw background
        glm_mat4_identity(transform);
        glm_translate(transform, (vec3){x_norm + w_norm, y_norm - h_norm, 0.0f});
        glm_scale(transform, (vec3){w_norm, h_norm, 1.0f});
        glUniformMatrix4fv(transform_loc, 1, GL_FALSE, (float*)transform);
        glUniform3f(color_loc, 1.0f, 1.0f, 1.0f); // White background
        glBindVertexArray(gl->ui->ui_vao);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        
        // Draw border
        glm_mat4_identity(transform);
        glm_translate(transform, (vec3){x_norm + w_norm, y_norm - h_norm, 0.0f});
        glm_scale(transform, (vec3){w_norm + 0.002f, h_norm + 0.002f, 1.0f});
        glUniformMatrix4fv(transform_loc, 1, GL_FALSE, (float*)transform);
        glUniform3f(color_loc, focused ? 0.0f : 0.5f, focused ? 0.5f : 0.5f, focused ? 1.0f : 0.5f);
        glDrawArrays(GL_TRIANGLES, 0, 6);
    }
    
    bool isPointInside(float px, float py) {
        return px >= x && px <= x + width && py >= y && py <= y + height;
    }
};

class NanoGUI {
public:
    std::vector<SimpleButton> buttons;
    std::vector<SimpleTextBox> textboxes;
    
    void initialize() {
        // Render mode buttons
        buttons.push_back(SimpleButton(20, 20, 80, 30, "Line"));
        buttons.push_back(SimpleButton(110, 20, 80, 30, "Solid"));
        buttons.push_back(SimpleButton(200, 20, 80, 30, "Shiny"));
        
        // Background buttons
        buttons.push_back(SimpleButton(20, 60, 80, 30, "Black"));
        buttons.push_back(SimpleButton(110, 60, 80, 30, "Gradient"));
        buttons.push_back(SimpleButton(200, 60, 80, 30, "Blue"));
        
        // Parameter input
        textboxes.push_back(SimpleTextBox(20, 100, 200, 30, "0.1 0.0 0.0 0.0 1.0"));
        buttons.push_back(SimpleButton(230, 100, 50, 30, "OK"));
        
        // Exit button
        buttons.push_back(SimpleButton(SRC_WIDTH - 70, 20, 50, 30, "Exit"));
        
        // Set defaults
        buttons[0].active = true; // Line
        buttons[3].active = true; // Black
    }
    
    void render(t_gl *gl) {
        // Save OpenGL state
        GLint current_program;
        glGetIntegerv(GL_CURRENT_PROGRAM, &current_program);
        
        // Enable blending
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glDisable(GL_DEPTH_TEST);
        
        // Use UI shader
        glUseProgram(gl->ui->ui_shader_program);
        glBindVertexArray(gl->ui->ui_vao);
        
        // Render textboxes
        for (auto& textbox : textboxes) {
            textbox.render(gl);
        }
        
        // Render buttons
        for (auto& button : buttons) {
            button.render(gl);
        }
        
        // Restore state
        glEnable(GL_DEPTH_TEST);
        glDisable(GL_BLEND);
        glUseProgram(current_program);
    }
    
    void handleMouse(float x, float y) {
        // Update hover states
        for (auto& button : buttons) {
            button.hovered = button.isPointInside(x, y);
        }
        for (auto& textbox : textboxes) {
            textbox.focused = textbox.isPointInside(x, y);
        }
    }
    
    void handleClick(float x, float y) {
        // Handle button clicks
        for (size_t i = 0; i < buttons.size(); i++) {
            if (buttons[i].isPointInside(x, y)) {
                // Deactivate all buttons in the same group
                if (i < 3) { // Render mode buttons
                    for (int j = 0; j < 3; j++) {
                        buttons[j].active = false;
                    }
                } else if (i >= 3 && i < 6) { // Background buttons
                    for (int j = 3; j < 6; j++) {
                        buttons[j].active = false;
                    }
                }
                
                buttons[i].active = true;
                printf("Button clicked: %s\n", buttons[i].text.c_str());
                
                // Handle specific actions
                if (buttons[i].text == "Exit") {
                    glfwSetWindowShouldClose(glfwGetCurrentContext(), GLFW_TRUE);
                }
                break;
            }
        }
    }
};

static NanoGUI* g_nanogui = nullptr;

void init_nanogui(t_gl *gl) {
    (void)gl;
    g_nanogui = new NanoGUI();
    g_nanogui->initialize();
    printf("NanoGUI initialized with professional widgets\n");
}

void cleanup_nanogui() {
    if (g_nanogui) {
        delete g_nanogui;
        g_nanogui = nullptr;
    }
    printf("NanoGUI cleaned up\n");
}

void run_nanogui_loop() {
    if (g_nanogui) {
        g_nanogui->render(nullptr); // We'll get gl from global context
    }
}

void handle_nanogui_mouse(float x, float y) {
    if (g_nanogui) {
        g_nanogui->handleMouse(x, y);
    }
}

void handle_nanogui_click(float x, float y) {
    if (g_nanogui) {
        g_nanogui->handleClick(x, y);
    }
}
