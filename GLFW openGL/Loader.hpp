//
//  Loader.hpp
//  GLFW openGL
//
//  Created by 史博闻 on 9/30/18.
//  Copyright © 2018 fergusshi. All rights reserved.
//

#ifndef Loader_hpp
#define Loader_hpp

#include <stdio.h>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <glm/glm.hpp>
#include "KeyFrame.hpp"

class Loader{
    
public:
    void  static load_obj(const char* filename, std::vector<glm::vec3> &vertices,std::vector<unsigned int> &vertexIndices) ;
     void static load_KeyFrame(const char* filename,std::vector<KeyFrame> &vector);
};

 void Loader :: load_KeyFrame(const char* filename,std::vector<KeyFrame> &vector){
    
    
    std::ifstream in;
    std::string newline,word;
    in.open(filename);
    if(!in){
        printf("Impossible to open the file of Keyframe !\n");
        return ;
    }
     while(getline(in,newline)){
         std::istringstream iss(newline);
         float x;
         float y;
         float z;
         float anglex;
         float angley;
         float anglez;
         float anglew;
         bool mode;
         iss>>x>>y>>z>>anglex>>angley>>anglez>>anglew>>mode;
         glm :: vec3 position = glm :: vec3(x,y,z);
         glm :: vec4 angle = glm :: vec4(anglex,angley,anglez,anglew);
         KeyFrame key(position,angle,mode);
         vector.push_back(key);
     }
     
    
    
}



void  Loader :: load_obj(const char* filename, std::vector<glm::vec3> &vertices,std::vector<unsigned int> &vertexIndices)
{
    std::vector< glm::vec3 > temp_vertices;
    std::vector< glm::vec2 > temp_uvs;
    std::vector< glm::vec3 > temp_normals;
    
    std::ifstream in;
    std::string newline,word;
    in.open(filename);
    if(!in){
        printf("Impossible to open the file !\n");
        return ;
    }
    while(getline(in,newline)){
        
        std::istringstream iss(newline);
        iss>>word;
        if(word =="v"){
            glm::vec3 vertex;
            iss>>vertex.x>>vertex.y>>vertex.z;
            vertices.push_back(vertex);
        }
        else if(word =="f"){
            replace(newline.begin(),newline.end(),'/',' ');
            std::istringstream f_is(newline);
            unsigned int vertexindice = 0;
            int count = 0;
            unsigned int a[4] ;
            std::string temp;
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


#endif /* Loader_hpp */
