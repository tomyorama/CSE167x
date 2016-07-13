//
//  IntersectionInfo.hpp
//  HW3
//
//  Created by Tomislav Slade on 5/25/16.
//  Copyright © 2016 Tomislav Slade. All rights reserved.
//

#ifndef IntersectionInfo_hpp
#define IntersectionInfo_hpp

#include <stdio.h>
#include "Ray.hpp"
#include <glm/glm.hpp>
using namespace glm;

class IntersectionInfo
{
public:
    float t;
    int objindex;
    vec3 P;
    vec3 N;
    Ray * ray;
    IntersectionInfo(float t,int objindex, vec3 P, vec3 N, Ray * ray);
    virtual ~IntersectionInfo();
    
};

#endif /* IntersectionInfo_hpp */
