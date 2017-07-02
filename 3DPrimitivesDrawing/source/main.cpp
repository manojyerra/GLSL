#include "DefinesAndIncludes.h"
#include <GL/freeglut.h>
#include "ShaderProgram.h"
#include "GLUtil/GLBatch.h"
#include "Primitives2D/Triangle2D.h"
#include "Primitives2D/Rect2D.h"
#include "Floor.h"
#include "Cam.h"
#include "Input.h"

ShaderProgram* shaderProgram;
  void DemoLight(void);

int CreateGlutWindow(char* title, int x, int y, int w, int h);
void MouseInput(int button, int updown, int x, int y);
void Display();

Triangle2D* _triangle2D;
Rect2D* _rect2D;
Floor* _floor;
Cam* _cam;

float _sw = 1300.0f;
float _sh = 600.0f;
float _zNear = 1.0f;
float _zFar = 10000.0f;
float _zNearPlaneW = 0.2f;


int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	CreateGlutWindow("First Window", 0,0,_sw,_sh);
	glutMouseFunc(MouseInput);
	glewInit();

	glShadeModel( GL_SMOOTH		);
	glCullFace	( GL_BACK		);
	glFrontFace	( GL_CCW		);
	
	glDisable	( GL_FOG		);
	glDisable	( GL_LIGHTING	);
	glDisable	( GL_CULL_FACE	);
	glDisable	( GL_LINE_SMOOTH);
	glHint		( GL_LINE_SMOOTH_HINT, GL_NICEST);
	
	glEnable	( GL_DEPTH_TEST	);
	glEnable	( GL_BLEND		);
	glBlendFunc	( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	_cam = Cam::GetInstance();
	_cam->Init(_sw, _sh, _zNear, _zFar, _zNearPlaneW);

	//CVector3 v1(-1.0f, -1.0f, 0.0f);
	//CVector3 v2(1.0f, -1.0f, 0.0f);
	//CVector3 v3(0.0f,  1.0f, 0.0f);

	CVector3 v1(-1.0f, 1.0f, 0.0f);
	CVector3 v2(0.5f, 0.5f, 0.0f);
	CVector3 v3(0.0f, 0.5f, 0.0f);

	_triangle2D = new Triangle2D(v1, v2, v3);
	_rect2D = new Rect2D(-1, -1, 2, 2);

	_floor = new Floor();

	DemoLight();
	shaderProgram = new ShaderProgram("shaders/passThroughShader.vs", "shaders/passThroughShader.fs");

	glutDisplayFunc(Display);
	glutMainLoop();

	return 0;
}

int CreateGlutWindow(char* title, int x, int y, int w, int h)
{
	glutInitWindowPosition(x,y);

	DWORD windowExtendedStyle = WS_EX_APPWINDOW;						// Define The Window's Extended Style
	DWORD windowStyle = WS_OVERLAPPEDWINDOW & ~WS_MAXIMIZEBOX;
	RECT windowRect = {x, y, w, h};
	
	AdjustWindowRectEx (&windowRect, windowStyle, 0, windowExtendedStyle);

	glutInitWindowSize(w,h);
	glutCreateWindow(title);

  	HWND hThisWnd = FindWindow( "FREEGLUT", title );
	if( hThisWnd )
	{
		LONG lStyle = GetWindowLong( hThisWnd, GWL_STYLE );
		SetWindowLong( hThisWnd, GWL_STYLE, 0);
		ShowWindow(hThisWnd, SW_SHOW);
	}

	_sh += 22;

	return 1;
}

void MouseInput(int button, int updown, int x, int y)
{
	//printf("\nButton = %d, updown = %d, x = %d, y = %d", button, updown, x, y);

	if(button == 0)
	{
		Input::LEFT_BUTTON_DOWN = (updown == 0);
	}
	else if(button == 1)
	{
		Input::MIDDLE_BUTTON_DOWN = (updown == 0);
	}
	else if(button == 2)
	{
		Input::RIGHT_BUTTON_DOWN = (updown == 0);
	}
}

void Display()
{
	Input::Update(1.0f/30.0f);

	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	
	glViewport(0,0,_sw,_sh);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0f,_sw,_sh,0.0f,0.0f,1.0f);

	_cam->SetProjection();
	_cam->SetModelViewMatrix();
	_cam->UpdateCamera();

	//_triangle2D->Draw();
	//_rect2D->Draw();
	_floor->Draw();

	//shaderProgram->Begin();
	//glEnable( GL_CULL_FACE );
	//glCullFace ( GL_BACK );
	////glColor3f( 1.0, 0.0, 0.0 );           //red
	//glFrontFace ( GL_CCW );
	//glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
	//glutSolidTeapot(5.0);
	//shaderProgram->End();

	glutSwapBuffers();
	glutPostRedisplay();
}


  void DemoLight(void)
   {
     glEnable(GL_LIGHTING);
     glEnable(GL_LIGHT0);
     glEnable(GL_NORMALIZE);
     
     // Light model parameters:
     // -------------------------------------------
     
     GLfloat lmKa[] = {0.0, 0.0, 0.0, 0.0 };
     glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmKa);
     
     glLightModelf(GL_LIGHT_MODEL_LOCAL_VIEWER, 1.0);
     glLightModelf(GL_LIGHT_MODEL_TWO_SIDE, 0.0);
     
     // -------------------------------------------
     // Spotlight Attenuation
     
     GLfloat spot_direction[] = {1.0, -1.0, -1.0 };
     GLint spot_exponent = 30;
     GLint spot_cutoff = 180;
     
     glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, spot_direction);
     glLighti(GL_LIGHT0, GL_SPOT_EXPONENT, spot_exponent);
     glLighti(GL_LIGHT0, GL_SPOT_CUTOFF, spot_cutoff);
    
     GLfloat Kc = 1.0;
     GLfloat Kl = 0.0;
     GLfloat Kq = 0.0;
     
     glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION,Kc);
     glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, Kl);
     glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, Kq);
     
     
     // ------------------------------------------- 
     // Lighting parameters:

     GLfloat light_pos[] = {0.0f, 5.0f, 5.0f, 1.0f};
     GLfloat light_Ka[]  = {1.0f, 0.5f, 0.5f, 1.0f};
     GLfloat light_Kd[]  = {1.0f, 0.1f, 0.1f, 1.0f};
     GLfloat light_Ks[]  = {1.0f, 1.0f, 1.0f, 1.0f};

     glLightfv(GL_LIGHT0, GL_POSITION, light_pos);
     glLightfv(GL_LIGHT0, GL_AMBIENT, light_Ka);
     glLightfv(GL_LIGHT0, GL_DIFFUSE, light_Kd);
     glLightfv(GL_LIGHT0, GL_SPECULAR, light_Ks);

     // -------------------------------------------
     // Material parameters:

     GLfloat material_Ka[] = {0.5f, 0.0f, 0.0f, 1.0f};
     GLfloat material_Kd[] = {0.4f, 0.4f, 0.5f, 1.0f};
     GLfloat material_Ks[] = {0.8f, 0.8f, 0.0f, 1.0f};
     GLfloat material_Ke[] = {0.1f, 0.0f, 0.0f, 0.0f};
     GLfloat material_Se = 20.0f;

     glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, material_Ka);
     glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, material_Kd);
     glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, material_Ks);
     glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, material_Ke);
     glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, material_Se);
   }