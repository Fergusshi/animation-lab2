//
//  KeyFrame.hpp
//  GLFW openGL
//
//  Created by 史博闻 on 9/30/18.
//  Copyright © 2018 fergusshi. All rights reserved.
//

#ifndef KeyFrame_hpp
#define KeyFrame_hpp

#include <stdio.h>
#include <glm/glm.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <cmath>

#define GLKEY_EULER                   2.0
#define    PI                         3.1415926

class KeyFrame {
    

public:
    glm::vec3 position;
    glm :: vec4 orientation;
    bool EulerMode;
    glm :: mat4 getThisRotationMatrix(bool mode);
    static glm :: mat4 getRotationMatrix(glm :: vec4 orient,bool eMode);
    glm :: vec4 getEuler();
    glm :: vec4 getQuaternion();
    KeyFrame(glm :: vec3 &positionKey,glm :: vec4 &orientationKey,bool mode)
    : position(positionKey),orientation(orientationKey),EulerMode(mode)
    {
    }
};


glm :: mat4 KeyFrame:: getThisRotationMatrix(bool mode){
    
    if(mode){
        return KeyFrame:: getRotationMatrix(this->orientation,this->EulerMode);
    }else{
        return KeyFrame:: getRotationMatrix(this->getQuaternion(),mode);
    }
   

}


glm :: mat4 KeyFrame:: getRotationMatrix(glm :: vec4 orient,bool eMode){
    float x = orient.x;
    float y = orient.y;
    float z = orient.z;
    float w = orient.w;
    
    if(eMode){
       float ca = cos(z*PI/180);
       float sa = sin(z*PI/180);
        float cb = cos(y*PI/180);
        float sb = sin(y*PI/180);
        float cr = cos(x*PI/180);
        float sr = sin(x*PI/180);
        glm :: mat3 rotation(1.0f);
        rotation[0][0] = ca*cb;
        rotation[0][1] = ca*sb*sr - sa*cr;
        rotation[0][2] = ca*sb*cr + sa*sr;
        rotation[1][0] = sa*cb;
        rotation[1][1] = sa*sb*sr + ca*cr;
        rotation[1][2] = sa*sb*cr - ca*sr;
        rotation[2][0] = -sb;
        rotation[2][1] = cb*sr;
        rotation[2][2] = cb*cr;
        
        //return glm :: orientate4(glm::vec3(yY*PI/180,xP*PI/180,zR*PI/180));
        return glm ::transpose(glm :: mat4(rotation));
    }
    else{
        glm :: mat3 rotation(1.0f);
        rotation[0][0] = 1 - 2*y*y - 2*z*z;
        rotation[0][1] = 2*x*y - 2*w*z;
        rotation[0][2] = 2*x*z + 2*w*y;
        rotation[1][0] = 2*x*y + 2*w*z;
        rotation[1][1] = 1 - 2*x*x - 2*z*z;
        rotation[1][2] = 2*y*z - 2*w*x;
        rotation[2][0] = 2*x*z - 2*w*y;
        rotation[2][1] = 2*y*z + 2*w*x;
        rotation[2][2] = 1 - 2*x*x - 2*y*y;
        return glm ::transpose(glm :: mat4(rotation));
        //return glm :: mat4(rotation);
    }
}

glm :: vec4 KeyFrame:: getEuler(){
    
    if(this->EulerMode){
        return  glm :: vec4(this->orientation.x,
                            this->orientation.y,
                            this->orientation.z,
                            this->orientation.w);
    }
    else{
        glm :: vec4  q = this->orientation;
        float t0 = -2.0f * (q.y * q.y + q.z * q.z) + 1.0f;
        float t1 = +2.0f * (q.x * q.y - q.w * q.z);
        float t2 = -2.0f * (q.x * q.z + q.w * q.y);
        float t3 = +2.0f * (q.y * q.z - q.w * q.x);
        float t4 = -2.0f * (q.x * q.x + q.y * q.y) + 1.0f;
        
        t2 = t2 > 1.0f ? 1.0f : t2;
        t2 = t2 < -1.0f ? -1.0f : t2;
        return glm :: vec4(asin(t2),atan2(t1, t0),atan2(t3, t4),0);
    }
}

glm :: vec4 KeyFrame:: getQuaternion(){
    float xP = this->orientation.x*PI/180;
    float yY = this->orientation.y*PI/180;
    float zR = this->orientation.z*PI/180;
    
    if(this->EulerMode){
        float cy = cos(yY * 0.5f);
        float sy = sin(yY * 0.5f);
        float cr = cos(zR * 0.5f);
        float sr = sin(zR * 0.5f);
        float cp = cos(xP * 0.5f);
        float sp = sin(xP * 0.5f);
        glm :: vec4 q;
        q.w = cy * cr * cp + sy * sr * sp;
        q.x = cy * sr * cp - sy * cr * sp;
        q.y = cy * cr * sp + sy * sr * cp;
        q.z = sy * cr * cp - cy * sr * sp;
        return  q;
    }
    else{
        return  this->orientation;
    }
}


#endif /* KeyFrame_hpp */
