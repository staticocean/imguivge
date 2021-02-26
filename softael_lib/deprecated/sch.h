
#ifndef __SCH__
#define __SCH__

//------------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <thread>
#include <mutex>

#include <vl.h>

//------------------------------------------------------------------------------

#define SCH_MAX_TASKS       32

//------------------------------------------------------------------------------

typedef struct sch_task
{
	mutable std::mutex mutex;
	
    std::thread     thread;
    uint8_t         state;
    
    void            *context;
	void           	(*func) (void *task);
	
	// fuck c++
	sch_task &operator=(const sch_task &o)
	{
		state 	= o.state;
		context = o.context;
		func 	= o.func;
		
		return *this;
	}
	
} 	s_sch_task;

typedef struct sch
{
    uint32_t        task_offset;
    s_sch_task      task_list[32];
    
}    s_sch;

typedef struct sch_init
{
    uint32_t        task_offset;
    s_sch_task      task_list[32];
    
}   s_sch_init;

//------------------------------------------------------------------------------

inline uint8_t sch_task_get_state(s_sch_task *self)
{
	self->mutex.lock();
	uint8_t state = self->state;
	self->mutex.unlock();
	
	return state;
}

inline uint8_t sch_task_set_state(s_sch_task *self, uint8_t state)
{
	self->mutex.lock();
	self->state = state;
	self->mutex.unlock();
	
	return state;
}

inline void sch_task_lock(s_sch_task *self) { self->mutex.lock(); return; }
inline void sch_task_unlock(s_sch_task *self) { self->mutex.unlock(); return; }

inline s_sch_task* sch_task_add(s_sch *self, s_sch_task task)
{
	uint32_t task_id = 0x00;
    s_sch_task *task_handle = NULL;
	
    task.state = 0x00;
    
	while ((task_id < SCH_MAX_TASKS) && (self->task_list[task_id].state != 0xFF))
    { ++task_id; }
	
    if (task_id < SCH_MAX_TASKS)
    {
        self->task_list[task_id] = task;
        task_handle = &self->task_list[task_id];
	
		sch_task_lock(task_handle);
        task_handle->thread = std::thread(task_handle->func, task_handle);
		sch_task_unlock(task_handle);
		task_handle->thread.detach();
    }
    
	return task_handle;
}

inline void sch_task_exit(s_sch_task *task)
{
    task->context = NULL;
    task->func    = NULL;
    task->state   = 0xFF;
    
    return;
}

inline void sch_task_rem(s_sch *self, s_sch_task *task)
{
    task->context = NULL;
    task->func    = NULL;
    task->state   = 0xFF;
    
    return;
}

//------------------------------------------------------------------------------

//inline void sch_update(s_sch *self)
//{
//    uint32_t i;
//
//    for (i = 0; i < SCH_MAX_TASKS; ++i)
//    {
//        s_sch_task *task = &self->task_list[i];
//
//        switch (task->state)
//        {
//            case    SCH_STATE_CLEAR  : { break; }
//            case    SCH_STATE_RUNNING: { task->func(self, task); break; }
////            default : { --task->state; }
//        }
//    }
//
//    return;
//}

inline void sch_init(s_sch *self, s_sch_init attr)
{
    uint32_t i;
    
    for (i = 0; i < SCH_MAX_TASKS; ++i)
    {
        self->task_list[i].state = 0xFF;
    }
    
    return;
}

//------------------------------------------------------------------------------


#endif /* __SCH__ */






