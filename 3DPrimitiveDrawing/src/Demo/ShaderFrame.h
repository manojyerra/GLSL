#ifndef ShaderFrame_H
#define ShaderFrame_H

#include "SUI/SUIFrame.h"
#include "SUI/SUIActionListener.h"
#include "SUI/SUIRadioButton.h"
#include "SUI/SUIButton.h"
#include "SUI/SUIBox.h"
#include "SUI/SUIChoice.h"
#include "SUI/SUITextField.h"
#include "SUI/SUISlider.h"

#include <vector>
using namespace std;

class LightBox {
	
public:
	SUICheckBox* lightSelection;

	SUITextField* lightDirectionX;
	SUITextField* lightDirectionY;
	SUITextField* lightDirectionZ;
	SUITextField* lightColorR;
	SUITextField* lightColorG;
	SUITextField* lightColorB;
	SUIBox* lightBox;

	LightBox(std::string boxName, SUIActionListener* actionListener, bool isOn) {
		lightBox = new SUIBox(SUIBox::V_ALIGNMENT);
		lightBox->SetMargin(3, 3, 5, 0);
		lightBox->SetName(boxName, SUIBox::LEFT);
		lightBox->SetBgVisible(true);
		lightBox->SetOnOffEnable(true);
		lightBox->SetOn(isOn);

		lightSelection = new SUICheckBox("Enable", SUICheckBox::LEFT);
		lightSelection->AddActionListener(actionListener);

		SUIBox* lightDirectionBox = new SUIBox(SUIBox::H_ALIGNMENT);
		lightDirectionX = new SUITextField("LightDirectionX ", SUITextField::INPUT_DOUBLE);
		lightDirectionX->AddActionListener(actionListener);
		lightDirectionY = new SUITextField("LightDirectionY ", SUITextField::INPUT_DOUBLE);
		lightDirectionY->AddActionListener(actionListener);
		lightDirectionZ = new SUITextField("LightDirectionZ ", SUITextField::INPUT_DOUBLE);
		lightDirectionZ->AddActionListener(actionListener);
		lightDirectionBox->AddLabel(new SUILabel("Direction", SUILabel::LEFT));
		lightDirectionBox->AddTextField(lightDirectionX);
		lightDirectionBox->AddTextField(lightDirectionY);
		lightDirectionBox->AddTextField(lightDirectionZ);

		SUIBox* lightColorBox = new SUIBox(SUIBox::H_ALIGNMENT);
		lightColorR = new SUITextField("LightColorX ", SUITextField::INPUT_DOUBLE);
		lightColorR->AddActionListener(actionListener);
		lightColorG = new SUITextField("LightColorY ", SUITextField::INPUT_DOUBLE);
		lightColorG->AddActionListener(actionListener);
		lightColorB = new SUITextField("LightColorZ ", SUITextField::INPUT_DOUBLE);
		lightColorB->AddActionListener(actionListener);
		lightColorBox->AddLabel(new SUILabel("Color", SUILabel::LEFT));
		lightColorBox->AddTextField(lightColorR);
		lightColorBox->AddTextField(lightColorG);
		lightColorBox->AddTextField(lightColorB);

		lightBox->AddCheckBox(lightSelection);
		lightBox->AddBox(lightDirectionBox);
		lightBox->AddBox(lightColorBox);
	}
};

class ShaderFrame : public SUIActionListener {

private:
	SUIFrame* _frame;

	SUIRadioButton* _shaderType;
	SUIChoice* _materialType;
	SUITextField* _albedoR;
	SUITextField* _albedoG;
	SUITextField* _albedoB;
	SUISlider* _metalic;
	SUISlider* _roughness;	

	SUIBox* SetShaderTypeBox();
	SUIBox* SetPropertyBox();
	SUIBox* SetMetalPropertyBox();

	int numLightBoxes;
	vector<LightBox*> lightboxVec;

public:
	ShaderFrame(int x, int y, int w, int h);
	~ShaderFrame();

	void SetPos(int x, int y);
	void actionPerformed(SUIActionEvent e);

};

#endif