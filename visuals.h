struct point 
{
	float x; 
	float y; 
	float z; 
};

struct face
{
	int vtx[3];
};

struct model
{
	point *obj_points;
	face *obj_faces;
	int vertices;
	int faces;
};


//-------- Functions --------------------------------

void Render();
// The function responsible for drawing everything in the 
// OpenGL context associated to a window. 

void Resize(int w, int h);
// Handle the window size changes and define the world coordinate 
// system and projection type

void Setup();
// Set up the OpenGL state machine and create a light source

void Keyboard(unsigned char key,int x,int y);
// do stuff with keyboard

void Idle();
// Idle function

void ReadFile(model *md);
// Read File function

void DisplayModel(model md);
// Display Model function