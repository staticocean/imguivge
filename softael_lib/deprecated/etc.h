
#ifndef __ETC__
#define __ETC__

//------------------------------------------------------------------------------

#include <time.h>

#include "vl.h"

//------------------------------------------------------------------------------

typedef struct etc_section
{
	uint64_t rt_start;
	uint64_t cpu_start;
	
	uint64_t rt_time;
	uint64_t cpu_time;

}	etc_section_t;

typedef struct etc_inst
{
	uint64_t section_count;
	uint64_t average_window;

	uint64_t rt_ref_start;
	uint64_t cpu_ref_start;

	uint64_t rt_ref_time;
	uint64_t cpu_ref_time;

	uint32_t rt_ref_count;
	uint32_t cpu_ref_count;

	etc_section_t *section_data;

	void (*__rt_time__ ) (uint64_t*);
	void (*__cpu_time__) (uint64_t*);
	
} 	etc_inst_t;

typedef struct etc_config
{
	uint32_t section_count;
	uint32_t average_window;

	etc_section_t *section_data;
	
	void (*__rt_time__ ) (uint64_t*);
	void (*__cpu_time__) (uint64_t*);

} 	etc_config_t;

//------------------------------------------------------------------------------


//------------------------------------------------------------------------------

void etc_init (etc_inst_t *inst, etc_config_t *config)
{
    inst->section_count  = config->section_count;
    inst->section_data   = config->section_data;
    inst->average_window = config->average_window;

    inst->__rt_time__    = config->__rt_time__;
    inst->__cpu_time__   = config->__cpu_time__;

    inst->rt_ref_time    = 0x00;
    inst->rt_ref_count   = 0x00;

    inst->cpu_ref_time   = 0x00;
    inst->cpu_ref_count  = 0x00;

    uint32_t i;

    for (i = 0; i < inst->section_count; ++i)
    {
        inst->section_data[i].rt_time   = 0x00;
        inst->section_data[i].rt_start  = 0x00;
        inst->section_data[i].cpu_time  = 0x00;
        inst->section_data[i].cpu_start = 0x00;
    }

    return;
}

//------------------------------------------------------------------------------

void etc_rt_ref_start (etc_inst_t *inst)
{
    inst->__rt_time__(&inst->rt_ref_start);

    return;
}

//------------------------------------------------------------------------------

void etc_rt_ref_stop (etc_inst_t *inst)
{
    uint64_t rt_time;

    inst->__rt_time__(&rt_time);

    inst->rt_ref_time += rt_time - inst->rt_ref_start;
    inst->rt_ref_count++;

    return;
}

//------------------------------------------------------------------------------

void etc_cpu_ref_start (etc_inst_t *inst)
{
    // inst->cpu_ref_start = (uint64_t) inst->__cpu_time__();

    return;
}

//------------------------------------------------------------------------------

void etc_cpu_ref_stop (etc_inst_t *inst)
{
    // inst->cpu_ref_time += (uint64_t) (inst->__cpu_time__() - (uint64_t) inst->rt_ref_start);
    inst->cpu_ref_count++;

    return;
}

//------------------------------------------------------------------------------

void etc_rt_start (etc_inst_t *inst, uint32_t section_id)
{
    uint64_t rt_time;

    inst->__rt_time__(&rt_time);

    inst->section_data[section_id].rt_start = rt_time;

    return;
}

//------------------------------------------------------------------------------

void etc_rt_stop (etc_inst_t *inst, uint32_t section_id)
{
    uint64_t rt_time;

    inst->__rt_time__(&rt_time);

    inst->section_data[section_id].rt_time *= (uint64_t) (inst->average_window - 1);
    inst->section_data[section_id].rt_time += (rt_time - inst->section_data[section_id].rt_start);
    inst->section_data[section_id].rt_time /= inst->average_window;

    inst->section_data[section_id].rt_time = rt_time;
    
    return;
}

//------------------------------------------------------------------------------

void etc_cpu_start (etc_inst_t *inst, uint32_t section_id)
{
    // inst->section_data[section_id].cpu_start = (uint64_t) __cpu_time__();

    return;
}

//------------------------------------------------------------------------------

void etc_cpu_stop (etc_inst_t *inst, uint32_t section_id)
{
    // inst->section_data[section_id].rt_time =
    //         inst->section_data[section_id].rt_time * (inst->average_window - 1)
    //         + (__cpu_time__() - (clock_t) inst->section_data[section_id].rt_start);
    
    inst->section_data[section_id].rt_time =
            inst->section_data[section_id].rt_time / inst->average_window;

    return;
}

//------------------------------------------------------------------------------

void etc_rt_ref_data (etc_inst_t *inst, uint64_t *data)
{
    *data = inst->rt_ref_time;

    return;
}

void etc_rt_data (etc_inst_t *inst, uint32_t section_id, uint64_t *data)
{
    *data = inst->section_data[section_id].rt_time;

    return;
}

void etc_cpu_ref_data (etc_inst_t *inst, uint64_t *data)
{
    *data = inst->cpu_ref_time;

    return;
}

void etc_cpu_data (etc_inst_t *inst, uint32_t section_id, uint64_t *data)
{
    *data = inst->section_data[section_id].cpu_time;
    
    return;
}

//------------------------------------------------------------------------------

#endif
