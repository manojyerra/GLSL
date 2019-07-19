#ifndef ShaderFrame_H
#define ShaderFrame_H

#include "SUI/SUIFrame.h"
#include "SUI/SUIActionListener.h"
#include "SUI/SUIRadioButton.h"
#include "SUI/SUIBox.h"
#include "SUI/SUIChoice.h"
#include "SUI/SUITextField.h"
#include "SUI/SUISlider.h"
#include "GLMeshRenderer.h"

#include <vector>
using namespace std;

class LightBox {
public:
	SUICheckBox* lightSelection;

	SUITextField* dirX;
	SUITextField* dirY;
	SUITextField* dirZ;
	SUITextField* colorR;
	SUITextField* colorG;
	SUITextField* colorB;
	SUIBox* box;

	LightBox(std::string boxName, SUIActionListener* actionListener, bool isOn) {
		box = new SUIBox(SUIBox::V_ALIGNMENT);
		box->SetMargin(3, 3, 5, 0);
		box->SetName(boxName, SUIBox::LEFT);
		box->SetBgVisible(true);
		box->SetOnOffEnable(true);
		box->SetOn(isOn);

		lightSelection = new SUICheckBox("Enable", SUICheckBox::LEFT);
		lightSelection->AddActionListener(actionListener);

		SUIBox* lightDirectionBox = new SUIBox(SUIBox::H_ALIGNMENT);
		dirX = new SUITextField("LightDirectionX ", SUITextField::INPUT_DOUBLE);
		dirX->AddActionListener(actionListener);
		dirY = new SUITextField("LightDirectionY ", SUITextField::INPUT_DOUBLE);
		dirY->AddActionListener(actionListener);
		dirZ = new SUITextField("LightDirectionZ ", SUITextField::INPUT_DOUBLE);
		dirZ->AddActionListener(actionListener);
		lightDirectionBox->AddLabel(new SUILabel("Direction", SUILabel::LEFT));
		lightDirectionBox->AddTextField(dirX);
		lightDirectionBox->AddTextField(dirY);
		lightDirectionBox->AddTextField(dirZ);

		SUIBox* lightColorBox = new SUIBox(SUIBox::H_ALIGNMENT);
		colorR = new SUITextField("LightColorX ", SUITextField::INPUT_DOUBLE);
		colorR->AddActionListener(actionListener);
		colorG = new SUITextField("LightColorY ", SUITextField::INPUT_DOUBLE);
		colorG->AddActionListener(actionListener);
		colorB = new SUITextField("LightColorZ ", SUITextField::INPUT_DOUBLE);
		colorB->AddActionListener(actionListener);
		lightColorBox->AddLabel(new SUILabel("Color", SUILabel::LEFT));
		lightColorBox->AddTextField(colorR);
		lightColorBox->AddTextField(colorG);
		lightColorBox->AddTextField(colorB);

		box->AddCheckBox(lightSelection);
		box->AddBox(lightDirectionBox);
		box->AddBox(lightColorBox);
	}
};

class ShaderFrame : public SUIActionListener {
private:
	SUIFrame* _frame;

	
	//SUIChoice* _materialType;
	SUIRadioButton* _materialType;
	SUITextField* _albedoR;
	SUITextField* _albedoG;
	SUITextField* _albedoB;
	SUISlider* _metalic;
	SUISlider* _roughness;
	SUISlider* _albedoSlider;
	float _albedoSliderPrevValue;

	SUIBox* SetShaderTypeBox();
	SUIBox* SetPropertyBox();
	SUIBox* SetMetalPropertyBox();

	int numLightBoxes;
	vector<LightBox*> lightboxVec;

	GLMeshRenderer* _mesh;
	SUIActionListener* _renderDemoListener;

public:
	SUIRadioButton* shaderType;

	ShaderFrame(int x, int y, int w, int h, SUIActionListener* renderDemoListener);
	~ShaderFrame();

	void SetPos(int x, int y);
	void SetMeshRenderer(GLMeshRenderer* mesh);
	void actionPerformed(SUIActionEvent e);
	SUIFrame* GetFrame();
};

#endif