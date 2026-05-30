#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>

#define PI 3.14159
#define SCREEN_HEIGHT 800
#define SCREEN_WIDTH 600
#define HORIZON SCREEN_HEIGHT/2.0f
#define NUM_RAYS 300

float px{0}, py{0} ;
float dirX{1}, dirY{0};
float newX{0}, newY{0};
float planeX{ 0.0f}, planeY{1.0f};

float rayEndX[NUM_RAYS];
float rayEndY[NUM_RAYS];


int playerRow = 4, playerCol = 5;

const float screen_size = 2.0f; // b/c in openGL the screen goes from -1 to 1 


const int map_rows = 10;
const int map_cols = 10;


float tile_width = screen_size / map_cols;
float tile_height = screen_size / map_rows;

int map[10][10] = {
    {1,1,1,1,1,1,1,1,1,1},
    {1,3,1,0,0,0,0,0,0,1},
    {2,2,2,0,0,3,3,3,3,1},
    {1,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,1,1},
    {1,0,0,0,4,0,2,2,2,1},
    {1,0,0,0,4,0,0,0,0,1},
    {1,0,2,2,4,0,0,0,0,1},
    {1,0,0,0,0,0,4,4,0,1},
    {1,1,1,1,1,1,1,1,1,1}
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

    // int x1 = -1, y1 = -1;

    for(int rows = 0; rows < map_rows; rows++){
        for(int cols = 0; cols < map_cols; cols++)
        {
           float x1 = -1.0f + (cols * tile_width);
           float y1 = 1.0f - (rows * tile_height);
           float x2 = x1 + tile_width;
           float y2 = y1 - tile_height;
           
           if(map[rows][cols] >= 1)
           {
                glColor3f(0.2f, 0.2f, 0.8f);
           }
           else
           {
                glColor3f(0.1f, 0.1f, 0.1f);
           }

           drawTile(x1, x2, y1, y2);

           glColor3f(0,0,0);

           glBegin(GL_LINE_LOOP);
           glVertex2f(x1, y1);
           glVertex2f(x1, y2);
           glVertex2f(x2, y2);
           glVertex2f(x2, y1);
           glEnd();

        }
    }
}


void drawPlayerEndpoint(float x_endpoint, float y_endpoint)
{
    glColor3f(1,1,0);

    glBegin(GL_LINES);
    glVertex2f(px, py);
    glVertex2f(x_endpoint, y_endpoint);
    glEnd();
}

void drawPlayerDirection()
{
    glColor3f(1,0,0);

    glBegin(GL_LINES);
    glVertex2f(px, py);
    glVertex2f(px + dirX * 0.08f, py + dirY * 0.08f);
    glEnd();
}

void createPlayer()
{
    glColor3f(1,1,0);
    glPointSize(10);
    glBegin(GL_POINTS);
    glVertex2f(px, py);
    glEnd();
}

int computePlayerCol(float px)
{
    int col = (px + 1)/ tile_width;
    return col;
}

int computePlayerRow(float py)
{
    int row = (1 - py)/ tile_height;
    return row;
}

bool is_collision(float px, float py)
{
    int row = computePlayerRow(py);
    int col = computePlayerCol(px);

    if(row < 0 || row >= map_rows || col < 0 || col >= map_cols)
    {
        return true;
    }

    if(map[row][col] > 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}

void floor_cast()
{
    for(int y = HORIZON + 1; y < SCREEN_HEIGHT; y++){
        float p = y - HORIZON; // number of horizontal lines down from the center
        float posZ =  HORIZON; //center of our screen (in terms of pixels)
        float row_dist = posZ/p; // dist of current row to the floor

        
        float rayDirX0 = dirX - planeX;
        float rayDirY0 = dirY - planeY;
        float rayDirX1 = dirX + planeX;
        float rayDirY1 = dirY + planeY; 

        float stepX = row_dist * (rayDirX1 - rayDirX0)/SCREEN_WIDTH;
        float stepY = row_dist * (rayDirY1 - rayDirY0)/SCREEN_WIDTH;

        // leftmost map coords
        float posX = px + row_dist * rayDirX0;
        float posY = py + row_dist * rayDirY0;

        for(int x = 0; x < SCREEN_WIDTH; x++){

            int row = int(posY)/tile_height;
            int col  = int(posX)/tile_width;

            if((row + col) % 2 == 0)
            {
                // std::cout << " true " << row << " " << col << std::endl;
                glColor3f(0.2f,0.2f,0.2f);
            }
            else
            {
                // std::cout << "false " << row << " " << col << std::endl;
                glColor3f(0.5f, 0.5f, 0.5f);
            }

            //convert pixel to screen coordinates

            float screen_x = -1.0f + 2.0f * x /SCREEN_WIDTH;
            float screen_y = 1.0f - 2.0f * y /SCREEN_HEIGHT;

            glBegin(GL_POINTS);
            glVertex2f(screen_x, screen_y);
            glEnd();

            posX += stepX;
            posY += stepY;
        }

    }
}

void roof_cast()
{
    for(int y = 0; y < HORIZON; y++){
        float p = HORIZON - y; 
        float posZ =  HORIZON; //center of our screen (in terms of pixels)
        float row_dist = posZ/p; // dist of current row to the floor

        
        float rayDirX0 = dirX - planeX;
        float rayDirY0 = dirY - planeY;
        float rayDirX1 = dirX + planeX;
        float rayDirY1 = dirY + planeY; 

        float stepX = row_dist * (rayDirX1 - rayDirX0)/SCREEN_WIDTH;
        float stepY = row_dist * (rayDirY1 - rayDirY0)/SCREEN_WIDTH;

        // leftmost map coords
        float posX = px + row_dist * rayDirX0;
        float posY = py + row_dist * rayDirY0;

        for(int x = 0; x < SCREEN_WIDTH; x++){

            int row = int(posY)/tile_height;
            int col  = int(posX)/tile_width;

            //  if((row + col) % 2 == 0)
            // {
            //     glColor3f(0.2f,0.2f,1.0f);
            // }
            // else
            // {
            //     glColor3f(1.0f, 1.0f, 1.0f);
            // }

             glColor3f(0.2f, 0.2f, 0.2f);
            //convert pixel to screen coordinates

            float screen_x = -1.0f + 2.0f * x /SCREEN_WIDTH;
            float screen_y = 1.0f - 2.0f * y /SCREEN_HEIGHT;

            glBegin(GL_POINTS);
            glVertex2f(screen_x, screen_y);
            glEnd();


            posX += stepX;
            posY += stepY;
        }

    }
}

void castRay(float px, float py, float dirX, float dirY, float column, float angle)
{
    float rayDirX = dirX, rayDirY = dirY;

    int p_column = computePlayerCol(px);
    int p_row = computePlayerRow(py);

    float deltaDistX = (rayDirX == 0) ? 1e30f : std::abs(tile_width/rayDirX); // make sure we don't div by 0
    float deltaDistY = (rayDirY == 0) ? 1e30f : std::abs(tile_height/rayDirY);

    float sideDistX = 0, sideDistY = 0;
    int x_step = 0, y_step = 0;
    int hit = 0;
    int side = 0;
    float dist_to_wall = 0;

    if(rayDirX < 0)
    {
        x_step = -1;
        float leftSide = -1.0f + (p_column * tile_width);
        float distToLeftSide = px - leftSide;
        sideDistX = distToLeftSide / std::abs(rayDirX); 
    }
    else if (rayDirX > 0)
    {
        x_step = 1;
        float rightSide = -1.0f + (p_column * tile_width) + tile_width;
        float distToRightSide = std::abs(px - rightSide);
        sideDistX = distToRightSide / std::abs(rayDirX); 
    }
    if(rayDirY < 0)
    {
        y_step = 1;
        float bottom = 1.0f - (p_row * tile_height + tile_height);
        float distToBottom = py - bottom;
        sideDistY = distToBottom / std::abs(rayDirY); 
    }
    else if(rayDirY > 0)
    {
        y_step = -1;
        float top = 1.0f - (p_row * tile_height);
        float distToTop = top - py;
        sideDistY = distToTop / std::abs(rayDirY); 
        
    }

    while(hit == 0)
    {
        if(sideDistX < sideDistY) // X wall (vertical line) is closer
        {
            sideDistX += deltaDistX;
            p_column += x_step;
            side = 0; // side 0 means vertical hit
            hit = (map[p_row][p_column] > 0) ? 1 : 0;
            if(hit)
            {
                dist_to_wall = sideDistX - deltaDistX;
            }
        }
        else
        {
            sideDistY += deltaDistY;
            p_row += y_step;
            side = 1; // side 1 means horizontal hit
            hit = (map[p_row][p_column] > 0) ? 1 : 0;
            if(hit)
            {
                dist_to_wall = sideDistY - deltaDistY ;
            }
        }
    }
    
    float x_endpoint = px + rayDirX * dist_to_wall;
    float y_endpoint = py + rayDirY * dist_to_wall;
  
    // drawPlayerEndpoint(x_endpoint, y_endpoint);
    rayEndX[(int)column] = x_endpoint;
    rayEndY[(int)column] = y_endpoint;

    float fisheye_dist = dist_to_wall * cos(angle);
    float wall_height = tile_width / fisheye_dist; // further the wall, the smaller it appears

    float column_slice = 2.0f / (float)NUM_RAYS; // my screen size / amount of rays I'm shooting out

    float x1 = -1.0f + column_slice * column;
    float x2 = x1 + column_slice;
    float y1 = -wall_height / 2.0f; // come back to this
    float y2 = wall_height / 2.0f;

    if(y1 < -1.0f) y1 = -1.0f;
    if(y2 > 1.0f) y2 = 1.0f;

    int color = map[p_row][p_column];
    float r = 0, g = 0, b = 0;

    switch(color)
    {
        case 1:
            r = 1, g = 0, b = 0;
            break;
        case 2:
             r = 0, g = 1, b = 0;
            break;
        case 3:
             r = 0, g = 0, b = 1;
             break;
        case 4:
             r = 1, g = 1, b = 0;
            break;
    }

    glColor3f(r, g, b);


    if(side == 1)
    {
        r = r*0.6f;
        g = g*0.6f;
        b = b*0.6f;
        glColor3f(r,g,b);
    }
  

    glBegin(GL_QUADS);
    glVertex2f(x1, y1);
    glVertex2f(x1, y2);
    glVertex2f(x2, y2);
    glVertex2f(x2, y1);
    glEnd();


    // std::cout << "p_row: " << p_row << " p_col: " << p_column << std::endl; //" dist_to_wall: " << dist_to_wall << " x_endpoint: " << x_endpoint << " y_endpoint: " << y_endpoint << " side " << side << std::endl;

}

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height); // this is the part of the screen we want to draw on
}

int main()
{
    // std::cout << " horizon is " << HORIZON << std::endl;
    // std::cout << __cplusplus << std::endl;
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);

    GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Raycaster", NULL, NULL);
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
    float speed = 0.005f;

    px = -1.0f + (playerCol * tile_width) + tile_width / 2.0f;
    py = 1.0f - (playerRow * tile_height) - tile_height / 2.0f;
    float turn_speed = 0.05f;

    while(!glfwWindowShouldClose(window))
    {
        if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        {
            float oldDirX = dirX;
            dirX = oldDirX * cos(-turn_speed) - dirY * sin(-turn_speed);
            dirY = oldDirX * sin(-turn_speed) + dirY * cos(-turn_speed);

            float oldPlaneX = planeX;
            planeX = oldPlaneX * cos(-turn_speed) - planeY * sin(-turn_speed);
            planeY = oldPlaneX * sin(-turn_speed) + planeY * cos(-turn_speed);
        }
        if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        {
            float oldDirX = dirX;
            dirX = oldDirX * cos(turn_speed) - dirY * sin(turn_speed);
            dirY = oldDirX * sin(turn_speed) + dirY * cos(turn_speed);

            float oldPlaneX = planeX;
            planeX = oldPlaneX * cos(turn_speed) - planeY * sin(turn_speed);
            planeY = oldPlaneX * sin(turn_speed) + planeY * cos(turn_speed);
        }
        if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        {
            if(!is_collision(px + speed * dirX, py)){
                px += dirX*speed;
            }
            if(!is_collision(px, py + speed * dirY)){
                py += dirY * speed;
            }
        }
        if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        {
            if(!is_collision(px - speed * dirX, py)){
              px -= dirX * speed;
            }
            if(!is_collision(px, py - speed * dirY)){
              py -= dirY * speed;
            }
        }
            int r = computePlayerRow(py);
            int c = computePlayerCol(px);
            bool collided = is_collision(px, py);

            // std::cout << "row: " << r << " col " << c <<  " is collided " << collided << std::endl;
        glClear(GL_COLOR_BUFFER_BIT); // delete old frame's previous image, without this I'd get stackable frame stuff
        int height, width;
        glfwGetFramebufferSize(window, &width, &height);
        glViewport(0, 0, width, height);
        roof_cast();
        floor_cast();
        float fov = PI/2;
        for(int i = 0; i < NUM_RAYS; i++)
        {
            float angle = -fov/2 + fov * (i/(float)NUM_RAYS);
            float newXDir = dirX * cos(angle) - dirY * sin(angle);
            float newYDir = dirY * cos(angle) + dirX * sin(angle);
            castRay(px, py, newXDir, newYDir, i, angle);
        }
        int minimap_height, minimap_width;
        glfwGetFramebufferSize(window, &minimap_width, &minimap_height);
        glViewport(0, minimap_height-200,200, 200 );

        drawMap();
        createPlayer();
        drawPlayerDirection();
        // drawPlayerEndpoint();
        for(int i = 0; i < NUM_RAYS; i++)
        {
            drawPlayerEndpoint(rayEndX[i], rayEndY[i]);
        }        

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();

    return 0;
}