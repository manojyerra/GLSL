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
	SUIRadioButton* model;
	SUICheckBox* applyShaderToAll;

	ModelSelectionFrame(float x, float y, float w, float h, SUIActionListener* actionListener);
	~ModelSelectionFrame();

	void SetPos(float x, float y);
	float GetWidth();
	SUIFrame* GetFrame();
};
#endif
