#ifndef VEC3_H
#define VEC3_H

//Small vector library
// Represents a vector as 3 floats



struct vec3{
  double x,y,z;

  vec3(double x, double y, double z) : x(x), y(y), z(z) {}
  vec3() : x(0), y(0), z(0) {}

  float operator[](int index){
    if(index == 0) return x;
    else if(index == 1) return y;
    else if(index == 2) return z;
  }
  vec3& operator=(const vec3& that){
    x = that.x;
    y = that.y;
    z = that.z;
    return *this;
  }
  //Clamp each component (used to clamp pixel colors)
  vec3 clampTo1(){
    return vec3(fmin(x,1),fmin(y,1),fmin(z,1));
  }

  //Compute vector length (you may also want length squared)
  double length(){
    return sqrt(x*x+y*y+z*z);
  }

  double lengthSquare(){
    return x*x+y*y+z*z;
  }

  //Create a unit-length vector
  vec3 normalized(){
    double len = sqrt(x*x+y*y+z*z);
    return vec3(x/len,y/len,z/len);
  }
  
  double distance(vec3 t){
    return sqrt(pow(t.x-x,2.0) + pow(t.y-y,2.0) + pow(t.z-z,2.0));
  }
  void print(){
    printf("x: %f, y: %f, z: %f\n",x,y,z);
  }

};

//Multiply float and vector
//TODO: you probably also want to multiply vector and float
inline vec3 operator*(double f, vec3 a){
  return vec3(a.x*f,a.y*f,a.z*f);
}

//Vector-vector dot product
inline double dot(vec3 a, vec3 b){
  return a.x*b.x + a.y*b.y + a.z*b.z;
}

//Vector-vector cross product
inline vec3 cross(vec3 a, vec3 b){
  return vec3(a.y*b.z - a.z*b.y, a.z*b.x - a.x*b.z, a.x*b.y-a.y*b.x);
}

//Vector addition
inline vec3 operator+(vec3 a, vec3 b){
  return vec3(a.x+b.x, a.y+b.y, a.z+b.z);
}

//Vector subtraction
inline vec3 operator-(vec3 a, vec3 b){
  return vec3(a.x-b.x, a.y-b.y, a.z-b.z);
}

#endif
