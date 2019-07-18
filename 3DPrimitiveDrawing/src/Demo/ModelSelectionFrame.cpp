#include "ModelSelectionFrame.h"
#include "SUI/SUIBox.h"

ModelSelectionFrame::ModelSelectionFrame(int x, int y, int w, int h, SUIActionListener* actionListener) {
	_frame = new SUIFrame((float)x, (float)y, (float)w, (float)h, SUIFrame::V_ALIGNMENT);
	_frame->SetName("Model Selection Frame", SUIFrame::LEFT);


	SUIBox* modelSelectionBox = new SUIBox(SUIBox::V_ALIGNMENT);
	modelSelectionBox->SetMargin(5, 5, 10, 5);
	modelSelectionBox->SetName("Model Selection", SUIBox::LEFT);
	modelSelectionBox->SetOnOffEnable(true);
	modelSelectionBox->SetOn(true);

	_model = new SUIRadioButton(SUIRadioButton::V_ALIGNMENT);
	_model->AddCheckBox(new SUICheckBox("Car", SUICheckBox::LEFT));
	_model->AddCheckBox(new SUICheckBox("Truck", SUICheckBox::LEFT));
	_model->AddCheckBox(new SUICheckBox("Model1", SUICheckBox::LEFT));
	_model->AddCheckBox(new SUICheckBox("Model2", SUICheckBox::LEFT));
	_model->AddCheckBox(new SUICheckBox("Model3", SUICheckBox::LEFT));
	_model->AddActionListener(actionListener);
	_model->SetSelect(0);

	modelSelectionBox->AddRadioButton(_model);
	modelSelectionBox->SetBgVisible(true);
	modelSelectionBox->SetBgColor(64, 64, 64, 255);

	_frame->Add(modelSelectionBox);
}

void ModelSelectionFrame::SetPos(int x, int y) {
	_frame->SetPos(x, y);
	_frame->ResetBounds();
}

int ModelSelectionFrame::GetWidth()
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