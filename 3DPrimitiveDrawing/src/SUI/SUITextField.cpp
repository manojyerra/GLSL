#ifdef _ENABLE_SUI

#include "SUITextField.h"
#include "SUIFont.h"
#include "SUIInput.h"
#include "SUIManager.h"

SUITextField::SUITextField(string name, int inputType) : SUIComponent(SUIComponent::TEXTFIELD)
{
	Init(name, SUIComponent::CENTER, NULL, inputType);
}

SUITextField::SUITextField(string name, int nameAlignment, int inputType) : SUIComponent(SUIComponent::TEXTFIELD)
{
	Init(name, nameAlignment, NULL, inputType);
}

SUITextField::SUITextField(string name, SUIActionListener* actionListener, int inputType) : SUIComponent(SUIComponent::TEXTFIELD)
{
	Init(name, SUIComponent::CENTER, actionListener, inputType);
}

SUITextField::SUITextField(string name, int nameAlignment, SUIActionListener* actionListener, int inputType) : SUIComponent(SUIComponent::TEXTFIELD)
{
	Init(name, nameAlignment, actionListener, inputType);
}

void SUITextField::Init(string name, int nameAlignment, SUIActionListener* actionListener, int inputType)
{
	_name = name;
	_nameAlignment = nameAlignment;
	_actionListener = actionListener;
	_inputType = inputType;
	_isActivated = false;

	if(_inputType == INPUT_UNSIGNED_INT)
	{
		if(!IsValidUnsignedInt(_name))
			_name = "0";
	}
	else if(_inputType == INPUT_INT)
	{
		if(!IsValidInt(_name))
			_name = "0";
	}
	else if(_inputType == INPUT_DOUBLE)
	{
		if(!IsValidDouble(_name))
			_name = "0";
	}


	_h = 22;

	_isBgVisible = true;
	_isBorderVisible = true;
	_isBgGradient = false;

	_bgR = 171;
	_bgG = 171;
	_bgB = 171;
	_bgA = 255;

	_borderR = 50;
	_borderG = 50;
	_borderB = 50;
	_borderA = 255;

	_mouseListener = NULL;
}

SUITextField::~SUITextField()
{
}

void SUITextField::Move(float dx, float dy)
{
	_x += dx;
	_y += dy;
}

void SUITextField::ResetBounds()
{
}

void SUITextField::Exception(string info)
{
	int a = 1;
	int b = 0;
	int c = a/b;
	c++;
}

void SUITextField::SetText(string text)
{
	if(_inputType == INPUT_UNSIGNED_INT)
	{
		if(!IsValidUnsignedInt(_name))
			_name = "0";
	}
	else if(_inputType == INPUT_INT)
	{
		if(!IsValidInt(_name))
			_name = "0";
	}
	else if(_inputType == INPUT_DOUBLE)
	{
		if(!IsValidDouble(_name))
			_name = "0";
	}
	else if(_inputType == INPUT_STRING)
	{
		_name = text;
	}
}

string SUITextField::GetText()
{
	return _name;
}

void SUITextField::SetInt(int val)
{
	if (_inputType != INPUT_INT)
		throw new exception("Input type mismatch");
	
	char arr[64];
	sprintf(arr, "%d",val);
	_name = string(arr);
}

int SUITextField::GetInt()
{
	if (_inputType != INPUT_INT)
		throw new exception("Input type mismatch");

	return atoi(_name.c_str());
}

void SUITextField::SetUInt(unsigned int val)
{
	if(_inputType != INPUT_UNSIGNED_INT)
		throw new exception("Input type mismatch");

	char arr[64];
	sprintf(arr, "%u",val);
	_name = string(arr);
}

unsigned int SUITextField::GetUInt()
{
	if(_inputType != INPUT_UNSIGNED_INT)
		throw new exception("Input type mismatch");

	return (unsigned int)atol(_name.c_str());
}

void SUITextField::SetDouble(double val, int numDecimalsToShow)
{
	if (_inputType != INPUT_DOUBLE)
		throw new exception("Input type mismatch");

	if(numDecimalsToShow < 0)		numDecimalsToShow = 0;
	else if(numDecimalsToShow > 6)	numDecimalsToShow = 6;
	
	char arr[64];

	if(numDecimalsToShow == 0)			sprintf(arr, "%lf",val);
	else if(numDecimalsToShow == 1)		sprintf(arr, "%.1lf",val);
	else if(numDecimalsToShow == 2)		sprintf(arr, "%.2lf",val);
	else if(numDecimalsToShow == 3)		sprintf(arr, "%.3lf",val);
	else if(numDecimalsToShow == 4)		sprintf(arr, "%.4lf",val);
	else if(numDecimalsToShow == 5)		sprintf(arr, "%.5lf",val);
	else if(numDecimalsToShow == 6)		sprintf(arr, "%.6lf",val);

	_name = string(arr);
}

double SUITextField::GetDouble()
{
	if(_inputType != INPUT_DOUBLE)
		throw new exception("Input type mismatch");

	return atof(_name.c_str());
}

void SUITextField::Draw()
{
	//if(_isBgVisible)
	//{
		//if(Contains((float)SUIInput::MX, (float)SUIInput::MY))
		//	DrawBackground(true);
		//else
		//	DrawBackground(false);
	//}

	SUIFont::GetInstance()->SetColor(0xffffffff);
	SUIFont::GetInstance()->Begin();

	SUIFont::GetInstance()->HorBorder(_x, _x+_w);

	float horGap = -0.5;
	float fontSize = SUIFont::GetInstance()->GetFontSize();

	if (_isActivated)
	{
		DrawBackground(true);
		_nameAlignment = LEFT;
		SUIFont::GetInstance()->DrawFromLeft(_name, _x + 1, _y + _h / 2, fontSize);
		float len = SUIFont::GetInstance()->GetLength(_name, fontSize);
		SUIRect::Draw(_x+1+len+1, _y+2, 2, _h-4, 32,32,32,255,false);
	}
	else
	{
		_nameAlignment = CENTER;
		SUIFont::GetInstance()->DrawFromCenter(_name, _x + 1 + _w / 2, _y + _h / 2, fontSize);
	}

	//if(_nameAlignment == LEFT)			SUIFont::GetInstance()->DrawFromLeft(_name, _x+1, _y+_h/2, fontSize);
	//else if(_nameAlignment == RIGHT)	SUIFont::GetInstance()->DrawFromRight(_name, _x+_w+1, _y+_h/2, fontSize);
	//else								SUIFont::GetInstance()->DrawFromCenter(_name, _x+1+_w/2, _y+_h/2, fontSize);

	SUIFont::GetInstance()->End();

	if(_isBorderVisible)
		DrawBorder();
}

SUIEvents SUITextField::UpdateByInput()
{
	SUIEvents eventsVec;

	//if(!Contains((float)SUIInput::MX, (float)SUIInput::MY))
	//	return eventsVec;

	if (SUIInput::IsMouseClicked() && Contains((float)SUIInput::MX, (float)SUIInput::MY))
	{
		_isActivated = true;
		SUIManager::GetInstance()->SetDialogCom(this);
	}
	else if(SUIInput::IsMouseClicked() && !Contains((float)SUIInput::MX, (float)SUIInput::MY))
	{
		_isActivated = false;
		SUIManager::GetInstance()->SetDialogCom(NULL);
	}

	if(_isActivated)
	{
		int key = SUIInput::GetReleasedKey();

		if (key != 0)
		{
			if (key == VK_RETURN)
			{
				if (_actionListener)
				{
					eventsVec.ACTION_PERFORMED = true;
				}

				_isActivated = false;
				SUIManager::GetInstance()->SetDialogCom(NULL);
			}
			else
				AppendChar(key);
		}
	}

	return eventsVec;
}


void SUITextField::AppendChar(int key)
{
	if(key == VK_BACK)
	{
		if(_name.length() > 0)
			_name = _name.substr(0, _name.length()-1);
	}
	else if(key == VK_ESCAPE)
	{
		_name = "";
	}
	else
	{
		if(_inputType == INPUT_UNSIGNED_INT)
		{
			if(key >= '0' && key <= '9')
				_name += key;
		}
		else if(_inputType == INPUT_INT)
		{
			string tempStr = _name;
			tempStr += key;

			if(IsValidInt(tempStr))
				_name += key;
		}
		else if(_inputType == INPUT_DOUBLE)
		{
			string tempStr = _name;
			tempStr += key;

			if(IsValidDouble(tempStr))
				_name += key;
		}
		else
		{
			_name += key;
		}
	}
}

bool SUITextField::IsValidUnsignedInt(string str)
{
	for(int i=0; i<str.length(); i++)
	{
		if(!(str[i] >= '0' && str[i] <= '9'))
			return false;
	}

	return true;
}

bool SUITextField::IsValidInt(string str)
{
	for(int i=0; i<str.length(); i++)
	{
		if(i==0 && (str[i] == '-' || str[i] == '+'))
			continue;

		if(!(str[i] >= '0' && str[i] <= '9'))
			return false;
	}

	return true;
}

bool SUITextField::IsValidDouble(string str)
{
	int dotCount = 0;

	for(int i=0; i<str.length(); i++)
	{
		if(i==0 && (str[i] == '-' || str[i] == '+'))
			continue;

		if(str[i] == '.')
		{
			dotCount++;

			if(dotCount == 1)
				continue;
		}

		if(!(str[i] >= '0' && str[i] <= '9'))
			return false;
	}

	return true;
}

#endif
