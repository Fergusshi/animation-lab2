#include <iostream>
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <vector>
#include<algorithm>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>
#include <glm/gtx/euler_angles.hpp>
#include "ShaderLoader.h"
#include "Loader.hpp"
#include <unistd.h>
#include "KeyFrame.hpp"

using namespace std;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);


// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;


int main(int arg1, char ** arg2)
{
    /*
     1.Change mode (below this comments) to true to initial CatmullRom-Splines
       Change mode (below this comments) to false to initial B-Splines
     2.Change eulerMode to true to initial Euler Angle Mode
       Change eulerMode to false to initial Quaternion Mode
     3.You can change the value under the comment of  "-----------------" to see different views
     */
    
    //-----------------------------------------------------------------------
    bool mode = false;
    bool eulerMode = false;
    
    
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
    
    // glfw window creation
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    // glew initial
    glewExperimental = GL_TRUE;
    if (GLEW_OK != glewInit())
    {
        std::cout << "Failed to initialize GLew" << std::endl;
        return -1;
    }
    

    
    
    
    
    
    float B_splinesArray[16] =
    {
        -1.0 / 6,    3.0 / 6,    -3.0 / 6,   1.0 / 6,
        3.0 / 6,    -6.0 / 6,         0.0,   4.0 / 6,
        -3.0 / 6,    3.0 / 6,    3.0 / 6,    1.0 / 6,
        1.0 / 6,         0.0,        0.0,        0.0
    };
    
    float catmullRomArray[16] =
    {
        -0.5,   1,   -0.5,  0,
        1.5,    -2.5,   0,  1,
        -1.5,   2,  0.5,    0,
        0.5,    -0.5,   0,  0
    };
    
    glm::mat4 B_SplinesM = glm::make_mat4(B_splinesArray);
    glm::mat4 CatmullRomM = glm::make_mat4(catmullRomArray);
    glm::mat4 M;
    
    if(!mode){
        M = B_SplinesM;
    }else{
        M = CatmullRomM;
    }
    
    
    
    //shader Load
    Shader sd("../../../source/rotate.vs","../../../source/fShader.fs");
    Shader sd_cube("../../../source/cube.vs","../../../source/cube.fs");
    
    
    //set VAO VBO
    vector< glm::vec3 > vertices_1;
    vector<unsigned int> indices_1;
    vector< glm::vec3 > vertices_cube;
    vector<unsigned int> indices_cube;
    std::vector<KeyFrame> keyframeSet;
    Loader :: load_obj("../../../source/Lamborghini_Aventador.obj", vertices_1, indices_1);
    Loader :: load_obj("../../../source/cube.obj", vertices_cube, indices_cube);
    Loader :: load_KeyFrame("../../../source/animation.kf", keyframeSet);
    
    //Set VAO VBO for  Lamborghini
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
    
  //Set VAO VBO for  Cube as control point
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
    
    
    

    //SET Orthographic Projection Matrix  HERE
    //-----------------------------------------------------------------------
    sd.use();
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f,3000.0f);
    sd.setMat4("projection", projection);
    
    sd_cube.use();
    sd_cube.setMat4("projection", projection);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glEnable(GL_DEPTH_TEST);

    
    //Set timeSlowRate to make the animation moving slower
    //-----------------------------------------------------------------------
    float timeSlowRate = 3;
    float lastKeyEndingTime = 0;
    int frameIndex = 0;
    int totalIndex = keyframeSet.size()-3;
    glm :: mat4 interOrientation;
    glm :: mat4 interPosition;
    interPosition =glm :: transpose(glm :: mat4
                                    (glm::vec4(keyframeSet[frameIndex].position,0),
                                     glm::vec4(keyframeSet[frameIndex+1].position,0),
                                     glm::vec4(keyframeSet[frameIndex+2].position,0),
                                     glm::vec4(keyframeSet[frameIndex+3].position,0)));
   
    if(eulerMode){
        interOrientation = glm :: transpose(glm :: mat4(
                                            keyframeSet[frameIndex].getEuler(),
                                            keyframeSet[frameIndex+1].getEuler(),
                                            keyframeSet[frameIndex+2].getEuler(),
                                            keyframeSet[frameIndex+3].getEuler()));
    }else{
        interOrientation = glm :: transpose(glm :: mat4(
                                      keyframeSet[frameIndex].getQuaternion(),
                                      keyframeSet[frameIndex+1].getQuaternion(),
                                      keyframeSet[frameIndex+2].getQuaternion(),
                                      keyframeSet[frameIndex+3].getQuaternion()));
    }
    

    
    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        
        processInput(window);
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        float currentTime = glfwGetTime();
        float t = (currentTime - lastKeyEndingTime)/timeSlowRate;
        
        if(t>1.0){
            lastKeyEndingTime = floor(currentTime/timeSlowRate)*timeSlowRate;
            t = (currentTime - lastKeyEndingTime)/timeSlowRate;
            frameIndex++;
            if(frameIndex>=totalIndex){
                cout <<"the final frame is "<< frameIndex;
                break;
            }
            cout << "current keyframe is "<<frameIndex<<"\n";
            interPosition =glm :: transpose(glm :: mat4( glm::vec4(keyframeSet[frameIndex].position,0),
                                      glm::vec4(keyframeSet[frameIndex+1].position,0),
                                      glm::vec4(keyframeSet[frameIndex+2].position,0),
                                      glm::vec4(keyframeSet[frameIndex+3].position,0)));
            if(eulerMode){
               interOrientation = glm :: transpose(glm :: mat4(keyframeSet[frameIndex].getEuler(),
                                         keyframeSet[frameIndex+1].getEuler(),
                                         keyframeSet[frameIndex+2].getEuler(),
                                         keyframeSet[frameIndex+3].getEuler()));

            }else{
                interOrientation =glm :: transpose( glm ::mat4( keyframeSet[frameIndex].getQuaternion(),
                                keyframeSet[frameIndex+1].getQuaternion(),
                                keyframeSet[frameIndex+2].getQuaternion(),
                                keyframeSet[frameIndex+3].getQuaternion()));
              
            }
        }
        glm :: vec4 T(t*t*t,t*t,t,1);
        glm :: vec4 C = T*M;
        glm :: vec4 Qp = C*interPosition;
        glm :: vec4 Qo = C*interOrientation;
        glm :: vec3 position(Qp.x,Qp.y,Qp.z);
        if(!eulerMode){
         glm :: vec4 Qo = glm :: normalize(Qo);
        }
        glm :: mat4 rotate = KeyFrame :: getRotationMatrix(Qo, eulerMode);
        glm :: mat4 model2 = glm::mat4(1.0);
                    model2 = glm::translate(model2, position);
                    model2 = model2*rotate;


        

        sd.use();
        glm::mat4 model = glm::mat4(1.0);
        glm::mat4 model1 = glm::translate(model, glm::vec3(500.0f, 0.0f, 0.0f));
        glm::mat4 view;
        float radius = 2000.0f;
        //float camX   = sin(glfwGetTime()) * radius;
        //float camZ   = cos(glfwGetTime()) * radius;
        
        
        
        //SET CAMERA VIEW HERE
        //-----------------------------------------------------------------------
        
        view = glm::lookAt(glm::vec3(2000.0f, 100.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        sd.setMat4("view", view);
        sd_cube.use();
        sd_cube.setMat4("view", view);
        
        sd.use();
        sd.setMat4("model", model);
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
       // glDrawElements(GL_TRIANGLES, indices_1.size(), GL_UNSIGNED_INT, 0);
        
       // sd.use();
        //sd.setMat4("model", model1);
        //glDrawElements(GL_TRIANGLES, indices_1.size(), GL_UNSIGNED_INT, 0);
        
        sd.use();
        sd.setMat4("model", model2);
        glDrawElements(GL_TRIANGLES, indices_1.size(), GL_UNSIGNED_INT, 0);
        
        
        glBindVertexArray(VAO_cube);
        glBindBuffer(GL_ARRAY_BUFFER, VBO_cube);
        sd_cube.use();
        glm::mat4 model = glm::mat4(1.0);
        model = glm::translate(model, keyframeSet[i].position);
        model =model*keyframeSet[i].getThisRotationMatrix(eulerMode);
            
        sd_cube.use();
        sd_cube.setMat4("model", model);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glDrawElements(GL_TRIANGLES, indices_cube.size(), GL_UNSIGNED_INT, 0);
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




