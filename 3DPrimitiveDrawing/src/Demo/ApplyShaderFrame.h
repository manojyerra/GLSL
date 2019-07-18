#ifndef ApplyShaderFrame_H
#define ApplyShaderFrame_H

#include "SUI/SUIFrame.h"
#include "SUI/SUIActionListener.h"
#include "SUI/SUIRadioButton.h"

class ApplyShaderFrame
{

private:
	SUIFrame* _frame;

public:
	SUIRadioButton* _model;

	ApplyShaderFrame(int x, int y, int w, int h, SUIActionListener* actionListener);
	~ApplyShaderFrame();

	void SetPos(int x, int y);
	int GetWidth();
	SUIFrame* GetFrame();
};
#endif
