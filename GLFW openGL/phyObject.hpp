//
//  phyObject.hpp
//  lab3
//
//  Created by 史博闻 on 12/5/18.
//  Copyright © 2018 fergusshi. All rights reserved.
//

#ifndef phyObject_hpp
#define phyObject_hpp

#include <stdio.h>
#include <glm/glm.hpp>
#include <vector>
#include "ShaderLoader.h"
#include "Loader.hpp"

class phyObject{
public:
    glm :: mat4 positionMatrix;
    glm :: mat4 rotationMatrix;
    glm :: vec3 velocity;
    glm :: vec4 anvelocity;
    glm :: vec3 Vvelocity;
    glm :: vec3 Hvelocity;
    std:: vector< glm::vec3 > vertices_ball1;
    std:: vector<unsigned int> indices_ball1;
    Shader* sd_ball1;

    unsigned int VBO_ball1,VAO_ball1,EBO_ball1;
    
    void setVelocity(glm :: vec3 v);
    void draw();
    void updateanVelocity();
    
    phyObject(glm::mat4 &position,glm :: mat4 &rotation, glm :: vec3 &v)
    :positionMatrix(position),rotationMatrix(rotation),velocity(v)
    {
        this->Vvelocity = v*glm::vec3(0.0,1.0,0.0);
        this->Hvelocity = v*glm::vec3(1.0,0.0,1.0);
        glm::vec3 temp = glm::vec3(this->velocity.z,0.0,-this->velocity.x);
        float rate = 1.0;
        this->anvelocity = glm::vec4(temp,glm::length(temp)*rate);
        this->sd_ball1 = new Shader("../../../source/ball.vs","../../../source/center.fs");
        Loader :: load_obj("../../../source/beachball.obj", vertices_ball1, indices_ball1);
        glGenVertexArrays(1, &VAO_ball1);
        glGenBuffers(1, &VAO_ball1);
        glGenBuffers(1, &EBO_ball1);
        glBindVertexArray(VAO_ball1);
        glBindBuffer(GL_ARRAY_BUFFER, VAO_ball1);
        glBufferData(GL_ARRAY_BUFFER, vertices_ball1.size()*sizeof(glm::vec3), &vertices_ball1[0], GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_ball1);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER,indices_ball1.size()*sizeof(unsigned int), &indices_ball1[0], GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_TRUE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

    }
    
    
};

void phyObject :: setVelocity(glm :: vec3 v){
    this->velocity = v;
    this->Vvelocity = v*glm::vec3(0.0,1.0,0.0);
    this->Hvelocity = v*glm::vec3(1.0,0.0,1.0);
}

void phyObject :: updateanVelocity(){
    glm::vec3 temp = glm::vec3(this->velocity.z,0.0,-this->velocity.x);
    float rate = 0.01;
    this->anvelocity = glm::vec4(temp,glm::length(temp)*rate);
}

void phyObject:: draw(){
    glBindVertexArray(VAO_ball1);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_ball1);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glDrawElements(GL_TRIANGLES, indices_ball1.size(), GL_UNSIGNED_INT, 0);
    
}
#endif /* phyObject_hpp */
