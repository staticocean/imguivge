//
//  ga.h
//  adaptive_control
//
//  Created by John Brown on 01/01/2018.
//  Copyright © 2018 okean. All rights reserved.
//

#ifndef __GA__
#define __GA__

//------------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include <string.h>

#include "vl.h"

//------------------------------------------------------------------------------

typedef struct ga_inst
{
    uint16_t    enc_count;
    uint32_t    enc_size;
    
    float64_t   step_size;
    
    uint32_t    select_rate;
    uint32_t    mutate_rate;
    uint32_t    crossover_rate;
    
    float64_t   *enc_data;
    float64_t   *enc_selection;
    float64_t   *enc_fit;
    float64_t   *enc_sect;
    
}   ga_inst_t;

typedef struct ga_config
{
    uint16_t    enc_count;
    uint32_t    enc_size;
    
    float64_t   step_size;
    
    uint32_t    select_rate;
    uint32_t    mutate_rate;
    uint32_t    crossover_rate;
    
    float64_t   *enc_data;
    float64_t   *enc_selection;
    float64_t   *enc_fit;
    float64_t   *enc_sect;
    
}   ga_config_t;

//------------------------------------------------------------------------------

float64_t __ga_rand__(float64_t limit_a, float64_t limit_b)
{
    return (limit_a + ((((float64_t) rand()) / ((float64_t) RAND_MAX)) * (limit_b - limit_a)));
}

//------------------------------------------------------------------------------

void ga_init (ga_inst_t *inst, ga_config_t *config)
{
    inst->enc_count         = config->enc_count;
    inst->enc_size          = config->enc_size;
    
    inst->step_size         = config->step_size;
    
    inst->mutate_rate       = config->mutate_rate;
    inst->select_rate       = config->select_rate;
    inst->crossover_rate    = config->crossover_rate;
    
    inst->enc_data          = config->enc_data;
    inst->enc_selection     = config->enc_selection;
    inst->enc_fit           = config->enc_fit;
    inst->enc_sect          = config->enc_sect;
    
    return;
}

//------------------------------------------------------------------------------

void ga_rand (ga_inst_t *inst)
{
    uint32_t i;
    uint32_t j;
    
    for (i = 0; i < inst->enc_count; ++i)
    {
        for (j = 0; j < inst->enc_size; ++j)
        {
            inst->enc_data[i*inst->enc_size+j] = __ga_rand__(0, 1);
        }
    }
    
    return;
}

//------------------------------------------------------------------------------

void ga_limit (ga_inst_t *inst)
{
    uint32_t    i;
    uint32_t    j;
    
    for (i = 0; i < inst->enc_count; ++i)
    {
        for (j = 0; j < inst->enc_size; ++j)
        {
            if (inst->enc_data[i*inst->enc_size + j] > 1.0)
            {
                inst->enc_data[i*inst->enc_size + j] = 1.0;
            }
            
            else if (inst->enc_data[i*inst->enc_size + j] < 0.0)
            {
                inst->enc_data[i*inst->enc_size + j] = 0.0;
            }
        }
    }
    
    return;
}

//------------------------------------------------------------------------------

void __ga_cross__ (ga_inst_t *inst, float64_t *dest_0, float64_t *dest_1,
        float64_t *src_0, float64_t *src_1)
{
    uint32_t i;
    uint32_t co_point;
    
    if (rand() < inst->crossover_rate)
    {
        co_point = rand() % inst->enc_size;
    }
    
    else
    {
        co_point = 0;
    }
    
    for (i = 0; i < co_point; ++i)
    {
        dest_0[i] = src_0[i];
        dest_1[i] = src_1[i];
    }

    for (i = co_point; i < inst->enc_size; ++i)
    {
        dest_0[i] = src_1[i];
        dest_1[i] = src_0[i];
    }
    
    return;
}

void ga_cross (ga_inst_t *inst)
{
    uint32_t i;

    uint32_t src_0;
    uint32_t src_1;
    
    for (i = 0; i < inst->enc_count / 2; ++i)
    {
        src_0 = 2*i+0;
        src_1 = 2*i+1;
        
        __ga_cross__(inst,
            &inst->enc_data      [1*i   * inst->enc_size],
            &inst->enc_data      [2*i   * inst->enc_size],
            &inst->enc_selection [src_0 * inst->enc_size],
            &inst->enc_selection [src_1 * inst->enc_size]);
    }
    
    return;
}

//------------------------------------------------------------------------------

void ga_mutate (ga_inst_t *inst)
{
    uint32_t    i;
    uint32_t    j;
    
    for (i = 0; i < inst->enc_count; ++i)
    {
        for (j = 0; j < inst->enc_size; ++j)
        {
            if (rand() < inst->mutate_rate)
            {
                inst->enc_data[i*inst->enc_size + j] +=
                        __ga_rand__(-inst->step_size, inst->step_size);
            }
        }
    }
    
    return;
}

//------------------------------------------------------------------------------

void ga_enc_print(ga_inst_t *inst, float64_t *enc)
{
//    uint32_t i;

//    printf("[");
//
//    for (i = 0; i < inst->enc_size; ++i)
//    {
//        printf("%1.3Lf ", enc[i]);
//    }
//
//    printf("] \r\n");
}

//------------------------------------------------------------------------------

void ga_sect (ga_inst_t *inst)
{
    uint32_t i;
    
    float64_t sect_accm = 0.0;
    
    memcpy(inst->enc_sect, inst->enc_fit, inst->enc_count * sizeof(float64_t));
    
    for (i = 0; i < inst->enc_count; ++i)
    {
        sect_accm += inst->enc_sect[i];
    }
    
    for (i = 0; i < inst->enc_count; ++i)
    {
        inst->enc_sect[i] /= sect_accm;
    }
    
    for (i = 1; i < inst->enc_count; ++i)
    {
        inst->enc_sect[i] += inst->enc_sect[i-1];
    }
    
    return;
}

//------------------------------------------------------------------------------

void ga_select (ga_inst_t *inst)
{
    uint32_t i;
    uint32_t j;

    float64_t rand_output;
    
    float64_t best_value[inst->select_rate];
    uint32_t  best_index[inst->select_rate];

    for (j = 0; j < inst->select_rate; ++j)
    {
        best_value[j] = inst->enc_fit[j];
        best_index[j] = j;
    }

    for (i = 0; i < inst->enc_count; ++i)
    {
        for (j = 0; j < inst->select_rate; ++j)
        {
            if (inst->enc_fit[i] > best_value[j])
            {
                best_value[j] = inst->enc_fit[i];
                best_index[j] = i;

                break;
            }
        }
    }

    for (j = 0; j < inst->select_rate; ++j)
    {
        memcpy(&inst->enc_selection[j             * inst->enc_size],
               &inst->enc_data     [best_index[j] * inst->enc_size],
               inst->enc_size * sizeof(float64_t));
    }

    for (i = inst->select_rate; i < inst->enc_count; ++i)
    {
        rand_output = __ga_rand__(0, 1);

        for (j = 0; j < inst->enc_count; ++j)
        {
            if (rand_output <= inst->enc_sect[j])
            {
                memcpy(&inst->enc_selection[i * inst->enc_size],
                       &inst->enc_data     [j * inst->enc_size],
                       inst->enc_size * sizeof(float64_t));
                
                break;
            }
        }
    }
    
    return;
}

//------------------------------------------------------------------------------

void ga_print (ga_inst_t *inst)
{
//    uint32_t i;
//    uint32_t offset;

//    for (i = 0; i < inst->enc_count; ++i)
//    {
//        offset = i * inst->enc_size;
//
//        printf("[%d] F: %Lf \r\n", i, inst->enc_fit[i]);
//    }
    
    return;
}

//------------------------------------------------------------------------------

void ga_copy(ga_inst_t *inst, float64_t *dest, float64_t *src)
{
    uint32_t i;
    
    for (i = 0; i < inst->enc_size; ++i)
    {
        dest[i] = src[i];
    }
    
    return;
}

//------------------------------------------------------------------------------

#endif /* __GA_H__ */
