//
//  IntersectionInfo.cpp
//  HW3
//
//  Created by Tomislav Slade on 5/25/16.
//  Copyright © 2016 Tomislav Slade. All rights reserved.
//

#include "IntersectionInfo.hpp"
#include "Ray.hpp"
IntersectionInfo::IntersectionInfo(float t, int objindex, vec3 P, vec3 N, Ray * ray){
    this->t = t;
    this->objindex = objindex;
    this->P = P;
    this->N = N;
    this->ray = ray;
}

IntersectionInfo::~IntersectionInfo(){
    
}