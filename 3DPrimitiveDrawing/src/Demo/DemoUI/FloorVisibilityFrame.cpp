#ifdef ENABLE_DEMO

#include "FloorVisibilityFrame.h"
#include "SUI/SUIBox.h"

FloorVisibilityFrame::FloorVisibilityFrame(float x, float y, float w, float h, SUIActionListener* action_listener)
{
	_frame = new SUIFrame(x, y, w, h, SUIFrame::V_ALIGNMENT);
	_frame->SetName("Floor Visibility Frame", SUIFrame::LEFT);

	SUIBox* floorBox = new SUIBox(SUIBox::V_ALIGNMENT);
	floorBox->SetMargin(5, 5, 10, 5);
	floorBox->SetName("Floor Visibility", SUIBox::LEFT);
	floorBox->SetOnOffEnable(true);
	floorBox->SetOn(true);

	hideFloor = new SUICheckBox("Hide Floor", SUICheckBox::LEFT);
	hideFloor->SetBgVisible(true);
	hideFloor->AddActionListener(action_listener);

	showOnlyGridLines = new SUICheckBox("Show only grid lines", SUICheckBox::LEFT);
	showOnlyGridLines->SetBgVisible(true);
	showOnlyGridLines->AddActionListener(action_listener);

	floorBox->AddCheckBox(hideFloor);
	floorBox->AddCheckBox(showOnlyGridLines);
	floorBox->SetBgVisible(true);
	floorBox->SetBgColor(64, 64, 64, 255);

	_frame->Add(floorBox);
}


void FloorVisibilityFrame::SetPos(float x, float y) {
	_frame->SetPos(x, y);
	_frame->ResetBounds();
}

void FloorVisibilityFrame::SetVisible(bool visible)
{
	_frame->SetVisible(visible);
}

float FloorVisibilityFrame::GetWidth()
{
	return _frame->GetW();
}

FloorVisibilityFrame::~FloorVisibilityFrame() {
	if (_frame)
	{
		delete _frame;
		_frame = NULL;
	}
}

#endif