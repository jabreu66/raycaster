#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <iostream>

float px, py;

const int screen_size = 2; // b/c in openGL the screen goes from -1 to 1 

const int map_rows = 10;
const int map_cols = 10;

int map[10][10] = {
    {1,0,0,0,0,0,0,0,0,1},
    {1,1,1,0,0,0,0,0,0,1},
    {1,1,1,0,0,1,1,1,1,1},
    {1,1,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,1,1},
    {1,0,0,0,1,0,1,1,1,1},
    {1,0,0,0,0,0,0,0,0,1},
    {1,0,1,1,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,1}
};

void drawTile(float x1, float x2, float y1, float y2)
{
    glBegin(GL_QUADS);
    glVertex2f(x1, y1);
    glVertex2f(x1, y2);
    glVertex2f(x2, y2);
    glVertex2f(x2, y1);
    glEnd();
    
}

void drawMap()
{
    int tile_width = screen_size / map_rows;
    int tile_height = screen_size / map_cols;

    int x1 = -1, y1 = -1;

    for(int x = 0; x < map_rows; x++){
        for(int y = 0; y < map_cols; y++)
        {
            x1 = 
        }
    }
}


void createPlayer()
{
    glColor3f(1,1,0);
    glPointSize(8);
    glBegin(GL_POINTS);
    glVertex2f(px, py);
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
    float speed = 0.001f;
    while(!glfwWindowShouldClose(window))
    {
        if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        {
            px -= speed;
        }
        if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        {
            px += speed;
        }
        if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        {
            py += speed;
        }
        if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        {
            py -= speed;
        }

        glClear(GL_COLOR_BUFFER_BIT);
        createPlayer();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();

    return 0;
}