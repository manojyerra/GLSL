#ifdef _ENABLE_DEMO

#include "ModelSelectionFrame.h"
#include "SUI/SUIBox.h"

ModelSelectionFrame::ModelSelectionFrame(float x, float y, float w, float h, SUIActionListener* actionListener) {
	_frame = new SUIFrame(x, y, w, h, SUIFrame::V_ALIGNMENT);
	_frame->SetName("Model Selection Frame", SUIFrame::LEFT);

	SUIBox* modelSelectionBox = new SUIBox(SUIBox::V_ALIGNMENT);
	modelSelectionBox->SetMargin(5, 5, 10, 5);
	modelSelectionBox->SetName("Model Selection", SUIBox::LEFT);
	modelSelectionBox->SetOnOffEnable(true);
	modelSelectionBox->SetOn(true);

	model = new SUIRadioButton(SUIRadioButton::V_ALIGNMENT);
	model->AddCheckBox(new SUICheckBox("Car", SUICheckBox::LEFT));
	model->AddCheckBox(new SUICheckBox("Trike", SUICheckBox::LEFT));
	model->AddCheckBox(new SUICheckBox("Truck", SUICheckBox::LEFT));
	model->AddCheckBox(new SUICheckBox("Plane", SUICheckBox::LEFT));
	model->AddCheckBox(new SUICheckBox("Teapot", SUICheckBox::LEFT));
	model->AddActionListener(actionListener);
	model->SetSelect(0);

	modelSelectionBox->AddRadioButton(model);
	modelSelectionBox->SetBgVisible(true);
	modelSelectionBox->SetBgColor(64, 64, 64, 255);

	applyShaderToAll = new SUICheckBox("Apply shader to all models", SUICheckBox::LEFT);

	_frame->Add(modelSelectionBox);
	_frame->Add(applyShaderToAll);
}

void ModelSelectionFrame::SetPos(float x, float y) {
	_frame->SetPos(x, y);
	_frame->ResetBounds();
}

float ModelSelectionFrame::GetWidth()
{
	return _frame->GetW();
}

SUIFrame* ModelSelectionFrame::GetFrame()
{
	return _frame;
}

ModelSelectionFrame::~ModelSelectionFrame() {
	if (_frame)
	{
		delete _frame;
		_frame = NULL;
	}
}

#endif