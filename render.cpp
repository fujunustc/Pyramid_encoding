/*
*     Vahid Mazdeh - 06/14/2015
*     A 3D scene of the oldest World Wonder, the Egyptian Pyramids of Giza.
*/

#include <opencv2/opencv.hpp>

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <math.h>
//  OpenGL with prototypes for glext
#define GL_GLEXT_PROTOTYPES
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#define GLUT_DISABLE_ATEXIT_HACK 
#include <GL/glut.h>
#endif
GLuint  texture[1];
int axes = 0;       //  Display axes
int mode = 0;       //  Projection mode
int th = 0;         //  Azimuth of view angle
int ph = 0;         //  Elevation of view angle
int fov = 90;       //  Field of view (for perspective)
double asp = 1;     //  Aspect ratio
double dim = 600;   //  Size of world

//  Macro for sin & cos in degrees
#define Cos(th) cos(3.1415927/180*(th))
#define Sin(th) sin(3.1415927/180*(th))

/*
*  Convenience routine to output raster text
*  Use VARARGS to make this more flexible
*/
#define LEN 8192  //  Maximum length of text string
void Print(const char* format, ...)
{
	char    buf[LEN];
	char*   ch = buf;
	va_list args;
	//  Turn the parameters into a character string
	va_start(args, format);
	vsnprintf(buf, LEN, format, args);
	va_end(args);
	//  Display the characters one at a time at the current raster position
	while (*ch)
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *ch++);
}

/*
*  Set projection
*/
static void Project()
{
	//  Tell OpenGL we want to manipulate the projection matrix
	glMatrixMode(GL_PROJECTION);
	//  Undo previous transformations
	glLoadIdentity();
	//  Perspective transformation
	if (mode)
		glOrtho(-asp*dim, +asp*dim, -dim, +dim, -dim, +dim);
		//gluPerspective(fov, asp, 0.0f, 2.0f);
		//gluPerspective(fov, asp, dim / 4, 4 * dim);
	//  Orthogonal projection
	else
		
		gluPerspective(fov, asp, 0.0f, 1.0f);
	//  Switch to manipulating the model matrix
	glMatrixMode(GL_MODELVIEW);
	//  Undo previous transformations
	glLoadIdentity();
}

// Render a pyramid consists of 4 triangles
static void pyramid(double x, double y, double z,
	double dx, double dy, double dz,
	double th) {
	glPushMatrix();                  // Reset the model-view matrix

	//  Offset
//	glTranslated(x, y, z);
//	glRotated(th, 0, 1, 0);
	glScaled(dx, dy, dz);

	//激活纹理功能
	glBindTexture(GL_TEXTURE_2D, texture[0]);

	// Front Face  
	//提供纹理坐标
	GLfloat t = 1;// 2 * sqrtf(2.0f) - 1;
	GLfloat d = sqrtf(2.0f)/2;
	
	glBegin(GL_TRIANGLES);           // Begin drawing the pyramid with 4 triangles

	glTexCoord2f(0.0f, 1.0f); glVertex3f(0.0f, t, 0.0f);
	glTexCoord2f(0.0f, 0.5f); glVertex3f(-1.0f, -1.0f, 1.0f);
	glTexCoord2f(0.5f, 1.0f); glVertex3f(1.0f, -1.0f, 1.0f);


	glTexCoord2f(1.0f, 1.0f); glVertex3f(0.0f, t, 0.0f);
	glTexCoord2f(0.5f, 1.0f); glVertex3f(1.0f, -1.0f, 1.0f);
	glTexCoord2f(1.0f, 0.5f); glVertex3f(1.0f, -1.0f, -1.0f);

	glTexCoord2f(1.0f, 0.0f); glVertex3f(0.0f, t, 0.0f);
	glTexCoord2f(1.0f, 0.5f); glVertex3f(1.0f, -1.0f, -1.0f);
	glTexCoord2f(0.5f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);


	glTexCoord2f(0.0f, 0.0f); glVertex3f(0.0f, t, 0.0f);
	glTexCoord2f(0.5f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
	glTexCoord2f(0.0f, 0.5f); glVertex3f(-1.0f, -1.0f, 1.0f);

	glEnd();   // Done drawing the pyramid

	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.5f); glVertex3f(-1.0f, -1.0f, 1.0f);
	glTexCoord2f(0.5f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
	glTexCoord2f(1.0f, 0.5f); glVertex3f(1.0f, -1.0f, -1.0f);
	glTexCoord2f(0.5f, 1.0f); glVertex3f(1.0f, -1.0f, 1.0f);
	glEnd();
	
	/*
	glBegin(GL_TRIANGLES);           // Begin drawing the pyramid with 4 triangles

	glTexCoord2f(0.0f, 1.0f); glVertex3f(0.0f, 0.0f, 2.0f-d);
	glTexCoord2f(0.0f, 0.5f); glVertex3f(0, 1.0f, -d);
	glTexCoord2f(0.5f, 1.0f); glVertex3f(1.0f, 0.0f, -d);


	glTexCoord2f(1.0f, 1.0f); glVertex3f(0.0f, 0.0f, 2.0f-d);
	glTexCoord2f(0.5f, 1.0f); glVertex3f(1.0f, 0.0f, -d);
	glTexCoord2f(1.0f, 0.5f); glVertex3f(-1.0f, 0.0f, -d);

	glTexCoord2f(1.0f, 0.0f); glVertex3f(0.0f, 0.0f, 2.0f - d);
	glTexCoord2f(1.0f, 0.5f); glVertex3f(0.0f, -1.0f, -d);
	glTexCoord2f(0.5f, 0.0f); glVertex3f(-1.0f, 0.0f, -d);


	glTexCoord2f(0.0f, 0.0f); glVertex3f(0.0f, 0.0f, 2.0f-d);
	glTexCoord2f(0.5f, 0.0f); glVertex3f(-1.0f, 0.0f, -d);
	glTexCoord2f(0.0f, 0.5f); glVertex3f(0.0f, 1.0f, -d);

	glEnd();   // Done drawing the pyramid

	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.5f); glVertex3f(0.0f, 1.0f, -d);
	glTexCoord2f(0.5f, 0.0f); glVertex3f(-1.0f, 0.0f, -d);
	glTexCoord2f(1.0f, 0.5f); glVertex3f(0.0f, -1.0f, -d);
	glTexCoord2f(0.5f, 1.0f); glVertex3f(1.0f, 0.0f, -d);
	glEnd();
	*/
	glPopMatrix();
}


/*
*  Draw vertex in polar coordinates
*/
static void Vertex(double th, double ph)
{
	glColor3f(0.9, 0.67, 0.440);
	glVertex3d(Sin(th)*Cos(ph), Sin(ph), Cos(th)*Cos(ph));
}


/*
*  Draw a sphere (version 2)
*     at (x,y,z)
*     radius (r)
*/
static void sphere2(double x, double y, double z, double r)
{
	const int d = 5;
	int th, ph;

	//  Save transformation
	glPushMatrix();
	//  Offset and scale
	glTranslated(x, y, z);
	glScaled(r, r, r);

	//  Latitude bands
	for (ph = -90; ph<90; ph += d)
	{
		glBegin(GL_QUAD_STRIP);
		for (th = 0; th <= 360; th += d)
		{
			Vertex(th, ph);
			Vertex(th, ph + d);
		}
		glEnd();
	}

	//  Undo transformations
	glPopMatrix();
}



/*
*  Draw a cube
*     at (x,y,z)
*     dimentions (dx,dy,dz)
*     rotated th about the y axis
*/
static void cube(double x, double y, double z,
	double dx, double dy, double dz,
	double th)
{
	//  Save transformation
	glPushMatrix();
	//  Offset
	glTranslated(x, y, z);
	glRotated(th, 0, 0, 0);
	glScaled(dx, dy, dz);
	//  Cube
	glBegin(GL_QUADS);
	//  Front
	glColor3f(0.730, 0.55, 0.34);
	glVertex3f(-1, 0, 1);
	glVertex3f(+1, 0, 1);
	glVertex3f(+1, +2, 1);
	glVertex3f(-1, +2, 1);
	//  Back
	glColor3f(0.730, 0.55, 0.34);
	glVertex3f(+1, 0, -1);
	glVertex3f(-1, 0, -1);
	glVertex3f(-1, +2, -1);
	glVertex3f(+1, +2, -1);
	//  Right
	glColor3f(0.730, 0.55, 0.34);
	glVertex3f(+1, 0, +1);
	glVertex3f(+1, 0, -1);
	glVertex3f(+1, +2, -1);
	glVertex3f(+1, +2, +1);
	//  Left
	glColor3f(0.730, 0.55, 0.34);
	glVertex3f(-1, 0, -1);
	glVertex3f(-1, 0, +1);
	glVertex3f(-1, +2, +1);
	glVertex3f(-1, +2, -1);
	//  Top
	glColor3f(0.730, 0.55, 0.34);
	glVertex3f(-1, +2, +1);
	glVertex3f(+1, +2, +1);
	glVertex3f(+1, +2, -1);
	glVertex3f(-1, +2, -1);
	//  Bottom
	glColor3f(0.730, 0.55, 0.34);
	glVertex3f(-1, 0, -1);
	glVertex3f(+1, 0, -1);
	glVertex3f(+1, 0, +1);
	glVertex3f(-1, 0, +1);
	//  End
	glEnd();
	//  Undo transofrmations
	glPopMatrix();
}


/*
*  OpenGL (GLUT) calls this routine to display the scene
*/
void display()
{
	const double len = 1200;  //  Length of axes
	//  Erase the window and the depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//  Enable Z-buffering in OpenGL
	glEnable(GL_DEPTH_TEST);
	//  Undo previous transformations
	glLoadIdentity();
	//  Perspective - set eye position
	if (mode)
	{
		glRotatef(ph, 1, 0, 0);
		glRotatef(th, 0, 1, 0);
		/*double Ex = -2 * dim*Sin(th)*Cos(ph);
		double Ey = +2 * dim        *Sin(ph);
		double Ez = +2 * dim*Cos(th)*Cos(ph);
		gluLookAt(Ex, Ey, Ez, 0, 0, 0, 0, Cos(ph), 0);*/
	}
	//  Orthogonal - set world orientation
	else
	{
		glRotatef(ph, 1, 0, 0);
		glRotatef(th, 0, 1, 0);
	}
	//glTranslatef(0, 0, 0);
	//  Draw cubes
	// for (i=-1;i<=1;i++)
	//    for (j=-1;j<=1;j++)
	//       for (k=-1;k<=1;k++)
	//          cube(i,j,k , 0.3,0.3,0.3 , 0);


	// drawing the 3 great pyramids of Egypt
	pyramid(0, 0, 0, 300, 300, 300, 0);

//	pyramid(300, 0, 115, 30, 30, 30, 0);
//	pyramid(300, 0, 20, 30, 30, 30, 0);
//	pyramid(300, 0, -75, 30, 30, 30, 0);

//	pyramid(-500, 0, 600, 230, 136, 230, 0);

//	cube(-100, 0, 600, 115, 30, 50, 0);
//	sphere2(-20, 90, 600, 30);

//	pyramid(-900, 0, 990, 103.327, 60.96, 103.327, 0);

	// drawig the queens' pyramids
//	pyramid(-900, 0, 1200, 30, 30, 30, 0);
//	pyramid(-1000, 0, 1200, 30, 30, 30, 0);
//	pyramid(-800, 0, 1200, 30, 30, 30, 0);
//	pyramid(0, 0, 0, 1, 1, 1, 0);
	//  Draw axes
//	glColor3f(1, 1, 1);
	if (axes)
	{
		glBegin(GL_LINES);
		glVertex3d(0.0, 0.0, 0.0);
		glVertex3d(len, 0.0, 0.0);
		glVertex3d(0.0, 0.0, 0.0);
		glVertex3d(0.0, len, 0.0);
		glVertex3d(0.0, 0.0, 0.0);
		glVertex3d(0.0, 0.0, len);
		glEnd();
		//  Label axes
		glRasterPos3d(len, 0.0, 0.0);
		Print("X");
		glRasterPos3d(0.0, len, 0.0);
		Print("Y");
		glRasterPos3d(0.0, 0.0, len);
		Print("Z");
	}
	//  Display parameters
	//glWindowPos2i(5, 5);
	//Print("Angle=%d,%d  Dim=%.1f FOV=%d Projection=%s", th, ph, dim, fov, mode ? "Perpective" : "Orthogonal");
	//  Render the scene and make it visible
	glFlush();
	glutSwapBuffers();
}


/*
*  GLUT calls this routine when an arrow key is pressed
*/
void special(int key, int x, int y)
{
	//  Right arrow key - increase angle by 5 degrees
	if (key == GLUT_KEY_RIGHT)
		th += 5;
	//  Left arrow key - decrease angle by 5 degrees
	else if (key == GLUT_KEY_LEFT)
		th -= 5;
	//  Up arrow key - increase elevation by 5 degrees
	else if (key == GLUT_KEY_UP)
		ph += 5;
	//  Down arrow key - decrease elevation by 5 degrees
	else if (key == GLUT_KEY_DOWN)
		ph -= 5;
	//  Keep angles to +/-360 degrees
	th %= 360;
	ph %= 360;
	//  Update projection
	Project();
	//  Tell GLUT it is necessary to redisplay the scene
	glutPostRedisplay();
}

/*
*  GLUT calls this routine when a key is pressed
*/
void key(unsigned char ch, int x, int y)
{
	//  Exit on ESC
	if (ch == 27)
		exit(0);
	//  Reset view angle
	else if (ch == '0')
		th = ph = 0;
	//  Toggle axes
	else if (ch == 'a' || ch == 'A')
		axes = 1 - axes;
	//  Switch display mode
	else if (ch == 'm' || ch == 'M')
		mode = 1 - mode;
	//  Change field of view angle
	else if (ch == '-' && ch>1)
		fov--;
	else if (ch == '+' && ch<179)
		fov++;
	// zoom in and out
	else if (ch == 'y' || ch == 'Y')
		dim -= 10;
	else if (ch == 'h' || ch == 'H')
		dim += 10;
	else if (ch == 'm')
	{
		if (mode == 1)
			mode = 0;
		else
			mode = 1;
	}
	//printf("")

	//  Reproject
	Project();
	//  Tell GLUT it is necessary to redisplay the scene
	glutPostRedisplay();
}

/*
*  GLUT calls this routine when the window is resized
*/
void reshape(int width, int height)
{
	//  Ratio of the width to the height of the window
	asp = (height>0) ? (double)width / height : 1;
	//  Set the viewport to the entire window
	glViewport(0, 0, width, height);
	//  Set projection
	Project();
}



int LoadGLTextures()
{
	int Status = FALSE;

	//AUX_RGBImageRec *TextureImage[1];
	cv::Mat TextureImage = cv::imread("pyramid.jpg");
	cv::cvtColor(TextureImage, TextureImage, CV_BGR2RGB);
	//memset(TextureImage, 0, sizeof(void *) * 1);


//	if (TextureImage[0] = LoadBMP("Data/NeHe.bmp"))
	{
		Status = TRUE;

		glGenTextures(1, &texture[0]);        //生成纹理ID        


		glBindTexture(GL_TEXTURE_2D, texture[0]);  //创建纹理对象
		//指定纹理内容
		glTexImage2D(GL_TEXTURE_2D, 0, 3, TextureImage.size().height, TextureImage.size().width, 0, GL_RGB, GL_UNSIGNED_BYTE, TextureImage.data);
		//指定纹理属性
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		
	}

	/*if (TextureImage[0])
	{
		if (TextureImage[0]->data)
		{
			free(TextureImage[0]->data);
		}

		free(TextureImage[0]);
	}*/

	return Status;
}

int InitGL(void)
{
	if (!LoadGLTextures())
	{
		return FALSE;
	}

	glEnable(GL_TEXTURE_2D);
	glShadeModel(GL_SMOOTH);
	glClearColor(0.0f, 0.0f, 0.0f, 0.5f);
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	return TRUE;
}
/*
*  Start up GLUT and tell it what to do
*/
int main(int argc, char* argv[])
{
	//  Initialize GLUT
	glutInit(&argc, argv);
	//  Request double buffered, true color window with Z buffering at 600x600
	glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
	glutInitWindowSize(800, 800);
	glutCreateWindow("Egyptian Pyramids");
	// background color
	glClearColor(0.95, 0.810, 0.4, 0.0);
	//  Set callbacks
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutSpecialFunc(special);
	glutKeyboardFunc(key);
	InitGL();
	//  Pass control to GLUT so it can interact with the user
	glutMainLoop();
	return 0;
}