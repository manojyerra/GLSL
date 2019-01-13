#include "GameLoop.h"

GameLoop::GameLoop(int sw, int sh)
{
	_sw = sw;
	_sh = sh;

	GLSettings();

	_zNear = 1.0f;
	_zFar = 10000.0f;
	_zNearPlaneW = 0.2f;

	//_cam = Cam::GetInstance();
	//_cam->Init(_sw, _sh, _zNear, _zFar, _zNearPlaneW);

	//_floor = new Floor();

	//CVector3 v1(0.0f, 0.0f, 0.0f);
	//CVector3 v2(1.0f, 0.0f, 0.0f);
	//CVector3 v3(1.0f, 1.0f, 0.0f);

	//_triangle = new Triangle(v1, v2, v3);

	//_box = new Box(0, 0, 0, 2, 3, 4);
	//_box->SetSize(3, 1, 6);
	//_box->SetPos(-10, 0, -10);

	//_cylinder = new Cylinder(0, 0, 0, 3, 2);
	//_cylinder->SetRadius(1.5);
	//_cylinder->SetHeight(2);
	//_cylinder->SetPos(0, 0, -10);

	//_cone = new Cone(0, 0, 0, 2, 3);
	//_cone->SetRadius(1.5);
	//_cone->SetHeight(2);
	//_cone->SetPos(-5, 0, -10);

	//_sphere = new Sphere(0, 0, 0, 2);
	//_sphere->SetPos(5, 0, 0);
	//_sphere->SetRadius(5);
}

void GameLoop::GLSettings()
{
	glShadeModel(GL_SMOOTH);
	//glFrontFace	( GL_CCW		);
	glDisable(GL_FOG);
	glDisable(GL_LIGHTING);

	glDisable(GL_LINE_SMOOTH);
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);

	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

GameLoop::~GameLoop()
{

}

void GameLoop::Update(float deltaTime)
{

}

void GameLoop::Draw()
{
	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glViewport(0, 0, _sw, _sh);
	glEnable(GL_CULL_FACE);

	//_cam->SetProjection();
	//_cam->SetModelViewMatrix();
	//_cam->UpdateCamera();

	//_floor->Draw();
	//_triangle->Draw();
	//_box->Draw();
	//_box2->Draw();
	//_cylinder->Draw();
	//_cone->Draw();
	//_sphere->Draw();
}


void GameLoop::DemoLight(void)
{
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_NORMALIZE);

	// Light model parameters:
	// -------------------------------------------

	GLfloat lmKa[] = { 0.0, 0.0, 0.0, 0.0 };
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmKa);

	glLightModelf(GL_LIGHT_MODEL_LOCAL_VIEWER, 1.0);
	glLightModelf(GL_LIGHT_MODEL_TWO_SIDE, 0.0);

	// -------------------------------------------
	// Spotlight Attenuation

	GLfloat spot_direction[] = { 1.0, -1.0, -1.0 };
	GLint spot_exponent = 30;
	GLint spot_cutoff = 180;

	glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, spot_direction);
	glLighti(GL_LIGHT0, GL_SPOT_EXPONENT, spot_exponent);
	glLighti(GL_LIGHT0, GL_SPOT_CUTOFF, spot_cutoff);

	GLfloat Kc = 1.0;
	GLfloat Kl = 0.0;
	GLfloat Kq = 0.0;

	glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, Kc);
	glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, Kl);
	glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, Kq);


	// ------------------------------------------- 
	// Lighting parameters:

	GLfloat light_pos[] = { 0.0f, 5.0f, 5.0f, 1.0f };
	GLfloat light_Ka[] = { 1.0f, 0.5f, 0.5f, 1.0f };
	GLfloat light_Kd[] = { 1.0f, 0.1f, 0.1f, 1.0f };
	GLfloat light_Ks[] = { 1.0f, 1.0f, 1.0f, 1.0f };

	glLightfv(GL_LIGHT0, GL_POSITION, light_pos);
	glLightfv(GL_LIGHT0, GL_AMBIENT, light_Ka);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_Kd);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_Ks);

	// -------------------------------------------
	// Material parameters:

	GLfloat material_Ka[] = { 0.5f, 0.0f, 0.0f, 1.0f };
	GLfloat material_Kd[] = { 0.4f, 0.4f, 0.5f, 1.0f };
	GLfloat material_Ks[] = { 0.8f, 0.8f, 0.0f, 1.0f };
	GLfloat material_Ke[] = { 0.1f, 0.0f, 0.0f, 0.0f };
	GLfloat material_Se = 20.0f;

	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, material_Ka);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, material_Kd);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, material_Ks);
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, material_Ke);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, material_Se);
}
