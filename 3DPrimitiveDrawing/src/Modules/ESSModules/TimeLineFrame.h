#ifdef _ENABLE_DEMO

#ifndef TimeLineFrame_H
#define TimeLineFrame_H

#include "SUI/SUIFrame.h"
#include "SUI/SUIActionListener.h"
#include "SUI/SUIRadioButton.h"
#include "SUI/SUICheckBox.h"
#include "SUI/SUIButton.h"
#include "SUI/SUITextField.h"

class TimeLineFrame
{
private:
	SUIFrame* _frame;

public:
	SUIButton* nextFrame;
	SUIButton* previousFrame;
	SUIButton* playAnimation;
	SUIButton* stopAnimation;
	SUITextField* particleSize;
	SUIRadioButton* selectedFrame;
	
	TimeLineFrame(float x, float y, float w, float h, int numberOfFrames, SUIActionListener* actionListener);
	~TimeLineFrame();

	void SetPos(float x, float y);

	float GetWidth();
	int GetSelectedFrameIndex();
	void SetVisible(bool visible);
};
#endif

#endif