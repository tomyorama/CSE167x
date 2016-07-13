//
//  Ray.hpp
//  HW3
//
//  Created by Tomislav Slade on 5/23/16.
//  Copyright © 2016 Tomislav Slade. All rights reserved.
//

#ifndef Ray_hpp
#define Ray_hpp

#include <stdio.h>
#include <glm/glm.hpp>
using namespace glm;

class Ray
{
public:
    float alpha;
    float beta;
    vec3 p0;
    vec3 p1;
    Ray(float alphaIn, float betaIn, vec3 p1, vec3 p0);
    Ray();
    virtual ~Ray();
    
};


#endif /* Ray_hpp */
