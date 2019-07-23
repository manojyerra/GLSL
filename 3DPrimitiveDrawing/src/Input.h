#ifndef Input_H
#define Input_H

#include "Platform.h"

class Input
{
public:
	static const int NUM_KEYS = 512;

private:
	static int currKeyStates[NUM_KEYS];
	static int prevKeyStates[NUM_KEYS];
	static float timeCountForKeyPress[NUM_KEYS];

	static bool isMouseClicked;
	static bool isMousePressed;
	static bool isMouseReleased;
	static bool isMouseDoubleClicked;

	static bool isRightMouseClicked;
	static bool isRightMousePressed;
	static bool isRightMouseReleased;
	static bool isRightMouseDoubleClicked;

	static bool isMiddleMouseClicked;
	static bool isMiddleMousePressed;
	static bool isMiddleMouseReleased;
	static bool isMiddleMouseDoubleClicked;

	static bool PREV_LEFT_BUTTON_DOWN;
	static bool PREV_RIGHT_BUTTON_DOWN;
	static bool PREV_MIDDLE_BUTTON_DOWN;

	static float clickTimeCount;
	static float rightClickTimeCount;
	static float middleClickTimeCount;

	static float DCTime;
	static bool enable;

public:
	enum
	{
		KEY_LEFT_CONTROL = GLFW_KEY_LEFT_CONTROL,
		KEY_RIGHT_CONTROL = GLFW_KEY_RIGHT_CONTROL,
		KEY_LEFT_SHIFT = GLFW_KEY_LEFT_SHIFT,
		KEY_RIGHT_SHIFT = GLFW_KEY_RIGHT_SHIFT,
		KEY_LEFT_ALT = GLFW_KEY_LEFT_ALT,
		KEY_RIGHT_ALT = GLFW_KEY_RIGHT_ALT,
		KEY_ENTER = GLFW_KEY_ENTER,
		KEY_SPACE = GLFW_KEY_SPACE,
		KEY_BACKSPACE = GLFW_KEY_BACKSPACE,
		KEY_ESCAPE = GLFW_KEY_ESCAPE,
		KEY_DELETE = GLFW_KEY_DELETE,
		KEY_END = GLFW_KEY_END,
		KEY_HOME = GLFW_KEY_HOME
	};

	static int storeKeyStates[NUM_KEYS];

	static const int MOUSE_CLICK = 4;
	static const int MOUSE_PRESS = 5;
	static const int MOUSE_RELEASE = 6;
	static const int MOUSE_DRAG = 7;
	static const int MOUSE_DOUBLE_CLICK = 8;
	static const int MOUSE_WHEEL_UP = 9;
	static const int MOUSE_WHEEL_DOWN = 10;

	//Mouse ID...
	static const int MOUSE_NONE = 0;
	static const int MOUSE_LEFT = 1;
	static const int MOUSE_MIDDLE = 2;
	static const int MOUSE_RIGHT = 3;
	static const int MOUSE_WHEEL = 4;

	static bool LEFT_BUTTON_DOWN;
	static bool RIGHT_BUTTON_DOWN;
	static bool MIDDLE_BUTTON_DOWN;

	static int SCROLL_STATE; 
	static int SCROLL_STATE_STORE;
	static const int SCROLL_NONE = 0;
	static const int SCROLL_UP = 1;
	static const int SCROLL_DOWN = 2;

	static int MX;
	static int MY;

	static int PrevMX;
	static int PrevMY;

	static int MouseClickX;
	static int MouseClickY;

	static int MouseReleaseX;
	static int MouseReleaseY;

	static bool MOUSE_MOVE;


	static void Init();
	static void Update(int mouseX, int mouseY, float deltaTime);
	static void SetEnable(bool enableInput);

	static bool IsKeyTyped(int key);
	static bool IsKeyPressed(int key);
	static bool IsKeyReleased(int key);
	static bool IsAnyKeyReleased(int* keys, int size);

	static bool IsKeyPressedStill(int key, float time=0.15f);
	
	static bool IsMousePressed();
	static bool IsMouseReleased();
	static bool IsMouseClicked();
	static bool IsMouseDragged();
	static bool IsMouseDoubleClicked();

	static bool IsRightMousePressed();
	static bool IsRightMouseReleased();
	static bool IsRightMouseClicked();
	static bool IsRightMouseDragged();
	static bool IsRightMouseDoubleClicked();

	static bool IsMiddleMousePressed();
	static bool IsMiddleMouseReleased();
	static bool IsMiddleMouseClicked();
	static bool IsMiddleMouseDragged();
	static bool IsMiddleMouseDoubleClicked();

	static bool IsMouseMoved();

	static bool IsScrollUp();
	static bool IsScrollDown();
	static void SetScrollState(int scrollState);

	static float GetDragDist();
	static float GetAngle();
};

#endif