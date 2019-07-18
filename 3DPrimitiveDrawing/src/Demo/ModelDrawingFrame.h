#ifndef ModelDrawingFrame_H
#define ModelDrawingFrame_H

#include "SUI/SUIFrame.h"
#include "SUI/SUIActionListener.h"
#include "SUI/SUIButton.h"
#include "SUI/SUIBox.h"
#include "SUI/SUICheckBox.h"
#include "glm/glm.hpp"


class ModelBox {
public:
	SUICheckBox* modelCheckBox;
	SUITextField* positionModelX;
	SUITextField* positionModelY;
	SUITextField* positionModelZ;
	SUIBox* model;

	ModelBox(std::string modelName, SUIActionListener* actionListener) {
		//Car model
		model = new SUIBox(SUIBox::H_ALIGNMENT);

		modelCheckBox = new SUICheckBox(modelName, SUICheckBox::LEFT);
		modelCheckBox->AddActionListener(actionListener);

		SUIBox* modelPosition = new SUIBox(SUIBox::H_ALIGNMENT);
		positionModelX = new SUITextField("PositionX ", SUITextField::INPUT_DOUBLE);
		positionModelX->AddActionListener(actionListener);
		positionModelY = new SUITextField("PositionY ", SUITextField::INPUT_DOUBLE);
		positionModelY->AddActionListener(actionListener);
		positionModelZ = new SUITextField("PositionZ ", SUITextField::INPUT_DOUBLE);
		positionModelZ->AddActionListener(actionListener);
		modelPosition->AddTextField(positionModelX);
		modelPosition->AddTextField(positionModelY);
		modelPosition->AddTextField(positionModelZ);

		model->AddCheckBox(modelCheckBox);
		model->AddBox(modelPosition);
	}

};
class ModelDrawingFrame
{
private:
	SUIFrame* _frame;

	SUIBox* SetModelBox(SUIActionListener* actionListener);
public:

	vector<ModelBox*> modelBoxVec;

	ModelDrawingFrame(int x, int y, int w, int h, SUIActionListener* actionListener);
	~ModelDrawingFrame();

	void SetPos(int x, int y);
	int GetWidth();
	SUIFrame* GetFrame();
	//void actionPerformed(SUIActionEvent e);
};
#endif
