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
#include "phyformula.hpp"

using namespace std;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);


// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

const float BALL_RADIUS=50.0;
const float EnergyLossRate = 0.6;
float gravity = -300.0;

glm::vec3 cameraPos   = glm::vec3(0.0f, 100.0f, 2000.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp    = glm::vec3(0.0f, 1.0f, 0.0f);

bool firstMouse = true;
float yaw   = -90.0f;    // yaw is initialized to -90.0 degrees since a yaw of 0.0 results in a direction vector pointing to the right so we initially rotate a bit to the left.
float pitch =  0.0f;
float lastX =  800.0f / 2.0;
float lastY =  600.0 / 2.0;
float fov   =  45.0f;

std::vector<phyObject*> all_objects;



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
    
    

   
    
    
    
    //Third Section
    
    //shader Load
    Shader sd("../../../source/rotate.vs","../../../source/fShader.fs");
    Shader sd_cube("../../../source/cube.vs","../../../source/cube.fs");
    Shader sd_center("../../../source/center.vs","../../../source/center.fs");
    Shader sd_ball1("../../../source/ball.vs","../../../source/center.fs");
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
    vector< glm::vec3 > vertices_ball1;
    vector<unsigned int> indices_ball1;
    std::vector<KeyFrame> keyframeSet;
    Loader :: load_obj("../../../source/tinker.obj", vertices_1, indices_1);
    Loader :: load_obj("../../../source/cube.obj", vertices_cube, indices_cube);
    Loader :: load_obj("../../../source/plane.obj", vertices_plane, indices_plane);
    Loader :: load_obj("../../../source/body.obj", vertices_body, indices_body);
    Loader :: load_obj("../../../source/beachball.obj", vertices_ball1, indices_ball1);
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
    
    //vao of ball
    unsigned int VBO_ball1, VAO_ball1, EBO_ball1;
    glGenVertexArrays(1, &VAO_ball1);
    glGenBuffers(1, &VBO_ball1);
    glGenBuffers(1, &EBO_ball1);
    glBindVertexArray(VAO_ball1);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_ball1);
    glBufferData(GL_ARRAY_BUFFER, vertices_ball1.size()*sizeof(glm::vec3), &vertices_ball1[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_ball1);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,indices_ball1.size()*sizeof(unsigned int), &indices_ball1[0], GL_STATIC_DRAW);
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
    float timeSlowRate = 1;
    float lastKeyEndingTime = 0;
    
    
    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        processInput(window);
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        float currentTime = glfwGetTime();
        float t = (currentTime - lastKeyEndingTime)/timeSlowRate;
        lastKeyEndingTime = currentTime;
        
        
        
        for(int i = 0; i<all_objects.size();i++){
            auto object = all_objects[i];
            
            for(int j = i+1;j<all_objects.size();j++){
                glm::vec3 center1(all_objects[i]->positionMatrix[3]);
                glm::vec3 center2(all_objects[j]->positionMatrix[3]);
                glm::vec3 c2_to_c1 = center1 - center2;
                
                if (glm::length(c2_to_c1) >= 2.0 * BALL_RADIUS) continue;
                
                // for perfect elastic collision velocity will swap
                glm ::vec3 tempvec =all_objects[i]->velocity;
                all_objects[i]->setVelocity(all_objects[j]->velocity);
                all_objects[j]->setVelocity(tempvec);
                all_objects[i]->updateanVelocity();
                all_objects[j]->updateanVelocity();
                
                
                auto c2_to_v1_normalized = glm::normalize(c2_to_c1);

            }
            if (object->positionMatrix[3][0] <= -1000 + BALL_RADIUS && object->velocity.x < 0) {
                glm::vec3 contact_normal(-EnergyLossRate, 1.0, 1.0);
                object->setVelocity(object->velocity*contact_normal);
                object->updateanVelocity();
            }
            
            if (object->positionMatrix[3][0] >= 1000 - BALL_RADIUS && object->velocity.x > 0) {
                glm::vec3 contact_normal(-EnergyLossRate, 1.0, 1.0);
                object->setVelocity(object->velocity*contact_normal);
                object->updateanVelocity();
            }
            
            if (object->positionMatrix[3][2] <= -1000 + BALL_RADIUS && object->velocity.z < 0) {
                glm::vec3 contact_normal(1.0, 1.0,-EnergyLossRate);
                object->setVelocity(object->velocity*contact_normal);
                object->updateanVelocity();
            }
            
            if (object->positionMatrix[3][2] >= 1000 - BALL_RADIUS && object->velocity.z > 0) {
                glm::vec3 contact_normal(1.0, 1.0,-EnergyLossRate);
                object->setVelocity(object->velocity*contact_normal);
                object->updateanVelocity();
            }
            
            if (object->positionMatrix[3][1] <= BALL_RADIUS && object->velocity.y <= 0){
                if (abs(object->velocity.y) < 0.1) {
                    // if the velocity is smaller than threshold
                    object->setVelocity(object->velocity * glm::vec3(0.5,0.0,0.5));
                    object->updateanVelocity();
                    object->positionMatrix[3][1] = BALL_RADIUS;
                }else{
                    object->positionMatrix[3][1] = BALL_RADIUS;
                    object->setVelocity(object->velocity * glm::vec3(1.0,-EnergyLossRate,1.0));
                    object->updateanVelocity();
                }
            }
            if (object->positionMatrix[3][1] > BALL_RADIUS){
                glm :: vec3 v1(object->velocity);
                v1.y += gravity*t;
                object->setVelocity(v1);
                object->updateanVelocity();
                
            }
            
            
        }
        
        glm::mat4 view;
        view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
        
        
        
        for (auto object:all_objects) {
            
            // move the object based on the velocity and then draw
            
            object->positionMatrix[3][0] += t * object->velocity.x;
            object->positionMatrix[3][1] += t * object->velocity.y;
            if(object->positionMatrix[3][1]>0)
                object->positionMatrix[3][1] +=1/2*gravity*t*t;
            object->positionMatrix[3][2] += t * object->velocity.z;
            
            object->rotationMatrix =object->rotationMatrix * KeyFrame::getRotationMatrix(phyformula::AngularVelocityToQuaternion(object->anvelocity,t),0);
            
            glm::mat4 Model1 = object->positionMatrix * object->rotationMatrix;
            
         //   std::cout<<object->positionMatrix[3][0]<<' '<<object->positionMatrix[3][1]<<' '<<object->positionMatrix[3][2]<<"\n";
            sd_ball1.use();
            sd_ball1.setMat4("view", view);
            sd_ball1.setMat4("projection", projection);
            sd_ball1.setMat4("model", Model1);
            object->draw();
        }

        
        
        
        
        
        
        
        

        
        float radius = 2000.0f;
        //float camX   = sin(glfwGetTime()) * radius;
        //float camZ   = cos(glfwGetTime()) * radius;
        
        
        
        //SET CAMERA VIEW HERE
        //-----------------------------------------------------------------------
        sd.use();
        sd.setMat4("view", view);
        sd_cube.use();
        sd_cube.setMat4("view", view);
        

       // sd.use();
       // sd.setMat4("model", model1);
       // glDrawElements(GL_TRIANGLES, indices_1.size(), GL_UNSIGNED_INT, 0);
        
        
        //sd.use();
        //sd.setMat4("model", model2);
        //glDrawElements(GL_TRIANGLES, indices_1.size(), GL_UNSIGNED_INT, 0);
        
        
        glBindVertexArray(VAO_plane);
        glBindBuffer(GL_ARRAY_BUFFER, VBO_plane);
        glm :: mat4 model = glm::mat4(1.0);
        sd_cube.use();
        sd_cube.setMat4("model", model);
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

float currentpresstime=0;
float lastpresstime=0;
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
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS){
        currentpresstime =glfwGetTime();
        if(currentpresstime - lastpresstime>0.15){
            lastpresstime = currentpresstime;
            glm::vec3 position=cameraPos;
            glm::vec3 direction=cameraFront;
            position+= direction*50.0f;
            glm::mat4 model = glm::mat4(1.0);
            glm::mat4 positionMat = glm::translate(model, position);
            glm::vec3 velocity = 300.0f * direction;
            phyObject *ball = new phyObject(positionMat,model,velocity);
            all_objects.push_back(ball);
            
            
        }
    }
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


