//CSCI 5607 HW3 - Rays & Files
//This HW has three steps:
// 1. Compile and run the program (the program takes a single command line argument)
// 2. Understand the code in this file (rayTrace_vec3.cpp), in particular be sure to understand:
//     -How ray-sphere intersection works
//     -How the rays are being generated
//     -The pipeline from rays, to intersection, to pixel color
//    After you finish this step, and understand the math, take the HW quiz on canvas
// 3. Update the file parse_vec3.h so that the function parseSceneFile() reads the passed in file
//     and sets the relevant global variables for the rest of the code to product to correct image

//To Compile: g++ -fsanitize=address -std=c++11 rayTrace_vec3.cpp

#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

//Images Lib includes:
#define STB_IMAGE_IMPLEMENTATION //only place once in one .cpp file
#define STB_IMAGE_WRITE_IMPLEMENTATION //only place once in one .cpp files
#include "headers/image_lib.h" //Defines an image class and a color class

//#Vec3 Library
#include "headers/vec3.h"

//High resolution timer
#include <chrono>
#include <omp.h>
//Scene file parser
#include "headers/parse_vec3.h"

#include "headers/scene.h"

#include <thread>
#include <mutex> 
#include <cstring>


std::mutex lock;

void RayCast(scene s, Image *image, int x_start, int y_start, int x_end, int y_end){
  
  float imgW = s.getWidth();
  float halfW = imgW/2.0;
  float imgH = s.getHeight();
  float halfH = imgH/2.0;
  float d = halfH / tanf(s.getCam().camera_fov_ha * (M_PI / 180.0f));
  camera eye = s.getCam();
  int sample = 5;
  

  //lock.lock();
  //#pragma omp parallel for schedule(dynamic, 5)
  for( int y = y_start; y < y_end; y++){
    for(int x = x_start; x < x_end; x++){
      rgb result;
      for(int i = 0; i < sample; i ++){
        float u = drand48() + (halfW - (imgW)*(x/imgW));
        float v = drand48() + (halfH - (imgH)*(y/imgH));

        vec3 p = eye.camera_pos - d * eye.camera_fwd + u * eye.camera_right + v * eye.camera_up_;
        
        ray *newRay = new eyeRay(eye.camera_pos, (p-eye.camera_pos).normalized(), rgb(0,0,0),0);
        Color c = s.EvaluateRayTree(newRay,0).trans();
        //image->setPixel(x,y,c);   
        result += s.EvaluateRayTree(newRay,0);
        delete newRay;
      }
      result = result * (1.0/sample);
      image->setPixel(x,y, Color(result.r,result.g,result.b));
    }
  }
  //lock.unlock();
}




int main(int argc, char** argv){

  //Read command line paramaters to get scene file
  if (argc != 4){
     std::cout << "Usage: ./a.out scenefile BVHuse parallel\n";
     return(0);
  }
  std::string secenFileName = argv[1];
  std::string s = argv[2];
  std::string s2 = argv[3];
  bool parallel = (s2 == "t") ? true:false;
  bool use = (s == "t") ? true:false;
  scene newScene;
  
  parseSceneFile(secenFileName,newScene);
  if(use){
    printf("using bvh\n");
    newScene.bvh.push_back(new BVHNode(newScene.objects)); 
  }
  newScene.setBVHuse(use);
  Image *img = new Image(newScene.getWidth(),newScene.getHeight());
  auto t_start = std::chrono::high_resolution_clock::now();

  if(parallel){
    printf("parallel\n");
    std::thread thread_1 = std::thread(RayCast,newScene,img,0,0,newScene.getWidth()/3,newScene.getHeight()/3);
    std::thread thread_2 = std::thread(RayCast,newScene,img,newScene.getWidth()/3,0,newScene.getWidth()*2/3,newScene.getHeight()/3);
    std::thread thread_3 = std::thread(RayCast,newScene,img,newScene.getWidth()*2/3,0,newScene.getWidth(),newScene.getHeight()/3);

    std::thread thread_4 = std::thread(RayCast,newScene,img,0,newScene.getHeight()/3,newScene.getWidth()/3,newScene.getHeight()*2/3);
    std::thread thread_5 = std::thread(RayCast,newScene,img,newScene.getWidth()/3,newScene.getHeight()/3,newScene.getWidth()*2/3,newScene.getHeight()*2/3);
    std::thread thread_6 = std::thread(RayCast,newScene,img,newScene.getWidth()*2/3,newScene.getHeight()/3,newScene.getWidth(),newScene.getHeight()*2/3);

    std::thread thread_7 = std::thread(RayCast,newScene,img,0,newScene.getHeight()*2/3,newScene.getWidth()/3,newScene.getHeight());
    std::thread thread_8 = std::thread(RayCast,newScene,img,newScene.getWidth()/3,newScene.getHeight()*2/3,newScene.getWidth()*2/3,newScene.getHeight());
    std::thread thread_9 = std::thread(RayCast,newScene,img,newScene.getWidth()*2/3,newScene.getHeight()*2/3,newScene.getWidth(),newScene.getHeight());
  
    thread_1.join();
    thread_2.join();
    thread_3.join();
    thread_4.join();
    thread_5.join();
    thread_6.join();
    thread_7.join();
    thread_8.join();
    thread_9.join();
  }else{
    std::thread thread_1 = std::thread(RayCast,newScene,img,0,0,newScene.getWidth(),newScene.getHeight());
    thread_1.join();
  }
  
  
  

  img->write(newScene.getOut().c_str());
  auto t_end = std::chrono::high_resolution_clock::now();
  printf("Rendering took %.2f ms\n",std::chrono::duration<double, std::milli>(t_end-t_start).count());
  delete img;

  return 0;
}
