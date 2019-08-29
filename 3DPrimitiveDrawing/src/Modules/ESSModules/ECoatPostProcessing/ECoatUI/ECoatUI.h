#ifndef ECoatUI_H
#define ECoatUI_H

#include "SUIFrame.h"
#include "SUIActionListener.h"
#include "SUICheckBox.h"
#include "SUIButton.h"
#include "SUITextField.h"
#include "SUIList.h"
#include "SUIBox.h"

class VisibilityBox
{
private:
	SUIBox* _box;

public:
	SUICheckBox* floor;
	SUICheckBox* fluid;
	SUICheckBox* anodes;
	SUICheckBox* solid;
	SUICheckBox* colorBar;

	VisibilityBox(SUIBox* box)
	{
		_box = box;

		floor = nullptr;
		fluid = nullptr;
		anodes = nullptr;
		solid = nullptr;
		colorBar = nullptr;
	}

	SUIBox* GetBox()
	{
		return _box;
	}
};

class CameraBox
{
private:
	SUIBox* _box;

public:
	SUIButton* fontView;
	SUIButton* backView;
	SUIButton* leftView;
	SUIButton* rightView;
	SUIButton* topView;
	SUIButton* bottomView;
	SUIButton* changeView;

	SUIRadioButton* pivotRadioBtn;
	SUIButton* resetPos;

	CameraBox(SUIBox* box)
	{
		_box = box;

		fontView = nullptr;
		backView = nullptr;
		leftView = nullptr;
		rightView = nullptr;
		topView = nullptr;
		bottomView = nullptr;
		changeView = nullptr;

		pivotRadioBtn = nullptr;
		resetPos = nullptr;
	}

	SUIBox* GetBox()
	{
		return _box;
	}
};


class SolidRenderBox
{
private:
	SUIBox* _box;

public:
	SUIRadioButton* radioBtn;

	SolidRenderBox(SUIBox* box)
	{
		_box = box;
		radioBtn = nullptr;
	}

	SUIBox* GetBox()
	{
		return _box;
	}
};


class ECoatUI
{
private:
	SUIFrame* _frame;

	VisibilityBox* CreateVisibilityBox(SUIActionListener* action_listener);
	CameraBox* CreateCameraBox(SUIActionListener* action_listener);
	SolidRenderBox* CreateSolidRenderBox(SUIActionListener* action_listener);

public:
	SUIButton* nextFrame;
	SUIButton* previousFrame;
	SUIButton* playAnimation;
	SUIButton* stopAnimation;
	SUITextField* particleSize;
	SUIList* selectedFrame;

	VisibilityBox* visibilityBox;
	CameraBox* camBox;
	SolidRenderBox* solidRenderBox;

	ECoatUI(float x, float y, float w, float h, int numberOfFrames, SUIActionListener* actionListener);
	~ECoatUI();

	void SetPos(float x, float y);

	float GetWidth();
	int GetSelectedFrameIndex();
	void SetVisible(bool visible);
};
#endif
