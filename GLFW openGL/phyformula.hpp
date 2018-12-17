//
//  phyformula.hpp
//  lab4
//
//  Created by 史博闻 on 12/5/18.
//  Copyright © 2018 fergusshi. All rights reserved.
//

#ifndef phyformula_hpp
#define phyformula_hpp

#include <stdio.h>
#include "phyObject.hpp"
#include <glm/glm.hpp>
#include <cmath>
#include "KeyFrame.hpp"
#include <glm/gtx/vector_angle.hpp>

//template <typename T>
class phyformula{
public:
    static void velocityCaculate(phyObject *a, phyObject *b);
    static void anvelocityCaculate(phyObject *a, phyObject *b);
    static glm::vec4 AngularVelocityToQuaternion (glm::vec4 anvelocity, float deltaTime);
    static glm::vec4 AngleToQuaternion (glm::vec3 angle,float w);
    static glm:: mat4 vectoquat(glm::vec3 vector);
    static void field(phyObject *applyObject, phyObject *carryObject,float boundry,float deltaTime);
    static float forceRate;
    static float pole_forceRate;
    static void polefield (glm::vec3 center, phyObject *carryObject,float deltaTime,float rate);
};
float phyformula :: forceRate = 1000.0;
float phyformula :: pole_forceRate = 800.0;
void phyformula :: velocityCaculate(phyObject *objectA, phyObject *objectB){
    glm::vec3 v1 = objectA->velocity;
    glm::vec3 v2 = objectB->velocity;
    objectA->velocity = v2;
    objectB->velocity = v1;

}

glm::vec4 phyformula :: AngularVelocityToQuaternion (glm::vec4 anvelocity, float deltaTime)
{
    glm :: vec3 temp = glm :: normalize(glm::vec3(anvelocity));
    float v = deltaTime * anvelocity.w * 0.5f;
    float q = cos(v);
    float s = sin(v);
    
    glm::vec4 integrated;
    integrated.w = q;
    integrated.x = s * temp.x;
    integrated.y = s * temp.y;
    integrated.z = s * temp.z;
    return glm :: normalize(integrated);

}

glm::vec4 phyformula :: AngleToQuaternion (glm::vec3 angle,float w)
{
    glm :: vec3 temp = glm :: normalize(angle);
    float v = w * 0.5f;
    float q = cos(v);
    float s = sin(v);
    
    glm::vec4 integrated;
    integrated.w = q;
    integrated.x = s * temp.x;
    integrated.y = s * temp.y;
    integrated.z = s * temp.z;
    return glm :: normalize(integrated);
}


void phyformula :: field (phyObject *applyObject, phyObject *carryObject,float boundry, float deltaTime)
{
    glm :: vec3 pos1 = glm::vec3(applyObject->positionMatrix[3]);
    glm :: vec3 pos2 = glm::vec3(carryObject->positionMatrix[3]);
    glm :: vec3 force_dirction = pos2-pos1;
    float distance = glm::length(force_dirction)-180;
    if(distance>boundry)
        return;
    if(distance<0.0)
        distance = 0.1;
    glm :: vec3 force = phyformula ::forceRate/(distance*distance)*glm :: normalize(force_dirction);
    glm :: vec3 vel = carryObject->velocity+force*deltaTime;
    if (glm :: length(vel)> carryObject->speedlimit)
        vel = carryObject->speedlimit * glm::normalize(vel);
    carryObject->setVelocity(vel);
    carryObject->updateanVelocity();
    carryObject->rotationMatrix = vectoquat(vel);
    
}

void phyformula :: polefield (glm::vec3 center, phyObject *carryObject, float deltaTime,float rate)
{
    glm :: vec3 pos2 = glm::vec3(carryObject->positionMatrix[3]);
    glm :: vec3 force_dirction = center-pos2;
    glm :: vec3 force = rate * pole_forceRate * glm :: normalize(force_dirction);
    glm :: vec3 vel = carryObject->velocity+force*deltaTime;
    if (glm :: length(vel)> carryObject->speedlimit)
        vel = carryObject->speedlimit * glm::normalize(vel);
    carryObject->setVelocity(vel);
    carryObject->updateanVelocity();
    carryObject->rotationMatrix = vectoquat(vel);
    
}
glm:: mat4 phyformula :: vectoquat(glm::vec3 vector)
{
    vector = glm :: normalize(vector);
    glm :: vec3 zaxis(0.0, 0.0, 1.0);
    glm :: vec3 rotationaxis = glm :: normalize(glm::cross(zaxis, vector));
    float w = glm :: angle(zaxis,vector);
    std::cout<<w<<"\n";
    glm :: vec4 integrated = AngleToQuaternion (rotationaxis,w);
    std::cout<<glm :: to_string(integrated)<<"\n";
    return KeyFrame:: getRotationMatrix(integrated,false);
    
}



#endif /* phyformula_hpp */
