#include "ModelVisibilityFrame.h"

ModelVisibilityFrame::ModelVisibilityFrame(int x, int y, int w, int h, SUIActionListener* actionListener) {
	_frame = new SUIFrame((float)x, (float)y, (float)w, (float)h, SUIFrame::V_ALIGNMENT);
	_frame->SetName("Model Drawing Frame", SUIFrame::LEFT);

	_frame->Add(SetModelBox(actionListener));
}

void ModelVisibilityFrame::SetPos(int x, int y) {
	_frame->SetPos(x, y);
	_frame->ResetBounds();
}

SUIBox* ModelVisibilityFrame::SetModelBox(SUIActionListener* actionListener) {
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
	vmodel->AddBox(header);

	string names[5] = {"Car", "Truck", "Model1", "Model2", "Model3"};

	for(int i=0; i<5; i++)
	{
		ModelBox* modelBox = new ModelBox(names[i], actionListener);
		modelBoxVec.push_back( modelBox );
		vmodel->AddBox(modelBox->model);

		modelBox->modelCheckBox->SetSelect(i == 0);
	}

	return vmodel;
}

int ModelVisibilityFrame::GetWidth()
{
	return _frame->GetW();
}

SUIFrame* ModelVisibilityFrame::GetFrame()
{
	return _frame;
}

ModelVisibilityFrame::~ModelVisibilityFrame() 
{
	if (_frame)
	{
		delete _frame;
		_frame = NULL;
	}

	for(int i=0; i<modelBoxVec.size(); i++)
	{
		if(modelBoxVec[i])
		{
			delete modelBoxVec[i];
			modelBoxVec[i] = NULL;
		}
	}
}