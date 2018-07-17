#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define GLEW_STATIC
// Нужно не забывать, что  GLEW обязательно необходимо подключать перед gl.h или glfw.h
#include <GL/glew.h>


//Потом подключаем GLFW. Эта библиотека будет делать всю магию управления окнами.
#include <GLFW/glfw3.h>
//#include <GLFW/glfw3native.h>

//На данном этапе нам не нужна эта библиотека, но она содержит математические функции и вскоре нам понадобится. Никакой магии в GLM нет, и если вам сильно хочется, вы можете использовать любую другую библиотеку по работе с матрицами и векторами. Мы подключаем «using namespace» для того, чтобы писать «vec3», а не «glm::vec3»
//#include <glm/glm.hpp>

#include "../my_head.h"

int cntr = 0;

void mouse_button_callback(GLFWwindow* win, int btn, int act, int mod_bts);
void error_callback(int error, const char* description);
void key_callback(GLFWwindow* win, int key, int scancode, int act, int mod_bts);

int main(){
    int hgt, wdt;

    if (!glfwInit()){
        puts("Failed to initialize GLFW");
        return(666);
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    GLFWwindow *my_win = glfwCreateWindow(1024, 768, "My title bois", NULL, NULL);
    if(!my_win){
        puts("No window! WTF????");
        glfwTerminate();
        return(-1);
    }
    printf("win: %p\n", my_win);

    glfwSetInputMode(my_win, GLFW_STICKY_MOUSE_BUTTONS, GLFW_TRUE);
    glfwSetInputMode(my_win, GLFW_STICKY_KEYS, GLFW_TRUE);

    glfwMakeContextCurrent(my_win);

    if (glewInit() != GLEW_OK){
        puts("Failed to initialize GLEW");
        return -1;
    }

    glfwGetFramebufferSize(my_win, &wdt, &hgt);
    glViewport(0, 0, wdt, hgt);

    glfwSetMouseButtonCallback(my_win, mouse_button_callback);
    glfwSetKeyCallback(my_win, key_callback);

    printf("GLFW_CURSOR: %d\n", glfwGetInputMode(my_win, GLFW_CURSOR));
    printf("GLFW_STICKY_KEYS: %d\n", glfwGetInputMode(my_win, GLFW_STICKY_KEYS));
    printf("GLFW_STICKY_MOUSE_BUTTONS: %d\n\n", glfwGetInputMode(my_win, GLFW_STICKY_MOUSE_BUTTONS));


    GLfloat vertices[] = {
    -0.5f, -0.5f, 0.0f,
     0.5f, -0.5f, 0.0f,
     0.0f,  0.5f, 0.0f
    };
    GLuint VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    GLuint vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);


    while(!glfwWindowShouldClose(my_win)){
        glfwPollEvents();

        glClearColor(0.4, 0.7, 0.5, 0.9);
        glClear(GL_COLOR_BUFFER_BIT);

        glfwSwapBuffers(my_win);
        if(glfwWindowShouldClose(my_win))
            puts("Cmon dude...");
    }
    puts("STOP RIGHT THERE!!!1");

    glfwTerminate();
}



void mouse_button_callback(GLFWwindow* win, int btn, int act, int mod_bts){
    if (btn == GLFW_MOUSE_BUTTON_LEFT && act == GLFW_RELEASE){
        puts("wow");
        glfwSetWindowShouldClose(win, GL_TRUE);
        return;
    }
}

void error_callback(int error, const char* description){
    puts(description);
}

void key_callback(GLFWwindow* win, int key, int scancode, int act, int mod_bts){
    if(act == GLFW_RELEASE)
        printf("%s: %d\n", glfwGetKeyName(key, scancode), ++cntr);
}

