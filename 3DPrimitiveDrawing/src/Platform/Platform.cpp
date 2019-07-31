#include "Platform.h"
#include <stdio.h>

void Platform::debugPrint(const char * pszFormat, ...)
{

#if (defined(_WIN32) || defined(_WIN64) || defined(__WIN32__) || defined(WIN32) || defined(__MINGW32__))
	const int MAX_LEN = 10000;
	char szBuf[MAX_LEN];

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
}

long Platform::GetTimeInMillis()
{
#if (defined(_WIN32) || defined(_WIN64) || defined(__WIN32__) || defined(WIN32) || defined(__MINGW32__))
	return GetTickCount();
	//return 0;
#else
	return 0;
#endif
}
