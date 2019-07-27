#ifdef _ENABLE_DEMO

#ifndef WholeWindowFrame_H
#define WholeWindowFrame_H

#include "SUI/SUIFrame.h"
#include "SUI/SUIActionListener.h"
#include "SUI/SUIRadioButton.h"
#include "SUI/SUICheckBox.h"

class WholeWindowFrame
{
private:
	SUIFrame* _frame;

public:
	SUIRadioButton* demoType;
	SUICheckBox* isSSAO;

	WholeWindowFrame(float x, float y, float w, float h, SUIActionListener* actionListener);
	~WholeWindowFrame();

	void SetPos(float x, float y);
	void actionPerformed(SUIActionEvent e);

	float GetWidth();
	int GetDemoIndex();
};
#endif

#endif