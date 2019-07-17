#ifndef ApplyShaderFrame_H
#define ApplyShaderFrame_H

#include "SUI/SUIFrame.h"
#include "SUI/SUIActionListener.h"
#include "SUI/SUIRadioButton.h"
#include "SUI/SUIButton.h"
#include "SUI/SUIBox.h"
#include "SUI/SUICheckBox.h"

class ApplyShaderFrame : public SUIActionListener {

private:
	SUIFrame* _frame;
	SUIRadioButton* _model;

public:
	ApplyShaderFrame(int x, int y, int w, int h);
	~ApplyShaderFrame();

	void SetPos(int x, int y);
	void actionPerformed(SUIActionEvent e);
};
#endif