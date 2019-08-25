#ifndef Platform_H
#define Platform_H

#if (defined(_WIN32) || defined(_WIN64) || defined(__WIN32__) || defined(WIN32) || defined(__MINGW32__))
#include "Windows.h"
#endif

#include <GLFW/glfw3.h>

class Platform
{
private:
	static char* _title;

public:
	static void debugPrint(const char * pszFormat, ...);
	static long GetTimeInMillis();
	static void SetWindowTitle(const char* title);
	static char* GetWindowTitle();
};

#endif


//#define MESSAGE_OK						0x00000000L
//#define MESSAGE_OKCANCEL				0x00000001L
//#define MESSAGE_ABORTRETRYIGNORE		0x00000002L
//#define MESSAGE_YESNOCANCEL				0x00000003L
//#define MESSAGE_YESNO					0x00000004L
//#define MESSAGE_RETRYCANCEL				0x00000005L
//
//#define ShowMessageBox					MessageBox
