#include "morphosis.h"

// GLFW stub implementations
static GLFWwindow dummy_window;
unsigned char glewExperimental = 0;

int glfwInit(void) {
    return 1; // Success
}

void glfwWindowHint(int hint, int value) {
    // Do nothing
}

GLFWwindow* glfwCreateWindow(int width, int height, const char* title, void* monitor, void* share) {
    return &dummy_window;
}

void glfwMakeContextCurrent(GLFWwindow* window) {
    // Do nothing
}

int glfwWindowShouldClose(GLFWwindow* window) {
    return 1; // Always close (for headless mode)
}

void glfwSetWindowShouldClose(GLFWwindow* window, int value) {
    // Do nothing
}

int glfwGetKey(GLFWwindow* window, int key) {
    return 0; // No key pressed
}

double glfwGetTime(void) {
    return 0.0;
}

void glfwSwapBuffers(GLFWwindow* window) {
    // Do nothing
}

void glfwPollEvents(void) {
    // Do nothing
}

void glfwTerminate(void) {
    // Do nothing
}

void glfwSetFramebufferSizeCallback(GLFWwindow* window, void (*callback)(GLFWwindow*, int, int)) {
    // Do nothing
}

// OpenGL stub implementations
GLuint glGetAttribLocation(GLuint program, const GLchar *name) {
    return 0;
}

void glVertexAttribPointer(GLuint index, GLint size, GLenum type, unsigned char normalized, int stride, const void *pointer) {
    // Do nothing
}

void glEnableVertexAttribArray(GLuint index) {
    // Do nothing
}

void glPolygonMode(GLenum face, GLenum mode) {
    // Do nothing
}

void glClearColor(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha) {
    // Do nothing
}

void glClear(GLuint mask) {
    // Do nothing
}

void glUniformMatrix4fv(GLint location, int count, unsigned char transpose, const GLfloat *value) {
    // Do nothing
}

void glDrawArrays(GLenum mode, GLint first, int count) {
    // Do nothing
}

void glViewport(GLint x, GLint y, int width, int height) {
    // Do nothing
}

void glEnable(GLenum cap) {
    // Do nothing
}

void glDeleteVertexArrays(int n, const GLuint *arrays) {
    // Do nothing
}

void glDeleteBuffers(int n, const GLuint *buffers) {
    // Do nothing
}

void glDeleteProgram(GLuint program) {
    // Do nothing
}

void glGenBuffers(int n, GLuint *buffers) {
    // Generate dummy buffer IDs
    for (int i = 0; i < n; i++) {
        buffers[i] = i + 1;
    }
}

void glBindBuffer(GLenum target, GLuint buffer) {
    // Do nothing
}

void glBufferData(GLenum target, GLsizeiptr size, const void *data, GLenum usage) {
    // Do nothing
}

void glGenVertexArrays(int n, GLuint *arrays) {
    // Generate dummy VAO IDs
    for (int i = 0; i < n; i++) {
        arrays[i] = i + 1;
    }
}

void glBindVertexArray(GLuint array) {
    // Do nothing
}

GLuint glCreateShader(GLenum type) {
    return 1; // Return dummy shader ID
}

void glShaderSource(GLuint shader, int count, const GLchar * const *string, const GLint *length) {
    // Do nothing
}

void glCompileShader(GLuint shader) {
    // Do nothing
}

void glGetShaderiv(GLuint shader, GLenum pname, GLint *params) {
    if (params) *params = 1; // Always success
}

void glGetShaderInfoLog(GLuint shader, int bufSize, int *length, GLchar *infoLog) {
    if (infoLog && bufSize > 0) infoLog[0] = '\0';
    if (length) *length = 0;
}

GLuint glCreateProgram(void) {
    return 1; // Return dummy program ID
}

void glAttachShader(GLuint program, GLuint shader) {
    // Do nothing
}

void glLinkProgram(GLuint program) {
    // Do nothing
}

void glGetProgramiv(GLuint program, GLenum pname, GLint *params) {
    if (params) *params = 1; // Always success
}

void glGetProgramInfoLog(GLuint program, int bufSize, int *length, GLchar *infoLog) {
    if (infoLog && bufSize > 0) infoLog[0] = '\0';
    if (length) *length = 0;
}

void glDeleteShader(GLuint shader) {
    // Do nothing
}

void glUseProgram(GLuint program) {
    // Do nothing
}

GLint glGetUniformLocation(GLuint program, const GLchar *name) {
    return 0; // Return dummy location
}

int glewInit(void) {
    return 0; // Success
}

// cglm stub implementations
void glm_mat4_identity(mat4 mat) {
    // Set identity matrix
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            mat[i][j] = (i == j) ? 1.0f : 0.0f;
        }
    }
}

void glm_rotate(mat4 m, float angle, vec3 axis) {
    // Stub rotation - do nothing for now
}

float glm_rad(float deg) {
    return deg * 3.14159265359f / 180.0f;
}

void glm_lookat(vec3 eye, vec3 center, vec3 up, mat4 dest) {
    // Simple stub - set identity matrix
    glm_mat4_identity(dest);
}

void glm_perspective(float fovy, float aspect, float nearZ, float farZ, mat4 dest) {
    // Simple stub - set identity matrix
    glm_mat4_identity(dest);
}

// OpenSSL SHA stub implementation
unsigned char *SHA256(const unsigned char *d, size_t n, unsigned char *md) {
    static unsigned char hash[32];
    // Simple stub hash - just use first 32 bytes or fill with pattern
    for (int i = 0; i < 32; i++) {
        hash[i] = (i < n) ? d[i % n] : (unsigned char)(i * 7);
    }
    return hash;
}
