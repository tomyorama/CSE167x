# version 120 

/* This is the fragment shader for reading in a scene description, including 
   lighting.  Uniform lights are specified from the main program, and used in 
   the shader.  As well as the material parameters of the object.  */

// Mine is an old machine.  For version 130 or higher, do 
// in vec4 color;  
// in vec3 mynormal; 
// in vec4 myvertex;
// That is certainly more modern

varying vec4 color;
varying vec3 mynormal; 
varying vec4 myvertex; 

const int numLights = 10;
uniform bool enablelighting; // are we lighting at all (global).
uniform vec4 lightposn[numLights]; // positions of lights
uniform vec4 lightcolor[numLights]; // colors of lights
uniform int numused;               // number of lights used

// Now, set the material parameters.  These could be varying and/or bound to 
// a buffer.  But for now, I'll just make them uniform.  
// I use ambient, diffuse, specular, shininess as in OpenGL.  
// But, the ambient is just additive and doesn't multiply the lights.  

uniform vec4 ambient;
uniform vec4 diffuse;
uniform vec4 specular;
uniform vec4 emission;
uniform float shininess;

//// Constants to set up lighting on the teapot
//const vec4 light0posn = vec4(0,5,-10,1);    // Position of light 0
//const vec4 light1posn = vec4(0,5,-10,1);  // Position of light 1
//const vec4 light0color = vec4(0.6,0.3,0,1);    // Specular of light 0
//const vec4 light1color = vec4(0,0.3,0.6,1);   // Specular of light 1
//const vec4 specularH = vec4(1,1,1,1);                 // Specular on teapot
//const vec4 diffuseH = vec4(0.2,0.2,0.2,1);        // Diffuse on teapot
//const vec4 ambientH = vec4(0.2,0.2,0.2,1);         // Ambient on teapot
//const float shininessH = 100;


vec4 ComputeLight (const in vec3 direction, const in vec4 lightcolor, const in vec3 normal, const in vec3 halfvec, const in vec4 mydiffuse, const in vec4 myspecular, const in float myshininess) {
    
    float nDotL = dot(normal, direction)  ;
    vec4 lambert = mydiffuse * lightcolor * max (nDotL, 0.0) ;
    
    float nDotH = dot(normal, halfvec) ;
    vec4 phong = myspecular * lightcolor * pow (max(nDotH, 0.0), myshininess) ;
    
    vec4 retval = lambert + phong ;
    return retval ;
}

void main (void)
{
    
    // Shininess of teapot
    //GLfloat light0[4],light1[4];
    if (enablelighting) {
        vec4 finalcolor;
        
        const vec3 eyepos = vec3(0,0,0) ;
        vec4 _mypos = gl_ModelViewMatrix * myvertex ;
        vec3 mypos = _mypos.xyz / _mypos.w ; // Dehomogenize current location
        vec3 eyedirn = normalize(eyepos - mypos) ;
        
        // Compute normal, needed for shading.
        // Simpler is vec3 normal = normalize(gl_NormalMatrix * mynormal) ;
        vec3 _normal = (gl_ModelViewMatrixInverseTranspose*vec4(mynormal,0.0)).xyz ;
        vec3 normal = normalize(_normal) ;
        
        vec4 allColors = vec4(0,0,0,0);
        for (int i = 0 ; i < numused ; i++) {
            // Light
            //vec3 position0 = light0posn.xyz / light0posn.w ;
            vec3 position0;
            vec3 direction0;
            if(lightposn[i].w != 0){
                position0 = lightposn[i].xyz / lightposn[i].w ;
                direction0 = normalize (position0 - mypos) ;
            }else{
                
                direction0 = normalize(lightposn[i].xyz);
            }
            vec3 half0 = normalize (direction0 + eyedirn) ;
            vec4 col0 = ComputeLight(direction0, lightcolor[i], normal, half0, diffuse, specular, shininess);
            allColors += col0;
        }
        gl_FragColor = ambient + allColors;
        // YOUR CODE FOR HW 2 HERE
        // A key part is implementation of the fragment shader

        // Color all pixels black for now, remove this in your implementation!
        //finalcolor = vec4(0,0,0,1);

        //gl_FragColor = vec4(1,0,0,1);
    } else {
        gl_FragColor = color; 
    }
}
