#ifdef _ENABLE_DEMO

#include "TimeLineFrame.h"
#include "SUI/SUIBox.h"

TimeLineFrame::TimeLineFrame(float x, float y, float w, float h, int numberOfFrames, SUIActionListener* action_listener)
{
	_frame = new SUIFrame(x, y, w, h, SUIFrame::V_ALIGNMENT);
	_frame->SetName("Frame Selection", SUIFrame::LEFT);
	_frame->SetRemoveCloseOption(true);
	_frame->SetRemoveMaximizedOption(true);

	SUIBox* animationBox = new SUIBox(SUIBox::V_ALIGNMENT);
	animationBox->SetMargin(5, 5, 10, 5);
	animationBox->SetName("Animation", SUIBox::LEFT);
	animationBox->SetOnOffEnable(true);
	animationBox->SetOn(true);

	SUIBox* animationRow1Box = new SUIBox(SUIBox::H_ALIGNMENT);
	animationBox->SetMargin(5, 5, 10, 5);
	animationBox->SetOnOffEnable(true);
	animationBox->SetOn(true);

	SUIBox* animationRow2Box = new SUIBox(SUIBox::H_ALIGNMENT);
	animationBox->SetMargin(5, 5, 10, 5);
	animationBox->SetOnOffEnable(true);
	animationBox->SetOn(true);

	nextFrame = new SUIButton("Next Frame");
	nextFrame->AddActionListener(action_listener);

	previousFrame = new SUIButton("Previous Frame");
	previousFrame->AddActionListener(action_listener);

	animationRow1Box->AddButton(nextFrame);
	animationRow1Box->AddButton(previousFrame);

	playAnimation = new SUIButton("Play");
	stopAnimation = new SUIButton("Stop");
	animationRow2Box->AddButton(playAnimation);
	animationRow2Box->AddButton(stopAnimation);

	animationBox->AddBox(animationRow1Box);
	animationBox->AddBox(animationRow2Box);

	SUIBox* particleSizeBox = new SUIBox(SUIBox::H_ALIGNMENT);
	particleSizeBox->SetMargin(5, 5, 10, 5);
	particleSizeBox->SetName("Particle Size", SUIBox::LEFT);
	particleSizeBox->SetOnOffEnable(true);
	particleSizeBox->SetOn(true);

	SUILabel* particleSizeLabel = new SUILabel("Particle Size", SUILabel::CENTER);
	particleSize = new SUITextField("Particle Size ", SUITextField::INPUT_DOUBLE);

	particleSize->AddActionListener(action_listener);
	particleSizeBox->AddLabel(particleSizeLabel);
	particleSizeBox->AddTextField(particleSize);

	SUIBox* timeLineBox = new SUIBox(SUIBox::V_ALIGNMENT);
	timeLineBox->SetMargin(5, 5, 10, 5);
	timeLineBox->SetName("TimeLine", SUIBox::LEFT);
	timeLineBox->SetOnOffEnable(true);
	timeLineBox->SetOn(true);

	selectedFrame = new SUIList(20);
	selectedFrame->SetBgVisible(false);
	selectedFrame->SetSelectColor(45, 45, 45, 255);
	selectedFrame->SetNonSelectColor(64, 64, 64, 255);

	for (int i = 0; i < numberOfFrames; i++) {
		selectedFrame->Add(std::to_string(i + 1));
	}

	selectedFrame->AddActionListener(action_listener);
	selectedFrame->SetSelect(0);

	timeLineBox->AddList(selectedFrame);
	timeLineBox->SetBgVisible(true);
	timeLineBox->SetBgColor(64, 64, 64, 255);

	_frame->Add(animationBox);
	_frame->Add(particleSizeBox);
	_frame->Add(timeLineBox);
}

void TimeLineFrame::SetPos(float x, float y)
{
	_frame->SetPos(x, y);
	_frame->ResetBounds();
}

float TimeLineFrame::GetWidth()
{
	return _frame->GetW();
}

int TimeLineFrame::GetSelectedFrameIndex()
{
	return selectedFrame->GetSelectedIndex();
}

void TimeLineFrame::SetVisible(bool visible)
{
	_frame->SetVisible(visible);
}

TimeLineFrame::~TimeLineFrame() {
	if (_frame)
	{
		delete _frame;
		_frame = NULL;
	}
}

#endif