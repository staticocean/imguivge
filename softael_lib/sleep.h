
#ifndef __SLEEP__
#define __SLEEP__

//------------------------------------------------------------------------------

#ifdef defined LINUX || defined TARGET_OS_MAC
#include <unistd.h>
#endif
#ifdef defined _WIN32 || defined _WIN64
#include <windows.h>
#endif

//------------------------------------------------------------------------------

void cpusleep(int milliseconds)
{
    #if defined LINUX || defined TARGET_OS_MAC
	usleep(milliseconds * 1000);   // usleep takes sleep time in us (1 millionth of a second)
	#endif
	
	#if defined _WIN32 || defined _WIN64
	Sleep(milliseconds);
	#endif
	
	return;
}

//------------------------------------------------------------------------------

#endif /* __SLEEP__ */
