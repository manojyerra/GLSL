#include "ShaderFrame.h"

ShaderFrame::ShaderFrame(int x, int y, int w, int h) {
	
	_frame = new SUIFrame((float)x, (float)y, (float)w, (float)h, SUIFrame::V_ALIGNMENT);
	_frame->SetName("Shader Frame", SUIFrame::LEFT);

	SUIBox* propertyBox = new SUIBox(SUIBox::V_ALIGNMENT);
	propertyBox->SetMargin(5, 5, 10, 5);
	propertyBox->SetName("Shader Options", SUIBox::LEFT);
	propertyBox->SetOnOffEnable(true);
	propertyBox->SetOn(true);

	propertyBox->AddBox(SetShaderTypeBox());
	propertyBox->AddBox(SetPropertyBox());
	propertyBox->SetBgVisible(true);
	propertyBox->SetBgColor(64, 64, 64, 255);

	_frame->Add(propertyBox);
}

void ShaderFrame::SetPos(int x, int y) {
	_frame->SetPos(x, y);
	_frame->ResetBounds();
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
	SUIBox* propertyBox = new SUIBox(SUIBox::V_ALIGNMENT);
	propertyBox->SetMargin(3, 3, 5, 0);
	propertyBox->SetName("Properties", SUIBox::LEFT);
	
	lightbox1 = new LightBox("Light_1", this, true);
	lightbox2 = new LightBox("Light_2", this, true);
	lightbox3 = new LightBox("Light_3", this, false);
	lightbox4 = new LightBox("Light_4", this, false);
	lightbox5 = new LightBox("Light_5", this, false);
	lightbox6 = new LightBox("Light_6", this, false);
	lightbox7 = new LightBox("Light_7", this, false);

	propertyBox->AddBox(SetMetalPropertyBox());
	propertyBox->AddBox(lightbox1->lightBox);
	propertyBox->AddBox(lightbox2->lightBox);
	propertyBox->AddBox(lightbox3->lightBox);
	propertyBox->AddBox(lightbox4->lightBox);
	propertyBox->AddBox(lightbox5->lightBox);
	propertyBox->AddBox(lightbox6->lightBox);
	propertyBox->AddBox(lightbox7->lightBox);

	return propertyBox;
}

SUIBox* ShaderFrame::SetMetalPropertyBox() {
	SUIBox* metalPropertypBox = new SUIBox(SUIBox::V_ALIGNMENT);
	metalPropertypBox->SetMargin(3, 3, 5, 0);
	metalPropertypBox->SetName("Metal property", SUIBox::LEFT);
	_materialType = new SUIChoice(3);
	_materialType->Add("Gold");
	_materialType->Add("plastic");
	_materialType->Add("Steel");
	_materialType->Add("Silver");
	_materialType->Add("Material1");
	_materialType->AddActionListener(this);
	_materialType->SetName("Material", SUIChoice::LEFT);
	_materialType->SetSelect(0);	

	SUIBox* albedoValuepBox = new SUIBox(SUIBox::H_ALIGNMENT);
	_albedoR = new SUITextField("AlbedoR ", SUITextField::INPUT_DOUBLE);
	_albedoR->AddActionListener(this);
	_albedoG = new SUITextField("AlbedoG ", SUITextField::INPUT_DOUBLE);
	_albedoG->AddActionListener(this);
	_albedoB = new SUITextField("AlbedoB ", SUITextField::INPUT_DOUBLE);
	_albedoB->AddActionListener(this);
	albedoValuepBox->AddLabel(new SUILabel("Albedo", SUILabel::LEFT));
	albedoValuepBox->AddTextField(_albedoR);
	albedoValuepBox->AddTextField(_albedoG);
	albedoValuepBox->AddTextField(_albedoB);
	
	_metalic = new SUISlider("Metalic", 0, 1, false, this);
	_roughness = new SUISlider("Roughness", 0, 1, false, this);

	metalPropertypBox->AddChoice(_materialType);	
	metalPropertypBox->AddBox(albedoValuepBox);
	metalPropertypBox->AddSlider(_metalic);
	metalPropertypBox->AddSlider(_roughness);
	metalPropertypBox->SetOnOffEnable(true);
	metalPropertypBox->SetBgVisible(true);

	return metalPropertypBox;
}

void ShaderFrame::actionPerformed(SUIActionEvent e) {
	SUIComponent* com = (SUIComponent*)e.GetComponent();
	if (com == _shaderType) {
		if (_shaderType->GetSelectedIndex() == 0) {
			printf("\n PBR shader selected");
		}
		else {
			printf("\n Phong shader selected");
		}		
	}
	else if (com == _materialType) {	

		printf("\n Selected Material is %s", _materialType->GetItem(_materialType->GetSelectedIndex()).c_str());
	}
	else if (com == _albedoR) {		
		printf(" Albedo R value = %s" , _albedoR->GetText().c_str());
	}
	else if (com == _albedoG) {
		printf("\n PBR shader selected");
	}
	else if (com == _albedoB) {
		printf("\n PBR shader selected");
	}
	else if (com == _metalic) {
		printf("\n PBR shader selected");
	}
	else if (com == _roughness) {
		printf("\n PBR shader selected");
	}
}

ShaderFrame::~ShaderFrame() {
	if (_frame)
	{
		delete _frame;
		_frame = NULL;
	}

	if (lightbox1) {
		delete lightbox1;
	}

	if (lightbox2) {
		delete lightbox2;
	}

	if (lightbox3) {
		delete lightbox3;
	}

	if (lightbox4) {
		delete lightbox4;
	}

	if (lightbox5) {
		delete lightbox5;
	}

	if (lightbox6) {
		delete lightbox6;
	}

	if (lightbox7) {
		delete lightbox7;
	}

}