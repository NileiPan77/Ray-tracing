#include "headers/primitive.h"

AABB boxOfBoxs(AABB *box1,AABB *box2){
      vec3 min_1 = vec3(fmin(box1->min_.x,box2->min_.x) ,
                        fmin(box1->min_.y,box2->min_.y) ,
                        fmin(box1->min_.z,box2->min_.z) ) ;

      vec3 max_1 = vec3(fmax(box1->max_.x,box2->max_.x) ,
                        fmax(box1->max_.y,box2->max_.y) ,
                        fmax(box1->max_.z,box2->max_.z) );
        
      return AABB(min_1,max_1);
}

int axisVariable;

primitive::primitive(){}
primitive::~primitive(){}
primitive::primitive(vec3 p): position(p){}
primitive::primitive(vec3 p,material m): position(p), matt(m){}
vec3 primitive::getPosition(){return position;}
material primitive::getMatt(){return matt;}

void primitive::setPosition(vec3 pos){position = pos;}
void primitive::setMateriall(material m){matt = m;}
  


//--------------------sphere-------------------------//
sphere::sphere(){
    radius = 1.0;
    setPosition(vec3(0.0,0.0,0.0));
}
sphere::~sphere(){}

sphere::sphere(vec3 pos, float r): primitive(pos){
radius = r;
}

sphere::sphere(vec3 pos, float r,material m): primitive(pos,m){
radius = r;
}

void sphere::setRadius(float r){
radius = r;
}

float sphere::getRadius(){return radius;}

float sphere::rayIntersect(ray* rs,hitInformation &h){
    //printf("inside\n");
    vec3 dir = rs->getDirection();
    vec3 start = rs->getPosition();
    vec3 center = getPosition();
    float a = dot(dir,dir);
    vec3 toStart = (start - center);
    float b = dot(dir,toStart);
    float c = dot(toStart,toStart) - radius*radius;
    float discr = b*b - a*c;
    if (discr < 0) return -1;
    else{
      float t0 = (-b + sqrt(discr))/(a);
      float t1 = (-b - sqrt(discr))/(a);
      if (t0 > 0.01 && t1 > 0.01){
          if(h.t == -1 || h.t > fmin(t0,t1)){
                    h.t = fmin(t0,t1);
                    h.hitObject = this;
                    h.hitPoint = rs->getPosition() + h.t * rs->getDirection();
                    h.hitDistance = rs->getPosition().distance(h.hitPoint);
                    h.normal = this->getNormal(h.hitPoint);
                    h.rayColor = rs->getColor();
            }
          return fmin(t0,t1);
      }
      else{
        return -1;
      }
    }
    return -1;
  }

vec3 sphere::getNormal(vec3 hitPoint){

    return (hitPoint-getPosition()).normalized();

}
void sphere::print(){

printf("Sphere:\n");
printf("Position: x:%f, y:%f, z:%f\n",getPosition().x,getPosition().y,getPosition().z);
printf("Material: ambient: %f %f %f\n",getMatt().ambient.r,getMatt().ambient.g,getMatt().ambient.b);
printf("Material: diffuse: %f %f %f\n",getMatt().diffuse.r,getMatt().diffuse.g,getMatt().diffuse.b);
printf("Material: specular: %f %f %f\n",getMatt().specular.r,getMatt().specular.g,getMatt().specular.b);
printf("Material: phong : %f\n",getMatt().phong);
printf("Material: transmissive: %f %f %f\n",getMatt().transmissive.r,getMatt().transmissive.g,getMatt().transmissive.b);
printf("Material: refractionIndex : %f\n",getMatt().refractionIndex);
printf("\n");
}
void sphere::identify(){
std::cout << "sphere with radius: " << radius << std::endl;
}
bool sphere::boundingBox(AABB &box){
    box = AABB(getPosition() - vec3(radius,radius,radius), getPosition()+vec3(radius,radius,radius));
    return true;
}
//----------------------Sphere_End--------------------------//

//------------------------Plane-----------------------------//
plane::plane(vec3 p, vec3 n, material m):primitive(p,m){
      normal = n;
}
float plane::rayIntersect(ray *rs,hitInformation& hit){
    float denom = dot(normal,rs->getDirection());
    if(fabs(denom) > 0.001f){
        float t = dot(normal,(getPosition() - rs->getPosition()))/denom;
        if(t > 0.001f){
            hit.t = t;
            hit.hitObject = this;
            return t;
        }
        return -1;
    }
}
void plane::print(){
    printf("plane\n");
}
vec3 plane::getNormal(vec3 hitPoint){
    return normal + (hitPoint - getPosition());
}

bool plane::boundingBox(AABB &box){
    return false;
}
//----------------------Plane_End--------------------------//




//-----------------------Triangle--------------------------//
triangle::triangle(vec3 pos1,vec3 pos2,vec3 pos3,material m): primitive(pos1,m){
    position3 = pos3;
    position2 = pos2;
    normalTri = false;
    normal = cross((pos3 - pos1),(pos2 - pos1)).normalized();
}
triangle::triangle(vec3 pos1,vec3 pos2,vec3 pos3,vec3 n1, vec3 n2, vec3 n3, material m): primitive(pos1,m){
    position3 = pos3;
    position2 = pos2;
    normal1 = n1;
    normal2 = n2;
    normal3 = n3;
    normal = cross((pos3 - pos1),(pos2 - pos1)).normalized();
    normalTri = true;
}

float triangle::magnitude(){
    vec3 alpha = position2 - getPosition();
    vec3 beta = position3 - getPosition();
    return 0.5 * (cross(alpha,beta)).length();
}

float triangle::rayIntersect(ray* rs,hitInformation& h) {
    float denom = dot(normal,rs->getDirection());
    if(fabs(denom) > 0.0001f){
        double t = dot((position2 - rs->getPosition()), normal) / denom;
        if(t >= 0.0001f){
            vec3 hit = rs->getPosition() + t * rs->getDirection();
            //Line from hit to three vertex
            //printf("almost\n");
            vec3 pb = position2 - hit;
            vec3 pc = position3 - hit;
            vec3 pa = getPosition() - hit;

            bary.x = (0.5 * cross(pb,pc).length())/this->magnitude();
            bary.y = (0.5 * cross(pa,pc).length())/this->magnitude();
            bary.z = (0.5 * cross(pa,pb).length())/this->magnitude();
            float sum = bary.x+bary.y+bary.z;
            //printf("sum of bary: %f\n",sum);
            if(bary.x <= 1 && bary.y <= 1 && bary.z <= 1 && bary.x+bary.y+bary.z <= 1.001f){
                //printf("hit\n");
                if(h.t == -1 || h.t > t){
                    h.hitObject = this;
                    h.t = t;
                    h.hitPoint = rs->getPosition() + t * rs->getDirection();
                    h.hitDistance = rs->getPosition().distance(h.hitPoint);
                    h.normal = this->getNormal(h.hitPoint);
                    h.rayColor = rs->getColor();
                }
                

                return t;
            }
        }
    }
    return -1;

}

vec3 triangle::getNormal(vec3 hitPoint){
    if(normalTri){
        // normal1 = bary.x * getPosition();
        // normal2 = bary.y * position2;
        // normal3 = bary.z * position3;
        return bary.x * normal1 + bary.y * normal2 + bary.z * normal3;
    }
    return (cross(hitPoint-position3,hitPoint-position2)).normalized();

}

void triangle::print(){
    if(normalTri){
        printf("Normal Triangle:\n");
        printf("vertex1: x:%f, y:%f, z:%f\n",getPosition().x,getPosition().y,getPosition().z);
        printf("vertex2: x:%f, y:%f, z:%f\n",position2.x,position2.y,position2.z);
        printf("vertex3: x:%f, y:%f, z:%f\n",position3.x,position3.y,position3.z);
        
        printf("normal1:  x:%f, y:%f, z:%f\n",normal1.x,normal1.y,normal1.z);
        printf("normal2:  x:%f, y:%f, z:%f\n",normal2.x,normal2.y,normal2.z);
        printf("normal3:  x:%f, y:%f, z:%f\n",normal3.x,normal3.y,normal3.z);

        printf("Material: ambient: %f %f %f\n",getMatt().ambient.r,getMatt().ambient.g,getMatt().ambient.b);
        printf("Material: diffuse: %f %f %f\n",getMatt().diffuse.r,getMatt().diffuse.g,getMatt().diffuse.b);
        printf("Material: specular: %f %f %f\n",getMatt().specular.r,getMatt().specular.g,getMatt().specular.b);
        printf("Material: phong : %f\n",getMatt().phong);
        printf("Material: transmissive: %f %f %f\n",getMatt().transmissive.r,getMatt().transmissive.g,getMatt().transmissive.b);
        printf("Material: refractionIndex : %f\n",getMatt().refractionIndex);
        printf("\n");
    }else{
        printf("Triangle:\n");
        printf("vertex1: x:%f, y:%f, z:%f\n",getPosition().x,getPosition().y,getPosition().z);
        printf("vertex2: x:%f, y:%f, z:%f\n",position2.x,position2.y,position2.z);
        printf("vertex3: x:%f, y:%f, z:%f\n",position3.x,position3.y,position3.z);

        printf("Material: ambient: %f %f %f\n",getMatt().ambient.r,getMatt().ambient.g,getMatt().ambient.b);
        printf("Material: diffuse: %f %f %f\n",getMatt().diffuse.r,getMatt().diffuse.g,getMatt().diffuse.b);
        printf("Material: specular: %f %f %f\n",getMatt().specular.r,getMatt().specular.g,getMatt().specular.b);
        printf("Material: phong : %f\n",getMatt().phong);
        printf("Material: transmissive: %f %f %f\n",getMatt().transmissive.r,getMatt().transmissive.g,getMatt().transmissive.b);
        printf("Material: refractionIndex : %f\n",getMatt().refractionIndex);
        printf("\n");
    }
    

}
void triangle::identify(){
    std::cout << "triangle" << std::endl;
}

bool triangle::boundingBox(AABB &box){
    double xmin = std::min({getPosition().x,position2.x,position3.x}) - 0.0001;
    double ymin = std::min({getPosition().y,position2.y,position3.y}) - 0.0001;
    double zmin = std::min({getPosition().z,position2.z,position3.z}) - 0.0001;

    double xmax = std::max({getPosition().x,position2.x,position3.x}) + 0.0001;
    double ymax = std::max({getPosition().y,position2.y,position3.y}) + 0.0001;
    double zmax = std::max({getPosition().z,position2.z,position3.z}) + 0.0001;
    box = AABB(vec3(xmin,ymin,zmin),vec3(xmax,ymax,zmax));
    return true;
}


cube::cube(vec3 minp, vec3 maxp,material m): primitive(minp,m){
    maxP = maxp;
}
cube::~cube(){}
void cube::setMaxP(vec3 MAXP){
    maxP = MAXP;
}

vec3 cube::getMaxP(){
    return maxP;
}

float cube::rayIntersect(ray* rs,hitInformation& hit){
    


    return 0.0;
}
vec3 cube::getNormal(vec3 hitPoint){

return (hitPoint-getPosition()).normalized();

}
void cube::print(){}

bool cube::boundingBox(AABB &box){
    box = AABB(getPosition(),maxP);
    return true;
}




AABB::AABB(vec3 minP,vec3 maxP){
    min_ = minP;
    max_ = maxP;
}
bool AABB::rayIntersect(ray* r, float tmin, float tmax){
    for(int i = 0; i < 3; i++){
        
        float t0 = fmin((min_[i] - r->getPosition()[i])/ r->getDirection()[i], (max_[i]-r->getPosition()[i])/r->getDirection()[i]);
        float t1 = fmax((min_[i] - r->getPosition()[i])/ r->getDirection()[i], (max_[i]-r->getPosition()[i])/r->getDirection()[i]);
        tmin = fmax(t0,tmin);
        tmax = fmin(t1,tmax);
        if(tmax <= tmin){
            return false;
        }
    }
    return true;
}


static bool compare(primitive* a,primitive* b){
    AABB *l = new AABB;
    AABB *r = new AABB;
    if(!a->boundingBox(*l) || !b->boundingBox(*r)){
        perror("error");
        exit(EXIT_FAILURE);
    }
    return l->min_[axisVariable] < r->min_[axisVariable]  ? false : true;
}

std::vector<primitive*> BVHNode::splitObjects(std::vector<primitive*> objects, int start, int end){
    std::vector<primitive*> ret;
    for (int i = 0; i <= end - start; i++){
        ret.push_back(objects[i+start]);
    }
    return ret;
}

BVHNode::BVHNode(std::vector<primitive*> objects){
    axisVariable = int(3 * drand48());
    std::sort(objects.begin(),objects.begin(),compare);
    //printf("object size: %ld\n",objects.size());
    if(objects.size() == 1){
        //printf("here:1 \n");

        left = right = objects[0];
    }else if(objects.size() == 2){
        //printf("here:2 \n");
        left = objects[0];
        right = objects[1];
        
    }else{
        //printf("object size: %ld\n",objects.size());

        left = new BVHNode(splitObjects(objects,0,objects.size()/2-1));
        right = new BVHNode(splitObjects(objects,objects.size()/2,objects.size()-1));
    }
    AABB *leftChild = new AABB();
    AABB *rightChild = new AABB();
    // left->identify();
    // right->identify();
    if(!left->boundingBox(*leftChild) || !right->boundingBox(*rightChild)){
        perror("error");
        exit(EXIT_FAILURE);
    }
    
    
    this->box = boxOfBoxs(leftChild,rightChild);
    //printf("size: %ld finished\n",objects.size());
}
void BVHNode::identify(){
    printf("BVH Node\n");
}

float BVHNode::rayIntersect(ray *rs,hitInformation& hit){
    // box->max_.print();
    // box->min_.print();
    if(box.rayIntersect(rs,0,1000)){
        //printf("in\n");
        hitInformation left_hit = hit;
        hitInformation right_hit = hit;
        bool hit_left = (left->rayIntersect(rs,left_hit) == -1) ? false : true;
        //left->identify();
        //printf("left hit, t: %ld\n",left_hit.t);
        bool hit_right = (right->rayIntersect(rs,right_hit) == -1) ? false : true;
        //right->identify();
        
        //printf("right hit, t: %ld\n",right_hit.t);

        if(hit_left && hit_right){
            hit = left_hit.t < right_hit.t ? left_hit : right_hit;
            return hit.t;
        }
        else if(hit_left){
            hit = left_hit;
            return hit.t;
        }
        else if(hit_right){
            hit = right_hit;
            return hit.t;
        }

        return -1;
    }
    return -1;

}
void BVHNode::print(){

}

vec3 BVHNode::getNormal(vec3 hitPoint){
    
}
bool BVHNode::boundingBox(AABB &b){
    b = this->box;
    return true;
}