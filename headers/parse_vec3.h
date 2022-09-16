
//Set the global scene parameter variables
//TODO: Set the scene parameters based on the values in the scene file

#ifndef PARSE_VEC3_H
#define PARSE_VEC3_H

#include <cstdio>
#include <iostream>
#include <fstream>
#include <cstring>
#include <sstream>

#include "scene.h"
#include "hitInfo.h"

#include "vec3.h"
//Camera & Scene Parmaters (Global Variables)
//Here we set default values, override them in parseSceneFile()

//Image Parmaters


// int img_width = 800, img_height = 600;
// std::string imgName = "raytraced.png";

// //Camera Parmaters
// vec3 eye = vec3(0,0,0); 
// vec3 forward = vec3(0,0,-1).normalized();
// vec3 up = vec3(0,1,0).normalized();
// vec3 right = vec3(-1,0,0).normalized();
// float halfAngleVFOV = 35; 

//Scene (Sphere) Parmaters
// vec3 spherePos = vec3(0,0,2);
// float sphereRadius = 1; 


void parseSceneFile(std::string fileName, scene &newScene){
  float pointLightEnergy = 10.0;
  float dirLightEnergy = 800.0;
  //TODO: Override the default values with new data from the file "fileName"
  std::fstream fd;
  fd.open(fileName);
  std::string lineRead;
  vec3 position(0,0,0);
  vec3 forward(0,0,-1);
  vec3 up(0,1,0);
  float fov = 45;
  material m;
  while(getline(fd,lineRead)){
    if(lineRead[0] != '#' && lineRead[0] != '\n'){
      
      std::size_t pos = lineRead.find_first_of(":");
      std::string name = lineRead.substr(0,pos);

      std::stringstream ss;

      if(name == "camera_pos"){
        std::string parameter = lineRead.substr(pos+1,lineRead.size());
        ss << parameter;
        std::string word;
        ss >> word;
        
        
        std::stringstream(word) >> position.x;
        
        ss >> word;
        std::stringstream(word) >> position.y;

        ss >> word;
        std::stringstream(word) >> position.z;

        
      }
      else if(name == "camera_fwd"){
        std::string parameter = lineRead.substr(pos+1,lineRead.size());
        ss << parameter;
        std::string word;
        ss >> word;

        std::stringstream(word) >> forward.x;
        ss >> word;
        std::stringstream(word) >> forward.y;
        ss >> word;
        std::stringstream(word) >> forward.z;

        
      }
      else if(name == "camera_up"){
        std::string parameter = lineRead.substr(pos+1,lineRead.size());
        ss << parameter;
        std::string word;
        ss >> word;
        
        std::stringstream(word) >> up.x;

        ss >> word;
        std::stringstream(word) >> up.y;

        ss >> word;
        std::stringstream(word) >> up.z;
      }
      else if (name == "camera_fov_ha"){
        std::string parameter = lineRead.substr(pos+1,lineRead.size());
        ss << parameter;
        std::string word;
        ss >> word;
        
        std::stringstream(word) >> fov;
      }
      else if(name == "background"){
        std::string parameter = lineRead.substr(pos+1,lineRead.size());
        ss << parameter;
        std::string word;
        ss >> word;

        rgb back;
        std::stringstream(word) >> back.r;
        ss >> word;
        std::stringstream(word) >> back.g;
        ss >> word;
        std::stringstream(word) >> back.b;

        newScene.setBackground(back);

      }
      else if(name == "material"){
        std::string parameter = lineRead.substr(pos+1,lineRead.size());
        ss << parameter;
        std::string word;
        ss >> word;


        vec3 pos;

        std::stringstream(word) >> m.ambient.r;
        ss >> word;
        std::stringstream(word) >> m.ambient.g;
        ss >> word;
        std::stringstream(word) >> m.ambient.b;

        ss >> word;
        std::stringstream(word) >> m.diffuse.r;
        ss >> word;
        std::stringstream(word) >> m.diffuse.g;
        ss >> word;
        std::stringstream(word) >> m.diffuse.b;

        ss >> word;
        std::stringstream(word) >> m.specular.r;
        ss >> word;
        std::stringstream(word) >> m.specular.g;
        ss >> word;
        std::stringstream(word) >> m.specular.b;

        ss >> word;
        std::stringstream(word) >> m.phong;
        ss >> word;
        std::stringstream(word) >> m.transmissive.r;
        ss >> word;
        std::stringstream(word) >> m.transmissive.g;
        ss >> word;
        std::stringstream(word) >> m.transmissive.b;
        ss >> word;
        std::stringstream(word) >> m.refractionIndex;

        
        

      }
      else if(name == "sphere"){
        std::string parameter = lineRead.substr(pos+1,lineRead.size());
        ss << parameter;
        std::string word;
        ss >> word;

        vec3 pos;
        float r;
        std::stringstream(word) >> pos.x;

        ss >> word;
        std::stringstream(word) >> pos.y;

        ss >> word;
        std::stringstream(word) >> pos.z;

        ss >> word;
        std::stringstream(word) >> r;
        
        newScene.addObject(new sphere(pos,r,m));
      }
      else if(name == "film_resolution"){
        std::string parameter = lineRead.substr(pos+1,lineRead.size());
        ss << parameter;
        std::string word;
        ss >> word;
        int num;
        std::stringstream(word) >> num;
        newScene.setWidth(num);

        ss >> word;
        std::stringstream(word) >> num;
        newScene.setHeight(num);

      }
      else if(name == "output_image"){
        newScene.setOut(lineRead.substr(pos+1,lineRead.size()));
      }
      else if(name == "point_light"){
        std::string parameter = lineRead.substr(pos+1,lineRead.size());
        ss << parameter;
        std::string word;
        ss >> word;

        

        rgb lightColor;
        std::stringstream(word) >> lightColor.r;
        ss >> word;
        std::stringstream(word) >> lightColor.g;
        ss >> word;
        std::stringstream(word) >> lightColor.b;
        //lightColor.clampInt();

        vec3 posInfo;
        ss >> word;
        std::stringstream(word) >> posInfo.x;
        ss >> word;
        std::stringstream(word) >> posInfo.y;
        ss >> word;
        std::stringstream(word) >> posInfo.z;

        newScene.addRay(new pointRay(posInfo,vec3(0,0,0),lightColor,pointLightEnergy));
        
      }
      else if(name == "directional_light"){
        std::string parameter = lineRead.substr(pos+1,lineRead.size());
        ss << parameter;
        std::string word;
        ss >> word;

        rgb lightColor;
        std::stringstream(word) >> lightColor.r;
        ss >> word;
        std::stringstream(word) >> lightColor.g;
        ss >> word;
        std::stringstream(word) >> lightColor.b;
        //lightColor.clampInt();

        vec3 dirInfo;
        ss >> word;
        std::stringstream(word) >> dirInfo.x;
        ss >> word;
        std::stringstream(word) >> dirInfo.y;
        ss >> word;
        std::stringstream(word) >> dirInfo.z;

        newScene.addRay(new directionalRay(vec3(0,0,0),dirInfo,lightColor,dirLightEnergy));
        
      }
      else if(name == "spot_light"){
        std::string parameter = lineRead.substr(pos+1,lineRead.size());
        ss << parameter;
        std::string word;
        ss >> word;

        rgb lightColor;
        std::stringstream(word) >> lightColor.r;
        ss >> word;
        std::stringstream(word) >> lightColor.g;
        ss >> word;
        std::stringstream(word) >> lightColor.b;
        //lightColor.clampInt();

        vec3 posInfo;
        ss >> word;
        std::stringstream(word) >> posInfo.x;
        ss >> word;
        std::stringstream(word) >> posInfo.y;
        ss >> word;
        std::stringstream(word) >> posInfo.z;

        vec3 dirInfo;
        ss >> word;
        std::stringstream(word) >> dirInfo.x;
        ss >> word;
        std::stringstream(word) >> dirInfo.y;
        ss >> word;
        std::stringstream(word) >> dirInfo.z;

        float a1,a2;
        ss >> word;
        std::stringstream(word) >> a1;
        ss >> word;
        std::stringstream(word) >> a2;

        newScene.addRay(new spotRay(posInfo,dirInfo,lightColor,a1,a2,pointLightEnergy));

      }
      else if(name == "ambient_light"){
        
        std::string parameter = lineRead.substr(pos+1,lineRead.size());
        ss << parameter;
        std::string word;
        ss >> word;
        rgb ab;

        std::stringstream(word) >> ab.r;
        ss >> word;
        std::stringstream(word) >> ab.g;
        ss >> word;
        std::stringstream(word) >> ab.b;
        newScene.setAmbient(ab);

      }
      else if(name == "max_depth"){
        std::string parameter = lineRead.substr(pos+1,lineRead.size());
        ss << parameter;
        std::string word;
        ss >> word;

        int num;
        std::stringstream(word) >> num;
        newScene.setDepth(num);
      }
      else if(name == "vertex"){
        std::string parameter = lineRead.substr(pos+1,lineRead.size());
        ss << parameter;
        std::string word;
        ss >> word;
        vec3 vert;
        
        std::stringstream(word) >> vert.x;
        
        ss >> word;
        std::stringstream(word) >> vert.y;

        ss >> word;
        std::stringstream(word) >> vert.z;
        newScene.addVertex(vert);
      }
      else if(name == "normal"){
        std::string parameter = lineRead.substr(pos+1,lineRead.size());
        ss << parameter;
        std::string word;
        ss >> word;
        vec3 norm;
        
        std::stringstream(word) >> norm.x;
        
        ss >> word;
        std::stringstream(word) >> norm.y;

        ss >> word;
        std::stringstream(word) >> norm.z;
        newScene.addNormal(norm);
      }
      else if(name == "triangle"){
        std::string parameter = lineRead.substr(pos+1,lineRead.size());
        ss << parameter;
        std::string word;
        ss >> word;
        int v1,v2,v3;

        
        std::stringstream(word) >> v1;
        
        ss >> word;
        std::stringstream(word) >> v2;

        ss >> word;
        std::stringstream(word) >> v3;

        
        newScene.addObject(new triangle(newScene.getVertex(v1),
                                        newScene.getVertex(v2),
                                        newScene.getVertex(v3),
                                        m ));
      }
      else if(name == "normal_triangle"){
        std::string parameter = lineRead.substr(pos+1,lineRead.size());
        ss << parameter;
        std::string word;
        ss >> word;
        int v1,v2,v3;
        int n1,n2,n3;

        
        std::stringstream(word) >> v1;
        
        ss >> word;
        std::stringstream(word) >> v2;

        ss >> word;
        std::stringstream(word) >> v3;

        ss >> word;
        std::stringstream(word) >> n1;
        
        ss >> word;
        std::stringstream(word) >> n2;

        ss >> word;
        std::stringstream(word) >> n3;

        newScene.addObject(new triangle(newScene.getVertex(v1),
                                        newScene.getVertex(v2),
                                        newScene.getVertex(v3),
                                        newScene.getNormal(n1),
                                        newScene.getNormal(n2),
                                        newScene.getNormal(n3),
                                        m ));
      }
      ss.str(std::string());
    }
  }
  forward = forward.normalized();
  up = up.normalized();
  if(dot(forward,up) != 0){
    double projf = dot(up,forward)/
      (forward.lengthSquare());
    vec3 proj = projf * forward;
    up = up - proj;
  }
  up = up.normalized();
  camera c(position,forward,up,fov);

  newScene.setCam(c);
  //newScene.printSeceneInfo();
  

  fd.close();
}

#endif