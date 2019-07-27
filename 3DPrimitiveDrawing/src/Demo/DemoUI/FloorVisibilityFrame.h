#ifndef FloorVisibilityFrame_H
#define FloorVisibilityFrame_H

#include "SUI/SUIFrame.h"
#include "SUI/SUIActionListener.h"
#include "SUI/SUICheckBox.h"

class FloorVisibilityFrame
{
private:
	SUIFrame* _frame;

public:
	
	SUICheckBox* hideFloor;
	SUICheckBox* showOnlyGridLines;

	FloorVisibilityFrame(float x, float y, float w, float h, SUIActionListener* actionListener);
	~FloorVisibilityFrame();

	void SetPos(float x, float y);
	void SetVisible(bool visible);
	void actionPerformed(SUIActionEvent e);

	float GetWidth();
	int GetDemoIndex();
};
#endif
