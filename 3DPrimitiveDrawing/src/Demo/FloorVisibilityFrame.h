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

	FloorVisibilityFrame(int x, int y, int w, int h, SUIActionListener* actionListener);
	~FloorVisibilityFrame();

	void SetPos(int x, int y);
	void SetVisible(bool visible);
	void actionPerformed(SUIActionEvent e);

	int GetWidth();
	int GetDemoIndex();
};
#endif
