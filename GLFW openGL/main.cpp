#include <iostream>

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include<algorithm>
#include <glm/glm.hpp>
#include "ShaderLoader.h"
#include <unistd.h>

using namespace std;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);
void load_obj(const char* filename, vector<glm::vec3> &vertices,std::vector<unsigned int> &vertexIndices);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

const char *vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x*0.4, aPos.y*0.4, aPos.z*0.4, 1.0);\n"
"}\0";
const char *fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
"}\n\0";

int main()
{
    // glfw: initialize and configure
    // ------------------------------
    char path[1024];
    getcwd(path, sizeof(path));
    std::string current_working_dir(path);
    std::cout << current_working_dir << std::endl;
    
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
#endif
    
    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    
    // glew: load all OpenGL function pointers
    // ---------------------------------------
    glewExperimental = GL_TRUE;
    if (GLEW_OK != glewInit())
    {
        std::cout << "Failed to initialize GLew" << std::endl;
        return -1;
    }
    
    //enable deep test

    
    //shader
    Shader sd("../../../source/vShader.vs","../../../source/fShader.fs");
    
    //set VAO VBO
    //---------
    
    float vertices[] = {
        // front
        -1.0, -1.0,  1.0,
        1.0, -1.0,  1.0,
        1.0,  1.0,  1.0,
        -1.0,  1.0,  1.0,
        // back
        -1.0, -1.0, -1.0,
        1.0, -1.0, -1.0,
        1.0,  1.0, -1.0,
        -1.0,  1.0, -1.0,
    };
    
    unsigned int  indices[] = {
        // front
        0, 1, 2,
        2, 3, 0,
        // right
        1, 5, 6,
        6, 2, 1,
        // back
        7, 6, 5,
        5, 4, 7,
        // left
        4, 0, 3,
        3, 7, 4,
        // bottom
        4, 5, 1,
        1, 0, 4,
        // top
        3, 2, 6,
        6, 7, 3,
    };
    
    vector< glm::vec3 > vertices_1;
    vector<unsigned int> indices_1;
     load_obj("../../../source/Lamborghini_Aventador.obj", vertices_1, indices_1);
    
    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(VAO);
    
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices_1.size()*sizeof(vertices_1),&vertices_1[0], GL_STATIC_DRAW);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,indices_1.size()*sizeof(indices_1),&indices_1[0], GL_STATIC_DRAW);
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_TRUE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glEnable(GL_DEPTH_TEST);
    
    
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    
    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // input
        // -----
        processInput(window);
        
        // render
        // ------
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        sd.use();
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, indices_1.size(), GL_UNSIGNED_INT, 0);
        
        
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    
    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
//------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}



void load_obj(const char* filename, vector<glm::vec3> &vertices,std::vector<unsigned int> &vertexIndices)
{
    std::vector< glm::vec3 > temp_vertices;
    std::vector< glm::vec2 > temp_uvs;
    std::vector< glm::vec3 > temp_normals;
    
    ifstream in;
    string newline,word;
    in.open(filename);
    if(!in){
        printf("Impossible to open the file !\n");
        return ;
    }
    while(getline(in,newline)){

        istringstream iss(newline);
        iss>>word;
        if(word =="v"){
            glm::vec3 vertex;
            iss>>vertex.x>>vertex.y>>vertex.z;
            vertices.push_back(vertex);
        }
        else if(word =="f"){
            replace(newline.begin(),newline.end(),'/',' ');
            istringstream f_is(newline);
            unsigned int vertexindice = 0;
            int count = 0;
            unsigned int a[4] ;
            string temp;
            f_is>>temp;
            while(f_is>>vertexindice){
                if(count%3 == 0){
                    a[count/3] = vertexindice;
                    
                }
                count++;
            }
            vertexIndices.push_back(a[0]);
            vertexIndices.push_back(a[1]);
            vertexIndices.push_back(a[2]);
            if(count>=9){
                vertexIndices.push_back(a[0]);
                vertexIndices.push_back(a[2]);
                vertexIndices.push_back(a[3]);
            }
            

        }
        
    }
    in.close();

    
}
