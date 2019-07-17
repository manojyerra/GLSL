#ifndef ShaderFrame_H
#define ShaderFrame_H

#include "SUI/SUIFrame.h"
#include "SUI/SUIActionListener.h"
#include "SUI/SUIRadioButton.h"
#include "SUI/SUICheckBox.h"
#include "SUI/SUIButton.h"
#include "SUI/SUILabel.h"
#include "SUI/SUIChoice.h"
#include "SUI/SUITextField.h"
#include "SUI/SUIBox.h"
#include "SUI/SUIChoice.h"
#include "SUI/SUITextField.h"

#include "Cam.h"


class ShaderFrame : public SUIActionListener {

private:
	SUIFrame* _frame;

	SUIRadioButton* _shaderType;
	SUIChoice* _materialType;
	SUITextField* _albedo;

	//////////////
	SUICheckBox* floorVisible;
	SUICheckBox* showAxis;
	SUICheckBox* showGrid;
	SUICheckBox* showGridLines;

	SUICheckBox* enablePhysics;
	SUICheckBox* pausePhysics;
	SUICheckBox* showMarkedObjs;
	SUICheckBox* showUnmarkedObjs;
	SUIButton* markAllObjs;
	SUIButton* unmarkAllObjs;
	SUICheckBox* showBoundShapes;
	SUICheckBox* showBoundBox;
	SUICheckBox* onBorder;
	SUIButton* changeViewBtn;

	Cam* _cam;

	SUIBox* SetPropertyBox();
	SUIBox* SetMetalPropertyBox();
	SUIBox* SetLightprpertyBox();
	SUIBox* SetShaderTypeBox();

public:
	ShaderFrame(int x, int y, int w, int h);
	~ShaderFrame();

	void actionPerformed(SUIActionEvent e);

};

#endif