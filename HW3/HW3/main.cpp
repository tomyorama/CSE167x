//
//  main.cpp
//  HW3
//
//  Created by Tomislav Slade on 5/23/16.
//  Copyright © 2016 Tomislav Slade. All rights reserved.
//

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <deque>
#include <stack>
#include "Transform.h"
#include <FreeImage.h>
#include "Ray.hpp"
#include "IntersectionInfo.hpp"
#include <algorithm>
using namespace std;

// Main variables in the program.
#define MAINPROGRAM
#include "variables.h"
#include "readfile.h" // prototypes for readfile.cpp

//int main(int argc, const char * argv[]) {
//    // insert code here...
//    std::cout << "Hello, World!\n";
//    return 0;
//}

bool pointInTriangle(vec3 A, vec3 B, vec3 C, vec3 P)
{
    // Prepare our barycentric variables
    vec3 u = B - A;
    vec3 v = C - A;
    vec3 w = P - A;
    
    vec3 vCrossW = cross(v, w);
    vec3 vCrossU = cross(v, u);
    
    // Test sign of r
    if (dot(vCrossW, vCrossU) < 0)
        return false;
    
    vec3 uCrossW = cross(u, w);
    vec3 uCrossV = cross(u, v);
    
    // Test sign of t
    if (dot(uCrossW, uCrossV) < 0)
        return false;
    
    // At this point, we know that r and t and both > 0.
    // Therefore, as long as their sum is <= 1, each must be less <= 1
    float denom = length(uCrossV);
    float r = length(vCrossW) / denom;
    float t = length(uCrossW) / denom;
    
    
    return (r + t <= 1);
}

void saveScreenshot(BYTE * pixels, string fname)
{
    
    FIBITMAP *img = FreeImage_ConvertFromRawBits(pixels, w, h, w * 3, 24, 0xFF0000, 0x00FF00, 0x0000FF, true);
    
    std::cout << "Saving screenshot: " << fname << "\n";
    
    FreeImage_Save(FIF_PNG, img, fname.c_str(), 0);
}


void printHelp()
{
    //    std::cout << "\npress 'h' to print this message again.\n"
    //    << "press '+' or '-' to change the amount of rotation that\noccurs with each arrow press.\n"
    //    << "press 'i' to run image grader test cases\n"
    //    << "press 'g' to switch between using glm::lookAt and glm::Perspective or your own LookAt.\n"
    //    << "press 'r' to reset the transformations.\n"
    //    << "press 'v' 't' 's' to do view [default], translate, scale.\n"
    //    << "press ESC to quit.\n";
}



Ray createRay(int i, int j){
    vec3 wV = glm::normalize(eye-center);
    vec3 uV = glm::normalize(glm::cross(up,wV));
    vec3 vV = glm::normalize(glm::cross(wV,uV));
    double iF=i+0.5;
    double jF=j+0.5;
    double fovyRad= fovy*pi/180;
    double aspect = w / (float) h;
    float alpha = tan(fovyRad/2)*aspect*((jF - (double)w/2)/(w/2));
    float beta = tan(fovyRad/2)*(((double)h/2 - iF)/(h/2));
    vec3 p1= glm::normalize(alpha*uV + beta*vV - wV);
    return Ray(alpha, beta, p1, eye);
}


IntersectionInfo intersect(Ray ray){
    
    float MINTALLOWED = 0.00001;
    float mint = INFINITY;
    int indexHit = -1;
    vec3 normal;
    vec3 PHIT;
    object * hitObject = NULL;
    for (int i=0; i<numobjects; i++) {
        object * obj = &(objects[i]);
        if(obj->type == triangle){
            vec3 AT = vec3(obj->transform * vec4(vertex[(obj->indices)[0]],1));
            vec3 BT = vec3(obj->transform * vec4(vertex[(obj->indices)[1]],1));
            vec3 CT = vec3(obj->transform * vec4(vertex[(obj->indices)[2]],1));
            vec3 abT = BT-AT;
            vec3 acT = CT-AT;
            vec3 normalC = glm::normalize(cross(abT, acT));
            float t =   (dot(AT,normalC) - dot(ray.p0,normalC))/(dot(ray.p1,normalC));
            vec3 p = ray.p0 + t*ray.p1;
            if(pointInTriangle(AT,BT,CT,p)){
                if(mint > t && t>MINTALLOWED){
                    mint = t;
                    hitObject = obj;
                    indexHit = i;
                    PHIT = p;
                    normal = normalC;
                }
            }
            
        }else if(obj->type == sphere){
            mat4 inverseTransf = inverse(obj->transform);
            vec4 P0T = inverseTransf * vec4(ray.p0,1);
            vec4 P1T = inverseTransf * vec4(ray.p1,0);
            Ray rayTransformed = Ray(ray.alpha,ray.beta,vec3(P1T),vec3(P0T));
            float a = dot(rayTransformed.p1,rayTransformed.p1);
            float b = 2*dot(rayTransformed.p1,(rayTransformed.p0-obj->center));
            float c = dot(rayTransformed.p0-obj->center,rayTransformed.p0-obj->center) - obj->size*obj->size;
            
            float sqrtPart =b*b - 4*a*c;
            
            if(sqrtPart<0){
                //none iracional
            }else{
                float root1= (-b + sqrt(sqrtPart))/(2*a);
                float root2= (-b - sqrt(sqrtPart))/(2*a);
                
                float t = std::min(root1,root2);
                
                if(mint > t && t>MINTALLOWED){
                    mint = t;
                    hitObject = obj;
                    indexHit = i;
                    //calc normal and p
                    vec3 pT = rayTransformed.p0 + t*rayTransformed.p1;
                    vec4 p = obj->transform * vec4(pT,1);
                    vec4 normalSphere = vec4(pT - obj->center,0);
                    PHIT = vec3(p/p.w);
                    normal =  normalize(vec3(transpose(inverseTransf)*normalSphere));
                }
            }
        }else if(obj->type == cube){
            
        }
        
    }
    
    return IntersectionInfo(mint,indexHit,PHIT,normal,&ray);
    
}
vec4 ComputeLight(vec3 direction, vec4 lightcolor, vec3 normal, vec3 halfvec, vec4 mydiffuse, vec4 myspecular, float myshininess, bool isTriangle) {
    
    float nDotL = dot(normal, direction)  ;

    vec4 lambert = mydiffuse * lightcolor * std::max(nDotL, 0.0f);

    float nDotH = dot(normal, halfvec);
    vec4 phong = myspecular * lightcolor * pow(std::max(nDotH, 0.0f), myshininess);
    
    vec4 retval = lambert + phong;
    return retval;
}



Ray * findColor(IntersectionInfo * intersectTmp, BYTE * color,float * specularIn){
    if(intersectTmp->t != INFINITY){
        if(currentDepth>2){
            int ii=0;
            ii++;
        }
        object * obj = &(objects[intersectTmp->objindex]);
        //
        vec4 allColors = vec4(0,0,0,0);
        vec3 rayDirection = normalize(intersectTmp->ray->p1);
        vec3 recDir= normalize(rayDirection -2*dot(rayDirection,intersectTmp->N)*intersectTmp->N);
        Ray recursiveRay = Ray(0,0,recDir,intersectTmp->P);
        
        for (int i = 0 ; i < numused ; i++) {
            vec3 position0 = vec3(lightposn[i*4 + 0],lightposn[i*4 + 1],lightposn[i*4 + 2]) ;
            vec3 direction0;
            float L = 1;
            float V = 1;
            if(lightposn[i*4 + 3] != 0){
                //position0 = position0 / lightposn[i*4 + 3];//no need!!
                direction0 = position0 - intersectTmp->P;
                float d = length(direction0);
                L = 1/(attenuation.x + attenuation.y*d + attenuation.z*d*d);
                Ray rayToLight = Ray(0,0, direction0, intersectTmp->P);
                IntersectionInfo visiblility = intersect(rayToLight);
                if(visiblility.t < 1){
                    V=0;
                }
                direction0=normalize(direction0);
            }else{
                direction0 = normalize(position0);
                Ray rayToLight = Ray(0,0, direction0, intersectTmp->P);
                IntersectionInfo visiblility = intersect(rayToLight);
                if(visiblility.t != INFINITY){
                    V=0;
                }
            }
            vec3 half0 = normalize (direction0 - rayDirection);
            vec4 lightClolorTMP  = vec4(lightcolor[i*4],lightcolor[i*4+1],lightcolor[i*4+2],1);
            vec4 diffuseTMP = vec4(1);
            vec4 specularTMP = vec4(1);
            for (int j = 0; j < 4; j++) {
                diffuseTMP[j] = (obj->diffuse)[j];
                specularTMP[j] = (obj->specular)[j];
            }
            
            vec4 col0 = ComputeLight(direction0,lightClolorTMP , intersectTmp->N, half0, diffuseTMP, specularTMP, obj->shininess,obj->type == triangle);
            allColors += V*L*col0;
        }
        
        BYTE currentColor[3];
        currentColor[0] = ((obj->ambient)[2] + (obj->emission)[2] + allColors[2]) *255;
        currentColor[1] = ((obj->ambient)[1] + (obj->emission)[1] + allColors[1]) *255;
        currentColor[2] = ((obj->ambient)[0] + (obj->emission)[0] + allColors[0]) *255;
        if(currentDepth>1){
            for (int i=0; i<currentDepth-1; i++) {
                currentColor[0] *=specularIn[(i)*3 + 2];
                currentColor[1] *=specularIn[(i)*3 + 1];
                currentColor[2] *=specularIn[(i)*3];
            }
            color[0] += currentColor[0];
            color[1] += currentColor[1];
            color[2] += currentColor[2];
            
        }else{
            color[0] = ((obj->ambient)[2] + (obj->emission)[2] + allColors[2]) *255;
            color[1] = ((obj->ambient)[1] + (obj->emission)[1] + allColors[1]) *255;
            color[2] = ((obj->ambient)[0] + (obj->emission)[0] + allColors[0]) *255;
            
        }
        return &recursiveRay;
    }else{
        
        if(currentDepth>1){
            //none
        }else{
            //first ray
            color[0] = 0;
            color[1] = 0;
            color[2] = 0;
        }
        
        return nullptr;
    }
    
}


void rayTracer(Ray ray, BYTE *color, float *specularIn){
    currentDepth++;
    
    IntersectionInfo intersectionInfo = intersect(ray);
    if(intersectionInfo.t != INFINITY){
        object * obj = &(objects[intersectionInfo.objindex]);
        specularIn[(currentDepth-1)*3] = (obj->specular)[0];
        specularIn[(currentDepth-1)*3+1] = (obj->specular)[1];
        specularIn[(currentDepth-1)*3+2] = (obj->specular)[2];
    }
    Ray * recursiveRay = findColor(&intersectionInfo,color, specularIn);
    
    if(recursiveRay != nullptr && currentDepth<maxDepth){
        rayTracer(*recursiveRay, color, specularIn);
    }
}



int main(int argc, char* argv[])
{
    int doneVal=0;
    maxDepth = 5;
    float allSpecularLights[maxDepth*3];
    attenuation = vec3(1,0,0);
    
    FreeImage_Initialise();
    string fname = readfile(argv[1]);
    int pix = w * h;
    BYTE pixels[3*pix];
    for (int i=0; i<h; i++) {
        for (int j=0; j<w; j++) {
            currentDepth = 0;
            Ray ray = createRay(i,j);
            rayTracer(ray,&pixels[(i*w+j)*3], allSpecularLights);
            //**print progress**
            int percent= (i*j/(double)pix)*100;
            if(percent%100 >= doneVal){
                printf("Done %i %%\n" , percent);
                doneVal+=2;
            }
            //******
        }
    }
    if(fname.empty()){
        saveScreenshot(pixels, "Result.png");
    }
    saveScreenshot(pixels, fname);
    printHelp();
    FreeImage_DeInitialise();
    return 0;
}

