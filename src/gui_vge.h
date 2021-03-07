
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

inline void gui_vge_obj(s_vge *vge, s_vl3d_obj *obj)
{
	ImGui::PushID(obj);
//	ImGui::BeginGroup();
	
	char obj_label[65];
	
	if (obj->type == vl3d_obj_type_line) { sprintf(obj_label, "line     [%016llX]", (uint64_t) obj); }
	if (obj->type == vl3d_obj_type_trngl){ sprintf(obj_label, "triangle [%016llX]", (uint64_t) obj); }
	if (obj->type == vl3d_obj_type_text) { sprintf(obj_label, "text     [%016llX]", (uint64_t) obj); }
	if (obj->type == vl3d_obj_type_rect) { sprintf(obj_label, "rect     [%016llX]", (uint64_t) obj); }
	
	if (ImGui::TreeNodeEx(obj_label, ImGuiTreeNodeFlags_Leaf))
	{ ImGui::TreePop(); }
	
	if (ImGui::IsItemClicked()) { vge_sel_obj(vge, obj); }
//
//	ImGui::EndGroup();
	
	if (ImGui::BeginPopupContextItem("var_menu"))
	{
		if (ImGui::Selectable("delete"))
		{
//			if (gui->sel_item == &obj->data_list[j])
//			{ gui->sel_item = NULL; }
			
//			vl3d_rem(vge, var);
		}
		
		ImGui::EndPopup();
	}
	
	ImGui::PopID();
	
	return;
}

inline bool __gui_vge_objctrl_ib__(const char *label, ImVec2 size, ImVec2 pos)
{
	ImVec2 cpos = ImGui::GetCursorScreenPos();
	
	size.x = abs(size.x);
	size.y = abs(size.y);
	
	ImGui::SetCursorScreenPos(pos);
	bool ret_value = ImGui::InvisibleButton(label, size);
	
	ImGui::SetCursorScreenPos(cpos);
	
	return ret_value;
}

inline void __gui_vge_objctrl_ctrlp__(s_vge *vge, vlf_t pos[3])
{
	vlf_t e0[3] = { vge->vl3d_view.rot[0][0], vge->vl3d_view.rot[1][0], vge->vl3d_view.rot[2][0] };
	vlf_t e1[3] = { vge->vl3d_view.rot[0][1], vge->vl3d_view.rot[1][1], vge->vl3d_view.rot[2][1] };
	vlf_t e2[3] = { vge->vl3d_view.rot[0][2], vge->vl3d_view.rot[1][2], vge->vl3d_view.rot[2][2] };
	
	ImGui::PushID(pos);
	
	ImGuiWindow* window = ImGui::GetCurrentWindow();
	ImGuiIO &io = ImGui::GetIO();
	ImVec2 lpos = vl3d_view_tf(&vge->vl3d_view, pos);
	
	window->DrawList->AddCircle(lpos, 5.0, vl3d_col_legacy);
	
	__gui_vge_objctrl_ib__("ctrl_p", ImVec2(16,16), lpos - ImVec2(8,8));
	
	if (ImGui::IsItemHovered())
	{ window->DrawList->AddCircleFilled(lpos, 3.0, vl3d_col_legacy); }
	
	if (ImGui::IsItemActive())
	{ window->DrawList->AddCircleFilled(lpos, 3.0, vl3d_col_d); }
	
	if (ImGui::IsItemActive() && ImGui::IsMouseDragging(ImGuiMouseButton_Left))
	{
		vl_vsumm(pos, pos, e1, - 2 * io.MouseDelta.y / vge->vl3d_view.window_rect.GetHeight() / vge->vl3d_view.scale);
		vl_vsumm(pos, pos, e2, + 2 * io.MouseDelta.x / vge->vl3d_view.window_rect.GetHeight() / vge->vl3d_view.scale);
	}
	
	if (ImGui::IsMouseReleased(ImGuiMouseButton_Left))
	{
		vl_vround(pos, pos);
	}
	
	ImGui::PopID();
}

inline void gui_vge_objctrl(s_vge *vge, s_vl3d_obj *obj)
{
	ImGui::PushID(obj);
	
	ImGuiWindow* window = ImGui::GetCurrentWindow();
	
	char obj_label[65];
	
	if (obj->type == vl3d_obj_type_line)
	{
		sprintf(obj_label, "line [%016llX]", (uint64_t) obj);
		
		ImVec2 p0 = vl3d_view_tf(&vge->vl3d_view, obj->line.p0);
		ImVec2 p1 = vl3d_view_tf(&vge->vl3d_view, obj->line.p1);
		
		window->DrawList->AddText  (p0, vl3d_col_legacy, obj_label);
		
		__gui_vge_objctrl_ctrlp__(vge, obj->line.p0);
		__gui_vge_objctrl_ctrlp__(vge, obj->line.p1);
	}
	
	if (obj->type == vl3d_obj_type_trngl)
	{
		sprintf(obj_label, "trianlge [%016llX]", (uint64_t) obj);
		
		ImVec2 p0 = vl3d_view_tf(&vge->vl3d_view, obj->trngl.p0);
		ImVec2 p1 = vl3d_view_tf(&vge->vl3d_view, obj->trngl.p1);
		ImVec2 p2 = vl3d_view_tf(&vge->vl3d_view, obj->trngl.p2);
		
		window->DrawList->AddText  (p0, vl3d_col_legacy, obj_label);
		
		__gui_vge_objctrl_ctrlp__(vge, obj->trngl.p0);
		__gui_vge_objctrl_ctrlp__(vge, obj->trngl.p1);
		__gui_vge_objctrl_ctrlp__(vge, obj->trngl.p2);
	}
	
	if (obj->type == vl3d_obj_type_rect)
	{
		sprintf(obj_label, "rectangle [%016llX]", (uint64_t) obj);
		
		ImVec2 p0 = vl3d_view_tf(&vge->vl3d_view, obj->rect.p0);
		ImVec2 p1 = vl3d_view_tf(&vge->vl3d_view, obj->rect.p1);
		ImVec2 p2 = vl3d_view_tf(&vge->vl3d_view, obj->rect.p2);
		ImVec2 p3 = vl3d_view_tf(&vge->vl3d_view, obj->rect.p3);
		
		window->DrawList->AddText  (p0, vl3d_col_legacy, obj_label);
		
		__gui_vge_objctrl_ctrlp__(vge, obj->rect.p0);
		__gui_vge_objctrl_ctrlp__(vge, obj->rect.p1);
		__gui_vge_objctrl_ctrlp__(vge, obj->rect.p2);
		__gui_vge_objctrl_ctrlp__(vge, obj->rect.p3);
	}
	
	if (obj->type == vl3d_obj_type_text)
	{
		sprintf(obj_label, "text [%016llX]", (uint64_t) obj);
		
		window->DrawList->AddText  (vl3d_view_tf(&vge->vl3d_view, obj->text.p0), vl3d_col_legacy, obj_label);

		__gui_vge_objctrl_ctrlp__(vge, obj->text.p0);
	}
	
//	if (obj->type == vl3d_obj_type_trngl){ sprintf(obj_label, "triangle [%016llX]", (uint64_t) obj); }
//	if (obj->type == vl3d_obj_type_text) { sprintf(obj_label, "text     [%016llX]", (uint64_t) obj); }
	
//	if (ImGui::BeginPopupContextItem("var_menu"))
//	{
//		if (ImGui::Selectable("delete"))
//		{
////			if (gui->sel_item == &obj->data_list[j])
////			{ gui->sel_item = NULL; }
//
////			vl3d_rem(vge, var);
//		}
//
//		ImGui::EndPopup();
//	}
//
	ImGui::PopID();
	
	return;
}


inline void gui_vge_objsel(s_vge *vge, s_vl3d_obj *obj)
{
	ImGui::PushID(obj);
	
	if (obj->type == vl3d_obj_type_line)
	{
		ImVec2 p0 = vl3d_view_tf(&vge->vl3d_view, obj->line.p0);
		ImVec2 p1 = vl3d_view_tf(&vge->vl3d_view, obj->line.p1);
		
		ImVec2 p = ImVec2(min(p0.x,p1.x), min(p0.y,p1.y));
		
		if (__gui_vge_objctrl_ib__("##ctrl_box",
								   p1-p0 + ImVec2(8,8), p-ImVec2(4,4)))
		{ vge_sel_obj(vge, obj); }
	}
	
	if (obj->type == vl3d_obj_type_trngl)
	{
		ImVec2 p0 = vl3d_view_tf(&vge->vl3d_view, obj->trngl.p0);
		ImVec2 p1 = vl3d_view_tf(&vge->vl3d_view, obj->trngl.p1);
		ImVec2 p2 = vl3d_view_tf(&vge->vl3d_view, obj->trngl.p2);
		
		ImVec2 p = ImVec2(min(p0.x,p1.x), min(p0.y,p1.y));
		
		if (__gui_vge_objctrl_ib__("##ctrl_box",
								   p1-p0 + ImVec2(8,8), p-ImVec2(4,4)))
		{ vge_sel_obj(vge, obj); }
	}
	
	if (obj->type == vl3d_obj_type_rect)
	{
		ImVec2 p0 = vl3d_view_tf(&vge->vl3d_view, obj->rect.p0);
		ImVec2 p1 = vl3d_view_tf(&vge->vl3d_view, obj->rect.p1);
		ImVec2 p2 = vl3d_view_tf(&vge->vl3d_view, obj->rect.p2);
		ImVec2 p3 = vl3d_view_tf(&vge->vl3d_view, obj->rect.p3);
		
		ImVec2 p = ImVec2(min(p0.x,p1.x), min(p0.y,p1.y));
		
		if (__gui_vge_objctrl_ib__("##ctrl_box",
								   p1-p0 + ImVec2(8,8), p-ImVec2(4,4)))
		{ vge_sel_obj(vge, obj); }
	}
	
	if (obj->type == vl3d_obj_type_text)
	{
		ImVec2 p0 = vl3d_view_tf(&vge->vl3d_view, obj->text.p0);
		
		if (__gui_vge_objctrl_ib__("##ctrl_box",
								   ImVec2(8,8), p0-ImVec2(4,4)))
		{ vge_sel_obj(vge, obj); }
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
		
		const s_vl3d_line  default_line  = { .flags = vl3d_obj_flags_none, .color = vl3d_col_legacy, .p0 = {0,0,-1}, .p1 = {0,0,1} };
		const s_vl3d_trngl default_trngl = { .flags = vl3d_obj_flags_none, .color = vl3d_col_legacy, .p0 = {1,0,0}, .p1 = {0,0,-1}, .p2 = {0,0,+1} };
		const s_vl3d_rect  default_rect  = { .flags = vl3d_obj_flags_none, .color = vl3d_col_legacy, .p0 = {0,0,0}, .p1 = {1,0,0}, .p2 = {1,0,1}, .p3 = {0,0,1} };
		const s_vl3d_text  default_text  = { .flags = vl3d_obj_flags_none, .color = vl3d_col_l 	 , .p0 = {0,0,0}, };
		
		if (ImGui::BeginPopup("vge_objects_add"))
		{
			if (ImGui::MenuItem("LINE    ")) { vl3d_add_lineat (&vge->vl3d, default_line , vge->vl3d_view.pos); }
			if (ImGui::MenuItem("TRIANGLE")) { vl3d_add_trnglat(&vge->vl3d, default_trngl, vge->vl3d_view.pos); }
			if (ImGui::MenuItem("TEXT    ")) { vl3d_add_textat (&vge->vl3d, default_text , vge->vl3d_view.pos); }
			if (ImGui::MenuItem("RECT    ")) { vl3d_add_rectat (&vge->vl3d, default_rect , vge->vl3d_view.pos); }
			
			ImGui::EndPopup();
		}
		
		ImGui::PopStyleVar(1);
		
		for (i = 0; i < vge->vl3d.obj_of; ++i)
		{
			if (!(vge->vl3d.obj_ls[i].flags & vl3d_obj_flags_spec)
			&& !(vge->vl3d.obj_ls[i].flags & vl3d_obj_flags_ignore))
			{
				gui_vge_obj(vge, &vge->vl3d.obj_ls[i]);
			}
		}
		
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
		
		vl_vfloor(vge->vl3d_gridline.pos, vge->vl3d_view.pos);
		vl3d_gridfline2d(&vge->vl3d, &vge->vl3d_gridline);
	}
	
	vl3d_view_show(&vge->vl3d, &vge->vl3d_view);
	
	// Interactive overflay
	{
		for (int i = 0; i < vge->vl3d.obj_of; ++i)
		{
			if (!(vge->vl3d.obj_ls[i].flags & vl3d_obj_flags_spec)
				&& !(vge->vl3d.obj_ls[i].flags & vl3d_obj_flags_ignore))
			{
				if (vge->sel_item != &vge->vl3d.obj_ls[i])
				{
					gui_vge_objsel(vge, &vge->vl3d.obj_ls[i]);
				}
			}
		}
		
		if (vge->sel_type == VGE_SEL_OBJ && vge->sel_item != NULL)
		{
			gui_vge_objctrl(vge, (s_vl3d_obj*) vge->sel_item);
		}
	}
	
	vl3d_end();
	
	return;
}

//----------------------------------------------------------------

#endif /* __GUI_W__ */


