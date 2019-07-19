#include "WholeWindowFrame.h"
#include "SUI/SUIBox.h"

WholeWindowFrame::WholeWindowFrame(int x, int y, int w, int h, SUIActionListener* action_listener)
{
	_frame = new SUIFrame((float)x, (float)y, (float)w, (float)h, SUIFrame::V_ALIGNMENT);
	_frame->SetName("Demo Frame", SUIFrame::LEFT);

	SUIBox* demoSelectionBox = new SUIBox(SUIBox::V_ALIGNMENT);
	demoSelectionBox->SetMargin(5, 5, 10, 5);
	demoSelectionBox->SetName("Demo Selection", SUIBox::LEFT);
	demoSelectionBox->SetOnOffEnable(true);
	demoSelectionBox->SetOn(true);

	demoType = new SUIRadioButton(SUIRadioButton::V_ALIGNMENT);
	demoType->AddCheckBox(new SUICheckBox("Renderer Demo", SUICheckBox::LEFT));
	demoType->AddCheckBox(new SUICheckBox("Particle Demo", SUICheckBox::LEFT));
	demoType->AddActionListener(action_listener);
	demoType->SetSelect(0);

	isSSAO = new SUICheckBox("SSAO", SUICheckBox::LEFT);
	isSSAO->SetBgVisible(true);
	isSSAO->AddActionListener(action_listener);

	floorSelection = new SUICheckBox("Hide Floor", SUICheckBox::LEFT);
	floorSelection->SetBgVisible(true);
	floorSelection->AddActionListener(action_listener);

	demoSelectionBox->AddRadioButton(demoType);
	demoSelectionBox->AddCheckBox(isSSAO);
	demoSelectionBox->AddCheckBox(floorSelection);
	demoSelectionBox->SetBgVisible(true);
	demoSelectionBox->SetBgColor(64, 64, 64, 255);

	_frame->Add(demoSelectionBox);
}

int WholeWindowFrame::GetDemoIndex()
{
	return demoType->GetSelectedIndex();
}

void WholeWindowFrame::SetPos(int x, int y) {
	_frame->SetPos(x, y);
	_frame->ResetBounds();
}

int WholeWindowFrame::GetWidth()
{
	return _frame->GetW();
}

WholeWindowFrame::~WholeWindowFrame() {
	if (_frame)
	{
		delete _frame;
		_frame = NULL;
	}
}