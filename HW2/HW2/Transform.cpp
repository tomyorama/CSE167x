// Transform.cpp: implementation of the Transform class.

// Note: when you construct a matrix using mat4() or mat3(), it will be COLUMN-MAJOR
// Keep this in mind in readfile.cpp and display.cpp
// See FAQ for more details or if you're having problems.

#include "Transform.h"
#include <stdio.h>
// Helper rotation function.  Please implement this.
mat3 Transform::rotate(const float degrees, const vec3& axis)
{
    float radians = degrees*pi/180;
    vec3 normAxis = glm::normalize(axis);
    
    float oneMinusCos = 1 - cos(radians);
    float mat11 = cos(radians)+(normAxis.x*normAxis.x)*oneMinusCos;
    float mat12 = (normAxis.x*normAxis.y)*oneMinusCos-normAxis.z*sin(radians);
    float mat13 = normAxis.x*normAxis.z*oneMinusCos+(normAxis.y*sin(radians));
    float mat21 = normAxis.y*normAxis.x*oneMinusCos+normAxis.z*sin(radians);
    float mat22 = cos(radians) + normAxis.y*normAxis.y*oneMinusCos;
    float mat23 = normAxis.y*normAxis.z*oneMinusCos - normAxis.x*sin(radians);
    float mat31 = normAxis.z*normAxis.x*oneMinusCos - normAxis.y*sin(radians);
    float mat32 = normAxis.z*normAxis.y*oneMinusCos + normAxis.x*sin(radians);
    float mat33 = cos(radians) + normAxis.z*normAxis.z*oneMinusCos;
    return glm::transpose(mat3(vec3(mat11,mat12,mat13),
                               vec3(mat21,mat22,mat23),
                               vec3(mat31,mat32,mat33)
                               ));
    
}

void Transform::left(float degrees, vec3& eye, vec3& up)
{
    mat3 rotationMx = rotate(degrees, up);
    eye =rotationMx*eye;
    up = rotationMx*up;
    //printf("Coordinates: %.2f, %.2f, %.2f; distance: %.2f\n", eye.x, eye.y, eye.z, sqrt(pow(eye.x, 2) + pow(eye.y, 2) + pow(eye.z, 2)));
}

void Transform::up(float degrees, vec3& eye, vec3& up)
{
    vec3 w = glm::cross(up, eye);
    mat3 rotationMx= rotate(-degrees, w);
    
    eye =rotationMx*eye;
    up = rotationMx*up;
}

mat4 Transform::lookAt(const vec3 &eye, const vec3 &center, const vec3 &up)
{
    vec3 w = glm::normalize(eye);
    vec3 u = glm::normalize(glm::cross(up,eye));
    vec3 v = glm::normalize(glm::cross(w,u));
    // You will change this return call
    return glm::transpose(mat4(vec4(u.x,u.y,u.z,glm::dot(u, -eye)),
                               vec4(v.x,v.y,v.z,glm::dot(v, -eye)),
                               vec4(w.x,w.y,w.z,glm::dot(w, -eye)),
                               vec4(0,0,0,1)
                               ));
}

mat4 Transform::perspective(float fovy, float aspect, float zNear, float zFar)
{
    mat4 ret(0);
    
    float scale = 1 / tan(fovy * 0.5 * M_PI / 180);
    ret[0][0] = scale/aspect; // scale the x coordinates of the projected point
    ret[1][1] = scale; // scale the y coordinates of the projected point
    ret[2][2] = -zFar / (zFar - zNear); // used to remap z to [0,1]
    ret[3][2] = -zFar * zNear / (zFar - zNear); // used to remap z [0,1]
    ret[2][3] = -1; // set w = -z
    ret[3][3] = 0;
    
    return ret;
    //return glm::perspective(fovy,aspect,zNear,zFar);
}

mat4 Transform::scale(const float &sx, const float &sy, const float &sz)
{
    //printf("scale: %.2f, %.2f, %.2f;", sx, sy, sz);
    return glm::transpose(mat4(vec4(sx,0,0,0),
                               vec4(0,sy,0,0),
                               vec4(0,0,sz,0),
                               vec4(0,0,0,1)
                               ));
}

mat4 Transform::translate(const float &tx, const float &ty, const float &tz)
{
    //printf("translate: %.2f, %.2f, %.2f;", tx, ty, tz);
    return glm::transpose(mat4(vec4(1,0,0,tx),
                               vec4(0,1,0,ty),
                               vec4(0,0,1,tz),
                               vec4(0,0,0,1)
                               ));
}

// To normalize the up direction and construct a coordinate frame.
// As discussed in the lecture.  May be relevant to create a properly
// orthogonal and normalized up.
// This function is provided as a helper, in case you want to use it.
// Using this function (in readfile.cpp or display.cpp) is optional.

vec3 Transform::upvector(const vec3 &up, const vec3 & zvec)
{
    vec3 x = glm::cross(up,zvec);
    vec3 y = glm::cross(zvec,x);
    vec3 ret = glm::normalize(y);
    return ret;
}


Transform::Transform()
{
    
}

Transform::~Transform()
{
    
}
