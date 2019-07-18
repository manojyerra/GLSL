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

	WholeWindowFrame(int x, int y, int w, int h, SUIActionListener* actionListener);
	~WholeWindowFrame();

	void SetPos(int x, int y);
	void actionPerformed(SUIActionEvent e);

	int GetWidth();
	int GetDemoIndex();
};
#endif
