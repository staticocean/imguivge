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
    
    s_vl3d_rect			frame;
    
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

inline void vge_render(s_vge *vge, char *proj_path)
{
	char *proj_name = proj_path + strlen(proj_path);
	while (proj_name > proj_path && *(proj_name-1) != '\\' && *(proj_name-1) != '/')
	{ --proj_name; }
	
	char name[64];
	char *name_ = name;
	while (*proj_name != '.') { *name_ = *proj_name; ++proj_name; ++name_; }
	*name_ = '\0';
	
	char file_path[256];
	strcpy(file_path, proj_path);
	char *file_path_ = file_path + strlen(file_path);
	while (*file_path_ != '.') { --file_path_; }
	file_path_[1] = 'h';
	file_path_[2] = '\0';
	
	vlf_t bound_min[3];
	vlf_t bound_max[3];
	
	vl3d_rect_min(&vge->frame, bound_min);
	vl3d_rect_max(&vge->frame, bound_max);
	
	ImVec2 bound_min_ = vl3d_view_tf(&vge->vl3d_view, bound_min);
	ImVec2 bound_max_ = vl3d_view_tf(&vge->vl3d_view, bound_max);
	
	std::swap(bound_min_.y, bound_max_.y);
	
	FILE *file_handle = fopen(file_path, "w+");
	
	fprintf(file_handle, "#ifndef __%s__ \n", name);
	fprintf(file_handle, "#define __%s__ \n", name);
	
	fprintf(file_handle, "typedef struct %s { \n", name);
	fprintf(file_handle, "} s_%s; \n", name);
	
	fprintf(file_handle, "inline void %s_draw(s_%s *%s) { \n", name, name, name);
	fprintf(file_handle, "ImGuiWindow* window = ImGui::GetCurrentWindow(); \n");
	fprintf(file_handle, "ImRect window_rect = window->InnerRect; \n");
	fprintf(file_handle, "ImVec2 window_size = window_rect.Max - window_rect.Min; \n");
	fprintf(file_handle, "float  scale       = window_size.x / %f; \n", (bound_max_ - bound_min_).x);
	
	for (int i = 0; i < vge->vl3d.obj_of; ++i)
	{
		s_vl3d_obj *obj = &vge->vl3d.obj_ls[i];
		
		if ((obj->flags & vl3d_obj_flags_ignore)
		|| (obj->flags & vl3d_obj_flags_spec))
		{
			continue;
		}
		
		if (obj->type == vl3d_obj_type_line)
		{
			ImVec2 p0 = vl3d_view_tf(&vge->vl3d_view, obj->line.p0) - bound_min_;
			ImVec2 p1 = vl3d_view_tf(&vge->vl3d_view, obj->line.p1) - bound_min_;
			
			fprintf(file_handle, "window->DrawList->AddLine(window_rect.Min+ImVec2(%f,%f)*scale, window_rect.Min+ImVec2(%f,%f)*scale, %u); \n",
					p0.x, p0.y, p1.x, p1.y, obj->color);
		}
		
		if (obj->type == vl3d_obj_type_text)
		{
			ImVec2 p0 = vl3d_view_tf(&vge->vl3d_view, obj->text.p0) - bound_min_;
			
			fprintf(file_handle, "window->DrawList->AddText(window_rect.Min+ImVec2(%f,%f)*scale, %u, \"%s\"); \n",
					p0.x, p0.y, obj->color, obj->text.data);
		}
		
		if (obj->type == vl3d_obj_type_rect)
		{
			ImVec2 p0 = vl3d_view_tf(&vge->vl3d_view, obj->rect.p0) - bound_min_;
			ImVec2 p1 = vl3d_view_tf(&vge->vl3d_view, obj->rect.p1) - bound_min_;
			ImVec2 p2 = vl3d_view_tf(&vge->vl3d_view, obj->rect.p2) - bound_min_;
			ImVec2 p3 = vl3d_view_tf(&vge->vl3d_view, obj->rect.p3) - bound_min_;
			
			fprintf(file_handle, "window->DrawList->AddTriangleFilled(window_rect.Min+ImVec2(%f,%f)*scale, window_rect.Min+ImVec2(%f,%f)*scale, window_rect.Min+ImVec2(%f,%f)*scale, %u); \n",
					p0.x, p0.y, p1.x, p1.y, p2.x, p2.y, obj->color);
			
			fprintf(file_handle, "window->DrawList->AddTriangleFilled(window_rect.Min+ImVec2(%f,%f)*scale, window_rect.Min+ImVec2(%f,%f)*scale, window_rect.Min+ImVec2(%f,%f)*scale, %u); \n",
					p2.x, p2.y, p3.x, p3.y, p0.x, p0.y, obj->color);
		}
	}
	
	fprintf(file_handle, "} \n");
	
	fprintf(file_handle, "#endif \n");
	
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





