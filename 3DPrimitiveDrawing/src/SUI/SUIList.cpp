#ifdef _ENABLE_SUI

#include "SUIList.h"
#include "SUIIncludes.h"
#include "SUIInput.h"
#include "SUIEvents.h"
#include "SUIManager.h"

SUIList::SUIList(int elementsToShow) : SUIComponent(SUI_LIST)
{
	_elementsToShow = elementsToShow;
	_elementsVec.clear();

	_isBorderVisible = true;
	_isBgVisible = true;
	_selectedIndex = -1;

	_selectedColor[0] = 45;
	_selectedColor[1] = 45;
	_selectedColor[2] = 45;
	_selectedColor[3] = 255;
	
	_nonSelectedColor[0] = 64;
	_nonSelectedColor[1] = 64;
	_nonSelectedColor[2] = 64;
	_nonSelectedColor[3] = 255;

	_bgR = 255;
	_bgG = 3;
	_bgB = 98;
	_bgA = 255;

	_actionListener = NULL;
	_mouseListener = NULL;
	_isDisableFrameScissor = false;

	//Scroller related...
	_scrollAmount = 0;
	_totViewHeight = 0;
	_scroller = new SUIScroller(_x+_w-(float)SLIDER_WIDTH, _y, (float)SLIDER_WIDTH, _h-SLIDER_WIDTH);
	_scrollActive = false;
	////////////////////
}

SUIList::~SUIList()
{
	for(int i=0; i<(int)_elementsVec.size(); i++)
	{
		delete _elementsVec[i];
		_elementsVec[i] = NULL;
	}

	if(_scroller)
	{
		delete _scroller;
		_scroller = NULL;
	}
}

void SUIList::SetSelectColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
	_selectedColor[0] = r;
	_selectedColor[1] = g;
	_selectedColor[2] = b;
	_selectedColor[3] = a;

	for(unsigned int i=0;i<_elementsVec.size();i++)
		_elementsVec[i]->SetBgColor(_nonSelectedColor);

	if(_selectedIndex >= 0 && _selectedIndex < (int)_elementsVec.size())
		_elementsVec[_selectedIndex]->SetBgColor(_selectedColor);
}

void SUIList::SetNonSelectColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
	_nonSelectedColor[0] = r;
	_nonSelectedColor[1] = g;
	_nonSelectedColor[2] = b;
	_nonSelectedColor[3] = a;

	for(unsigned int i=0;i<_elementsVec.size();i++)
		_elementsVec[i]->SetBgColor(_nonSelectedColor);

	if(_selectedIndex >= 0 && _selectedIndex < (int)_elementsVec.size())
		_elementsVec[_selectedIndex]->SetBgColor(_selectedColor);
}

void SUIList::Add(string name)
{
	SUILabel* label = new SUILabel(name, SUILabel::LEFT);
	label->SetBgColor(_nonSelectedColor);
	label->SetBgVisible(true);
	label->SetBorderVisible(true);
	label->SetFontColor(0x000000ff);

	_elementsVec.push_back(label);

	if(_selectedIndex >= 0 && _selectedIndex < (int)_elementsVec.size())
		_elementsVec[_selectedIndex]->SetBgColor(_selectedColor);
}

unsigned int SUIList::Size()
{
	return (unsigned int)_elementsVec.size();
}

void SUIList::Move(float dx, float dy)
{
	_x += dx;
	_y += dy;

	_scroller->Move(dx, dy);
	MoveElements(dx, dy);
}

void SUIList::MoveElements(float dx, float dy)
{
	for(int i=0; i<(int)_elementsVec.size(); i++)
		_elementsVec[i]->Move(dx, dy);
}

void SUIList::Scroll(float percent)
{
	float comAreaH = _h;
	float scrollH = (_totViewHeight - comAreaH) * percent / 100.0f;
	
	float moveInY = -(scrollH - _scrollAmount);
	MoveElements(0, moveInY);

	_scrollAmount -= moveInY;
}

void SUIList::ResetBounds()
{
	ResetBounds(_x, _y-_scrollAmount, _w);

	_scroller->SetBounds(_x + _w - (float)SLIDER_WIDTH, _y, (float)SLIDER_WIDTH, _h);
	_scroller->SetTotalViewLength(_totViewHeight);
	_scroller->SetViewLength(_h);
	_scroller->SetScrollPercent(0);
	_scroller->Calc();

	//START : Calculate ScrollPercent based on _scrollAmount and Set it Slider.

	float totMovable = _totViewHeight - _h;

	if(totMovable > 0)
	{
		float scrollPercent = _scrollAmount * 100.0f / totMovable;
		if(totMovable < 0)
		{
			scrollPercent = 0;
			_scrollAmount = 0;
		}
		_scroller->SetScrollPercent(scrollPercent);
		_scroller->Calc();
	}

	//END : //////////////////////////////////////////////////////////////////////////////////////

	if(_scroller->IsScrollEnabled())
		ResetBounds(_x, _y-_scrollAmount, _w- SLIDER_WIDTH);
}

void SUIList::ResetBounds(float x, float y, float w)
{
	_h = 0;
	_totViewHeight = 0;

	for(int i=0; i<(int)_elementsVec.size(); i++)
	{
		_elementsVec[i]->SetPos(x,y+_totViewHeight);
		_elementsVec[i]->SetWidth(w);
		_elementsVec[i]->ResetBounds();

		if(i < _elementsToShow)
			_h += _elementsVec[i]->GetH();

		_totViewHeight += _elementsVec[i]->GetH();
	}
}

bool SUIList::Contains(float x, float y)
{
	if(_scrollActive)
		return true;
	return (x >= _x && x <= _x+_w && y >= _y && y <= _y+_h);
}

SUIEvents SUIList::UpdateByInput()
{
	SUIEvents eventsVec;

	float mx = (float)SUIInput::MX;
	float my = (float)SUIInput::MY;

	if(SUIInput::IsMouseClicked())
	{
		if(_scroller->Contains(mx, my))
		{
			_scrollActive = true;
			return eventsVec;
		}
		else
			_scrollActive = false;
	}
	else if(SUIInput::IsMousePressed())
	{
		if(_scrollActive)
		{
			_scroller->Update();
			if(_scroller->IsScrollEnabled())
				Scroll(_scroller->GetScrollPercent());

			return eventsVec;
		}
	}
	else if(SUIInput::IsMouseReleased())
	{
		_scrollActive = false;
		return eventsVec;
	}

	if(!Contains((float)SUIInput::MX, (float)SUIInput::MY))
		return eventsVec;

	if(SUIInput::IsMouseClicked())
	{
		for(int i=0; i<(int)_elementsVec.size(); i++)
		{
			if(_elementsVec[i]->Contains((float)SUIInput::MX, (float)SUIInput::MY) && _selectedIndex != i)
			{
				if(_selectedIndex >= 0 && _selectedIndex < (int)_elementsVec.size())
					_elementsVec[_selectedIndex]->SetBgColor(_nonSelectedColor);
				
				_elementsVec[i]->SetBgColor(_selectedColor);
				_selectedIndex = i;

				if(_actionListener)
					eventsVec.ACTION_PERFORMED = true;
				break;
			}
		}
	}

	return eventsVec;
}

void SUIList::SetSelect(int index)
{
	if(index >= 0 && index < (int)_elementsVec.size())
	{
		if(_selectedIndex >= 0 && _selectedIndex < (int)_elementsVec.size())
			_elementsVec[_selectedIndex]->SetBgColor(_nonSelectedColor);
		
		_selectedIndex = index;
		_elementsVec[_selectedIndex]->SetBgColor(_selectedColor);
	}
	else
	{
		if(_selectedIndex >= 0 && _selectedIndex < (int)_elementsVec.size())
			_elementsVec[_selectedIndex]->SetBgColor(_nonSelectedColor);

		_selectedIndex = -1;
	}
}

int SUIList::GetSelectedIndex()
{
	return _selectedIndex;
}

string SUIList::GetSelectedStr()
{
	return GetItem(_selectedIndex);
}

string SUIList::GetItem(int index)
{
	if(index >= 0 && index < (int)_elementsVec.size())
		return _elementsVec[index]->GetName();

	return "";
}

void SUIList::DisableScissor()
{
	_isDisableFrameScissor = true;
}

void SUIList::Draw()
{
	if(_isBgVisible)
		DrawBackground();

	if(_elementsVec.size() > 0)
	{
		int i = 0;
		int eleH = _elementsVec[i]->GetH();

		for ( ; i < (int)_elementsVec.size(); i++)
		{
			if (_elementsVec[i]->GetY() > _y-eleH + 1)
			{
				break;
			}
		}

		int eleStartY = _y;
		int eleCount = 0;

		for ( ; i < (int)_elementsVec.size(); i++)
		{
			int eleY = eleStartY + eleCount * eleH;

			if (eleY+eleH > _y+_h+1)
				break;

			int oldY = _elementsVec[i]->GetY();
			_elementsVec[i]->SetY(eleY);
			_elementsVec[i]->Draw();
			//_elementsVec[i]->SetY(oldY);

			eleCount++;
		}
	}

	if(_scroller)
		_scroller->Draw();

	if(_isBorderVisible)
		DrawBorder();
}

#endif
