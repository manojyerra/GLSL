#include "Platform.h"
#include <stdio.h>

char* Platform::_title = nullptr;

void Platform::debugPrint(const char * pszFormat, ...)
{
	const int MAX_LEN = 10000;
	char szBuf[MAX_LEN];

#if (defined(_WIN32) || defined(_WIN64) || defined(__WIN32__) || defined(WIN32) || defined(__MINGW32__))
	
	va_list ap;
	va_start(ap, pszFormat);
	vsnprintf_s(szBuf, MAX_LEN, MAX_LEN, pszFormat, ap);
	va_end(ap);

	WCHAR wszBuf[MAX_LEN] = { 0 };
	MultiByteToWideChar(CP_UTF8, 0, szBuf, -1, wszBuf, sizeof(wszBuf));
	OutputDebugStringW(wszBuf);
	//OutputDebugStringA("");

	//WideCharToMultiByte(CP_ACP, 0, wszBuf, sizeof(wszBuf), szBuf, sizeof(szBuf), NULL, FALSE);
	//printf("%s\n", szBuf);
#endif

	printf("%s",szBuf);
}

long Platform::GetTimeInMillis()
{
//#if (defined(_WIN32) || defined(_WIN64) || defined(__WIN32__) || defined(WIN32) || defined(__MINGW32__))
//	return GetTickCount();
//#else
//	return 0;
//#endif

	return (long)(glfwGetTime() * 1000.0f);
}

void Platform::SetWindowTitle(const char* title)
{
	_title = (char*)title;
}

char* Platform::GetWindowTitle()
{
	return _title;
}