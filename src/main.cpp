#include <stdio.h>

#include "include/glad/glad.h"
#include "glad.c"
#include "GLFW/glfw3.h"
#define STB_IMAGE_IMPLEMENTATION
#include "Image.h"
#include "shader.eg"
typedef unsigned int UI;

#include "glm/common.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

UI Program;

struct DrawData
{
    UI VAO;
    UI VBO;
    UI Tex;
    float ImgWidth;
    float ImgHeight;
};

DrawData SpriteRec(const char *texPath)
{
    DrawData Result = {};

    glGenVertexArrays(1, &Result.VAO);
    glBindVertexArray(Result.VAO);

    glGenBuffers(1, &Result.VBO);
    glBindBuffer(GL_ARRAY_BUFFER, Result.VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 24, 0, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, 0, 16, 0);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, 0, 16, (void *)8);

    glGenTextures(1, &Result.Tex);
    glBindTexture(GL_TEXTURE_2D, Result.Tex);

    int x, y, z;
    // stbi_set_flip_vertically_on_load(true);
    unsigned char *data = stbi_load(texPath, &x, &y, &z, 0);
    Result.ImgHeight = y;
    Result.ImgWidth = x;
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, x, y, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        printf("Failed to load spite");
    };
    return Result;
}

void DrawSprite(DrawData a, float index, float heightIndex, int transX, int transY)
{
    float Xmin = (index * 130.0f) / a.ImgWidth;
    float Ymin = (heightIndex * 200.0f) / a.ImgHeight;
    float Xmax = 130.0f / a.ImgWidth;
    float Ymax = 200.0f / a.ImgHeight;

    float Rectangle[] =
        {
            0,
            0,
            Xmin,
            Ymin,
            0,
            1,
            Xmin,
            Ymax + Ymin,
            1,
            1,
            Xmax + Xmin,
            Ymax + Ymin,
            1,
            1,
            Xmax + Xmin,
            Ymax + Ymin,
            1,
            0,
            Xmax + Xmin,
            Ymin,
            0,
            0,
            Xmin,
            Ymin,
        };

    glBindBuffer(GL_ARRAY_BUFFER, a.VBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(Rectangle), Rectangle);

    glUniform3f(glGetUniformLocation(Program, "objColor"), 1, 1, 1);
    glm::mat4 View(1.0f);
    View = glm::translate(View, glm::vec3(transX, transY, 0));
    View = glm::scale(View, glm::vec3(100, 100, 0));
    glUniformMatrix4fv(glGetUniformLocation(Program, "View"), 1, 0, glm::value_ptr(View));

    glBindVertexArray(a.VAO);

    glBindTexture(GL_TEXTURE_2D, a.Tex);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}

DrawData BackImg(const char *texPath)
{
    DrawData Result = {};
    float Rectangle[] =
        {
            0,
            0,
            0,
            0,
            0,
            1,
            0,
            1,
            1,
            1,
            1,
            1,
            1,
            1,
            1,
            1,
            1,
            0,
            1,
            0,
            0,
            0,
            0,
            0,
        };
    glGenVertexArrays(1, &Result.VAO);
    glBindVertexArray(Result.VAO);

    glGenBuffers(1, &Result.VBO);
    glBindBuffer(GL_ARRAY_BUFFER, Result.VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 24, Rectangle, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, 0, 16, 0);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, 0, 16, (void *)8);

    glGenTextures(1, &Result.Tex);
    glBindTexture(GL_TEXTURE_2D, Result.Tex);
    int x, y, z;
    // stbi_set_flip_vertically_on_load(true);
    unsigned char *data = stbi_load(texPath, &x, &y, &z, 0);
    Result.ImgHeight = y;
    Result.ImgWidth = x;

    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, x, y, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        printf("Failed to load BackGround Image");
    };
    return Result;
}
void BGimg(DrawData a, float transX, float transY, int scaleX, int scaleY, float colorX, float colorY, float colorZ)
{
    glm::mat4 View(1.0f);
    View = glm::translate(View, glm::vec3(transX, transY, 0));
    View = glm::scale(View, glm::vec3(scaleX, scaleY, 0));
    glUniformMatrix4fv(glGetUniformLocation(Program, "View"), 1, 0, glm::value_ptr(View));

    glUniform3f(glGetUniformLocation(Program, "objColor"), colorX, colorY, colorZ);
    glBindVertexArray(a.VAO);
    glBindBuffer(GL_ARRAY_BUFFER, a.VBO);
    glBindTexture(GL_TEXTURE_2D, a.Tex);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}

DrawData ScoreRec()
{
    DrawData Result = {};

    glGenVertexArrays(1, &Result.VAO);
    glBindVertexArray(Result.VAO);

    glGenBuffers(1, &Result.VBO);
    glBindBuffer(GL_ARRAY_BUFFER, Result.VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 24, 0, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, 0, 16, 0);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, 0, 16, (void *)8);

    glGenTextures(1, &Result.Tex);
    glBindTexture(GL_TEXTURE_2D, Result.Tex);

    int x, y, z;
    // stbi_set_flip_vertically_on_load(true);
    unsigned char *data = stbi_load("../a.png", &x, &y, &z, 0);
    Result.ImgHeight = y;
    Result.ImgWidth = x;
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, x, y, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        printf("Failed to load score sprite");
    };
    return Result;
}

void DrawScore(DrawData a, float index, float heightIndex, int transX, int transY)
{
    float Xmin = (index * 20.0f) / a.ImgWidth;
    float Ymin = (heightIndex * 20.0f) / a.ImgHeight;
    float Xmax = 20.0f / a.ImgWidth;
    float Ymax = 20.0f / a.ImgHeight;

    float Rectangle[] =
        {
            0,
            0,
            Xmin,
            Ymin,
            0,
            1,
            Xmin,
            Ymax + Ymin,
            1,
            1,
            Xmax + Xmin,
            Ymax + Ymin,
            1,
            1,
            Xmax + Xmin,
            Ymax + Ymin,
            1,
            0,
            Xmax + Xmin,
            Ymin,
            0,
            0,
            Xmin,
            Ymin,
        };

    glBindBuffer(GL_ARRAY_BUFFER, a.VBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(Rectangle), Rectangle);

    glUniform3f(glGetUniformLocation(Program, "objColor"), 1, 1, 1);
    glm::mat4 View(1.0f);
    View = glm::translate(View, glm::vec3(transX, transY, 0));
    View = glm::scale(View, glm::vec3(100, 100, 0));
    glUniformMatrix4fv(glGetUniformLocation(Program, "View"), 1, 0, glm::value_ptr(View));

    glBindVertexArray(a.VAO);

    glBindTexture(GL_TEXTURE_2D, a.Tex);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}


int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *window = glfwCreateWindow(800, 600, "Dino", 0, 0);
    glfwMakeContextCurrent(window);
    gladLoadGL();

    UI VS = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(VS, 1, &vertexShaderSource, 0);
    glCompileShader(VS);

    char Check[1000];

    glGetShaderInfoLog(VS, 1000, 0, Check);
    printf("%s", Check);

    UI FS = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(FS, 1, &fragmentShaderSource, 0);
    glCompileShader(FS);

    glGetShaderInfoLog(FS, 1000, 0, Check);
    printf("%s", Check);

    Program = glCreateProgram();
    glAttachShader(Program, VS);
    glAttachShader(Program, FS);
    glLinkProgram(Program);
    glUseProgram(Program);

    glGetProgramInfoLog(Program, 1000, 0, Check);
    printf("%s", Check);

    glm::mat4 Proj(1.0f);
    Proj = glm::ortho(0.0f, 800.0f, 600.0f, 0.0f);
    glUniformMatrix4fv(glGetUniformLocation(Program, "Proj"), 1, 0, glm::value_ptr(Proj));

    DrawData a = SpriteRec("../new.png");

    DrawData Block = BackImg("../block.png");
    DrawData Background = BackImg("../i.png");

    DrawData ScoreBox = ScoreRec();

    float index = 0;
    float HeightIndex = 0;
    float transX = 0;
    float transY = 500;

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    int ScoreX = 1;
    int ScoreY = 1;
    int ScoreZ = 1;
    float blockX = 500;
    int interval = 3;

    bool playAgain = false;

    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT);
        //  glEnable(GL_DEPTH_TEST);
        glClearColor(0, 0, 0, 1);

        glfwSwapInterval(interval);

        BGimg(Background, 0, 0, 800, 600, 1, 1, 1);
        if (playAgain == false)
        {
            DrawScore(ScoreBox, 6, 3, 50, 50);
            DrawScore(ScoreBox, 5, 2, 125, 50);
            DrawScore(ScoreBox, 4, 5, 200, 50);
            DrawScore(ScoreBox, 5, 3, 275, 50);
            DrawScore(ScoreBox, 7, 2, 350, 50);

            for (int i = 0; i < 100; i += 1)
            {
                float randn = rand() % 3000;
                BGimg(Block, i * 300 + blockX, 500, 80, 80, 1, 0, 0);

                if (transX == i * 300 + blockX && transY >= 420)
                {
                    printf("Out\n");
                    playAgain = true;
                }
                else
                {
                    DrawScore(ScoreBox, ScoreX, 1, 425, 50);
                    DrawScore(ScoreBox, ScoreY, 1, 500, 50);
                    DrawScore(ScoreBox, ScoreZ, 1, 575, 50);
                }
            }

            DrawSprite(a, index, HeightIndex, transX, transY);

            ScoreZ += 1;
            if (ScoreZ > 10)
            {
                ScoreZ = 1;
                ScoreY += 1;
                printf("ScoreZ:%d\n", ScoreZ);
                if (ScoreY > 10)
                {
                    ScoreY = 1;
                    ScoreX += 1;
                    printf("ScoreY:%d\n", ScoreY);
                    if (ScoreX > 10)
                    {
                        ScoreX = 1;
                        printf("ScoreX:%d\n", ScoreX);
                    }
                }
            }

            if (index < 6)
            {
                index += 1.0f;
                blockX -= 10.0f;
            }
            else
            {
                index = 0;
                HeightIndex = 0;
            }

            if(glfwGetKey(window,GLFW_KEY_SPACE) == GLFW_PRESS)
            {
                if(transY == 500)
                {
                    transY -= 150; 
                } 
                //transX += 10;
                HeightIndex = 2; 
            }
            else
            {
                if(transY < 500) {
                    transY += 15;
                    HeightIndex = 3;
                } else {
                    transY = 500;
                }

                HeightIndex = 0;
            }
        }
        else
        {
            DrawScore(ScoreBox, 9, 2, 250, 250);
            DrawScore(ScoreBox, 3, 2, 325, 250);
            DrawScore(ScoreBox, 0, 3, 410, 250);
            DrawScore(ScoreBox, 7, 2, 495, 250);

            DrawScore(ScoreBox, 2, 3, 250, 350);
            DrawScore(ScoreBox, 9, 3, 325, 350);
            DrawScore(ScoreBox, 7, 2, 400, 350);
            DrawScore(ScoreBox, 5, 3, 475, 350);
            DrawScore(ScoreBox, 6, 3, 50, 50);
            DrawScore(ScoreBox, 5, 2, 125, 50);
            DrawScore(ScoreBox, 4, 5, 200, 50);
            DrawScore(ScoreBox, 5, 3, 275, 50);
            DrawScore(ScoreBox, 7, 2, 350, 50);
            DrawScore(ScoreBox, ScoreX, 1, 425, 50);
            DrawScore(ScoreBox, ScoreY, 1, 500, 50);
            DrawScore(ScoreBox, ScoreZ, 1, 575, 50);

            if (glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS)
            {
                playAgain = false;
                ScoreX = 1;
                ScoreY = 1;
                ScoreZ = 1;
                blockX = 500;
            }
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwTerminate();
    return 07;
}