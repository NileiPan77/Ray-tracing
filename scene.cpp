#include "scene.h"



scene::scene(){
    background = rgb(0,0,0);
    ambient = rgb(0,0,0);
    film_height = 480;
    film_width = 640;
    output = "raytraced.bmp";
    max_depth = 5;

}

scene::scene(std::vector<primitive*> obs, rgb b, camera ci, int w, int h, std::string out, int m){
    for(size_t i = 0; i < obs.size(); i++){
      objects.push_back(obs[i]);
    }
    background = b;
    camInfo = ci;
    film_height = h;
    film_width = w;
    output = out;
    max_depth = m;
}

scene::~scene(){
    for(size_t i = 0; i < objects.size(); i++){
      objects.pop_back();
    }
    for(size_t i = 0; i < rays.size(); i++){
      rays.pop_back();
    }
    
}

void scene::addObject(primitive *ob){
    objects.emplace_back(ob);
}
void scene::addVertex(vec3 v){

  vertices.push_back(v);
}
void scene::addNormal(vec3 n){
  normals.push_back(n);
}

void scene::addRay(ray* r){
    rays.emplace_back(r);
}

void scene::setBackground(rgb ci){background = ci;}
void scene::setCam(camera c){camInfo = c;}
void scene::setWidth(int w){film_width = w;}
void scene::setHeight(int h){film_height = h;}
void scene::setOut(std::string o){output = o;}
void scene::setDepth(int m){max_depth = m;}
void scene::setAmbient(rgb a){ambient = a;}

rgb scene::getBackground(){return background;}
camera scene::getCam(){return camInfo;}
int scene::getWidth(){return film_width;}
int scene::getHeight(){return film_height;}
std::string scene::getOut(){return output;}
int scene::getDepth(){return max_depth;}
rgb scene::getAmbient(){
    //rgb a(ambient.r,ambient.g,ambient.b);
    //ambient = rgb(0,0,0);
    return ambient;
}

vec3 scene::getVertex(int index){

  return vertices[index];

}
vec3 scene::getNormal(int index){

  return normals[index];
}

void scene::setBVHuse(bool b){
  BVHuse = b;
}


void scene::printSeceneInfo(){
    printf("Background: %f %f %f\n",background.r,background.g,background.b);
    printf("ambient: %f %f %f\n",ambient.r,ambient.g,ambient.b);
    printf("Camera_pos: x: %f, y: %f, z: %f\n", camInfo.camera_pos.x,camInfo.camera_pos.y,camInfo.camera_pos.z );
    printf("Camera_fwd: x: %f, y: %f, z: %f\n", camInfo.camera_fwd.x,camInfo.camera_fwd.y,camInfo.camera_fwd.z );
    printf("Camera_up: x: %f, y: %f, z: %f\n", camInfo.camera_up_.x,camInfo.camera_up_.y,camInfo.camera_up_.z );
    printf("Camera_right: x: %f, y: %f, z: %f\n", camInfo.camera_right.x,camInfo.camera_right.y,camInfo.camera_right.z );
    printf("camera_fov: %f\n",camInfo.camera_fov_ha);

    printf("Width: %d\n",film_width);
    printf("Height: %d\n",film_height);

    printf("Depth: %d\n",max_depth);

    std::cout << "Output name:" << output << std::endl;

    printf("Ray informations: \n");
    printf("%ld total rays.",rays.size());
    for(size_t i = 0; i < rays.size(); i++){
      rays[i]->print();
    }

    printf("primitive informations: \n");
    printf("%ld total primtives",objects.size());
    for(size_t i = 0; i < objects.size(); i++){
      objects[i]->print();
    }

  }

  
bool scene::findIntersection(ray* r,hitInformation &hit){

    float min_t = INFINITY;
    primitive *min_p = nullptr;
    r->setDirection(r->getDirection().normalized());
    hit.t = -1;
    if(!BVHuse){
        
        for(size_t i = 0; i < objects.size(); i++){
            float t = objects[i]->rayIntersect(r,hit);
            float dist = r->getPosition().distance(r->getPosition() + t * r->getDirection());
            if(t >= 0 && min_t > dist ){

                min_p = objects[i];
                hit.hitObject = objects[i];
                min_t = dist;
                hit.hitDistance = min_t ;
                hit.hitPoint = r->getPosition() + t * r->getDirection();
                hit.hitDistance = r->getPosition().distance(hit.hitPoint);

            }
        }
        if(min_p == nullptr){
          return false;
        }
        hit.normal = (hit.hitPoint - min_p->getPosition()).normalized();
        hit.rayColor = r->getColor();

        return true;
    }else{
      r->setDirection(r->getDirection().normalized());

      float t = bvh[0]->rayIntersect(r,hit);
      
      return t >= 0 ? true:false;
    }
    
} 

rgb scene::EvaluateRayTree(ray* r, int limit){
    bool hit;
    hitInformation hitInfo;
    hit = findIntersection(r,hitInfo);
    if(hit && limit <= max_depth){
      return ApplyLightingModel(r,hitInfo,limit);
    }else{
      return getBackground();
    }
}

rgb scene::ApplyLightingModel(ray* r, hitInformation hit, int limit){
  rgb contribution;
    r->setTraveled(r->getTraveled()+hit.hitDistance);
    for(size_t i = 0; i < rays.size(); i++){
      
      
      rays[i]->updateDirection(hit.hitPoint);
      rays[i]->updatePosition(hit.hitPoint);

      
      vec3 hitToRay = (-1 * rays[i]->getDirection()).normalized();

      straightRay shadow(hit.hitPoint,hitToRay,rgb(0,0,0),0);
      hitInformation shadow_hit;
      bool blocked = findIntersection(new straightRay(hit.hitPoint + 0.001 * hitToRay,hitToRay,rgb(0,0,0),0), shadow_hit);

      float hitToRayDist = hit.hitPoint.distance(rays[i]->getPosition());
      if(blocked &&  shadow_hit.hitDistance < hitToRayDist){
        
        continue;
      }
     

      vec3 hitToCam = (camInfo.camera_pos - hit.hitPoint).normalized();
      vec3 hitToEye = (r->getPosition() - hit.hitPoint).normalized();

      hit.hitDistance = hit.hitPoint.distance(r->getPosition());

      vec3 refDirection = (rays[i]->getDirection() - (2 * 
           dot(rays[i]->getDirection(), hit.normal) * 
           hit.normal)).normalized();

      rgb diffuseComponent = hit.hitObject->getMatt().diffuse * (fmax(0.0, dot(hit.normal,hitToRay)));
      rgb specularComponent = hit.hitObject->getMatt().specular * (pow(fmax(0.0, dot(hitToEye, refDirection)),hit.hitObject->getMatt().phong));

      rgb light = (rays[i]->attenuation(hit.hitPoint));


      contribution += light * (diffuseComponent + specularComponent);

    }

    bool inside = false;
    if(dot(r->getDirection(),hit.normal) > 0){
      hit.normal = -1.0 * hit.normal;
      inside = true;
    }
    float faceAngle = -1.0 * (dot(r->getDirection(),hit.normal));
    float fresnel = pow(1-faceAngle,5.0) * 0.9 + 0.1;

    vec3 eyeRayReflect = (r->getDirection() - (2 * dot(hit.normal, r->getDirection()) * hit.normal)).normalized();
    
    straightRay *reflectRay = new straightRay(hit.hitPoint + 0.001f * eyeRayReflect, eyeRayReflect, rgb(0,0,0), r->getTraveled());

    contribution +=  hit.hitObject->getMatt().specular *  EvaluateRayTree(reflectRay,limit+1);

    float nr = (inside) ? hit.hitObject->getMatt().refractionIndex : 1/hit.hitObject->getMatt().refractionIndex;
    float cosi = dot(-1.0 * hit.normal,r->getDirection());
    float k = 1 - nr * nr * (1 - cosi * cosi);
    vec3 refraDir = (nr * r->getDirection() + (nr * cosi - sqrt(k)) * hit.normal).normalized();
    straightRay *refractRay = new straightRay(hit.hitPoint - 0.001f * refraDir,refraDir,rgb(0,0,0), r->getTraveled());
    contribution +=  hit.hitObject->getMatt().transmissive * (1.0-fresnel) * EvaluateRayTree(refractRay, limit+1);
    
    delete reflectRay;
    delete refractRay;
    contribution += ambient * hit.hitObject->getMatt().ambient;
    return contribution;


}

