#ifndef SCENE_H_
#define SCENE_H_

#include <vector>
#include <string>
#include "ray.h"
#include "hitInfo.h"
struct hitInformation;
struct camera{
  vec3 camera_pos;
  vec3 camera_fwd;
  vec3 camera_up_;
  vec3 camera_right;
  float camera_fov_ha;

  camera(){
    camera_pos = vec3(0,0,0);
    camera_fwd = vec3(0,0,1);
    camera_up_ = vec3(0,1,0);
    camera_right = cross(camera_up_,camera_fwd);
    camera_fov_ha = 45;
  }

  camera(vec3 p, vec3 f, vec3 up, float fov){
    camera_pos = p;
    camera_fwd = f;
    camera_up_ = up;
    camera_right = cross(camera_up_,camera_fwd);
    camera_fov_ha = fov;
  }
  void set_right(vec3 v){
    camera_right.x = v.x;
    camera_right.y = v.y;
    camera_right.z = v.z;
  }
};





class scene{
 private:
  bool BVHuse;
  std::vector<vec3> vertices;
  std::vector<vec3> normals;
  int max_vertices;
  int max_normals;
  rgb background;
  rgb ambient;
  camera camInfo;
  int film_height;
  int film_width;
  std::string output;
  int max_depth;

 public:
  std::vector<primitive*> bvh;
  std::vector<primitive*> objects;
  std::vector<ray*> rays;
  scene();

  scene(std::vector<primitive*> obs, rgb b, camera ci, int w, int h, std::string out, int m);

  ~scene();
  void addObject(primitive *ob);
  void addRay(ray* r);
  void addVertex(vec3 v);
  void addNormal(vec3 n);

  
  void setBVHuse(bool b);
  void setBackground(rgb ci);
  void setCam(camera c);
  void setWidth(int w);
  void setHeight(int h);
  void setOut(std::string o);
  void setDepth(int m);
  void setAmbient(rgb a);

  rgb getBackground();
  camera getCam();
  int getWidth();
  int getHeight();
  std::string getOut();
  int getDepth();
  rgb getAmbient();
  vec3 getVertex(int index);
  vec3 getNormal(int index);

  void printSeceneInfo();
  
  bool findIntersection(ray* r,hitInformation &hit);
  rgb EvaluateRayTree(ray* r, int limit);

  rgb ApplyLightingModel(ray* r, hitInformation hit, int limit);

};



#endif  //SCENE_H_
