#include <iostream>
#include <ogl/glew.h>
#include <ogl/freeglut.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shaders.h"
#include "Model.h"

#include <sstream>
std::string toString(const int &i) {
    std::stringstream ss;
    ss << i;
    return ss.str();
}

#define I glm::mat4(1.0)

void funInit();
void funReshape(int w, int h);
void funDisplay();

void drawObject(Model model, glm::vec3 color, glm::mat4 P, glm::mat4 V, glm::mat4 M);
void drawSuelo (glm::mat4 P, glm::mat4 V, glm::mat4 M);
void drawAspa  (glm::mat4 P, glm::mat4 V, glm::mat4 M);
void drawHelice(glm::mat4 P, glm::mat4 V, glm::mat4 M);
void drawBrazo (glm::mat4 P, glm::mat4 V, glm::mat4 M);
void drawAlas  (glm::mat4 P, glm::mat4 V, glm::mat4 M);
void drawCuerpo(glm::mat4 P, glm::mat4 V, glm::mat4 M);
void drawDron  (glm::mat4 P, glm::mat4 V, glm::mat4 M);

// Shaders
   Shaders shaders;
   
// Modelos
   Model modelPlane;
   Model modelCone;
   Model modelCylinder;
   Model modelSphere;
   
// Viewport
   int w = 600;
   int h = 600;
   
   
   
   
      
int main(int argc, char** argv) {

 // Inicializamos GLUT
    glutInit(&argc, argv);
    glutInitContextVersion(3,3);   
    glutInitContextFlags(GLUT_FORWARD_COMPATIBLE); 
    glutInitContextProfile(GLUT_CORE_PROFILE); 
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(w,h);
    glutInitWindowPosition(50,50);
    glutCreateWindow("Practica 1");
    
 // Inicializamos GLEW
    glewExperimental = GL_TRUE;
    GLenum err = glewInit();
    if(GLEW_OK != err) {
        std::cout << "Error: " << glewGetErrorString(err) << std::endl;
        return false;
    }
    std::cout << "Status: Using GLEW " << glewGetString(GLEW_VERSION) << std::endl;
    const GLubyte *oglVersion = glGetString(GL_VERSION);
    std::cout <<"This system supports OpenGL Version: " << oglVersion << std::endl;
    
 // Inicializaciones específicas
    funInit();
    
 // Configuración CallBacks
    glutReshapeFunc(funReshape);
    glutDisplayFunc(funDisplay);
              
 // Bucle principal
    glutMainLoop();
    
    return 0;
}

void funInit() {
      
 // Test de profundidad
    glEnable(GL_DEPTH_TEST);
    glPolygonOffset(1.0,1.0);
    
 // Shaders
    shaders.initShaders("resources/shaders/vshader.glsl","resources/shaders/fshader.glsl");
    
 // Modelos
    modelPlane.initModel("resources/models/plane.obj");
    modelCone.initModel("resources/models/cone.obj");
    modelCylinder.initModel("resources/models/cylinder.obj");
    modelSphere.initModel("resources/models/sphere.obj");
    
}

void funReshape(int wnew, int hnew) {
    
 // Configuración del Viewport
    glViewport(0, 0, wnew, hnew);

 // Captura de w y h
    w = wnew;
    h = hnew;
    
}

void funDisplay() {
    
 // Borramos el buffer de color
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
 // Indicamos los shaders a utilizar
    shaders.useShaders();
    
 // Matriz de Proyección P (Perspectiva)
    float fovy   = 30.0;
    float nplane =  0.1;
    float fplane = 25.0;
    float aspect = (float)w/(float)h;
    glm::mat4 P  = glm::perspective(glm::radians(fovy), aspect, nplane, fplane); 

 // Matriz de Vista V (Cámara)
    glm::vec3 pos   (4.0, 2.0,  4.0);
    glm::vec3 lookat(0.0, 0.0,  0.0);
    glm::vec3 up    (0.0, 1.0,  0.0);
    glm::mat4 V = glm::lookAt(pos, lookat, up);
    
 // Dibujamos la escena
    
    //Dibujamos el suelo
    drawSuelo(P,V,I);
    
    //Apartado 3
    //drawDron(P,V,I);
    
    glm::mat4 TA = glm::translate(I, glm::vec3(-2.0, 0.5, -2.0));
    glm::mat4 RA  = glm::rotate(I, glm::radians(27.f),  glm::vec3(0.0, 1.0, 0.0));
    
    glm::mat4 SB = glm::scale(I, glm::vec3(0.5, 0.5, 0.5));
    glm::mat4 RB  = glm::rotate(I, glm::radians(30.f),  glm::vec3(1.0, 0.0, 1.0));
    
    //Aparatdo 4.a
    glm::mat4 MA = TA*RA;
    //drawDron(P,V,MA);

    //Apartado 4.b
    glm::mat4 MB = TA*RB*RA*SB;
    drawDron(P,V,MB);
    
    
    
    
    
 // Intercambiamos los buffers
    glutSwapBuffers();
    
}

void drawObject(Model model, glm::vec3 color, glm::mat4 P, glm::mat4 V, glm::mat4 M) {
 
    shaders.setMat4("uPVM",P*V*M);
    glEnable(GL_POLYGON_OFFSET_FILL);
    shaders.setVec3("uColor",color);
    model.renderModel(GL_FILL);
    glDisable(GL_POLYGON_OFFSET_FILL);

    
    shaders.setVec3("uColor",glm::vec3(color.r*0.75, color.g*0.75, color.b*0.75));
    model.renderModel(GL_LINE);
    
}

void drawCuerpo(glm::mat4 P, glm::mat4 V, glm::mat4 M){
    glm::mat4 S = glm::scale(I, glm::vec3(0.5, 0.2, 0.5));   
    drawObject(modelSphere,glm::vec3(0.0, 1.0, 0.0),P,V,M*S);
}

void drawBrazo(glm::mat4 P, glm::mat4 V, glm::mat4 M){
    glm::mat4 S = glm::scale(I, glm::vec3(0.05, 0.5, 0.05));
    glm::mat4 T = glm::translate(I, glm::vec3(0.0, 1.0, 0.0));
    glm::mat4 RzN90  = glm::rotate(I, glm::radians(-90.f),  glm::vec3(0.0, 0.0, 1.0));
    
    drawObject(modelCylinder,glm::vec3(0.0, 0.0, 1.0),P,V,M*RzN90*S*T);
    
    glm::mat4 Ry45  = glm::rotate(I, glm::radians(45.0f),glm::vec3(0.0, 1.0, 0.0));
    glm::mat4 T2    = glm::translate(I, glm::vec3(1.0, 0.0, 0.0));
    drawHelice(P,V,M*T2*Ry45);
}

void drawAlas(glm::mat4 P, glm::mat4 V, glm::mat4 M){
   glm::mat4 Ry72  = glm::rotate(I, glm::radians(72.f),glm::vec3(0.0, 1.0, 0.0));
   drawBrazo(P,V,M);
   drawBrazo(P,V,M*Ry72);
   drawBrazo(P,V,M*Ry72*Ry72);
   drawBrazo(P,V,M*Ry72*Ry72*Ry72);
   drawBrazo(P,V,M*Ry72*Ry72*Ry72*Ry72);
   
}

void drawSuelo(glm::mat4 P, glm::mat4 V, glm::mat4 M) {
    
    glm::mat4 S = glm::scale(I, glm::vec3(2.0, 1.0, 2.0));
    drawObject(modelPlane,glm::vec3(0.5, 0.5, 0.5),P,V,M*S);

}

void drawAspa(glm::mat4 P, glm::mat4 V, glm::mat4 M) {
    glm::mat4 S = glm::scale(I, glm::vec3(0.0372, 0.0618, 0.011));
    glm::mat4 T = glm::translate(I, glm::vec3(0.0, -2.75, 0.0));
    drawObject(modelCone,glm::vec3(1.0, 0.0, 0.0),P,V,M*S*T);

}

void drawHelice(glm::mat4 P, glm::mat4 V, glm::mat4 M) {
    glm::mat4 Ry90  = glm::rotate(I, glm::radians(90.f),  glm::vec3(0.0, 1.0, 0.0));
    glm::mat4 Rz90  = glm::rotate(I, glm::radians(90.f),  glm::vec3(0.0, 0.0, 1.0));
    glm::mat4 Rx90  = glm::rotate(I, glm::radians(90.f),  glm::vec3(1.0, 0.0, 0.0));
    glm::mat4 RzN90 = glm::rotate(I, glm::radians(-90.f), glm::vec3(0.0, 0.0, 1.0));
    glm::mat4 RxN90 = glm::rotate(I, glm::radians(-90.f), glm::vec3(1.0, 0.0, 0.0));
    
    drawAspa(P,V,M*Rz90*Ry90);
    drawAspa(P,V,M*Rx90);
    drawAspa(P,V,M*RzN90*Ry90);
    drawAspa(P,V,M*RxN90);
    
}

void drawDron(glm::mat4 P, glm::mat4 V, glm::mat4 M){
    drawCuerpo(P,V,M);
    drawAlas(P,V,M);
}