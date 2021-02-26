
#ifndef __TASK__
#define __TASK__

//------------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include <__pthread__.h>
#include <vl.h>

//------------------------------------------------------------------------------

typedef struct task
{
	pthread_t 		thread;
	uint8_t         state;
    
    void            *context;
	void          	*(*func) (void *task);
	
} 	s_task;

typedef struct task_init
{
	void            *context;
	void           	*(*func) (void *task);

}	s_task_init;

//------------------------------------------------------------------------------

inline uint8_t task_get_state(s_task *self)
{
    return __sync_add_and_fetch(&self->state, 0x00);
}

inline void task_set_state(s_task *self, uint8_t state)
{
    uint8_t oldval = __sync_add_and_fetch(&self->state, 0x00);
    __sync_val_compare_and_swap(&self->state, oldval, state);
    
	return;
}

//------------------------------------------------------------------------------

inline void task_init(s_task *self, s_task_init attr)
{
    self->func = attr.func;
    self->context = attr.context;
 
    task_set_state(self, 0xFF);
    
	return;
}

inline void task_stop(s_task *self)
{
    task_set_state(self, 0xFF);
	
    pthread_join(self->thread, NULL);
    
	return;
}

inline void task_run(s_task *self)
{
	// checks internally if task is running so its safe
	task_stop(self);
	task_set_state(self, 0x00);
	
	pthread_create(&self->thread, NULL, self->func, self);
//    pthread_create();
	
	return;
}

//------------------------------------------------------------------------------

#endif /* __SCH__ */






