#ifndef HIT_INFO_H_
#define HIT_INFO_H_

#include "primitive.h"
#include "vec3.h"

class primitive;
struct rgb;

struct hitInformation{
  vec3 hitPoint;
  vec3 normal;
  primitive *hitObject;
  rgb hitColor;
  rgb rayColor;
  float incomeEnergy;
  float hitDistance;
  double t;

  hitInformation(){}
};



#endif