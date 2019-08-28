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

public:
	SUIButton* nextFrame;
	SUIButton* previousFrame;
	SUIButton* playAnimation;
	SUIButton* stopAnimation;
	SUITextField* particleSize;
	SUIList* selectedFrame;
	VisibilityBox* visibilityBox;

	ECoatUI(float x, float y, float w, float h, int numberOfFrames, SUIActionListener* actionListener);
	~ECoatUI();

	void SetPos(float x, float y);

	float GetWidth();
	int GetSelectedFrameIndex();
	void SetVisible(bool visible);
};
#endif
