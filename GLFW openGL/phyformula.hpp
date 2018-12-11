//
//  phyformula.hpp
//  lab3
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

//template <typename T>
class phyformula{
public:
    static void velocityCaculate(phyObject *a, phyObject *b);
    static void anvelocityCaculate(phyObject *a, phyObject *b);
    static glm::vec4 AngularVelocityToQuaternion (glm::vec4 anvelocity, float deltaTime);
};

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


#endif /* phyformula_hpp */
