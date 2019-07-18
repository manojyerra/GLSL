#ifndef ModelSelectionFrame_H
#define ModelSelectionFrame_H

#include "SUI/SUIFrame.h"
#include "SUI/SUIActionListener.h"
#include "SUI/SUIRadioButton.h"

class ModelSelectionFrame
{

private:
	SUIFrame* _frame;

public:
	SUIRadioButton* _model;

	ModelSelectionFrame(int x, int y, int w, int h, SUIActionListener* actionListener);
	~ModelSelectionFrame();

	void SetPos(int x, int y);
	int GetWidth();
	SUIFrame* GetFrame();
};
#endif
