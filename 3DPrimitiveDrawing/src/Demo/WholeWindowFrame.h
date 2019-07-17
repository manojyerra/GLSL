#ifndef WholeWindowFrame_H
#define WholeWindowFrame_H

#include "SUI/SUIFrame.h"
#include "SUI/SUIActionListener.h"
#include "SUI/SUIRadioButton.h"
#include "SUI/SUIButton.h"
#include "SUI/SUIBox.h"
#include "SUI/SUICheckBox.h"

class WholeWindowFrame : public SUIActionListener {

private:
	SUIFrame* _frame;
	SUIRadioButton* _demoType;
	SUICheckBox* _isSSAO;

public:
	WholeWindowFrame(int x, int y, int w, int h);
	~WholeWindowFrame();

	void SetPos(int x, int y);
	void actionPerformed(SUIActionEvent e);
};
#endif
