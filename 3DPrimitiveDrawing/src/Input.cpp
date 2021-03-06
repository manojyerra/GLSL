#include "Input.h"
#include <cmath>

int Input::MX = 0;
int Input::MY = 0;

int Input::PrevMX = 0;
int Input::PrevMY = 0;

int Input::MouseClickX = 0;
int Input::MouseClickY = 0;

int Input::MouseReleaseX = 0;
int Input::MouseReleaseY = 0;

//bool Input::MOUSE_MOVE = false;

bool Input::PREV_LEFT_BUTTON_DOWN = false;
bool Input::LEFT_BUTTON_DOWN = false;

bool Input::PREV_RIGHT_BUTTON_DOWN = false;
bool Input::RIGHT_BUTTON_DOWN = false;

bool Input::PREV_MIDDLE_BUTTON_DOWN = false;
bool Input::MIDDLE_BUTTON_DOWN = false;

int Input::SCROLL_STATE = Input::SCROLL_NONE; 
int Input::SCROLL_STATE_STORE = Input::SCROLL_NONE;

bool Input::isMouseClicked = false;
bool Input::isMousePressed = false;
bool Input::isMouseReleased = false;
bool Input::isMouseDoubleClicked = false;

bool Input::isRightMouseClicked = false;
bool Input::isRightMousePressed = false;
bool Input::isRightMouseReleased = false;
bool Input::isRightMouseDoubleClicked = false;

bool Input::isMiddleMouseClicked = false;
bool Input::isMiddleMousePressed = false;
bool Input::isMiddleMouseReleased = false;
bool Input::isMiddleMouseDoubleClicked = false;

float Input::clickTimeCount = 0;
float Input::rightClickTimeCount = 0;
float Input::middleClickTimeCount = 0;
float Input::DCTime = 0.2f;

int Input::currKeyStates[] = { 0 };
int Input::prevKeyStates[] = { 0 };
int Input::storeKeyStates[] = {0};
float Input::timeCountForKeyPress[] = {0};

bool Input::enable = true;

void Input::Init()
{
	for (int i = 0; i < NUM_KEYS; i++)
	{
		currKeyStates[i] = 0;
		prevKeyStates[i] = 0;
		storeKeyStates[i] = 0;
		timeCountForKeyPress[i] = 0.0f;
	}
}

void Input::Update(int mouseX, int mouseY, float deltaTime)
{
	for (int i = 0; i < NUM_KEYS; i++)
	{
		prevKeyStates[i] = currKeyStates[i];
		currKeyStates[i] = storeKeyStates[i];

		timeCountForKeyPress[i] = currKeyStates[i] ? (timeCountForKeyPress[i] + deltaTime) : 0.0f;
	}

	SCROLL_STATE = SCROLL_STATE_STORE;
	SCROLL_STATE_STORE = SCROLL_NONE;

	isMouseDoubleClicked = false;
	isRightMouseDoubleClicked = false;
	isMiddleMouseDoubleClicked = false;
	
	isMouseClicked = (!PREV_LEFT_BUTTON_DOWN && LEFT_BUTTON_DOWN);
	isMousePressed = (PREV_LEFT_BUTTON_DOWN && LEFT_BUTTON_DOWN);
	isMouseReleased = (PREV_LEFT_BUTTON_DOWN && !LEFT_BUTTON_DOWN);

	PREV_LEFT_BUTTON_DOWN = LEFT_BUTTON_DOWN;

	isRightMouseClicked = (!PREV_RIGHT_BUTTON_DOWN && RIGHT_BUTTON_DOWN);
	isRightMousePressed = (PREV_RIGHT_BUTTON_DOWN && RIGHT_BUTTON_DOWN);
	isRightMouseReleased = (PREV_RIGHT_BUTTON_DOWN && !RIGHT_BUTTON_DOWN);

	PREV_RIGHT_BUTTON_DOWN = RIGHT_BUTTON_DOWN;

	isMiddleMouseClicked = (!PREV_MIDDLE_BUTTON_DOWN && MIDDLE_BUTTON_DOWN);
	isMiddleMousePressed = (PREV_MIDDLE_BUTTON_DOWN && MIDDLE_BUTTON_DOWN);
	isMiddleMouseReleased = (PREV_MIDDLE_BUTTON_DOWN && !MIDDLE_BUTTON_DOWN);

	PREV_MIDDLE_BUTTON_DOWN = MIDDLE_BUTTON_DOWN;

	PrevMX = MX;
	PrevMY = MY;

	MX = mouseX;
	MY = mouseY;

	if(IsMouseClicked() || IsRightMouseClicked() || IsMiddleMouseClicked())
	{
		if(clickTimeCount < DCTime && isMouseClicked)				{ isMouseDoubleClicked = true;		}
		if(rightClickTimeCount < DCTime && isRightMouseClicked)		{ isRightMouseDoubleClicked = true; }
		if(middleClickTimeCount < DCTime && isMiddleMouseClicked)	{ isMiddleMouseDoubleClicked = true;}

		clickTimeCount = 0;
		rightClickTimeCount = 0;
		middleClickTimeCount = 0;

		MouseClickX = MX;
	    MouseClickY = MY;

		PrevMX = MX;
		PrevMY = MY;
	}
	else if(IsMouseReleased() || IsRightMouseReleased() || IsMiddleMouseReleased())
	{
		MouseReleaseX = MX;
	    MouseReleaseY = MY;

		PrevMX = MX;
		PrevMY = MY;
	}

	clickTimeCount += deltaTime;
	rightClickTimeCount += deltaTime;
	middleClickTimeCount += deltaTime;
}

void Input::SetEnable(bool enableInput)
{
	enable = enableInput;
}

bool Input::IsKeyTyped(int key)		{ return enable && (bool)(!(prevKeyStates[key])  && (currKeyStates[key]) );		}
bool Input::IsKeyReleased(int key)	{ return enable && (bool)((prevKeyStates[key])  && !(currKeyStates[key]) );		}
bool Input::IsKeyPressed(int key)	{ return enable && currKeyStates[key]; }
bool Input::IsKeyPressedStill(int key, float time) { return enable && (IsKeyPressed(key) && timeCountForKeyPress[key] > time);	}

bool Input::IsAnyKeyReleased(int* keys, int size)
{
	for(int i=0; i<size; i++)
	{
		if(IsKeyReleased(keys[i]))
			return true;
	}

	return false;
}

bool Input::IsMousePressed()		{ return enable && isMousePressed;								}
bool Input::IsMouseReleased()		{ return enable && isMouseReleased;								}
bool Input::IsMouseClicked()		{ return enable && (isMouseClicked && !isMouseDoubleClicked);	}
bool Input::IsMouseDragged()		{ return enable && (IsMouseMoved() && IsMousePressed());		}
bool Input::IsMouseDoubleClicked()	{ return enable && isMouseDoubleClicked;						}

bool Input::IsRightMousePressed()		{ return enable && isRightMousePressed;									}
bool Input::IsRightMouseReleased()		{ return enable && isRightMouseReleased;								}
bool Input::IsRightMouseClicked()		{ return enable && (isRightMouseClicked && !isRightMouseDoubleClicked); }
bool Input::IsRightMouseDragged()		{ return enable && (IsMouseMoved() && IsRightMousePressed());			}
bool Input::IsRightMouseDoubleClicked()	{ return enable && isRightMouseDoubleClicked;							}

bool Input::IsMiddleMousePressed()			{ return enable && isMiddleMousePressed;									}
bool Input::IsMiddleMouseReleased()			{ return enable && isMiddleMouseReleased;									}
bool Input::IsMiddleMouseClicked()			{ return enable && (isMiddleMouseClicked && !isMiddleMouseDoubleClicked);	}
bool Input::IsMiddleMouseDragged()			{ return enable && (IsMouseMoved() && IsMiddleMousePressed());				}
bool Input::IsMiddleMouseDoubleClicked()	{ return enable && isMiddleMouseDoubleClicked;								}

bool Input::IsScrollUp()					{ return enable && (SCROLL_STATE == SCROLL_UP);		}
bool Input::IsScrollDown()					{ return enable && (SCROLL_STATE == SCROLL_DOWN);	}
void Input::SetScrollState(int scrollState)	{ SCROLL_STATE_STORE = scrollState;					}

bool Input::IsMouseMoved()
{
	return enable && (PrevMX != MX || PrevMY != MY);
}

float Input::GetDragDist()
{
	if(IsMouseDragged() || IsRightMouseDragged() || IsMiddleMouseDragged())
	{
		float dx = (float)(MX - MouseClickX);
		float dy = (float)(MY - MouseClickY);

		return sqrt( dx*dx + dy*dy );
	}

	return 0;
}

float Input::GetAngle()
{
	float dx = (float)(Input::MX - Input::PrevMX);
	float dy = (float)(Input::MY - Input::PrevMY);

	float piVal = 3.14159265f;
	float moveAngle = atan2( dy, dx ) * 180.0f / piVal;
	moveAngle = (int)moveAngle % 360 + (moveAngle-(int)moveAngle);
	if(moveAngle < 0)
		moveAngle = 360 + moveAngle;

	return moveAngle;
}
