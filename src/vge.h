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

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include <lib_internal/vl.h>
#include <lib_internal/vl3d.h>
#include <lib_internal/task.h>
#include <lib_internal/sda.h>

//------------------------------------------------------------------------------

typedef uint16_t s_vge_p[2];

typedef struct vge_var_color
{
    uint32_t    type;
    uint32_t    hash;
    char        name[32];
    
    uint32_t    color;
    
}   s_vge_var_color;

typedef struct vge_var_param
{
    uint32_t    type;
    uint32_t    hash;
    char        name[32];
    
    float32_t   param;
    
}   s_vge_var_param;

typedef struct vge_var
{
    uint32_t    type;
    uint32_t    hash;
    char        name[32];
    
    s_var_color color;
    s_var_param param;
}

typedef struct vge_line
{
    
    s_vl3d_line *obj;
    
}   s_vge_line;

typedef struct vge
{
    s_vl3d      vl3d;
    s_vl3d_view view;
    
    uint32_t    *obj_sz;
    s_vl3d_obj  obj_ls[1024*16];
    
    uint32_t    var_sz;
    s_vge_var   var_ls[256];
    
}   s_vge;

typedef struct vge_attr
{
    uint32_t    temp;
    
}   s_vge_attr;

//------------------------------------------------------------------------------

inline void vge_init(s_vge *vge, s_vge_attr attr)
{
//    vge->obj_sz = vge->vl3d->
    
    vl3d_init(&vge->vl3d, (s_vl3d_attr) {
        .obj_ls = vge->obj_ls,
        .obj_sz = sizeof(vge->obj_ls) / sizeof(s_vge_obj)
    });
    
    vge->var_sz = 0x00;
    
    return;
}

//------------------------------------------------------------------------------

#endif /* __VGE__ */





