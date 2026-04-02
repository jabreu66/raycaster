#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <iostream>

float px, py;

void createPlayer()
{
    glColor3f(1,1,0);
    glPointSize(8);
    glBegin(GL_POINTS);
    glVertex2i(px, py);
    glEnd();
}


void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
}

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);

    GLFWwindow* window = glfwCreateWindow(800, 600, "Raycaster", NULL, NULL);
    if(window == NULL) {
        std::cout << "failed to create GLFW window " << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    int version = gladLoadGL(glfwGetProcAddress);

    if (version == 0)
    {
        std::cout << "failed to initialize GLAD" << std::endl;
        return -1;
    }
    
    glViewport(0, 0, 800, 600);

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    px = 0.0f, py = 0.0f;
    while(!glfwWindowShouldClose(window))
    {
        if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        {
            px -= 0.1f;
        }
        if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        {
            px += 0.1f;
        }
        if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        {
            py -= 0.1f;
        }
        if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        {
            py += 0.1f;
        }

        glClear(GL_COLOR_BUFFER_BIT);
        createPlayer();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();

    return 0;
}