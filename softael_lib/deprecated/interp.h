
#ifndef __INTERP_H__
#define __INTERP_H__

//------------------------------------------------------------------------------

#include <stdint.h>
#include <stdlib.h>
#include "types.h"

//------------------------------------------------------------------------------

typedef struct interp_data
{
	int64_t 		time_stamp;
	float64_t 		value;

}	interp_data_t;

typedef struct interp_inst
{
	int64_t 		ts_threshold;
	uint16_t 		interp_size;
	uint16_t 		interp_data_offset;
	interp_data_t 	*interp_data;

}	interp_inst_t;

typedef struct interp_config
{
	float64_t 		ts_threshold;
	uint16_t 		interp_size;
	interp_data_t 	*interp_data;

}	interp_config_t;

//------------------------------------------------------------------------------

void interp_init (interp_inst_t *inst, interp_config_t *config)
{
    inst->interp_size        = config->interp_size;
    inst->interp_data_offset = 0x00;
    inst->interp_data        = config->interp_data;
    inst->ts_threshold       = config->ts_threshold;

    uint32_t i;

    for (i = 0; i < inst->interp_size; ++i)
    {
        inst->interp_data[i].value = 0x00;
        inst->interp_data[i].time_stamp = i * inst->ts_threshold;
    }

    return;
}

//------------------------------------------------------------------------------

uint8_t interp_push (interp_inst_t *inst, int64_t time_stamp, float64_t value)
{
    uint8_t return_code = 0x00;
    
    if ((time_stamp - inst->interp_data[(inst->interp_data_offset + inst->interp_size - 1) % inst->interp_size].time_stamp) > inst->ts_threshold)
    {
        return_code = 0x00;
        
        inst->interp_data[inst->interp_data_offset % inst->interp_size].value = value;
        inst->interp_data[inst->interp_data_offset % inst->interp_size].time_stamp = time_stamp;
        inst->interp_data_offset++;
    }
    
    else
    {
        return_code = 0x01;
    }

    return return_code;
}

//------------------------------------------------------------------------------

uint8_t interp_get (interp_inst_t *inst, int64_t time_stamp, float64_t *value)
{
    uint32_t i;

    uint32_t m0_index = 0x00;
    uint32_t m1_index = 0x00;

    uint8_t return_code = 0x00;

    for (i = 0; i < inst->interp_size; ++i)
    {
        if (inst->interp_data[(inst->interp_data_offset + i) % inst->interp_size].time_stamp > time_stamp)
        {
            m0_index = (inst->interp_data_offset + i + inst->interp_size - 1) % inst->interp_size;
            m1_index = (inst->interp_data_offset + i) % inst->interp_size;
        }

    }

    float64_t m0_ts  = (float64_t) inst->interp_data[m0_index].time_stamp * 1E-6;
    float64_t m1_ts  = (float64_t) inst->interp_data[m1_index].time_stamp * 1E-6;
    float64_t ref_ts = (float64_t) time_stamp * 1E-6;

    float64_t m0_value = inst->interp_data[m0_index].value;
    float64_t m1_value = inst->interp_data[m1_index].value;

    if (   (m0_index != m1_index)
        && (m0_ts < m1_ts)
        && (m0_ts <= ref_ts)
        && (m1_ts >= ref_ts))
    {
        return_code = 0x00;

        *value = m0_value + (m1_value - m0_value)
            * (1 / (m1_ts / ref_ts - m0_ts / ref_ts) - 1 / (m1_ts / m0_ts - 1));
    }

    else
    {
        return_code = 0x01;
    }

    return return_code;
}

//------------------------------------------------------------------------------

void interp_get_oldest (interp_inst_t *inst, int64_t *time_stamp, float64_t *value)
{
    uint32_t index = inst->interp_data_offset % inst->interp_size;
    
    if (time_stamp != NULL)
    {
        *time_stamp = inst->interp_data[index].time_stamp;
    }

    if (value != NULL)
    {
        *value = inst->interp_data[index].value;
    }

    return;
}

//------------------------------------------------------------------------------

void interp_get_newest (interp_inst_t *inst, int64_t *time_stamp, float64_t *value)
{
    uint32_t index = (inst->interp_data_offset + inst->interp_size - 1) % inst->interp_size;
    
    if (time_stamp != NULL)
    {
        *time_stamp = inst->interp_data[index].time_stamp;
    }

    if (value != NULL)
    {
        *value = inst->interp_data[index].value;
    }

    return;
}

//------------------------------------------------------------------------------

#endif /* __INTERP_H__ */








