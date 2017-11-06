#include <stdio.h>
#include <stdlib.h>
#include "GL/glut.h"
#include "visuals.h"
#include <string.h>
#include <time.h>

// glowing settings
float opacity = 0.0f;
float changeRate = 0.008f;
bool sunReverse = false;

// camera loc
float camera_y = 0.0f;
float zoom = 1.0f;

//planet1 stats
float angle1 = 0;
float angle2 = 0;
//float angle3 = 0;

bool isPause = false;

time_t myrandomtime;

// model
model md;

void createStar(float x, float y, float z, float radius){
	glPushMatrix();
		glTranslatef(x, y, z);
		glColor3f(1.0f, 1.0f, 1.0f);
		glutSolidSphere(radius,30,24);
		glColor4f(1.0, 1.0f, 1.0f, opacity);
		glutSolidSphere(radius + 0.5,30,24);
	glPopMatrix();
}

void Keyboard(unsigned char key,int x,int y)
{
	switch(key)
	{
	case 'w': zoom += 5.0f;
		break;
	case 's': zoom -= 5.0f;
		break;
	case 'a' : camera_y += 5.0f;
		break;
	case 'd' : camera_y -= 5.0f;
		break;
	case 'x': exit(0);
		break;
	case 'p': isPause = isPause ? false : true;
		break;
	default : break;
	}

	glutPostRedisplay();

}


void Render()
{    
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  // Clean up the colour of the window
													   // and the depth buffer

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  glTranslatef(0.0f, 0.0f, -200.0f + zoom); 

  glPushMatrix();
    glColor4f(1.0f, 0.8f, 0.2f, 1.0f);
    glutSolidSphere(20,30,24); // the sun
  glPopMatrix();
  glPushMatrix();
    glColor4f(1.0, 0.8f, 0.2f, opacity); //make second sphere transparent
    glutSolidSphere(20 + opacity * 10,30,24); // sun's glow
  glPopMatrix();

  // Rotate world
  glRotatef(camera_y,0,1,0);

  // Create Planets
  //Planet2
  glPushMatrix();
    glColor4f(0.0f,0.0f,0.96f, 1.0);
	glRotatef(angle1, 0, -200.0f, 0);
	glTranslatef(120.0f, 0, 0);
	glRotatef(angle2, 0, 1, 0);
	glScalef(0.015f, 0.015f, 0.015f);
	DisplayModel(md);
  glPopMatrix();

  //Planet1
  glPushMatrix();
    glColor4f(1.0f,0.0f,0.0f, 1.0f);
	glRotatef(angle1, 0, -200.0f, 0);
	glTranslatef(120.0f, 0, 0);
	
	glRotatef(angle2, 0, 20, 0);
	glTranslatef(20.0f, 0, 0);
	glScalef(0.005f, 0.005f, 0.005f);
	DisplayModel(md);
  glPopMatrix();

  

  
  
  

  // Create stars at random locations
  srand(myrandomtime);
  for(int i = 0;i < 70; i++){
	  createStar(rand() % 600 - 300, rand() % 600 - 300, rand() % 600 - 300, rand() % 1 + 0.5);
  }


	glutSwapBuffers();        
	
}

//-----------------------------------------------------------


void Resize(int w, int h)
{ 
  // define the visible area of the window ( in pixels )
  if (h==0) h=1;
  glViewport(0,0,w,h); 

  // Setup viewing volume

  glMatrixMode(GL_PROJECTION); 
  glLoadIdentity();
 

  float aspect = (float)w/(float)h;             /// aspect ratio
  gluPerspective(60.0f, aspect, 0.3, 700.0);
	
}

void Idle()
{
	if(!isPause){
		if(opacity >= 0.7){
			sunReverse = true;
		}
		else if(opacity <= 0){
			sunReverse = false;
		}
		if(sunReverse){
			opacity -= changeRate;
		}
		else{
			opacity += changeRate;
		}

		angle1 += 0.6;
		angle2 += 2;
	}
	glutPostRedisplay();
}

//-----------------------------------------------------------

void Setup()
{ 
	myrandomtime = time(NULL);
	ReadFile(&md);

   glEnable( GL_CULL_FACE );

   glShadeModel( GL_SMOOTH );
   	//glShadeModel (GL_FLAT);
   glEnable(GL_DEPTH_TEST);
   glDepthFunc( GL_LEQUAL );      
   glClearDepth(1.0); 	

 
	// polygon rendering mode
	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial( GL_FRONT, GL_AMBIENT_AND_DIFFUSE );
  
	// set up transparency
	glEnable (GL_BLEND);
	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//Set up light source
	GLfloat light_position[] = { 0.0, 0, -200, 0.0 };
	glLightfv( GL_LIGHT0, GL_POSITION, light_position);

	//GLfloat ambientLight[] = { 0.3, 0.3, 0.3, 1.0 };
	//GLfloat diffuseLight[] = { 0.8, 0.8, 0.8, 1.0 };
	//GLfloat specularLight[] = { 1.0, 1.0, 1.0, 1.0 };

	GLfloat ambientLight[] = { 0.8, 0.8, 0.8, 1.0 };
	GLfloat diffuseLight[] = { 0.8, 0.8, 0.8, 1.0 };
	GLfloat specularLight[] = { 2.0, 2.0, 2.0, 2.0 };

	glLightfv( GL_LIGHT0, GL_AMBIENT, ambientLight );
	glLightfv( GL_LIGHT0, GL_DIFFUSE, diffuseLight );

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	glEnable(GL_POINT_SMOOTH);

	glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
    //glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
	
	glFrontFace(GL_CW);
	//glFrontFace(GL_CCW);

	// Black background
	glClearColor(0.0f,0.0f,0.0f,1.0f);

}

void ReadFile(model *md)
{
	FILE *f = fopen("planet.obj", "r");
	if(f == NULL){
		printf("Error opening planet file \n");
	}
	int lines = 1, ch;
	int number_f = 0, number_v = 0;
	while((ch = fgetc(f)) != EOF){
		if(ch == '\n') lines++;
		else if(ch == 'f') number_f++;
		else if(ch == 'v') number_v++;
		else if(ch == 'n') number_v--;
    }
	printf("Lines: %d \n", lines);
	printf("F = %d V = %d \n", number_f, number_v);
	md->faces = number_f;
	md->vertices = number_v;

	// allocate memory for model
	md->obj_points = (point *) malloc(number_v * sizeof(point)); //TODO
	md->obj_faces = (face *) malloc(number_f * sizeof(face));

	fclose(f);
	f = fopen("planet.obj", "r");

	int fi, vi, ret;
	char type[50];
	char buf1[50], buf2[50], buf3[50];
	fi = vi = 0;
	for(int i = 0; i < lines; i++){
		ret = fscanf(f, "%s %s %s %s \n", type, buf1, buf2, buf3);
		if(strcmp(type, "f") == 0){
			int a,b,c;
			a = atoi(buf1);
			b = atoi(buf2);
			c = atoi(buf3);
			md->obj_faces[fi].vtx[0] = a;
			md->obj_faces[fi].vtx[1] = b;
			md->obj_faces[fi].vtx[2] = c;
			fi++;
		}
		else if(strcmp(type, "v") == 0){
			float a,b,c;
			a = atof(buf1);
			b = atof(buf2);
			c = atof(buf3);
			md->obj_points[vi].x = a;
			md->obj_points[vi].y = b;
			md->obj_points[vi].z = c;
			vi++;
		}
	}
	printf("last fi %d \n", fi);
	printf("faces: %d \n", md->faces);
	printf("last vi %d \n", vi);
	printf("vertices: %d \n", md->vertices);
	printf("%d \n", md->faces + md->vertices);
	fclose(f);
}

void DisplayModel(model md)
{

	glPushMatrix();
	glBegin(GL_TRIANGLES);
	for (int i = 0; i < md.faces; i++)
	{
		glVertex3f(md.obj_points[md.obj_faces[i].vtx[0]-1].x,md.obj_points[md.obj_faces[i].vtx[0]-1].y,md.obj_points[md.obj_faces[i].vtx[0]-1].z);
		glVertex3f(md.obj_points[md.obj_faces[i].vtx[1]-1].x,md.obj_points[md.obj_faces[i].vtx[1]-1].y,md.obj_points[md.obj_faces[i].vtx[1]-1].z);
		glVertex3f(md.obj_points[md.obj_faces[i].vtx[2]-1].x,md.obj_points[md.obj_faces[i].vtx[2]-1].y,md.obj_points[md.obj_faces[i].vtx[2]-1].z);
	}

	glEnd();
	glPopMatrix();

}




