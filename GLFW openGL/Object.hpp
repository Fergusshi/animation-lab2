//
//  Object.hpp
//  lab2
//
//  Created by 史博闻 on 11/2/18.
//  Copyright © 2018 fergusshi. All rights reserved.
//

#ifndef Object_hpp
#define Object_hpp

#include <stdio.h>
#include <glm/glm.hpp>
#include <vector>
#include <unordered_map>



class Object{
public:
    std::unordered_map<std::string, Object> children;
    glm :: mat4 selfAnimateMatrix;
    glm :: mat4 bindLocalMatrix;
    glm :: mat4 parentMatrix;
    Object  getChild(std::string name);
    void addChild(std::string name, Object* newchild);
    void setParentMatrix(glm::mat4 parentMatrix);
    void setBindLocalMatrix(glm::mat4 bindLocalMatrix);
    void setSelfAnimateMatrix(glm::mat4 selfAnimateMatrix);
    glm :: mat4 getselfMatrix();
    Object(void){}
    Object(glm::mat4 currentMatrix);
    Object(const Object &c){
        selfAnimateMatrix = c.selfAnimateMatrix;
        bindLocalMatrix = c.bindLocalMatrix;
        parentMatrix = c.parentMatrix;
        parentMatrix = c.parentMatrix;
    }
    
};

Object::Object(glm::mat4 currentMatrix){
    this->bindLocalMatrix = currentMatrix;
}
    
Object Object:: getChild(std::string name){
        return this->children[name];
    }
void Object:: addChild(std::string name, Object* newchild){
    this->children.emplace(name,*newchild);
    newchild->setParentMatrix(this->parentMatrix*this->bindLocalMatrix*this->selfAnimateMatrix);
    }
    
void Object:: setBindLocalMatrix(glm::mat4 bindLocalMatrix){
    this->bindLocalMatrix = bindLocalMatrix;
    }
void Object:: setSelfAnimateMatrix(glm::mat4 selfAnimateMatrix){
    this->selfAnimateMatrix = selfAnimateMatrix;
}
void Object:: setParentMatrix(glm::mat4 parentMatrix){
    this->parentMatrix = parentMatrix;
}






#endif /* Object_hpp */
