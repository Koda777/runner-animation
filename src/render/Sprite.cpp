#include "Sprite.h"

#ifdef __APPLE__
    #include <OpenGL/gl3.h>
#else
    #include <GL/gl.h>
#endif
#include <GLFW/glfw3.h>

#define STB_IMAGE_IMPLEMENTATION
#include "../stb_image.h"

#include <iostream>

/* ===================== SHADERS ===================== */

static const char* vertexShaderSrc = R"(#version 150 core
in vec2 aPos;
in vec2 aTex;

out vec2 vTex;

void main() {
    vTex = aTex;
    gl_Position = vec4(aPos, 0.0, 1.0);
}
)";

static const char* fragmentShaderSrc = R"(#version 150 core
in vec2 vTex;
out vec4 FragColor;

uniform sampler2D uTexture;

void main() {
    FragColor = texture(uTexture, vTex);
}
)";

/* ===================== UTILS ===================== */

static GLuint compileShader(GLenum type, const char* src) {
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &src, nullptr);
    glCompileShader(shader);

    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char log[512];
        glGetShaderInfoLog(shader, 512, nullptr, log);
        std::cerr << "Shader compile error:\n" << log << std::endl;
    }
    return shader;
}

static GLuint createProgram() {
    GLuint vs = compileShader(GL_VERTEX_SHADER, vertexShaderSrc);
    GLuint fs = compileShader(GL_FRAGMENT_SHADER, fragmentShaderSrc);

    GLuint program = glCreateProgram();
    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glBindFragDataLocation(program, 0, "FragColor");
    glLinkProgram(program);

    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;
}

/* ===================== SPRITE ===================== */

Sprite::Sprite(const char* path)
    : textureID(0), vao(0), vbo(0), shader(0)
{
    /* ---------- TEXTURE ---------- */
    int w, h, c;
    unsigned char* data = stbi_load(path, &w, &h, &c, 4);
    if (!data) {
        std::cerr << "Failed to load texture: " << path << std::endl;
        return;
    }

    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        GL_RGBA8,
        w,
        h,
        0,
        GL_RGBA,
        GL_UNSIGNED_BYTE,
        data
    );

    stbi_image_free(data);

    /* ---------- GEOMETRY ---------- */
    float vertices[] = {
        // pos      // tex
        -1.f, -1.f, 0.f, 1.f,
         1.f, -1.f, 1.f, 1.f,
         1.f,  1.f, 1.f, 0.f,
        -1.f,  1.f, 0.f, 0.f
    };

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    /* ---------- SHADER ---------- */
    shader = createProgram();
    glUseProgram(shader);

    GLint posLoc = glGetAttribLocation(shader, "aPos");
    GLint texLoc = glGetAttribLocation(shader, "aTex");

    glEnableVertexAttribArray(posLoc);
    glVertexAttribPointer(posLoc, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);

    glEnableVertexAttribArray(texLoc);
    glVertexAttribPointer(texLoc, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

    glBindVertexArray(0);
}

Sprite::~Sprite() {
    glDeleteTextures(1, &textureID);
    glDeleteBuffers(1, &vbo);
    glDeleteVertexArrays(1, &vao);
    glDeleteProgram(shader);
}

void Sprite::draw() {
    glUseProgram(shader);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glUniform1i(glGetUniformLocation(shader, "uTexture"), 0);

    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    glBindVertexArray(0);
}

/* ===================== POSITION ===================== */

Sprite::Position Sprite::getPosition() const {
    return position;
}

void Sprite::setPosition(int x, int y) {
    position.x = x;
    position.y = y;
}
