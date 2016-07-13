//
//  Ray.cpp
//  HW3
//
//  Created by Tomislav Slade on 5/23/16.
//  Copyright © 2016 Tomislav Slade. All rights reserved.
//

#include "Ray.hpp"
using namespace glm;

Ray::Ray(float alphaIn, float betaIn, vec3 p1, vec3 p0){
    this->alpha = alphaIn;
    this->beta = betaIn;
    this->p1 = p1;
    this->p0 = p0;
    
    
}
Ray::~Ray(){
}

Ray::Ray(){
}
