#ifdef _ENABLE_DEMO

#include "ModelVisibilityFrame.h"

ModelVisibilityFrame::ModelVisibilityFrame(float x, float y, float w, float h, SUIActionListener* actionListener) 
{
	_frame = new SUIFrame(x, y, w, h, SUIFrame::V_ALIGNMENT);
	_frame->SetName("Model Drawing Frame", SUIFrame::LEFT);
	_frame->Add(SetModelBox(actionListener));
}

void ModelVisibilityFrame::SetPos(float x, float y) 
{
	_frame->SetPos(x, y);
	_frame->ResetBounds();
}

SUIBox* ModelVisibilityFrame::SetModelBox(SUIActionListener* actionListener) 
{
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

	string names[5] = {"Car", "Trike", "Truck", "Plane", "Teapot"};

	for(int i=0; i<5; i++)
	{
		ModelBox* modelBox = new ModelBox(names[i], actionListener);
		modelBoxVec.push_back( modelBox );
		vmodel->AddBox(modelBox->model);

		modelBox->modelCheckBox->SetSelect(true);
	}

	return vmodel;
}

float ModelVisibilityFrame::GetWidth()
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

#endif