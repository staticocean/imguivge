
#ifndef __CALIB_H__
#define __CALIB_H__

//------------------------------------------------------------------------------

#include "vl.h"
#include "ga.h"

//------------------------------------------------------------------------------

typedef struct calib_D3_MEAN_3B_inst
{
	uint32_t  counter;

	float64_t accm[3];
	float64_t bias[3];

}	calib_D3_MEAN_3B_inst_t;

typedef struct calib_D3_MEAN_3B_config
{

}	calib_D3_MEAN_3B_config_t;

//------------------------------------------------------------------------------

void calib_D3_MEAN_3B_init (calib_D3_MEAN_3B_inst_t *inst, calib_D3_MEAN_3B_config_t *config)
{
    inst->counter = 0x00;
    
    inst->accm[0] = 0;
    inst->accm[1] = 0;
    inst->accm[2] = 0;

    inst->bias[0] = 0;
    inst->bias[1] = 0;
    inst->bias[2] = 0;

    return;
}

void calib_D3_MEAN_3B_update (calib_D3_MEAN_3B_inst_t *inst, float64_t *data)
{
    inst->counter++;

    inst->accm[0] -= data[0];
    inst->accm[1] -= data[1];
    inst->accm[2] -= data[2];

    if (inst->counter != 0x00)
    {
        inst->bias[0] = inst->accm[0] / inst->counter;
        inst->bias[1] = inst->accm[1] / inst->counter;
        inst->bias[2] = inst->accm[2] / inst->counter;
    }

    return;
}

//------------------------------------------------------------------------------

// 6RLS3G3B - 6 parameters Recurrent least square 3 gain 3 bias

typedef struct calib_D6_GA_3G3B_data
{
    float64_t bias[3];
    float64_t gain[3];
    
}   calib_D6_GA_3G3B_data_t;

typedef struct calib_D6_GA_3G3B_inst
{
    float64_t       invalid_range_gain;
    float64_t       initial_step_size;
    
    float64_t       range_threshold[4];
    float64_t       range_step_size[4];
    float64_t       range_error_gain[4];
    
    uint32_t        solution_count;
    uint32_t        solution_size;
    float64_t       mutate_rate;
    float64_t       select_rate;
    float64_t       crossover_rate;
    float64_t       error_gain;
    
    float64_t       *solution_data;
    float64_t       *solution_selection;
    float64_t       *solution_fit;
    float64_t       *solution_sect;
    
    calib_D6_GA_3G3B_data_t best_solution_data;
    float64_t       best_solution_score;
    float64_t       best_solution_magn;
    float64_t       best_solution_error;
    
    float64_t       bias_range;
    float64_t       bias_min;
    float64_t       gain_range;
    float64_t       gain_min;
    
    ga_inst_t       ga_inst;
    
}   calib_D6_GA_3G3B_inst_t;

typedef struct calib_D6_GA_3G3B_config
{
    float64_t       invalid_range_gain;
    float64_t       initial_step_size;
    
    float64_t       range_threshold[4];
    float64_t       range_step_size[4];
    float64_t       range_error_gain[4];
    
    uint32_t        solution_count;
    uint32_t        solution_size;
    float64_t       mutate_rate;
    float64_t       select_rate;
    float64_t       crossover_rate;
    float64_t       error_gain;
    
    float64_t       *solution_data;
    float64_t       *solution_selection;
    float64_t       *solution_fit;
    float64_t       *solution_sect;
    
    float64_t       bias_min;
    float64_t       bias_max;
    float64_t       gain_min;
    float64_t       gain_max;
    
}   calib_D6_GA_3G3B_config_t;

//------------------------------------------------------------------------------

float64_t __calib_D6_GA_3G3B_magn__(calib_D6_GA_3G3B_data_t *calib_data, float64_t data[3])
{
    float64_t __data__[3] =
    {
        (data[0] + calib_data->bias[0]) * (1 + calib_data->gain[0]),
        (data[1] + calib_data->bias[1]) * (1 + calib_data->gain[1]),
        (data[2] + calib_data->bias[2]) * (1 + calib_data->gain[2]),
    };
    
    return sqrt(__data__[0] * __data__[0]
              + __data__[1] * __data__[1]
              + __data__[2] * __data__[2]);
}

float64_t calib_D6_GA_3G3B_score(calib_D6_GA_3G3B_data_t *calib_data, float64_t data[3])
{
    float64_t error_temp = 1 - __calib_D6_GA_3G3B_magn__(calib_data, data);
    return error_temp * error_temp;
}

//------------------------------------------------------------------------------

void calib_D6_GA_3G3B_encode(calib_D6_GA_3G3B_inst_t *inst, calib_D6_GA_3G3B_data_t *calib_data, float64_t *ga_enc)
{
    ga_enc[0] = (calib_data->bias[0] - inst->bias_min) / inst->bias_range;
    ga_enc[2] = (calib_data->bias[1] - inst->bias_min) / inst->bias_range;
    ga_enc[4] = (calib_data->bias[2] - inst->bias_min) / inst->bias_range;
   
    ga_enc[1] = (calib_data->gain[0] - inst->gain_min) / inst->gain_range;
    ga_enc[3] = (calib_data->gain[1] - inst->gain_min) / inst->gain_range;
    ga_enc[5] = (calib_data->gain[2] - inst->gain_min) / inst->gain_range;
//
    // ga_enc[0] = (calib_data->bias[0] + 4) / 8;
    // ga_enc[2] = (calib_data->bias[1] + 4) / 8;
    // ga_enc[4] = (calib_data->bias[2] + 4) / 8;
    
    // ga_enc[1] = (calib_data->gain[0] + 0) / 2;
    // ga_enc[3] = (calib_data->gain[1] + 0) / 2;
    // ga_enc[5] = (calib_data->gain[2] + 0) / 2;
    
    return;
}

void calib_D6_GA_3G3B_decode(calib_D6_GA_3G3B_inst_t *inst, calib_D6_GA_3G3B_data_t *calib_data, float64_t *ga_enc)
{
    calib_data->bias[0] = ga_enc[0] * inst->bias_range + inst->bias_min;
    calib_data->bias[1] = ga_enc[2] * inst->bias_range + inst->bias_min;
    calib_data->bias[2] = ga_enc[4] * inst->bias_range + inst->bias_min;
   
    calib_data->gain[0] = ga_enc[1] * inst->gain_range + inst->gain_min;
    calib_data->gain[1] = ga_enc[3] * inst->gain_range + inst->gain_min;
    calib_data->gain[2] = ga_enc[5] * inst->gain_range + inst->gain_min;
    
    // calib_data->bias[0] = ga_enc[0] * 8 - 4;
    // calib_data->bias[1] = ga_enc[2] * 8 - 4;
    // calib_data->bias[2] = ga_enc[4] * 8 - 4;
    
    // calib_data->gain[0] = ga_enc[1] * 2 - 0;
    // calib_data->gain[1] = ga_enc[3] * 2 - 0;
    // calib_data->gain[2] = ga_enc[5] * 2 - 0;
    
    return;
}

uint8_t calib_D6_GA_3G3B_validate(calib_D6_GA_3G3B_inst_t *inst, calib_D6_GA_3G3B_data_t *calib_data)
{
    uint8_t return_code = 0x00;
    return return_code;
}

//------------------------------------------------------------------------------

// void calib_print (calib_data_t *calib_data)
// {
//     printf("BIAS %10.6Lf %10.6Lf %10.6Lf GAIN %10.6Lf %10.6Lf %10.6Lf ",
//            calib_data->bias[0], calib_data->bias[1], calib_data->bias[2],
//            calib_data->gain[0], calib_data->gain[1], calib_data->gain[2]);

// //    printf("    \r\n");

//     printf("BIAS %10.6Lf %10.6Lf %10.6Lf GAIN %10.6Lf %10.6Lf %10.6Lf ",
//            calib_data->bias[0],
//            calib_data->bias[1],
//            calib_data->bias[2],
//            1 / (1 + calib_data->gain[0]),
//            1 / (1 + calib_data->gain[1]),
//            1 / (1 + calib_data->gain[2]));

//     printf("    \r\n");

//     return;
// }

//------------------------------------------------------------------------------

void calib_D6_GA_3G3B_reset(calib_D6_GA_3G3B_inst_t *inst)
{
    srand(0x12345678);
    
    ga_rand(&inst->ga_inst);
    
    return;
}

//------------------------------------------------------------------------------

void calib_D6_GA_3G3B_init (calib_D6_GA_3G3B_inst_t *inst, calib_D6_GA_3G3B_config_t *config)
{
    inst->bias_range = config->bias_max - config->bias_min;
    inst->gain_range = config->gain_max - config->gain_min;
    inst->bias_min = config->bias_min;
    inst->gain_min = config->gain_min;
    
    inst->invalid_range_gain     = config->invalid_range_gain;
    inst->initial_step_size      = config->initial_step_size;

    inst->range_threshold[0]      = config->range_threshold[0];
    inst->range_threshold[1]      = config->range_threshold[1];
    inst->range_threshold[2]      = config->range_threshold[2];
    inst->range_threshold[3]      = config->range_threshold[3];
    
    inst->range_step_size[0]      = config->range_step_size[0];
    inst->range_step_size[1]      = config->range_step_size[1];
    inst->range_step_size[2]      = config->range_step_size[2];
    inst->range_step_size[3]      = config->range_step_size[3];
    
    inst->range_error_gain[0]      = config->range_error_gain[0];
    inst->range_error_gain[1]      = config->range_error_gain[1];
    inst->range_error_gain[2]      = config->range_error_gain[2];
    inst->range_error_gain[3]      = config->range_error_gain[3];

    inst->solution_count    = config->solution_count;
    inst->solution_size     = config->solution_size;
    inst->mutate_rate       = config->mutate_rate;
    inst->select_rate       = config->select_rate;
    inst->crossover_rate    = config->crossover_rate;
    inst->error_gain        = config->error_gain;
    
    inst->solution_data     = config->solution_data;
    inst->solution_selection= config->solution_selection;
    inst->solution_fit      = config->solution_fit;
    inst->solution_sect     = config->solution_sect;

    ga_config_t ga_config;
    
    ga_config.enc_count     = config->solution_count;
    ga_config.enc_size      = config->solution_size;
    
    ga_config.step_size     = config->initial_step_size;
    
    ga_config.select_rate   = config->select_rate;
    ga_config.mutate_rate   = RAND_MAX * config->mutate_rate;
    ga_config.crossover_rate= RAND_MAX * config->crossover_rate;
    
    ga_config.enc_data      = config->solution_data;
    ga_config.enc_selection = config->solution_selection;
    ga_config.enc_fit       = config->solution_fit;
    ga_config.enc_sect      = config->solution_sect;

    srand(0x12345678);
    dsp_rand_init(0x00);

    ga_init(&inst->ga_inst, &ga_config);
    ga_rand(&inst->ga_inst);
    
    inst->best_solution_score = -1E+6;
    inst->best_solution_error = 1E+6;
    
    return;
}

//------------------------------------------------------------------------------

void calib_D6_GA_3G3B_update(calib_D6_GA_3G3B_inst_t *inst, uint32_t data_set_size, float64_t *data_set)
{
    uint32_t i;
    uint32_t j;
    
    ga_inst_t *ga_inst = &inst->ga_inst;
    
    calib_D6_GA_3G3B_data_t calib_data_temp;
    uint8_t                 calib_data_valid;
    
    float64_t               best_fit_value;
    uint32_t                best_fit_index;
    
    uint32_t rand_offset[256];

    for (i = 0; i < 256; ++i)
    {
        rand_offset[i] = rand() % data_set_size;
    }

    for (j = 0; j < ga_inst->enc_count; ++j)
    {
        calib_D6_GA_3G3B_decode(inst, &calib_data_temp, &ga_inst->enc_data[j*ga_inst->enc_size]);
        calib_data_valid = calib_D6_GA_3G3B_validate(inst, &calib_data_temp);
        
        ga_inst->enc_fit[j] = 0.0;
        
        for (i = 0; i < 256; ++i)
        {
            // ga_inst->enc_fit[j] += calib_D6_GA_3G3B_score(&calib_data_temp, &data_set[i * 3]);
            ga_inst->enc_fit[j] += calib_D6_GA_3G3B_score(&calib_data_temp, &data_set[rand_offset[i] * 3]);
        }
        
        ga_inst->enc_fit[j] = sqrt(ga_inst->enc_fit[j]);
        ga_inst->enc_fit[j] = inst->error_gain / (inst->error_gain + ga_inst->enc_fit[j]);
        
        if (calib_data_valid == 0x01)
        {
            ga_inst->enc_fit[j] *= inst->invalid_range_gain;
        }
    }
    
    best_fit_value = ga_inst->enc_fit[0];
    best_fit_index = 0;
    
    for (j = 0; j < ga_inst->enc_count; ++j)
    {
        if (ga_inst->enc_fit[j] > best_fit_value)
        {
            best_fit_value = ga_inst->enc_fit[j];
            best_fit_index = j;
        }
    }

    // if (best_fit_value > inst->best_solution_score)
    // {
    //     calib_D6_GA_3G3B_decode(inst, &calib_data_temp,
    //                         &ga_inst->enc_data[best_fit_index*ga_inst->enc_size]);
    
    //     calib_data_valid = calib_D6_GA_3G3B_validate(&calib_data_temp);
    
    //     if (calib_data_valid == 0x00)
    //     {
    //         inst->best_solution_score = best_fit_value;
    //         inst->best_solution_error = 1 / (best_fit_value / inst->error_gain) - inst->error_gain;
    
    //         calib_D6_GA_3G3B_decode(inst, &inst->best_solution_data,
    //                             &ga_inst->enc_data[best_fit_index*ga_inst->enc_size]);
    //     }
    // }
    
    inst->best_solution_score = best_fit_value;
    inst->best_solution_error = 1 / (best_fit_value / inst->error_gain) - inst->error_gain;
    
    calib_D6_GA_3G3B_decode(inst, &inst->best_solution_data,
                        &ga_inst->enc_data[best_fit_index*ga_inst->enc_size]);

    for (i = 0; i < 4; ++i)
    {
        if (inst->best_solution_error < inst->range_threshold[i])
        {
            ga_inst->step_size = inst->range_step_size[i];
        }
    }
    
    ga_sect     (ga_inst);
    ga_select   (ga_inst);
    ga_cross    (ga_inst);
    ga_mutate   (ga_inst);
    ga_limit    (ga_inst);
    
    return;
}


//------------------------------------------------------------------------------

// 6RLS3G3B - 6 parameters Recurrent least square 3 gain 3 bias

typedef struct calib_D9_GA_6G3B_data
{
    float64_t bias[3];
    float64_t gain[3][3];
    
}   calib_D9_GA_6G3B_data_t;

typedef struct calib_D9_GA_6G3B_inst
{
    float64_t       invalid_range_gain;
    float64_t       initial_step_size;
    
    float64_t       range_threshold[4];
    float64_t       range_step_size[4];
    float64_t       range_error_gain[4];
    
    uint32_t        solution_count;
    uint32_t        solution_size;
    float64_t       mutate_rate;
    float64_t       select_rate;
    float64_t       crossover_rate;
    float64_t       error_gain;
    
    float64_t       *solution_data;
    float64_t       *solution_selection;
    float64_t       *solution_fit;
    float64_t       *solution_sect;
    
    calib_D9_GA_6G3B_data_t best_solution_data;
    float64_t       best_solution_score;
    float64_t       best_solution_magn;
    float64_t       best_solution_error;
    
    float64_t       bias_range;
    float64_t       bias_min;
    float64_t       gain_range[2];
    float64_t       gain_min[2];
    
    ga_inst_t       ga_inst;
    
}   calib_D9_GA_6G3B_inst_t;

typedef struct calib_D9_GA_6G3B_config
{
    float64_t       invalid_range_gain;
    float64_t       initial_step_size;
    
    float64_t       range_threshold[4];
    float64_t       range_step_size[4];
    float64_t       range_error_gain[4];
    
    uint32_t        solution_count;
    uint32_t        solution_size;
    float64_t       mutate_rate;
    float64_t       select_rate;
    float64_t       crossover_rate;
    float64_t       error_gain;
    
    float64_t       *solution_data;
    float64_t       *solution_selection;
    float64_t       *solution_fit;
    float64_t       *solution_sect;
    
    float64_t       bias_min;
    float64_t       bias_max;
    float64_t       gain_min[2];
    float64_t       gain_max[2];
    
}   calib_D9_GA_6G3B_config_t;

//------------------------------------------------------------------------------

float64_t __calib_D9_GA_6G3B_magn__(calib_D9_GA_6G3B_data_t *calib_data, float64_t data[3])
{
    float64_t __data__[3] =
    {
        (data[0] + calib_data->bias[0]) * (1 + calib_data->gain[0][0]),
        (data[0] + calib_data->bias[0]) * (    calib_data->gain[1][0]) + (data[1] + calib_data->bias[1]) * (1 + calib_data->gain[1][1]),
        (data[0] + calib_data->bias[0]) * (    calib_data->gain[2][0]) + (data[1] + calib_data->bias[1]) * (    calib_data->gain[2][1]) + (data[2] + calib_data->bias[2]) * (1 + calib_data->gain[2][2]),
    };
    
    return   (__data__[0] * __data__[0]
            + __data__[1] * __data__[1]
            + __data__[2] * __data__[2]);
}

float64_t calib_D9_GA_6G3B_score(calib_D9_GA_6G3B_data_t *calib_data, float64_t data[3])
{
    float64_t temp = 1 - __calib_D9_GA_6G3B_magn__(calib_data, data);
    return temp * temp;
}

//------------------------------------------------------------------------------

void calib_D9_GA_6G3B_encode(calib_D9_GA_6G3B_inst_t *inst, calib_D9_GA_6G3B_data_t *calib_data, float64_t *ga_enc)
{
    ga_enc[0] = (calib_data->bias[0] - inst->bias_min) / inst->bias_range;
    ga_enc[1] = (calib_data->bias[1] - inst->bias_min) / inst->bias_range;
    ga_enc[2] = (calib_data->bias[2] - inst->bias_min) / inst->bias_range;
   
    ga_enc[3] = (calib_data->gain[0][0] - inst->gain_min[0]) / inst->gain_range[0];
    ga_enc[4] = (calib_data->gain[1][1] - inst->gain_min[0]) / inst->gain_range[0];
    ga_enc[5] = (calib_data->gain[2][2] - inst->gain_min[0]) / inst->gain_range[0];

    ga_enc[6] = (calib_data->gain[1][0] - inst->gain_min[1]) / inst->gain_range[1];
    ga_enc[7] = (calib_data->gain[2][0] - inst->gain_min[1]) / inst->gain_range[1];
    ga_enc[8] = (calib_data->gain[2][1] - inst->gain_min[1]) / inst->gain_range[1];

    return;
}

void calib_D9_GA_6G3B_decode(calib_D9_GA_6G3B_inst_t *inst, calib_D9_GA_6G3B_data_t *calib_data, float64_t *ga_enc)
{
    calib_data->bias[0] = ga_enc[0] * inst->bias_range + inst->bias_min;
    calib_data->bias[1] = ga_enc[1] * inst->bias_range + inst->bias_min;
    calib_data->bias[2] = ga_enc[2] * inst->bias_range + inst->bias_min;

    calib_data->gain[0][0] = ga_enc[3] * inst->gain_range[0] + inst->gain_min[0];
    calib_data->gain[1][1] = ga_enc[4] * inst->gain_range[0] + inst->gain_min[0];
    calib_data->gain[2][2] = ga_enc[5] * inst->gain_range[0] + inst->gain_min[0];

    calib_data->gain[1][0] = ga_enc[6] * inst->gain_range[1] + inst->gain_min[1];
    calib_data->gain[2][0] = ga_enc[7] * inst->gain_range[1] + inst->gain_min[1];
    calib_data->gain[2][1] = ga_enc[8] * inst->gain_range[1] + inst->gain_min[1];
    
    return;
}

uint8_t calib_D9_GA_6G3B_validate(calib_D9_GA_6G3B_inst_t *inst, calib_D9_GA_6G3B_data_t *calib_data)
{
    uint8_t return_code = 0x00;
    return return_code;
}

//------------------------------------------------------------------------------

void calib_D9_GA_6G3B_reset(calib_D9_GA_6G3B_inst_t *inst)
{
    srand(0x12345678);
    
    ga_rand(&inst->ga_inst);
    
    return;
}

//------------------------------------------------------------------------------

void calib_D9_GA_6G3B_init (calib_D9_GA_6G3B_inst_t *inst, calib_D9_GA_6G3B_config_t *config)
{
    inst->bias_range = config->bias_max - config->bias_min;
    inst->gain_range[0] = config->gain_max[0] - config->gain_min[0];
    inst->gain_range[1] = config->gain_max[1] - config->gain_min[1];
    inst->bias_min = config->bias_min;
    inst->gain_min[0] = config->gain_min[0];
    inst->gain_min[1] = config->gain_min[1];
    
    inst->invalid_range_gain     = config->invalid_range_gain;
    inst->initial_step_size      = config->initial_step_size;

    inst->range_threshold[0]      = config->range_threshold[0];
    inst->range_threshold[1]      = config->range_threshold[1];
    inst->range_threshold[2]      = config->range_threshold[2];
    inst->range_threshold[3]      = config->range_threshold[3];
    
    inst->range_step_size[0]      = config->range_step_size[0];
    inst->range_step_size[1]      = config->range_step_size[1];
    inst->range_step_size[2]      = config->range_step_size[2];
    inst->range_step_size[3]      = config->range_step_size[3];
    
    inst->range_error_gain[0]      = config->range_error_gain[0];
    inst->range_error_gain[1]      = config->range_error_gain[1];
    inst->range_error_gain[2]      = config->range_error_gain[2];
    inst->range_error_gain[3]      = config->range_error_gain[3];

    inst->solution_count    = config->solution_count;
    inst->solution_size     = config->solution_size;
    inst->mutate_rate       = config->mutate_rate;
    inst->select_rate       = config->select_rate;
    inst->crossover_rate    = config->crossover_rate;
    inst->error_gain        = config->error_gain;
    
    inst->solution_data     = config->solution_data;
    inst->solution_selection= config->solution_selection;
    inst->solution_fit      = config->solution_fit;
    inst->solution_sect     = config->solution_sect;

    ga_config_t ga_config;
    
    ga_config.enc_count     = config->solution_count;
    ga_config.enc_size      = config->solution_size;
    
    ga_config.step_size     = config->initial_step_size;
    
    ga_config.select_rate   = config->select_rate;
    ga_config.mutate_rate   = RAND_MAX * config->mutate_rate;
    ga_config.crossover_rate= RAND_MAX * config->crossover_rate;
    
    ga_config.enc_data      = config->solution_data;
    ga_config.enc_selection = config->solution_selection;
    ga_config.enc_fit       = config->solution_fit;
    ga_config.enc_sect      = config->solution_sect;

    srand(0x12345678);
    dsp_rand_init(0x00);

    ga_init(&inst->ga_inst, &ga_config);
    ga_rand(&inst->ga_inst);
    
    inst->best_solution_score = -1E+6;
    inst->best_solution_error = 1E+6;
    
    return;
}

//------------------------------------------------------------------------------

void calib_D9_GA_6G3B_update(calib_D9_GA_6G3B_inst_t *inst, uint32_t data_set_size, float64_t *data_set)
{
    uint32_t i;
    uint32_t j;
    
    ga_inst_t *ga_inst = &inst->ga_inst;
    
    calib_D9_GA_6G3B_data_t calib_data_temp;
    uint8_t                 calib_data_valid;
    
    float64_t               best_fit_value;
    uint32_t                best_fit_index;
    
    uint32_t rand_offset[256];

    for (i = 0; i < 256; ++i)
    {
        rand_offset[i] = rand() % data_set_size;
    }

    for (j = 0; j < ga_inst->enc_count; ++j)
    {
        calib_D9_GA_6G3B_decode(inst, &calib_data_temp, &ga_inst->enc_data[j*ga_inst->enc_size]);
        calib_data_valid = calib_D9_GA_6G3B_validate(inst, &calib_data_temp);
        
        ga_inst->enc_fit[j] = 0.0;
        
        for (i = 0; i < 256; ++i)
        {
            // ga_inst->enc_fit[j] += calib_D6_GA_3G3B_score(&calib_data_temp, &data_set[i * 3]);
            ga_inst->enc_fit[j] += calib_D9_GA_6G3B_score(&calib_data_temp, &data_set[rand_offset[i] * 3]);
        }
        
        ga_inst->enc_fit[j] = sqrt(ga_inst->enc_fit[j]);
        ga_inst->enc_fit[j] = inst->error_gain / (inst->error_gain + ga_inst->enc_fit[j]);
        
        if (calib_data_valid == 0x01)
        {
            ga_inst->enc_fit[j] *= inst->invalid_range_gain;
        }
    }
    
    best_fit_value = ga_inst->enc_fit[0];
    best_fit_index = 0;
    
    for (j = 0; j < ga_inst->enc_count; ++j)
    {
        if (ga_inst->enc_fit[j] > best_fit_value)
        {
            best_fit_value = ga_inst->enc_fit[j];
            best_fit_index = j;
        }
    }

    // if (best_fit_value > inst->best_solution_score)
    // {
    //     calib_D6_GA_3G3B_decode(inst, &calib_data_temp,
    //                         &ga_inst->enc_data[best_fit_index*ga_inst->enc_size]);
    
    //     calib_data_valid = calib_D6_GA_3G3B_validate(&calib_data_temp);
    
    //     if (calib_data_valid == 0x00)
    //     {
    //         inst->best_solution_score = best_fit_value;
    //         inst->best_solution_error = 1 / (best_fit_value / inst->error_gain) - inst->error_gain;
    
    //         calib_D6_GA_3G3B_decode(inst, &inst->best_solution_data,
    //                             &ga_inst->enc_data[best_fit_index*ga_inst->enc_size]);
    //     }
    // }
    
    inst->best_solution_score = best_fit_value;
    inst->best_solution_error = 1 / (best_fit_value / inst->error_gain) - inst->error_gain;
    
    calib_D9_GA_6G3B_decode(inst, &inst->best_solution_data,
                        &ga_inst->enc_data[best_fit_index*ga_inst->enc_size]);

    for (i = 0; i < 4; ++i)
    {
        if (inst->best_solution_error < inst->range_threshold[i])
        {
            ga_inst->step_size = inst->range_step_size[i];
        }
    }
    
    ga_sect     (ga_inst);
    ga_select   (ga_inst);
    ga_cross    (ga_inst);
    ga_mutate   (ga_inst);
    ga_limit    (ga_inst);
    
    return;
}

//------------------------------------------------------------------------------

#endif /* __CALIB_H__ */
