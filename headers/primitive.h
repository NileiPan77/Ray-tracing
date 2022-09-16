#ifndef PRIMITIVE_H_
#define PRIMITIVE_H_

#include <string>
#include <cmath>
#include <vector>
#include <algorithm>
#include <iostream>
#include "ray.h"
#include "hitInfo.h"

struct hitInformation;
class AABB;
struct material{

  rgb ambient;
  rgb diffuse;
  rgb specular;
  float phong;
  rgb transmissive;
  float refractionIndex;

  material(){
    ambient.r = ambient.g = ambient.b = 0.0;
    diffuse = rgb(1.0,1.0,1.0);
    specular.r = specular.g = specular.b = 0.0;
    phong = 5.0;
    transmissive.r = transmissive.g = transmissive.b = 0.0;
    refractionIndex = 1.0;
  }

  material(rgb a, rgb d, rgb s, float p, rgb trans, float refrac){
    ambient = a;
    diffuse = d;
    specular = s;
    phong = p;
    transmissive = trans;
    refractionIndex = refrac;
  }
  
};
class primitive{
 private:
  vec3 position;
  material matt;
 public:
  primitive();
  virtual ~primitive();
  primitive(vec3 p);
  primitive(vec3 p,material m);
  vec3 getPosition();
  material getMatt();

  void setPosition(vec3 pos);
  void setMateriall(material m);
  virtual float rayIntersect(ray *rs,hitInformation& hit){};
  virtual void print() = 0;
  virtual void identify(){
    std::cout << "base" << std::endl;
  }
  virtual vec3 getNormal(vec3 hitPoint){};
  virtual bool boundingBox(AABB &box){};
};

class sphere: public primitive{
 private:
  float radius;

 public:
  sphere();
    
   ~sphere();
  
  sphere(vec3 pos, float r);

  sphere(vec3 pos, float r,material m);
  void setRadius(float r);
  float getRadius();
  
  float rayIntersect(ray* rs,hitInformation& hit) override;
  vec3 getNormal(vec3 hitPoint);
  void print();
  void identify();
  bool boundingBox(AABB &box);
};

class plane: public primitive{
  private:
    vec3 normal;
  public:
    plane(vec3 p, vec3 n, material m);
    float rayIntersect(ray *rs,hitInformation& hit);
    void print();
    vec3 getNormal(vec3 hitPoint);
    bool boundingBox(AABB &box);
};

class triangle: public primitive{
  private:
    vec3 position2;
    vec3 position3;
    vec3 normal;
    vec3 normal1;
    vec3 normal2;
    vec3 normal3;
    vec3 bary;
    bool normalTri;
  public:
    triangle(vec3 pos1,vec3 pos2,vec3 pos3,material m);
    triangle(vec3 pos1,vec3 pos2,vec3 pos3,vec3 n1, vec3 n2, vec3 n3, material m);

    float magnitude();
    float rayIntersect(ray* rs,hitInformation& hit) override;

    vec3 getNormal(vec3 hitPoint);
    void print();
    void identify();
    bool boundingBox(AABB &box);
};

class cube: public primitive{
 private:
  vec3 maxP;

 public:

  cube(vec3 minP, vec3 maxP,material m);
  ~cube();
  void setMaxP(vec3 MAXP);

  vec3 getMaxP();

  float rayIntersect(ray* rs,hitInformation& hit);
  vec3 getNormal(vec3 hitPoint);
  void print();
  bool boundingBox(AABB &box);
};



class AABB{
    
  public: 
    vec3 min_,max_;
    AABB(){}
    AABB(vec3 minP,vec3 maxP);
    bool rayIntersect(ray* r, float tmin, float tmax);
    

};
    static bool compare(primitive* a,primitive* b);

class BVHNode: public primitive{
  private:
    primitive* left, *right;
    AABB box;
    

  public:

    std::vector<primitive*> splitObjects(std::vector<primitive*> objects, int start, int end);

    BVHNode(std::vector<primitive*> objects);

    float rayIntersect(ray *rs,hitInformation& hit);
    void print();
    void identify();
    vec3 getNormal(vec3 hitPoint);
    bool boundingBox(AABB &box);
};



#endif
