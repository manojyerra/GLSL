#ifdef _ENABLE_DEMO

#ifndef Demo_H
#define Demo_H

#include "Floor.h"
#include "ParticlesDemo.h"
#include "RenderDemo.h"
#include "WholeWindowFrame.h"
#include "SUIActionListener.h"
#include "FloorVisibilityFrame.h"
#include "SUI.h"

class Demo : public SUIActionListener
{
private:
	Floor* _floor;

	ParticlesDemo* _particleDemo;
	RenderDemo* _rendererDemo;

	WholeWindowFrame* _windowFrame;
	FloorVisibilityFrame*_floorFrame;

	void actionPerformed(SUIActionEvent ae);

public:
	Demo(float sw, float sh);
	~Demo();

	void SetScreenSize(float sw, float sh);
	void Update(float deltaTime);
	void Draw();
};

#endif

#else

class Demo
{
private:

public:
	Demo(float sw, float sh){}
	~Demo() {}

	void SetScreenSize(float sw, float sh) {}
	void Update(float deltaTime) {}
	void Draw() {}
};

#endif
