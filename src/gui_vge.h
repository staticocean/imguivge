
#ifndef __GUI_VGE__
#define __GUI_VGE__

//----------------------------------------------------------------

#include <lib/imgui/imgui.h>
#include <lib/imgui/imgui_internal.h>

#include <lib_internal/vl.h>
#include <lib_internal/vl3d.h>
#include <lib_internal/imgui_w.h>

#include "vge.h"

//----------------------------------------------------------------

inline void gui_vge_var(s_vge *vge, s_vge_var *var)
{
	ImGui::PushID(var);
	ImGui::BeginGroup();

	if (var->type == VGE_TYPE_COLOR)
	{
		ImVec4 col = ImGui::ColorConvertU32ToFloat4(var->value.data);
		ImGui::ColorEdit4("##value", (float*) &col, ImGuiColorEditFlags_NoInputs);
		var->value.data = ImGui::ColorConvertFloat4ToU32(col);

		ImGui::SameLine();
		
		ImGui::SetNextItemWidth(ImGui::GetContentRegionAvailWidth());
		ImGui::InputText("##name", var->name, 32);
		ImGui::SameLine();
	}
	
	if (var->type == VGE_TYPE_VALUE)
	{
		ImGui::SetNextItemWidth(200);
		ImGui::InputText("##name", var->name, 32);
		ImGui::SameLine();
		
		ImGui::SetNextItemWidth(ImGui::GetContentRegionAvailWidth());
		ImGui::DragInt("##value", &var->value.data);
		ImGui::SameLine();
	}
	
//	if (ImGui::Button("DEL",
//			ImVec2(ImGui::GetContentRegionAvailWidth(), 0)))
//	{
//		vge_var_rem(vge, var);
//	}
	
	ImGui::Dummy(ImVec2(0,0));
	
	ImGui::EndGroup();
	
	if (ImGui::BeginPopupContextItem("var_menu"))
	{
		if (ImGui::Selectable("delete"))
		{
//			if (gui->sel_item == &obj->data_list[j])
//			{ gui->sel_item = NULL; }
			
			vge_var_rem(vge, var);
		}
		
		ImGui::EndPopup();
	}
	
	ImGui::PopID();
	
	return;
}

inline void gui_vge_varlist(s_vge *vge)
{
	ImGui::PushID(vge->var_ls);
	
	uint32_t i;
	
	if (ImGui::CollapsingHeader("COLORS##vge_colors", ImGuiTreeNodeFlags_DefaultOpen))
	{
		if (ImGui::Button("ADD COLOR##vge_params_add",
				ImVec2(ImGui::GetContentRegionAvailWidth(), 0)))
		{
			vge_var_add_color(vge, "new_color", IM_COL32(0,0,0,255));
		}
		
//		uint8_t no_colors = 0x01;
		
		for (i = 0; i < vge->var_of; ++i)
		{
			if (vge->var_ls[i].type == VGE_TYPE_COLOR)
			{
//				no_colors = 0x00;
				gui_vge_var(vge, &vge->var_ls[i]);
			}
		}
		
//		if (no_colors != 0x00)
//		{ ImGui::Text("[no colors found]"); }
	}
	
	if (ImGui::CollapsingHeader("VALUES##vge_values", ImGuiTreeNodeFlags_DefaultOpen))
	{
		if (ImGui::Button("ADD VALUE##vge_values_add",
					  ImVec2(ImGui::GetContentRegionAvailWidth(), 0)))
		{
			vge_var_add_value(vge, "new_value", 0x00);
		}
		
//		uint8_t no_params = 0x01;
		
		for (i = 0; i < vge->var_of; ++i)
		{
			if (vge->var_ls[i].type == VGE_TYPE_VALUE)
			{
//				no_params = 0x00;
				gui_vge_var(vge, &vge->var_ls[i]);
			}
		}
		
//		if (no_params != 0x00)
//		{ ImGui::Text("[no values found]"); }
	}
	
	ImGui::PopID();
	
	return;
}

inline void gui_vge_objlist(s_vge *vge)
{
	ImGui::PushID(vge->var_ls);
	
	uint32_t i;
	
	if (ImGui::CollapsingHeader("OBJECTS##vge_colors", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImVec2 popup_pos = ImGui::GetCursorScreenPos() + ImVec2(0, ImGui::GetTextLineHeightWithSpacing());
		
		if (ImGui::Button("ADD OBJECT##vge_objects_add",
						  ImVec2(ImGui::GetContentRegionAvailWidth(), 0)))
		{ ImGui::OpenPopup("vge_objects_add"); }
		
		ImGui::SetNextWindowPos(popup_pos, ImGuiCond_Always);
		ImGui::SetNextWindowSize(ImVec2(200,0), ImGuiCond_Always);
		
		ImGui::GetStyle().DisplayWindowPadding = ImVec2(0,0);
		ImGui::GetStyle().DisplaySafeAreaPadding = ImVec2(0,0);
		
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(16, 8));
		
		const s_vl3d_line  default_line  = { .color = vl3d_col_legacy, .p0 = {0,0,-1}, .p1 = {0,0,1} };
		const s_vl3d_trngl default_trngl = { .color = vl3d_col_legacy, .p0 = {1,0,0}, .p1 = {0,0,-1}, .p2 = {0,0,+1} };
		const s_vl3d_text  default_text  = { .color = vl3d_col_l, .p0 = {0,0,0}, };
		
		if (ImGui::BeginPopup("vge_objects_add"))
		{
			if (ImGui::MenuItem("LINE    ")) {
				vl3d_add_line (&vge->vl3d, default_line );
			}
			if (ImGui::MenuItem("TRIANGLE")) { vl3d_add_trngl(&vge->vl3d, default_trngl); }
			if (ImGui::MenuItem("TEXT    ")) { vl3d_add_text (&vge->vl3d, default_text ); }
			
			ImGui::EndPopup();
		}
		
		ImGui::PopStyleVar(1);
		
		ImGui::SameLine();
	}
	
	ImGui::PopID();
	
	return;
}

//----------------------------------------------------------------

inline void gui_vge_canvas(s_vge *vge)
{
	vl3d_begin(&vge->vl3d);
	vl3d_tbar_show(&vge->vl3d, &vge->vl3d_tbar, &vge->vl3d_view);
	vl3d_view_ctrl2d(&vge->vl3d, &vge->vl3d_view);
	
	if (vge->vl3d_view.scale > 0.25) { vge->vl3d_view.scale = 0.25; }
	if (vge->vl3d_view.scale < 0.01) { vge->vl3d_view.scale = 0.01; }
	
	{
		vge->vl3d_xyz.scale = floor(0.25 / vge->vl3d_view.scale);
		if (vge->vl3d_xyz.scale < 1.0) { vge->vl3d_xyz.scale = 1.0; }
		
		vl3d_xyz2d(&vge->vl3d, &vge->vl3d_xyz);
	}
	{
		vl_vfloor(vge->vl3d_gridline.pos, vge->vl3d_view.pos);
		vl3d_gridfline2d(&vge->vl3d, &vge->vl3d_gridline);
	}
	
	vl3d_view_show(&vge->vl3d, &vge->vl3d_view);
	vl3d_end();
	
	return;
}

//----------------------------------------------------------------

#endif /* __GUI_W__ */


