#include "ModelDrawingFrame.h"

ModelDrawingFrame::ModelDrawingFrame(int x, int y, int w, int h) {
	_frame = new SUIFrame((float)x, (float)y, (float)w, (float)h, SUIFrame::V_ALIGNMENT);
	_frame->SetName("Model Drawing Frame", SUIFrame::LEFT);

	_frame->Add(SetModelBox());
}

void ModelDrawingFrame::SetPos(int x, int y) {
	_frame->SetPos(x, y);
	_frame->ResetBounds();
}

SUIBox* ModelDrawingFrame::SetModelBox() {
	SUIBox* vmodel = new SUIBox(SUIBox::V_ALIGNMENT);
	vmodel->SetMargin(5, 5, 10, 5);
	vmodel->SetName("Model Drawing Selection", SUIBox::LEFT);
	vmodel->SetOnOffEnable(true);
	vmodel->SetOn(true);
	vmodel->SetBgVisible(true);

	//heading
	SUIBox* header = new SUIBox(SUIBox::H_ALIGNMENT);
	header->AddLabel(new SUILabel("Model", SUILabel::CENTER));
	header->AddLabel(new SUILabel("Position", SUILabel::CENTER));
	
	model1 = new ModelBox("Car", this);
	model2 = new ModelBox("Truck", this);
	model3 = new ModelBox("Model1", this);
	model4 = new ModelBox("Model2", this);
	model5 = new ModelBox("Model3", this);

	vmodel->AddBox(header);
	vmodel->AddBox(model1->model);
	vmodel->AddBox(model2->model);
	vmodel->AddBox(model3->model);
	vmodel->AddBox(model4->model);
	vmodel->AddBox(model5->model);

	return vmodel;
}
void ModelDrawingFrame::actionPerformed(SUIActionEvent e) {

}

ModelDrawingFrame::~ModelDrawingFrame() {
	if (_frame)
	{
		delete _frame;
		_frame = NULL;
	}

	if (model1) {
		delete model1;
	}

	if (model2) {
		delete model2;
	}

	if (model3) {
		delete model3;
	}

	if (model4) {
		delete model4;
	}

	if (model5) {
		delete model5;
	}
}