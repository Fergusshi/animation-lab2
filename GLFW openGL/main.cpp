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
#include "Object.hpp"

using namespace std;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);


// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

glm::vec3 cameraPos   = glm::vec3(0.0f, 100.0f, 2000.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp    = glm::vec3(0.0f, 1.0f, 0.0f);

bool firstMouse = true;
float yaw   = -90.0f;    // yaw is initialized to -90.0 degrees since a yaw of 0.0 results in a direction vector pointing to the right so we initially rotate a bit to the left.
float pitch =  0.0f;
float lastX =  800.0f / 2.0;
float lastY =  600.0 / 2.0;
float fov   =  45.0f;



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
    bool eulerMode = true;
    
    
    // Frist Section
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
    
    //callbacks
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    
    // glew initial
    glewExperimental = GL_TRUE;
    if (GLEW_OK != glewInit())
    {
        std::cout << "Failed to initialize GLew" << std::endl;
        return -1;
    }
    

    
    
    
    //Second Section
    
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
    
    
    float tt = 0.1;
    glm :: vec4 TT(tt*tt*tt,tt*tt,tt,1);
    glm :: vec4 CC = TT*B_SplinesM;
   // cout << glm :: to_string(CC)<<"\n";
    
    if(mode){
        M = B_SplinesM;
    }else{
        M = CatmullRomM;
    }
    
    
    
    //Third Section
    
    //shader Load
    Shader sd("../../../source/rotate.vs","../../../source/fShader.fs");
    Shader sd_cube("../../../source/cube.vs","../../../source/cube.fs");
    Shader sd_center("../../../source/center.vs","../../../source/center.fs");
    Shader sd_body("../../../source/body.vs","../../../source/body.fs");
    
    //set VAO VBO
    vector< glm::vec3 > vertices_1;
    vector<unsigned int> indices_1;
    vector< glm::vec3 > vertices_cube;
    vector<unsigned int> indices_cube;
    vector< glm::vec3 > vertices_plane;
    vector<unsigned int> indices_plane;
    vector< glm::vec3 > vertices_body;
    vector<unsigned int> indices_body;
    std::vector<KeyFrame> keyframeSet;
    Loader :: load_obj("../../../source/tinker.obj", vertices_1, indices_1);
    Loader :: load_obj("../../../source/cube.obj", vertices_cube, indices_cube);
    Loader :: load_obj("../../../source/plane.obj", vertices_plane, indices_plane);
    Loader :: load_obj("../../../source/body.obj", vertices_body, indices_body);
    Loader :: load_KeyFrame("../../../source/animation.kf", keyframeSet);
    

    
    //Set VAO VBO for  leg
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
    
    unsigned int VBO_body, VAO_body, EBO_body;
    glGenVertexArrays(1, &VAO_body);
    glGenBuffers(1, &VBO_body);
    glGenBuffers(1, &EBO_body);
    glBindVertexArray(VAO_body);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_body);
    glBufferData(GL_ARRAY_BUFFER, vertices_body.size()*sizeof(glm::vec3), &vertices_body[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_body);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,indices_body.size()*sizeof(unsigned int), &indices_body[0], GL_STATIC_DRAW);
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
    
     //Set Plane
    unsigned int VBO_plane, VAO_plane, EBO_plane;
    glGenVertexArrays(1, &VAO_plane);
    glGenBuffers(1, &VBO_plane);
    glGenBuffers(1, &EBO_plane);
    glBindVertexArray(VAO_plane);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_plane);
    glBufferData(GL_ARRAY_BUFFER, vertices_plane.size()*sizeof(glm::vec3), &vertices_plane[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_plane);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,indices_plane.size()*sizeof(unsigned int), &indices_plane[0], GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_TRUE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
    

    //SET Orthographic Projection Matrix  HERE
    //-----------------------------------------------------------------------
    sd.use();
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f,30000.0f);
     //glm::mat4 projection = glm::ortho(-1000.0f, 1000.0f, -1000.0f, 1000.0f, 0.1f, 3000.0f);
    ;
    sd.setMat4("projection", projection);
    
    sd_cube.use();
    sd_cube.setMat4("projection", projection);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glEnable(GL_DEPTH_TEST);

    
    //Section Four
    //Set timeSlowRate to make the animation moving slower
    //-----------------------------------------------------------------------
    float timeSlowRate = 4;
    float lastKeyEndingTime = 0;
    int frameIndex = 0;
    int totalIndex = keyframeSet.size()-3;
    glm :: mat4 interOrientation;
    glm :: mat4 interPosition;
    
    
    
   
    float leg_len = 69.7f*4.0f;
    int steps=0;
    
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
            steps++;
            if(steps==3)
                steps=0;
        }
        
        glm::vec4 Qrotation=glm::vec4(-0.707,0.0,0.0,0.707);
        glm::mat4 model = glm::mat4(1.0);
        glm::mat4 modelcube = glm::mat4(1.0);
        glm::mat4 model_right_leg = glm::translate(model, glm::vec3(0.0f,-leg_len, -100.0f))* KeyFrame :: getRotationMatrix(Qrotation, 0);
        glm::mat4 model_left_leg =glm::translate(model, glm::vec3(0.0f,-leg_len, -100.0f))* KeyFrame :: getRotationMatrix(Qrotation, 0);
        glm::vec4 leg_forward=glm::vec4(-0.259,0.0,0.0,0.966);
        glm::vec4 leg_backward=glm::vec4(0.259,0.0,0.0,0.966);
        
        
        glm::vec4 leg_left_pos;
        glm::vec4 leg_right_pos;
        float stand =leg_len*0.9;
        float walk = 0.866f*leg_len*0.9;
        
        if(t<0.5){
            leg_left_pos = leg_forward*2.0f*t+leg_backward*2.0f*(0.5f-t);
            leg_right_pos = leg_backward*2.0f*t+leg_forward*2.0f*(0.5f-t);
        }else{
            leg_left_pos = leg_backward*2.0f*(t-0.5f)+leg_forward*2.0f*(1.0f-t);
            leg_right_pos = leg_forward*2.0f*(t-0.5f)+leg_backward*2.0f*(1.0f-t);
        }
        model_left_leg = glm::translate(model, glm::vec3(75.0f,40.0f, 50.0f))*KeyFrame :: getRotationMatrix(glm::normalize(leg_left_pos), 0)*model_left_leg;
        model_right_leg = glm::translate(model, glm::vec3(-125.0f,40.0f, 50.0f))*KeyFrame :: getRotationMatrix(glm::normalize(leg_right_pos), 0)*model_right_leg;
        
        

       // body tran
        float t0 = fmodf(t,0.25f);
        int temp = int(t/0.25f);
        float bodyheight;
        if(temp%2==0){
            bodyheight = t0/0.25f*stand+(1.0f-t0/0.25f)*walk;
        }else{
            bodyheight = t0/0.25f*walk+(1.0f-t0/0.25f)*stand;
        }
        std::cout<<t0<<" "<<temp<<" "<<bodyheight<<"\n";
        glm :: mat4 bodyMatrix = glm::translate(model, glm::vec3(0.0f,bodyheight, 2*(t+steps)*leg_len-1000.0));
        glm::mat4 bodyModel = glm::mat4(1.0);
        bodyModel = bodyMatrix*bodyModel;
        model_left_leg =bodyMatrix*model_left_leg;
        model_right_leg =bodyMatrix*model_right_leg;
        





        glm::mat4 view;
        float radius = 2000.0f;
        //float camX   = sin(glfwGetTime()) * radius;
        //float camZ   = cos(glfwGetTime()) * radius;
        
        
        
        //SET CAMERA VIEW HERE
        //-----------------------------------------------------------------------
        sd.use();
        view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
        sd.setMat4("view", view);
        sd_cube.use();
        sd_cube.setMat4("view", view);
        
        sd.use();
        sd.setMat4("model", model_left_leg);
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glDrawElements(GL_TRIANGLES, indices_1.size(), GL_UNSIGNED_INT, 0);
        sd.setMat4("model", model_right_leg);
        glDrawElements(GL_TRIANGLES, indices_1.size(), GL_UNSIGNED_INT, 0);
       // sd.use();
       // sd.setMat4("model", model1);
       // glDrawElements(GL_TRIANGLES, indices_1.size(), GL_UNSIGNED_INT, 0);
        
        
        sd.use();
        //sd.setMat4("model", model2);
        //glDrawElements(GL_TRIANGLES, indices_1.size(), GL_UNSIGNED_INT, 0);
        
        
        glBindVertexArray(VAO_plane);
        glBindBuffer(GL_ARRAY_BUFFER, VBO_plane);
        sd_cube.use();
        sd_cube.setMat4("model", modelcube);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glDrawElements(GL_TRIANGLES, indices_plane.size(), GL_UNSIGNED_INT, 0);

        
        glBindVertexArray(VAO_cube);
        glBindBuffer(GL_ARRAY_BUFFER, VBO_cube);
        sd_center.use();
        glm::mat4 worldCenterModel = glm::mat4(1.0);
        sd_center.setMat4("model", worldCenterModel);
        sd_center.setMat4("view", view);
        sd_center.setMat4("projection", projection);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glDrawElements(GL_TRIANGLES, indices_cube.size(), GL_UNSIGNED_INT, 0);
        
        glBindVertexArray(VAO_body);
        glBindBuffer(GL_ARRAY_BUFFER, VBO_body);
        sd_body.use();
        sd_center.setMat4("model", bodyModel);
        sd_center.setMat4("view", view);
        sd_center.setMat4("projection", projection);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glDrawElements(GL_TRIANGLES, indices_body.size(), GL_UNSIGNED_INT, 0);
        
        
        
        
//        for(int i = 0; i<keyframeSet.size();i++){
//        glm::mat4 model = glm::mat4(1.0);
//        model = glm::translate(model, keyframeSet[i].position);
//        model =model*keyframeSet[i].getThisRotationMatrix(eulerMode);
//
//        sd_cube.use();
//        sd_cube.setMat4("model", model);
//        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
//        glDrawElements(GL_TRIANGLES, indices_cube.size(), GL_UNSIGNED_INT, 0);
//        }
        
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
    
    float cameraSpeed = 25.0;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        cameraPos += cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cameraPos -= cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if(firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }
    
    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;
    
    float sensitivity = 0.05;
    xoffset *= sensitivity;
    yoffset *= sensitivity;
    
    yaw   += xoffset;
    pitch += yoffset;
    
    if(pitch > 89.0f)
        pitch = 89.0f;
    if(pitch < -89.0f)
        pitch = -89.0f;
    
    glm::vec3 front;
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraFront = glm::normalize(front);
}


