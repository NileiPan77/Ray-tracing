#ifndef RAY_H_
#define RAY_H_

#include <cmath>
#include "vec3.h"
#include "image_lib.h"

struct rgb{
  double r;
  double g;
  double b;

  rgb(){
    r = g = b = 0.0;
  }
  
  rgb(float r,float g, float b){
    this->r = r;
    this->g = g;
    this->b = b;
  }
  
  rgb operator+(const rgb& that){

    return rgb(this->r + that.r, this->g + that.g, this->b + that.b);
  }

  rgb &operator+=(const rgb& that){
    if(this != &that){
      this->r += that.r;
      this->g += that.g;
      this->b += that.b;
    }
    return *this;
  }

  rgb operator*(const rgb& that){

    return rgb(this->r * that.r, this->g * that.g, this->b * that.b);
  }

  bool operator==(const rgb& that){

    return (this->r == that.r) && (g == that.g) && (b == that.b);
  }
  rgb &operator=(const rgb& that){
    this->r = that.r;
    this->g = that.g;
    this->b = that.b;

    return *this;
  }
  

  float brightness(){
    return 0.3 * r + 0.6 * g + 0.1 * b;
  }

  void clampInt(){
    if(r >=1 && g >=1 && b >=1){
      r = g = b = 1;
    }
  }

  void print(){
    printf("r: %f, g: %f, b: %f\n", r,g,b);
  }
  Color trans(){
    return Color(r,g,b);
  }
  
};




inline rgb operator*(rgb a,float f){
    return rgb(a.r*f,a.g*f,a.b*f);
}

class ray{
 private:
  vec3 position;
  vec3 direction;
  rgb color;
  float traveled;
  
 public:
  ray(){}

  ray(vec3 p, vec3 d, rgb c, float e): position(p), direction(d),color(c), traveled(e){
  }

  void setPosition(vec3 p){position = p;}
  void setDirection(vec3 d){direction = d;}
  void setColor(rgb c){color = c;}
  void setTraveled(float e){traveled = e;}
  vec3 getPosition(){
    return position;
  }

  vec3 getDirection(){return direction;}
  rgb getColor(){return color;}
  float getTraveled(){return traveled;}
  
  virtual rgb attenuation(vec3 point) = 0;
  virtual void updatePosition(vec3 p) = 0;
  virtual void updateDirection(vec3 p) = 0;
  virtual void print() = 0;
  virtual void identify(){
    printf("base ray\n");
  }
};

class spotRay: public ray{
private:
  
  float angle1,angle2;
public:
  spotRay(vec3 p, vec3 d, rgb c,float a1,float a2, float e): ray(p,d,c,e){
    angle1 = a1 * (M_PI/180.0);
    angle2 = a2 * (M_PI/180.0);
  }

  void updatePosition(vec3 p) override{}
  void updateDirection(vec3 d) override{}

  rgb attenuation(vec3 point){
    vec3 dirToLight = (point - getPosition()).normalized();
    float dist = (1.0/(pow(point.distance(this->getPosition()),2.0)));
    float angle = acos(dot(getDirection().normalized(),dirToLight));
    //printf("angle: %f\n",angle);
    //printf("dot: %f\n",dot(dirToLight,getDirection()));
    
    if(angle < angle1){
      //return rgb(1,1,1);
      return getColor() * (1.0/(getTraveled()));
    }else if(angle1 < angle && angle < angle2){
      //return rgb(1,1,1);
      return getColor() * (1.0/(dist*dist));
    }else if (angle > angle2){
      return rgb(0,0,0);
    }
    return getColor() * (1.0/(dist* dist));
    //return rgb(1,1,1);
  }

  
  void print(){

    printf("Spot Ray:\n");
    printf("Position: x:%f, y:%f, z:%f\n",getPosition().x,getPosition().y,getPosition().z);
    printf("Direction: x:%f, y:%f, z:%f\n",getDirection().x,getDirection().y,getDirection().z);
    printf("Color: r:%f, g:%f, b: %f\n", getColor().r,getColor().g,getColor().b);
    printf("Angle1: %f\n",angle1);
    printf("Angle2: %f\n",angle2);
    printf("\n");
  }

  void identify() override{
    printf("spot ray\n");
  }
};

class directionalRay: public ray{
  public:
    directionalRay(vec3 p, vec3 d, rgb c, float e): ray(p,d,c,e){}

    void updatePosition(vec3 p) override{
      setPosition(-1000.0 * p);
    }
    void updateDirection(vec3 p) override{}

    rgb attenuation(vec3 point){
      return getColor();
    }
    void print(){

      printf("Directional Ray:\n");
      printf("Position: x:%f, y:%f, z:%f\n",getPosition().x,getPosition().y,getPosition().z);
      printf("Direction: x:%f, y:%f, z:%f\n",getDirection().x,getDirection().y,getDirection().z);
      printf("Color: r:%f, g:%f, b: %f\n", getColor().r,getColor().g,getColor().b);
      printf("\n");
    }
  void identify() override{
    printf("directional ray\n");
  }
};

class pointRay: public ray{
  public:
    pointRay(vec3 p, vec3 d, rgb c, float e): ray(p,d,c,e){
      //printf("point ray\n");
    }

    void updateDirection(vec3 thatPos) override{
      setDirection((thatPos-getPosition()).normalized() );
    }
    void updatePosition(vec3 p) override{}

    rgb attenuation(vec3 point){
      return getColor() * (1.0/(pow(point.distance(this->getPosition()),2.0)));
    }


    void print(){

      printf("Point Ray:\n");
      printf("Position: x:%f, y:%f, z:%f\n",getPosition().x,getPosition().y,getPosition().z);
      printf("Direction: x:%f, y:%f, z:%f\n",getDirection().x,getDirection().y,getDirection().z);
      printf("Color: r:%f, g:%f, b: %f\n", getColor().r,getColor().g,getColor().b);
      printf("\n");
  }

    void identify() override{
      printf("point ray, position: ");
      getPosition().print();
    }
};


class eyeRay: public ray{
  public:
    eyeRay(vec3 p, vec3 d, rgb c, float e): ray(p,d,c,e){
      //printf("eye ray\n");
    }

    void updatePosition(vec3 p){};
    void updateDirection(vec3 p){};

    rgb attenuation(vec3 p){
      return getColor();
    }
    void print(){
      printf("Eye Ray:\n");
      printf("Position: x:%f, y:%f, z:%f\n",getPosition().x,getPosition().y,getPosition().z);
      printf("Direction: x:%f, y:%f, z:%f\n",getDirection().x,getDirection().y,getDirection().z);
      printf("Color: r:%f, g:%f, b: %f\n", getColor().r,getColor().g,getColor().b);
      printf("\n");
    };

    void identify() override{
      printf("eye ray\n");
    }
};

class straightRay: public ray{
  public:
  straightRay(vec3 p, vec3 d, rgb c, float e): ray(p,d,c,e){}

  void updatePosition(vec3 p){};
  void updateDirection(vec3 p){};

  rgb attenuation(vec3 point){
    return getColor() * (1.0/(getTraveled() * getTraveled()));
  }
  void print(){
    printf("Eye Ray:\n");
    printf("Position: x:%f, y:%f, z:%f\n",getPosition().x,getPosition().y,getPosition().z);
    printf("Direction: x:%f, y:%f, z:%f\n",getDirection().x,getDirection().y,getDirection().z);
    printf("Color: r:%f, g:%f, b: %f\n", getColor().r,getColor().g,getColor().b);
    printf("\n");
  };
  void identify() override{
      printf("stright ray\n");
  }

};



#endif   //RAY_H_
