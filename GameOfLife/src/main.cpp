//
//  main.cpp
//  GameOfLife
//
//  Created by Matthew Everette on 9/22/22.
//

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <sstream>

int main() {
    GLFWwindow* window;

    if (!glfwInit())
        return -1;
    
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    window = glfwCreateWindow(640, 480, "Game of Life", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    
    if (glewInit() != GLEW_OK)
        return -1;
    
    std::cout << glGetString(GL_VERSION) << std::endl;
        
    unsigned int vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    
    unsigned int vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    
    float positions[] = {
        -0.5f, -0.5f, 0.0f,     1.0f, 0.0f, 0.0f,
         0.5f, -0.5f, 0.0f,     0.0f, 1.0f, 0.0f,
         0.0f,  0.5f, 0.0f,     0.0f, 0.0f, 1.0f
    };
    
    glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_STATIC_DRAW);
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*) (3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    
    unsigned int vs;
    vs = glCreateShader(GL_VERTEX_SHADER);
    
    std::ifstream vs_f("res/shaders/shader.vs");
    std::string vs_str;
    const char* vs_src;
    
    if (!vs_f.fail()) {
        std::stringstream ss;
        ss << vs_f.rdbuf();
        vs_str = ss.str();
    } else {
        std::cout << "Failed to parse vertex shader" << std::endl;
    }
    vs_src = vs_str.c_str();
    
    vs_f.close();
        
    glShaderSource(vs, 1, &vs_src, NULL);
    glCompileShader(vs);
    
    int result;
    glGetShaderiv(vs, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE) {
        char message[512];
        glGetShaderInfoLog(vs, 512, nullptr, message);
        std::cout << "Failed to compile vertex shader" << std::endl;
        std::cout << message << std::endl;
        glDeleteShader(vs);
    }
    
    unsigned int fs;
    fs = glCreateShader(GL_FRAGMENT_SHADER);
    
    std::ifstream fs_f("res/shaders/shader.fs");
    std::string fs_str;
    const char* fs_src;
    
    if (!fs_f.fail()) {
        std::stringstream ss;
        ss << fs_f.rdbuf();
        fs_str = ss.str();
    } else {
        std::cout << "Failed to parse fragment shader" << std::endl;
    }
    fs_src = fs_str.c_str();
        
    fs_f.close();
    
    glShaderSource(fs, 1, &fs_src, NULL);
    glCompileShader(fs);
    
    glGetShaderiv(fs, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE) {
        char message[512];
        glGetShaderInfoLog(fs, 512, nullptr, message);
        std::cout << "Failed to compile fragment shader" << std::endl;
        std::cout << message << std::endl;
        glDeleteShader(fs);
    }
    
    unsigned int program;
    program = glCreateProgram();
    
    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glValidateProgram(program);
    
    glGetProgramiv(program, GL_LINK_STATUS, &result);
    if (result == GL_FALSE) {
        char message[512];
        glGetProgramInfoLog(program, 512, nullptr, message);
        std::cout << "Failed to link program" << std::endl;
        std::cout << message << std::endl;
        glDeleteProgram(program);
    }
    
    glDeleteShader(vs);
    glDeleteShader(fs);
    
    glUseProgram(program);
    
    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers(window);

        glfwPollEvents();
    }
    
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
    glDeleteProgram(program);

    glfwTerminate();
    return 0;
}
