#ifndef ECoatUI_H
#define ECoatUI_H

#include "SUI/SUIFrame.h"
#include "SUI/SUIActionListener.h"
#include "SUI/SUICheckBox.h"
#include "SUI/SUIButton.h"
#include "SUI/SUITextField.h"
#include "SUI/SUIList.h"

class ECoatUI
{
private:
	SUIFrame* _frame;

public:
	SUIButton* nextFrame;
	SUIButton* previousFrame;
	SUIButton* playAnimation;
	SUIButton* stopAnimation;
	SUITextField* particleSize;
	SUIList* selectedFrame;

	ECoatUI(float x, float y, float w, float h, int numberOfFrames, SUIActionListener* actionListener);
	~ECoatUI();

	void SetPos(float x, float y);

	float GetWidth();
	int GetSelectedFrameIndex();
	void SetVisible(bool visible);
};
#endif
