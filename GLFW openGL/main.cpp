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
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>
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
    
    glewExperimental = GL_TRUE;
    if (GLEW_OK != glewInit())
    {
        std::cout << "Failed to initialize GLew" << std::endl;
        return -1;
    }
    
    //shader
    Shader sd("../../../source/rotate.vs","../../../source/fShader.fs");
    Shader sd_cube("../../../source/cube.vs","../../../source/cube.fs");
    
    glm::vec3 cubePositions[] = {
        glm::vec3( 300.0f,  300.0f,  300.0f),
        glm::vec3( 300.0f,  300.0f, 300.0f),
        glm::vec3(300.0f,   300.0f, 300.0f),
        glm::vec3(300.0f,   300.0f, 300.0f),
        glm::vec3(0.0f,   0.0f, 0.0f)

    };
    //set VAO VBO
    //---------
    vector< glm::vec3 > vertices_1;
    vector<unsigned int> indices_1;
    load_obj("../../../source/Lamborghini_Aventador.obj", vertices_1, indices_1);
    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices_1.size()*sizeof(glm::vec3), &vertices_1[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,indices_1.size()*sizeof(unsigned int), &indices_1[0], GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_TRUE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
    
    
    vector< glm::vec3 > vertices_cube;
    vector<unsigned int> indices_cube;
    load_obj("../../../source/cube.obj", vertices_cube, indices_cube);
    unsigned int VBO_cube, VAO_cube, EBO_cube;
    glGenVertexArrays(1, &VAO_cube);
    glGenBuffers(1, &VBO_cube);
    glGenBuffers(1, &EBO_cube);
    glBindVertexArray(VAO_cube);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_cube);
    glBufferData(GL_ARRAY_BUFFER, vertices_cube.size()*sizeof(glm::vec3), &vertices_cube[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_cube);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,indices_cube.size()*sizeof(unsigned int), &indices_cube[0], GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_TRUE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
    

    
    
    
    
    
    
    
    
    
    
    glEnable(GL_DEPTH_TEST);
    sd.use();
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f,1500.0f);
    sd.setMat4("projection", projection);
    cout << glm::to_string(projection);
    
    sd_cube.use();
    sd_cube.setMat4("projection", projection);
    cout << glm::to_string(projection);
    
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    
    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        
        processInput(window);
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        
        sd.use();
        glm::mat4 model = glm::mat4(1.0);
        sd.setMat4("model", model);
        glm::mat4 model1 = model = glm::translate(model, glm::vec3(500.0f, 0.0f, 0.0f));
        glm::mat4 view;
        float radius = 1000.0f;
        float camX   = sin(glfwGetTime()) * radius;
        float camZ   = cos(glfwGetTime()) * radius;
        view = glm::lookAt(glm::vec3(camX, 100.0f, camZ), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        sd.setMat4("view", view);
        sd_cube.use();
        sd_cube.setMat4("view", view);
        
        sd.use();
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glDrawElements(GL_TRIANGLES, indices_1.size(), GL_UNSIGNED_INT, 0);
        
        sd.use();
        sd.setMat4("model", model1);
        
        
        
        glBindVertexArray(VAO_cube);
       // glBindBuffer(GL_ARRAY_BUFFER, VBO_cube);
        sd_cube.use();
        for(int i = 0; i<5;i++){
        model = glm::translate(model, cubePositions[i]);
        sd_cube.setMat4("model", model);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glDrawElements(GL_TRIANGLES, indices_1.size(), GL_UNSIGNED_INT, 0);            
        }
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    
    
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
