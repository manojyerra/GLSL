#include "ShaderFrame.h"

ShaderFrame::ShaderFrame(int x, int y, int w, int h, SUIActionListener* renderDemoListener) 
{
	_renderDemoListener = renderDemoListener;

	shaderType = nullptr;
	_materialType = nullptr;
	_albedoR = nullptr;
	_albedoG = nullptr;
	_albedoB = nullptr;
	_metalic = nullptr;
	_roughness = nullptr;
	_albedoSlider = nullptr;
	_albedoSliderPrevValue = 0.5f;

	numLightBoxes = 7;

	_frame = new SUIFrame(static_cast<float>(x), (float)y, (float)w, (float)h, SUIFrame::V_ALIGNMENT);
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
	_mesh = nullptr;
}

void ShaderFrame::SetPos(int x, int y) {
	_frame->SetPos(x, y);
	_frame->ResetBounds();
}

void ShaderFrame::SetMeshRenderer(GLMeshRenderer* mesh)
{
	_mesh = mesh;

	PBRShader* pbrShader = (PBRShader*)_mesh->GetShader(GLMeshRenderer::PBR_SHADER);

	glm::vec3 albedo = pbrShader->GetAlbedo();

	_albedoR->SetDouble(albedo.r, 4);
	_albedoG->SetDouble(albedo.g, 4);
	_albedoB->SetDouble(albedo.b, 4);

	_metalic->SetValue(pbrShader->GetMetallic());
	_roughness->SetValue(pbrShader->GetRoughness());

	//Resetting lightbox ui values

	for (int i = 0; i < lightboxVec.size(); i++)
	{
		LightBox* lightBox = lightboxVec[i];

		lightBox->colorR->SetDouble(1.0, 4);
		lightBox->colorG->SetDouble(1.0, 4);
		lightBox->colorB->SetDouble(1.0, 4);

		lightBox->dirX->SetDouble(1.0, 4);
		lightBox->dirY->SetDouble(1.0, 4);
		lightBox->dirZ->SetDouble(1.0, 4);

		lightBox->lightSelection->SetSelect(false);
		lightBox->box->SetOnOffEnable(true);
		lightBox->box->SetOn(false);
	}

	//fetching data from PBRShader

	std::map<string, LightInfo> lightMap = pbrShader->GetLightMap();
	std::map<string, LightInfo>::iterator iter;
	int count = 0;

	for (iter = lightMap.begin(); iter != lightMap.end(); ++iter)
	{
		string key = iter->first;
		LightInfo lightInfo = iter->second;

		LightBox* lightBox = lightboxVec[count];

		lightBox->colorR->SetDouble(lightInfo.color.r, 4);
		lightBox->colorG->SetDouble(lightInfo.color.g, 4);
		lightBox->colorB->SetDouble(lightInfo.color.b, 4);

		lightBox->dirX->SetDouble(lightInfo.dir.x, 4);
		lightBox->dirY->SetDouble(lightInfo.dir.y, 4);
		lightBox->dirZ->SetDouble(lightInfo.dir.z, 4);

		lightBox->lightSelection->SetSelect(true);
		lightBox->box->SetOn(true);

		count++;
	}
}

SUIBox* ShaderFrame::SetShaderTypeBox() {
	SUIBox* shaderTypeBox = new SUIBox(SUIBox::V_ALIGNMENT);
	shaderTypeBox->SetMargin(3, 3, 5, 0);
	shaderTypeBox->SetName("Shader Type", SUIBox::LEFT);
	shaderType = new SUIRadioButton(SUIRadioButton::V_ALIGNMENT);
	shaderType->AddCheckBox(new SUICheckBox("PBR Shader", SUICheckBox::LEFT));
	shaderType->AddCheckBox(new SUICheckBox("Phong Shader", SUICheckBox::LEFT));
	shaderType->AddActionListener(this);
	shaderType->SetName("Shader", SUIRadioButton::LEFT);
	shaderType->SetSelect(0);
	shaderTypeBox->AddRadioButton(shaderType);
	shaderTypeBox->SetOnOffEnable(true);

	return shaderTypeBox;
}

SUIBox* ShaderFrame::SetPropertyBox() {
	SUIBox* propertyBox = new SUIBox(SUIBox::V_ALIGNMENT);
	propertyBox->SetMargin(3, 3, 5, 0);
	propertyBox->SetName("Properties", SUIBox::LEFT);

	for (int i = 0; i < numLightBoxes; i++)
	{
		string lightName = "Light ";
		lightName = lightName.append(std::to_string(i + 1));
		lightboxVec.push_back(new LightBox(lightName, this, (i < 2)));
	}

	propertyBox->AddBox(SetMetalPropertyBox());

	for (int i = 0; i < numLightBoxes; i++) {
		propertyBox->AddBox(lightboxVec[i]->box);
	}

	return propertyBox;
}

SUIBox* ShaderFrame::SetMetalPropertyBox() {
	SUIBox* metalPropertypBox = new SUIBox(SUIBox::V_ALIGNMENT);
	metalPropertypBox->SetMargin(3, 3, 5, 0);
	metalPropertypBox->SetName("Metal property", SUIBox::LEFT);
	_materialType = new SUIRadioButton(SUIRadioButton::LEFT);
	_materialType->AddCheckBox(new SUICheckBox("Plastic / Glass (Low)", SUICheckBox::LEFT));
	_materialType->AddCheckBox(new SUICheckBox("Plastic High", SUICheckBox::LEFT));
	_materialType->AddCheckBox(new SUICheckBox("Glass (high) / Ruby", SUICheckBox::LEFT));
	_materialType->AddCheckBox(new SUICheckBox("Diamond", SUICheckBox::LEFT));
	_materialType->AddCheckBox(new SUICheckBox("Iron", SUICheckBox::LEFT));
	_materialType->AddCheckBox(new SUICheckBox("Copper", SUICheckBox::LEFT));
	_materialType->AddCheckBox(new SUICheckBox("Gold", SUICheckBox::LEFT));
	_materialType->AddCheckBox(new SUICheckBox("Aluminium", SUICheckBox::LEFT));
	_materialType->AddCheckBox(new SUICheckBox("Silver", SUICheckBox::LEFT));

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
	_albedoSlider = new SUISlider("Tweak", 0, 1, false, this);
	_albedoSlider->SetPointerAt(50);

	metalPropertypBox->AddRadioButton(_materialType);
	metalPropertypBox->AddBox(albedoValuepBox);
	metalPropertypBox->AddSlider(_metalic);
	metalPropertypBox->AddSlider(_roughness);
	metalPropertypBox->AddSlider(_albedoSlider);
	metalPropertypBox->SetOnOffEnable(true);
	metalPropertypBox->SetBgVisible(true);

	return metalPropertypBox;
}

void ShaderFrame::actionPerformed(SUIActionEvent e) {
	if (_mesh == NULL)
		return;

	PBRShader* pbrShader = (PBRShader*)_mesh->GetShader(GLMeshRenderer::PBR_SHADER);
	SUIComponent* com = (SUIComponent*)e.GetComponent();

	if (com == shaderType) {

		_renderDemoListener->actionPerformed(e);

		if (shaderType->GetSelectedIndex() == 0) {
			_mesh->SetShader(GLMeshRenderer::PBR_SHADER);
		}
		else {
			_mesh->SetShader(GLMeshRenderer::PHONG_PER_VERTEX_SHADER);
		}
	}
	else if (com == _materialType) {
		auto  materialType = _materialType->GetSelectedItemName();

		glm::vec3 albedo(0.03);
		float roughness = 0.3;
		float metallic = 0.3;

		if (materialType == "Plastic / Glass (Low)")
		{
			pbrShader->SetMeterialProps(albedo = glm::vec3(0.03), metallic = 1.0, roughness = 0.3);
		}
		else if (materialType == "Plastic High")
		{
			pbrShader->SetMeterialProps(albedo = glm::vec3(0.05), metallic = 1.0, roughness = 0.3);
		}
		else if (materialType == "Glass (high) / Ruby")
		{
			pbrShader->SetMeterialProps(albedo = glm::vec3(0.08), metallic = 1.0, roughness = 0.3);
		}
		else if (materialType == "Diamond")
		{
			pbrShader->SetMeterialProps(albedo = glm::vec3(0.17), metallic = 1.0, roughness = 0.3);
		}
		else if (materialType == "Iron")
		{
			pbrShader->SetMeterialProps(albedo = glm::vec3(0.56, 0.57, 0.58), metallic = 1.0, roughness = 0.3);
		}
		else if (materialType == "Copper")
		{
			pbrShader->SetMeterialProps(albedo = glm::vec3(0.95, 0.64, 0.54), metallic = 1.0, roughness = 0.3);
		}
		else if (materialType == "Gold")
		{
			pbrShader->SetMeterialProps(albedo = glm::vec3(1.00, 0.71, 0.29), metallic = 1.0, roughness = 0.3);
		}
		else if (materialType == "Aluminium")
		{
			pbrShader->SetMeterialProps(albedo = glm::vec3(0.91, 0.92, 0.92), metallic = 1.0, roughness = 0.3);
		}
		else if (materialType == "Silver")
		{
			pbrShader->SetMeterialProps(albedo = glm::vec3(0.95, 0.93, 0.88), metallic = 1.0, roughness = 0.3);
		}

		_albedoR->SetDouble(albedo.r, 4);
		_albedoG->SetDouble(albedo.g, 4);
		_albedoB->SetDouble(albedo.b, 4);

		_metalic->SetValue(metallic);
		_roughness->SetValue(roughness);
		_albedoSlider->SetPointerAt(50);
		_albedoSliderPrevValue = 0.5f;
	}
	else if (com == _albedoR || com == _albedoG || com == _albedoB)
	{
		glm::vec3 values = glm::vec3(_albedoR->GetDouble(), _albedoG->GetDouble(), _albedoB->GetDouble());
		pbrShader->SetAlbedo(values);
	}
	else if (com == _metalic) {
		pbrShader->SetMetallic(_metalic->GetValue());
	}
	else if (com == _roughness) {
		pbrShader->SetRoughness(_roughness->GetValue());
	}
	else if (com == _albedoSlider)
	{
		if (_albedoSlider->GetValue() > 0.001)
		{
			float factor = _albedoSlider->GetValue() / _albedoSliderPrevValue;

			glm::vec3 newAlbedo = glm::vec3(_albedoR->GetDouble()*factor, _albedoG->GetDouble()*factor, _albedoB->GetDouble()*factor);

			pbrShader->SetAlbedo(newAlbedo);
			_albedoR->SetDouble(newAlbedo.r, 4);
			_albedoG->SetDouble(newAlbedo.g, 4);
			_albedoB->SetDouble(newAlbedo.b, 4);

			_albedoSliderPrevValue = _albedoSlider->GetValue();
		}
	}
	else
	{
		for (int i = 0; i < lightboxVec.size(); i++)
		{
			if (com == lightboxVec[i]->lightSelection)
			{
				//printf("\nLightBox1 selected = %d", lightboxVec[i]->lightSelection->IsSelected());

				if (lightboxVec[i]->lightSelection->IsSelected())
				{
					LightBox* lBox = lightboxVec[i];

					glm::vec3 dir = glm::vec3(lBox->dirX->GetDouble(), lBox->dirY->GetDouble(), lBox->dirZ->GetDouble());
					glm::vec3 col = glm::vec3(lBox->colorR->GetDouble(), lBox->colorG->GetDouble(), lBox->colorB->GetDouble());

					LightInfo lightInfo(dir, col);

					pbrShader->AddLight(std::to_string(i), lightInfo);
				}
				else
				{
					pbrShader->RemoveLight(std::to_string(i));
				}
			}

			else if (com == lightboxVec[i]->dirX || com == lightboxVec[i]->dirY || com == lightboxVec[i]->dirZ)
			{
				printf("\n lightboxVec[%d]->lightDirectionX value = %s", i, lightboxVec[i]->dirX->GetText().c_str());
				printf("\n lightboxVec[%d]->lightDirectionY value = %s", i, lightboxVec[i]->dirY->GetText().c_str());
				printf("\n lightboxVec[%d]->lightDirectionY value = %s", i, lightboxVec[i]->dirZ->GetText().c_str());
			}
			else if (com == lightboxVec[i]->colorR || com == lightboxVec[i]->colorG || com == lightboxVec[i]->colorB)
			{
				printf("\n lightboxVec[%d]->lightColorR value = %s", i, lightboxVec[i]->colorR->GetText().c_str());
				printf("\n lightboxVec[%d]->lightColorG value = %s", i, lightboxVec[i]->colorG->GetText().c_str());
				printf("\n lightboxVec[%d]->lightColorB value = %s", i, lightboxVec[i]->colorB->GetText().c_str());
			}
		}
	}
}

SUIFrame* ShaderFrame::GetFrame()
{
	return _frame;
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