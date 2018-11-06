#include <stdlib.h>
#include <GL/glut.h>
#include <math.h>
#include "arcball.h"


// ROBOT ARM CONTROLS
float BaseTransX = -0.5f;  // 0
float BaseTransZ = 0;
float BaseSpin = 0;        // 1
float ShoulderAng = -10;   // 2
float ElbowAng = -120;
float WristAng = 90;       // 3
float WristTwistAng = 10;
float FingerAng1 = 45;     // 4
float FingerAng2 = -90;

// ROBOT COLORS
GLubyte Arms[] = { 128,128,128 };
GLubyte Joints[] = { 0,68,119 };
GLubyte Fingers[] = { 150,0,24 };
GLubyte FingerJoints[] = { 128,128,128 };

// USER INTERFACE GLOBALS
int LeftButtonDown = 0;
int RightButtonDown = 0;// MOUSE STUFF
int OldX, OldY, NewX, NewY;
int RobotControl = 1;


// scene parameters
const vec eye(0.0f, 2.0f, 4.0f);
const vec centre(0.0f, 0.5f, 0.0f);
const vec up(0.0f, 1.0f, 0.0f);
const float SPHERE_RADIUS = 1.0f;

//onclick ¿¡ ÇÊ¿ä
static float aspect_ratio = 1.0f;
static int width, height;


void DrawUnitCylinder(int NumSegs)  // x,y,z in [0,1], Y-axis is up
{
	int i;
	float *Px = new float[NumSegs];
	float *Py = new float[NumSegs];
	float AngIncr = (2.0f*3.1415927f) / (float)NumSegs;
	float Ang = AngIncr;
	Px[0] = 1;
	Py[0] = 0;
	for (i = 1; i < NumSegs; i++, Ang += AngIncr)
	{
		Px[i] = (float)cos(Ang);
		Py[i] = (float)sin(Ang);
	}

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glTranslatef(0.5f, 0.5f, 0.5f);
	glScalef(0.5f, 0.5f, 0.5f);

	// TOP
	glNormal3f(0, 1, 0);
	glBegin(GL_TRIANGLE_FAN);
	glVertex3f(0, 1, 0);
	for (i = 0; i < NumSegs; i++)
		glVertex3f(Px[i], 1, -Py[i]);
	glVertex3f(Px[0], 1, -Py[0]);
	glEnd();

	// BOTTOM
	glNormal3f(0, -1, 0);
	glBegin(GL_TRIANGLE_FAN);
	glVertex3f(0, -1, 0);
	for (i = 0; i < NumSegs; i++)
		glVertex3f(Px[i], -1, Py[i]);
	glVertex3f(Px[0], -1, Py[0]);
	glEnd();

	// SIDES
	glBegin(GL_QUAD_STRIP);
	for (i = 0; i < NumSegs; i++)
	{
		glNormal3f(Px[i], 0, -Py[i]);
		glVertex3f(Px[i], 1, -Py[i]);
		glVertex3f(Px[i], -1, -Py[i]);
	}
	glNormal3f(Px[0], 0, -Py[0]);
	glVertex3f(Px[0], 1, -Py[0]);
	glVertex3f(Px[0], -1, -Py[0]);
	glEnd();

	glPopMatrix();

	delete[] Px;
	delete[] Py;
}

void DrawUnitSphere(int NumSegs)  // x,y,z in [0,1]
{
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glTranslatef(0.5f, 0.5f, 0.5f);
	glutSolidSphere(0.5f, NumSegs, NumSegs);
	glPopMatrix();
}

void DrawUnitCone(int NumSegs)  // x,y,z in [0,1], apex is in +Y direction
{
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glTranslatef(0.5f, 0, 0.5f);
	glRotatef(-90, 1, 0, 0);
	glutSolidCone(0.5f, 1, NumSegs, NumSegs);
	glPopMatrix();
}

void DrawGroundPlane(int NumSegs)
{
	arcball_rotate();
	glColor3f(0.7f, 0.7f, 0.7f);
	glBegin(GL_QUADS);
	glNormal3f(0, 1, 0);
	glVertex3f(-1, 0, 1);
	glVertex3f(1, 0, 1);
	glVertex3f(1, 0, -1);
	glVertex3f(-1, 0, -1);
	glEnd();
}

void DrawJoint(int NumSegs)
{
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glScalef(0.15f, 0.15f, 0.12f);
	glRotatef(90, 1, 0, 0);
	glTranslatef(-0.5f, -0.5f, -0.5f);
	glColor3ubv(Joints);
	DrawUnitCylinder(NumSegs);
	glPopMatrix();
}

void DrawBase(int NumSegs)
{
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glScalef(0.2f, 0.025f, 0.2f);
	glTranslatef(-0.5f, 0, -0.5f);
	glColor3ubv(Joints);
	DrawUnitCylinder(NumSegs);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(-0.05f, 0, -0.05f);
	glScalef(0.1f, 0.4f, 0.1f);
	glColor3ubv(Arms);
	DrawUnitCylinder(NumSegs);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(0, 0.4f, 0);
	DrawJoint(NumSegs);
	glPopMatrix();
}

void DrawArmSegment(int NumSegs)
{
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glTranslatef(-0.05f, 0, -0.05f);
	glScalef(0.1f, 0.5f, 0.1f);
	glColor3ubv(Arms);
	DrawUnitCylinder(NumSegs);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(0, 0.5f, 0);
	DrawJoint(NumSegs);
	glPopMatrix();
}

void DrawWrist(int NumSegs)
{
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glTranslatef(-0.04f, 0, -0.04f);
	glScalef(0.08f, 0.2f, 0.08f);
	glColor3ubv(Fingers);
	DrawUnitCylinder(NumSegs);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(0, 0.2f, 0);
	glScalef(0.12f, 0.12f, 0.12f);
	glTranslatef(-0.5f, -0.5f, -0.5f);
	glColor3ubv(FingerJoints);
	DrawUnitSphere(NumSegs);
	glPopMatrix();
}

void DrawFingerBase(int NumSegs)
{
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glTranslatef(-0.025f, 0, -0.025f);
	glScalef(0.05f, 0.3f, 0.05f);
	glColor3ubv(Fingers);
	DrawUnitCylinder(NumSegs);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(0, 0.3f, 0);
	glScalef(0.08f, 0.08f, 0.08f);
	glTranslatef(-0.5f, -0.5f, -0.5f);
	glColor3ubv(FingerJoints);
	DrawUnitSphere(NumSegs);
	glPopMatrix();
}

void DrawFingerTip(int NumSegs)
{
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glScalef(0.05f, 0.25f, 0.05f);
	glTranslatef(-0.5f, 0, -0.5f);
	glColor3ubv(Fingers);
	DrawUnitCone(NumSegs);
	glPopMatrix();
}

void DrawRobotArm(int NumSegs)
{



	glMatrixMode(GL_MODELVIEW);
	
	// ADD YOUR DISPLAY ROUTINE HERE
	glPushMatrix();
	glTranslatef(BaseTransX, 0.0f, BaseTransZ);
	glRotatef(BaseSpin, 0, 1, 0);
	DrawBase(NumSegs); // base

	glTranslatef(0, 0.4f, 0);
	glRotatef(ShoulderAng, 0, 0, 1);
	DrawArmSegment(NumSegs); //first segment1

	glTranslatef(0, 0.5f, 0);
	glRotatef(ElbowAng, 0, 0, 1);
	DrawArmSegment(NumSegs); // second segment


	glTranslatef(0, 0.5f, 0);
	glRotatef(WristAng, 0, 0, 1);
	glRotatef(WristTwistAng, 0, 1, 0);
	DrawWrist(NumSegs); // wrist

	glPushMatrix();
	glPushMatrix();// make two copies

	glPopMatrix(); //use one

	glTranslatef(0, 0.2f, 0);
	glRotatef(FingerAng1, 0, 0, 1);

	DrawFingerBase(NumSegs);


	glTranslatef(0, 0.3f, 0);
	glRotatef(FingerAng2, 0, 0, 1);
	DrawFingerTip(NumSegs);



	glPopMatrix(); // use the other copy

	glTranslatef(0, 0.2f, 0);
	glRotatef(-FingerAng1, 0, 0, 1);

	DrawFingerBase(NumSegs);


	glTranslatef(0, 0.3f, 0);
	glRotatef(-FingerAng2, 0, 0, 1);

	DrawFingerTip(NumSegs);

	glPopMatrix();



}



void myDisplay()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	DrawGroundPlane(16);
	DrawRobotArm(16);


	glutSwapBuffers();
}

void myReshape(int w, int h)
{

	width = w;
	height = h;
	glViewport(0, 0, w, h);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(30, (float)w / h, 0.1, 10);
	gluLookAt(
		0.0f, 2.0f, 4.0f,
		0.0f, 0.5f, 0.0f,
		0.0f, 1.0f, 0.0f);
	arcball_setzoom(SPHERE_RADIUS, eye, up);
}

void myIdle()
{
	glutPostRedisplay();
}

void KeyboardFunc(unsigned char Key, int x, int y)
{
	if (Key >= '1' && Key <= '5') RobotControl = Key - '1';
	if (Key == 27) exit(0);         // ESC
}

static void click_scene(int x, int y)
{
	int invert_y = (height - y) - 1; // OpenGL viewport coordinates are Cartesian
	arcball_start(x, invert_y);
}

void MouseFunc(int button, int state, int x, int y)
{
	NewX = x; NewY = y;
	if (button == GLUT_LEFT_BUTTON) {
		LeftButtonDown = !LeftButtonDown;

	}
	if ( button == GLUT_RIGHT_BUTTON) {
		RightButtonDown = !RightButtonDown;
		click_scene(x, y);
	}

}



static void drag_scene(int x, int y)
{
	int invert_y = (height - y) - 1;
		arcball_move(x, invert_y);
}



void MotionFunc(int x, int y)
{
	OldX = NewX; OldY = NewY;
	NewX = x;    NewY = y;
	float RelX = (NewX - OldX) / (float)glutGet((GLenum)GLUT_WINDOW_WIDTH);
	float RelY = (NewY - OldY) / (float)glutGet((GLenum)GLUT_WINDOW_HEIGHT);
	if (LeftButtonDown)
		switch (RobotControl)
		{
		case 0: BaseTransX += RelX; BaseTransZ += RelY; break;
		case 1: BaseSpin += RelX * 180; break;
		case 2: ShoulderAng += RelY * -90; ElbowAng += RelX * 90; break;
		case 3: WristAng += RelY * -180; WristTwistAng += RelX * 180; break;
		case 4: FingerAng1 += RelY * 90; FingerAng2 += RelX * 180; break;
		};

	if (RightButtonDown) drag_scene(x, y);

}


void main(void)
{
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(512, 512);
	glutInitWindowPosition(180, 100);
	glutCreateWindow("The Robot Arm");

	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_NORMALIZE);
	glEnable(GL_CULL_FACE);

	glutDisplayFunc(myDisplay); // draw plane and robot arm
	glutReshapeFunc(myReshape);
	glutIdleFunc(myIdle);

	glutKeyboardFunc(KeyboardFunc);
	glutMouseFunc(MouseFunc);
	glutMotionFunc(MotionFunc);


	glutMainLoop();
}
