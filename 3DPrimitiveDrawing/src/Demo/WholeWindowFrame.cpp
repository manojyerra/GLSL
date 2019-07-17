#include "WholeWindowFrame.h"

WholeWindowFrame::WholeWindowFrame(int x, int y, int w, int h) {
	_frame = new SUIFrame((float)x, (float)y, (float)w, (float)h, SUIFrame::V_ALIGNMENT);
	_frame->SetName("Demo Frame", SUIFrame::LEFT);


	SUIBox* demoSelectionBox = new SUIBox(SUIBox::V_ALIGNMENT);
	demoSelectionBox->SetMargin(5, 5, 10, 5);
	demoSelectionBox->SetName("Demo Selection", SUIBox::LEFT);
	demoSelectionBox->SetOnOffEnable(true);
	demoSelectionBox->SetOn(true);

	_demoType = new SUIRadioButton(SUIRadioButton::V_ALIGNMENT);
	_demoType->AddCheckBox(new SUICheckBox("Renderer Demo", SUICheckBox::LEFT));
	_demoType->AddCheckBox(new SUICheckBox("Particle Demo", SUICheckBox::LEFT));
	_demoType->AddActionListener(this);
	_demoType->SetSelect(0);

	_isSSAO = new SUICheckBox("SSAO", SUICheckBox::LEFT);
	_isSSAO->SetBgVisible(true);
	_isSSAO->AddActionListener(this);

	demoSelectionBox->AddRadioButton(_demoType);
	demoSelectionBox->AddCheckBox(_isSSAO);
	demoSelectionBox->SetBgVisible(true);
	demoSelectionBox->SetBgColor(64, 64, 64, 255);

	_frame->Add(demoSelectionBox);


}

void WholeWindowFrame::SetPos(int x, int y) {
	_frame->SetPos(x, y);
	_frame->ResetBounds();
}

void WholeWindowFrame::actionPerformed(SUIActionEvent e) {
	SUIComponent* com = (SUIComponent*)e.GetComponent();
	if (com == _demoType) {
		printf("Slected Demo %d", _demoType->GetSelectedIndex());
	}
	else if(com == _isSSAO)	{
		printf("IS SSAO Slected =%d",_isSSAO->IsSelected());
	}
}


WholeWindowFrame::~WholeWindowFrame() {
	if (_frame)
	{
		delete _frame;
		_frame = NULL;
	}
}