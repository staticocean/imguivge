
#ifndef __PTHREAD__
#define __PTHREAD__

//------------------------------------------------------------------------------

#ifdef _WIN32
#include <windows.h>
#else
#include <pthread.h>
#endif

//------------------------------------------------------------------------------

#ifdef _WIN32

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

typedef void *(*thd_thread_method)(void *);
typedef HANDLE                  thd_thread;
typedef CRITICAL_SECTION        thd_mutex;
typedef CONDITION_VARIABLE      thd_condition;

typedef struct _internal_parameters
{
	thd_thread_method   i_method;
	void*               i_data;
    
}   t_internal_parameters;

static DWORD WINAPI internal_method_ptr(LPVOID arg)
{
	t_internal_parameters *params = (t_internal_parameters *)arg;
	params->i_method(params->i_data);
	
    free(params);
    
	return 0;
}

inline int thd_thread_detach(thd_thread* thread, thd_thread_method method, void* data)
{
	t_internal_parameters* params = (t_internal_parameters *)malloc(sizeof(t_internal_parameters));
	if(params)
	{
		params->i_method = method;
		params->i_data   = data;
		*thread = CreateThread(NULL, 0, internal_method_ptr, params, 0, NULL);
		if(*thread == NULL)
		{
			free(params);
			return 1;
		}
		return 0;
	}
	return 1;
}

inline int thd_thread_join(thd_thread* thread)
{
	if(WaitForSingleObject(*thread, INFINITE) != WAIT_FAILED)
	{
		if(CloseHandle(*thread))
		{
			return 0;
		}
	}
	return 1;
}

inline int thd_mutex_init(thd_mutex* mutex)
{
	InitializeCriticalSection(mutex);
	return 0;
}

inline int thd_mutex_lock(thd_mutex* mutex)
{
	EnterCriticalSection(mutex);
	return 0;
}

inline int thd_mutex_trylock(thd_mutex* mutex)
{
	return !TryEnterCriticalSection(mutex);
}

inline int thd_mutex_unlock(thd_mutex* mutex)
{
	LeaveCriticalSection(mutex);
	return 0;
}

inline int thd_mutex_destroy(thd_mutex* mutex)
{
	DeleteCriticalSection(mutex);
	return 0;
}

inline int thd_condition_init(thd_condition* cond)
{
	InitializeConditionVariable(cond);
	return 0;
}

inline int thd_condition_signal(thd_condition* cond)
{
	WakeConditionVariable(cond);
	return 0;
}

inline int thd_condition_wait(thd_condition* cond, thd_mutex* mutex)
{
	return !SleepConditionVariableCS(cond, mutex, INFINITE);
}

inline int thd_condition_destroy(thd_condition* cond)
{
	return 0;
}

#define pthread_mutex_t                     thd_mutex
#define pthread_cond_t                      thd_condition
#define pthread_t                           thd_thread

#define pthread_create(t, attr, m, data)    thd_thread_detach(t, m, data)
#define pthread_join(t, attr)               thd_thread_join(&t)

#define pthread_mutex_init(mutex, attr)     thd_mutex_init(mutex)
#define pthread_mutex_lock(mutex)           thd_mutex_lock(mutex)
#define pthread_mutex_trylock(mutex)        thd_mutex_trylock(mutex)
#define pthread_mutex_unlock(mutex)         thd_mutex_unlock(mutex)
#define pthread_mutex_destroy(mutex)        thd_mutex_destroy(mutex)

#define pthread_cond_init(cond, attr)       thd_condition_init(cond)
#define pthread_cond_signal(cond)           thd_condition_signal(cond)
#define pthread_cond_wait(cond, mutex)      thd_condition_wait(cond, mutex)
#define pthread_cond_destroy(cond)          thd_condition_destroy(cond)

#endif

//------------------------------------------------------------------------------

#endif /* __PTHREAD__ */

