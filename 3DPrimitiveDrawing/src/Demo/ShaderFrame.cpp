#include "ShaderFrame.h"

ShaderFrame::ShaderFrame(int x, int y, int w, int h) {
	
	_frame = new SUIFrame((float)x, (float)y, (float)w, (float)h, SUIFrame::V_ALIGNMENT);
	_frame->SetName("Shader Frame", SUIFrame::LEFT);

	/*_frame->Add(new SUIButton("Load Models", this));
	_frame->Add(new SUIButton("Save Models", this));

	_frame->Add(enablePhysics = new SUICheckBox("Enable Physics (P)", SUICheckBox::LEFT, this));
	_frame->Add(pausePhysics = new SUICheckBox("Pause Physics (Space)", SUICheckBox::LEFT, this));

	enablePhysics->SetShortCut(SUIKeyCombi('P'));
	pausePhysics->SetShortCut(SUIKeyCombi(VK_SPACE));

	_frame->Add(showMarkedObjs = new SUICheckBox("Show Marked Objects", SUICheckBox::LEFT, this));
	_frame->Add(showUnmarkedObjs = new SUICheckBox("Show Unmarked Objects", SUICheckBox::LEFT, this));
	_frame->Add(markAllObjs = new SUIButton("Mark all objects", SUIButton::LEFT, this));
	_frame->Add(unmarkAllObjs = new SUIButton("Unmark all objects", SUIButton::LEFT, this));

	_frame->Add(showBoundShapes = new SUICheckBox("Show Bounding Shapes (B)", SUICheckBox::LEFT, this));
	_frame->Add(showBoundBox = new SUICheckBox("Show Bounding Box", SUICheckBox::LEFT, this));
	_frame->Add(onBorder = new SUICheckBox("Selected Object Border", SUICheckBox::LEFT, this));

	showBoundShapes->SetShortCut(SUIKeyCombi('B'));

	showMarkedObjs->SetSelect(true);
	showUnmarkedObjs->SetSelect(true);
	onBorder->SetSelect(true);*/

	SUIBox* propertyBox = new SUIBox(SUIBox::V_ALIGNMENT);
	propertyBox->SetMargin(5, 5, 10, 5);
	propertyBox->SetName("Shader Options", SUIBox::LEFT);
	propertyBox->SetOnOffEnable(true);
	propertyBox->SetOn(true);

	propertyBox->AddBox(SetShaderTypeBox());
	propertyBox->AddBox(SetPropertyBox());
	propertyBox->AddBox(SetMetalPropertyBox());
	propertyBox->AddBox(SetLightprpertyBox());
	propertyBox->SetBgVisible(true);
	propertyBox->SetBgColor(64, 64, 64, 255);

	_frame->Add(propertyBox);
	

}

SUIBox* ShaderFrame::SetShaderTypeBox() {

	SUIBox* shaderTypeBox = new SUIBox(SUIBox::V_ALIGNMENT);
	shaderTypeBox->SetMargin(3, 3, 5, 0);
	shaderTypeBox->SetName("Shader Type", SUIBox::LEFT);
	_shaderType = new SUIRadioButton(SUIRadioButton::V_ALIGNMENT);
	_shaderType->AddCheckBox(new SUICheckBox("PBR Shader", SUICheckBox::LEFT));
	_shaderType->AddCheckBox(new SUICheckBox("Phong Shader", SUICheckBox::LEFT));
	_shaderType->AddActionListener(this);
	_shaderType->SetName("Shader", SUIRadioButton::LEFT);
	_shaderType->SetSelect(0);
	shaderTypeBox->AddRadioButton(_shaderType);
	shaderTypeBox->SetOnOffEnable(true);

	return shaderTypeBox;
}

SUIBox* ShaderFrame::SetPropertyBox() {
	SUIBox* shaderTypeBox = new SUIBox(SUIBox::V_ALIGNMENT);
	shaderTypeBox->SetMargin(3, 3, 5, 0);
	shaderTypeBox->SetName("Shader Type", SUIBox::LEFT);
	_shaderType = new SUIRadioButton(SUIRadioButton::V_ALIGNMENT);
	_shaderType->AddCheckBox(new SUICheckBox("PBR Shader", SUICheckBox::LEFT));
	_shaderType->AddCheckBox(new SUICheckBox("Phong Shader", SUICheckBox::LEFT));
	_shaderType->AddActionListener(this);
	_shaderType->SetName("Shader", SUIRadioButton::LEFT);
	_shaderType->SetSelect(0);
	shaderTypeBox->AddRadioButton(_shaderType);
	shaderTypeBox->SetOnOffEnable(true);

	return shaderTypeBox;

}

SUIBox* ShaderFrame::SetMetalPropertyBox() {
	SUIBox* metalProperypBox = new SUIBox(SUIBox::V_ALIGNMENT);
	metalProperypBox->SetMargin(3, 3, 5, 0);
	metalProperypBox->SetName("Metal property", SUIBox::LEFT);
	_materialType = new SUIChoice(3);
	_materialType->Add("Gold");
	_materialType->Add("plastic");
	_materialType->Add("Steel");
	_materialType->Add("Silver");
	_materialType->Add("Material1");
	_materialType->AddActionListener(this);
	_materialType->SetName("Material", SUIChoice::LEFT);
	_materialType->SetSelect(0);
	metalProperypBox->AddChoice(_materialType);
	metalProperypBox->SetOnOffEnable(true);

	SUIBox* albedoValuepBox = new SUIBox(SUIBox::V_ALIGNMENT);
	//_albedoR = new SUITextField("Albedo ", SUITextField::INPUT_DOUBLE);
	//_albedoR->AddActionListener(this);
	
	metalProperypBox->AddTextField(_albedo);
	metalProperypBox->SetOnOffEnable(true);
	

	return metalProperypBox;
}

SUIBox* ShaderFrame::SetLightprpertyBox() {
	SUIBox* shaderTypeBox = new SUIBox(SUIBox::V_ALIGNMENT);
	shaderTypeBox->SetMargin(3, 3, 5, 0);
	shaderTypeBox->SetName("Shader Type", SUIBox::LEFT);
	_shaderType = new SUIRadioButton(SUIRadioButton::V_ALIGNMENT);
	_shaderType->AddCheckBox(new SUICheckBox("PBR Shader", SUICheckBox::LEFT));
	_shaderType->AddCheckBox(new SUICheckBox("Phong Shader", SUICheckBox::LEFT));
	_shaderType->AddActionListener(this);
	_shaderType->SetName("Shader", SUIRadioButton::LEFT);
	_shaderType->SetSelect(0);
	shaderTypeBox->AddRadioButton(_shaderType);
	shaderTypeBox->SetOnOffEnable(true);

	return shaderTypeBox;
}

void ShaderFrame::actionPerformed(SUIActionEvent e) {

}

ShaderFrame::~ShaderFrame() {

}