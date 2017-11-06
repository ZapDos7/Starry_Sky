

#include <stdlib.h>
#include <stdio.h>
#include "GL/glut.h" 

#include "visuals.h"


extern model md;

int main(int argc, char* argv[])
{ 
  // initialize GLUT library state
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGBA|GLUT_DOUBLE|GLUT_DEPTH);
 
  glutInitWindowSize(480,480);
  glutInitWindowPosition(50,50);
  
  glutCreateWindow("Solar System");
  
  Setup();
  
  glutDisplayFunc(Render);
  glutReshapeFunc(Resize);
  glutIdleFunc(Idle);
  glutKeyboardFunc(Keyboard);

  //Enter main event handling loop
  glutMainLoop();

  //Free memory
  free(md.obj_points);
  free(md.obj_faces);
  return 0;


}  

