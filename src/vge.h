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

#define VGE_TYPE_NONE  	0x00
#define VGE_TYPE_COLOR 	0x01
#define VGE_TYPE_VALUE	0x02

#define VGE_SEL_NONE	0x00
#define VGE_SEL_OBJ  	0x01

error_(vge_vof, "[vge] var list overflow");

//------------------------------------------------------------------------------

typedef struct vge_var_color
{
    uint32_t    	type;
    uint32_t    	hash;
    char        	name[32];
    
    uint32_t    	data;
    
}   s_vge_var_color;

typedef struct vge_var_param
{
    uint32_t    	type;
    uint32_t    	hash;
    char        	name[32];
    
    int32_t   		data;
    
}   s_vge_var_value;

typedef struct vge_var
{
    uint32_t    	type;
    uint32_t    	hash;
    char        	name[32];
    
    s_vge_var_color color;
	s_vge_var_value value;
    
} 	s_vge_var;

typedef struct vge_line
{
    s_vl3d_line 	*obj;
    
}   s_vge_line;

typedef struct vge
{
    s_vl3d      		vl3d;
	s_vl3d_view 		vl3d_view;
	s_vl3d_tbar 		vl3d_tbar;
	s_vl3d_gridfline2d 	vl3d_gridline;
	s_vl3d_xyz2d		vl3d_xyz;
	
    s_vl3d_obj  		obj_ls[UINT16_MAX];
	
	uint32_t    		var_of;
	uint32_t    		var_sz;
    s_vge_var   		var_ls[256];
    
    void 				*sel_item;
    uint8_t 			sel_type;
    
}   s_vge;

typedef struct vge_attr
{
    uint32_t    temp;
    
}   s_vge_attr;

//------------------------------------------------------------------------------

inline void vge_init(s_vge *vge, s_vge_attr attr)
{
	vge->sel_item = NULL;
	vge->sel_item = VGE_SEL_NONE;
	
	vge->var_sz = sizeof(vge->var_ls) / sizeof(s_vge_var);
	vge->var_of = 0x00;
	
	// Set default parameters here, bacuse view will not be saved/loaded
	// from imgui context, because it is kept in static vge struct
	vge->vl3d_view.scale = 1.0;
	vl_vset(vge->vl3d_view.pos, 0.0);
	vl_mid(&vge->vl3d_view.rot[0][0]);
	
	vl3d_init(&vge->vl3d, (s_vl3d_attr) {
		.obj_sz = sizeof(vge->obj_ls) / sizeof(s_vl3d_obj),
        .obj_ls = vge->obj_ls,
    });
    
	// also draw grid/xyz right now
	// coz objlist is static is will be kept
	
	vge->vl3d_gridline.range = vl3d_range_null;
	vl_vset(vge->vl3d_gridline.pos, 0.0);
	vge->vl3d_gridline.count = 1000;
	vge->vl3d_gridline.scale = 1.0;
	vl3d_gridfline2d(&vge->vl3d, &vge->vl3d_gridline);
	
	// XYZ init should be after gridfline2d
	// bacause XYZ needs to be drawn on top of grid
	// thus should be added last
	vge->vl3d_xyz.range = vl3d_range_null;
	vge->vl3d_xyz.scale = 1.0;
	vl_vset(vge->vl3d_xyz.pos, 0.0);
	
	vl3d_xyz2d(&vge->vl3d, &vge->vl3d_xyz);
	
    return;
}

//------------------------------------------------------------------------------

inline void vge_save(s_vge *vge, char *file_path)
{
	FILE *file_handle = fopen(file_path, "wb+");
	fwrite(vge, 1, sizeof(s_vge), file_handle);
	fclose(file_handle);
	
	return;
}

//------------------------------------------------------------------------------

inline void vge_load(s_vge *vge, char *file_path)
{
	FILE *file_handle = fopen(file_path, "rb");
	
	if (file_handle != NULL)
	{
		fread(vge, 1, sizeof(s_vge), file_handle);
		fclose(file_handle);
		
		// Fix pointers
		vge->sel_item = NULL;
		vge->sel_type = VGE_SEL_NONE;
		vge->vl3d.obj_ls = vge->obj_ls;
	}
	
	return;
}

//------------------------------------------------------------------------------

inline void vge_render(s_vge *vge, char *file_path)
{
	FILE *file_handle = fopen(file_path, "w+");
	
	char *file_name = file_path + strlen(file_path);
	while (file_name > file_path && *(file_name-1) != '\\' && *(file_name-1) != '/')
	{ --file_name; }
	
	fprintf(file_handle, "#ifndef __%s__", file_name);
	fprintf(file_handle, "#define __%s__", file_name);
	
	fprintf(file_handle, "typedef struct %s {", file_name);
	fprintf(file_handle, "} s_%s;", file_name);
	
	fprintf(file_handle, "inline void %s_draw(s_%s %s) {", file_name, file_name, file_name);
	fprintf(file_handle, "ImGuiWindow* window = ImGui::GetCurrentWindow();");
	
	fprintf(file_handle, "}");
	
	fprintf(file_handle, "#endif");
	
	fclose(file_handle);
	
	return;
}

//------------------------------------------------------------------------------

inline void vge_sel_none(s_vge *vge)
{
	vge->sel_item = NULL;
	vge->sel_type = VGE_SEL_NONE;
	
	return;
}

//------------------------------------------------------------------------------

inline void vge_sel_obj(s_vge *vge, s_vl3d_obj *obj)
{
	vge->sel_item = obj;
	vge->sel_type = VGE_SEL_OBJ;
	
	return;
}

//------------------------------------------------------------------------------

inline error_t vge_var_add_color(s_vge *vge, char *name, uint32_t color)
{
	if (vge->var_of < vge->var_sz)
	{
		vge->var_ls[vge->var_of].type = VGE_TYPE_COLOR;
		vge->var_ls[vge->var_of].hash = vl_crc32(name);
		strcpy(vge->var_ls[vge->var_of].name, name);
		vge->var_ls[vge->var_of].color.data = color;
		
		vge->var_of++;
		
		return error_none;
	}
	
	else
	{
		return error_vge_vof;
	}
	
	return error_ub;
}

//------------------------------------------------------------------------------

inline error_t vge_var_add_value(s_vge *vge, char *name, uint32_t value)
{
	if (vge->var_of < vge->var_sz)
	{
		vge->var_ls[vge->var_of].type = VGE_TYPE_VALUE;
		vge->var_ls[vge->var_of].hash = vl_crc32(name);
		strcpy(vge->var_ls[vge->var_of].name, name);
		vge->var_ls[vge->var_of].value.data = value;
		
		vge->var_of++;
		
		return error_none;
	}
	
	else
	{
		return error_vge_vof;
	}
	
	return error_ub;
}

//------------------------------------------------------------------------------

inline uint32_t vge_var_index(s_vge *vge, s_vge_var *var)
{
	uintptr_t offset = (uintptr_t) var - (uintptr_t) vge->var_ls;
	uint32_t index = (uint32_t) (offset / (uintptr_t) sizeof(s_vge_var));
	
	return index;
}

inline error_t vge_var_rem(s_vge *vge, s_vge_var *var)
{
	// TODO add vel (var empty list) error
	if (vge->var_of < 0x01) { return error_none; }
	
	uint32_t index = vge_var_index(vge, var);
	uint32_t i;
	
	for (i = index; i < vge->var_of-1; ++i)
	{
		vge->var_ls[i] = vge->var_ls[i+1];
	}
	
	vge->var_of--;
	
	return error_none;
}

//------------------------------------------------------------------------------

#endif /* __VGE__ */





