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
	
	numLightBoxes = 7;

	for (int i = 0; i < numLightBoxes; i++)
	{
		string lightName = "Light ";
		lightName = lightName.append(std::to_string(i+1));
		lightboxVec.push_back(new LightBox(lightName, this, (i < 2)));
	}

	propertyBox->AddBox(SetMetalPropertyBox());

	for (int i = 0; i < numLightBoxes; i++) {
		propertyBox->AddBox(lightboxVec[i]->lightBox);
	}

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
	else if (com == _albedoR || com == _albedoG || com == _albedoB) {
		printf(" Albedo R value = %s" , _albedoR->GetText().c_str());
	}
	else if (com == _metalic) {		
		printf("Metalic value = %lf", _metalic->GetValue());
	}
	else if (com == _roughness) {
		printf("Metalic value = %lf", _roughness->GetValue());
	}
	else
	{
		for(int i=0; i<lightboxVec.size(); i++)
		{
			if (com == lightboxVec[i]->lightSelection) 
			{
				printf("\nLightBox1 selected = %d", lightboxVec[i]->lightSelection->IsSelected());
			}
			else if (com == lightboxVec[i]->lightDirectionX || com == lightboxVec[i]->lightDirectionY || com == lightboxVec[i]->lightDirectionZ)
			{
				printf("\n lightboxVec[%d]->lightDirectionX value = %s", i, lightboxVec[i]->lightDirectionX->GetText().c_str());
				printf("\n lightboxVec[%d]->lightDirectionY value = %s", i, lightboxVec[i]->lightDirectionY->GetText().c_str());
				printf("\n lightboxVec[%d]->lightDirectionY value = %s", i, lightboxVec[i]->lightDirectionZ->GetText().c_str());
			}
			else if (com == lightboxVec[i]->lightColorR || com == lightboxVec[i]->lightColorG || com == lightboxVec[i]->lightColorB)
			{
				printf("\n lightboxVec[%d]->lightColorR value = %s", i, lightboxVec[i]->lightColorR->GetText().c_str());
				printf("\n lightboxVec[%d]->lightColorG value = %s", i, lightboxVec[i]->lightColorG->GetText().c_str());
				printf("\n lightboxVec[%d]->lightColorB value = %s", i, lightboxVec[i]->lightColorB->GetText().c_str());
			}
		}
	}
}

ShaderFrame::~ShaderFrame() {
	if (_frame)
	{
		delete _frame;
		_frame = NULL;
	}

	for (int i = 0; i < lightboxVec.size(); i++)
	{
		if (lightboxVec[i]) {
			delete lightboxVec[i];
		}
	}
}