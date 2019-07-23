#ifndef Platform_H
#define Platform_H

#include <GLFW/glfw3.h>

class Platform
{
public:
	static void debugPrint(const char * pszFormat, ...);
	
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
