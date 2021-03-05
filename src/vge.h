//
//  main.c
//  gps_clock
//
//  Created by John Brown on 26/08/2016.
//  Copyright © 2016 Control System Interfaces. All rights reserved.
//

#ifndef __VGE__
#define __VGE__

//------------------------------------------------------------------------------
// INCLUDES
//------------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include <lib_internal/vl.h>
#include <lib_internal/task.h>
#include <lib_internal/sda.h>

//------------------------------------------------------------------------------

typedef uint16_t s_vge_p[2];

typedef struct
{
    s_vge_p p0;
    s_vge_p p1;
    
}   s_vge_line;

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------

#endif /* __VGE__ */





