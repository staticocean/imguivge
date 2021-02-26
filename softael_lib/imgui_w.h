
#ifndef __IMGUI_W__
#define __IMGUI_W__

//------------------------------------------------------------------------------

#define IMGUI_DEFINE_MATH_OPERATORS
#include <lib/imgui/imgui.h>

#include <softael_lib/vl.h>
#include <softael_lib/vl3d.h>

//------------------------------------------------------------------------------

inline void imgui_mat_get(void *ptr, vlf_t *mat, vlf_t *def)
{
	ImGuiWindow* window = ImGui::GetCurrentWindow();
	
	s_vl_hpr def_hpr;
	
	vl_hpr(&def_hpr, def);
	
	s_vl_hpr hpr = {
//			.heading = window->StateStorage.GetFloat(ImGui::GetID((void*) ((uintptr_t) ptr + 0x00)), vl_rad(45)),
//			.pitch 	 = window->StateStorage.GetFloat(ImGui::GetID((void*) ((uintptr_t) ptr + 0x01)), vl_rad(45)),
//			.roll    = window->StateStorage.GetFloat(ImGui::GetID((void*) ((uintptr_t) ptr + 0x02)), vl_rad( 0)),
			.heading = window->StateStorage.GetFloat(ImGui::GetID((void*) ((uintptr_t) ptr + 0x00)), def_hpr.heading),
			.pitch 	 = window->StateStorage.GetFloat(ImGui::GetID((void*) ((uintptr_t) ptr + 0x01)), def_hpr.pitch),
			.roll    = window->StateStorage.GetFloat(ImGui::GetID((void*) ((uintptr_t) ptr + 0x02)), def_hpr.roll),
	};
	
	vl_rot(mat, hpr);
	
	return;
}

//------------------------------------------------------------------------------

inline void imgui_mat_set(void *ptr, vlf_t *mat)
{
	ImGuiWindow* window = ImGui::GetCurrentWindow();
	
	s_vl_hpr hpr;
	
	vl_hpr(&hpr, mat);
	
	window->StateStorage.SetFloat(ImGui::GetID((void*) ((uintptr_t) ptr + 0x00)), (float) hpr.heading);
	window->StateStorage.SetFloat(ImGui::GetID((void*) ((uintptr_t) ptr + 0x01)), (float) hpr.pitch);
	window->StateStorage.SetFloat(ImGui::GetID((void*) ((uintptr_t) ptr + 0x02)), (float) hpr.roll);
	
	return;
}

//------------------------------------------------------------------------------

inline void imgui_vec(char *label, vlf_t *vec, float v_speed, vlf_t *min, vlf_t *max, char *format)
{
	ImGui::SetNextItemWidth(ImGui::GetContentRegionAvailWidth());
	ImGui::DragScalarN(label, ImGuiDataType_Double, vec, 3, v_speed, min, max, format);
	
	return;
}

//------------------------------------------------------------------------------

inline void imgui_hpr(char *label, s_vl_hpr *hpr, float v_speed, char *format)
{
	static vlf_t heading_min = 0.0;
	static vlf_t heading_max = 360.0;
	
	static vlf_t pitch_min = -90.0;
	static vlf_t pitch_max = +90.0;
	
	static vlf_t roll_min = -180.0;
	static vlf_t roll_max = +180.0;
	
	s_vl_hpr hpr_deg;
	
	hpr_deg.heading = vl_deg(hpr->heading);
	hpr_deg.pitch   = vl_deg(hpr->pitch);
	hpr_deg.roll    = vl_deg(hpr->roll);

//	vlf_t hpr_deg[3] = {
//			(float) vl_deg(hpr->heading),
//			(float) vl_deg(hpr->pitch),
//			(float) vl_deg(hpr->roll)
//	};
//
	float item_width = ImGui::GetContentRegionAvailWidth() / 3 - 2.0/3.0 * ImGui::GetStyle().ItemInnerSpacing.x;
	
	ImGui::PushID(label);

//	vl_gui_vec("##hpr", hpr_deg, 1.0, NULL, NULL, "%.0f");
	
	ImGui::SetNextItemWidth(item_width);
	ImGui::DragScalar("##heading", ImGuiDataType_Double, &hpr_deg.heading, v_speed, &heading_min, &heading_max, format);
	if (ImGui::IsItemHovered()) { ImGui::SetTooltip("heading [deg]"); }
	ImGui::SameLine(1*item_width+ImGui::GetStyle().ItemInnerSpacing.x, 0.0);
	ImGui::SetNextItemWidth(item_width);
	ImGui::DragScalar("##pitch", ImGuiDataType_Double, &hpr_deg.pitch  , v_speed, &pitch_min, &pitch_max, format);
	if (ImGui::IsItemHovered()) { ImGui::SetTooltip("pitch [deg]"); }
	ImGui::SameLine(2*(item_width+ImGui::GetStyle().ItemInnerSpacing.x), 0.0);
	ImGui::SetNextItemWidth(item_width);
	ImGui::DragScalar("##roll", ImGuiDataType_Double, &hpr_deg.roll   , v_speed, &pitch_min, &pitch_max, format);
	if (ImGui::IsItemHovered()) { ImGui::SetTooltip("roll [deg]"); }
	
	ImGui::PopID();
	
	hpr->heading = vl_rad(hpr_deg.heading);
	hpr->pitch   = vl_rad(hpr_deg.pitch);
	hpr->roll    = vl_rad(hpr_deg.roll);

//	hpr->heading = vl_rad(hpr_deg[0]);
//	hpr->pitch   = vl_rad(hpr_deg[1]);
//	hpr->roll    = vl_rad(hpr_deg[2]);
//
	return;
}

//------------------------------------------------------------------------------

inline void imgui_mat(char *label, vlf_t *mat, float v_speed, vlf_t *min, vlf_t *max, char *format)
{
	ImGui::PushID(label);
	
	ImGuiWindow* window = ImGui::GetCurrentWindow();
	
	ImGui::BeginGroup();
	
	for (int i = 0; i < 3; ++i)
	{
		ImGui::PushID(i);
		ImGui::SetNextItemWidth(ImGui::GetContentRegionAvailWidth());
		ImGui::DragScalarN("##mat_view", ImGuiDataType_Double, &mat[3 * i], 3, v_speed, min, max, format);
		ImGui::PopID();
	}
	
	ImGui::EndGroup();
	
	ImGui::PopID();
	
	return;
}

//------------------------------------------------------------------------------

inline void imgui_rot(char *label, vlf_t *mat)
{
	enum st
	{
		st_mode = 100,
		st_heading,
		st_pitch,
		st_roll,
		st_scale
	};
	
	ImGui::PushID(label);
	
	ImGuiWindow* window = ImGui::GetCurrentWindow();
	
	char st_id_mode[32];
	char st_id_heading[32];
	char st_id_pitch[32];
	char st_id_roll[32];
	char st_id_scale[32];
	
	sprintf(st_id_mode, "%s%d", label, st_mode);
	
	int mode = window->StateStorage.GetInt(ImGui::GetID(st_id_mode), 0x00);
	
	ImGui::BeginGroup();
	
	switch (mode)
	{
		default:
		{
			s_vl3d_eng vl3d_eng;
			s_vl3d_obj vl3d_obj_list[32];
			s_vl3d_view vl3d_view;
			
			sprintf(st_id_heading, "%s%d", label, st_heading);
			sprintf(st_id_pitch, "%s%d", label, st_pitch);
			sprintf(st_id_roll, "%s%d", label, st_roll);
			sprintf(st_id_scale, "%s%d", label, st_scale);
			
			s_vl_hpr view_hpr;
			
			view_hpr.heading = window->StateStorage.GetFloat(ImGui::GetID(st_id_heading), vl_rad(45));
			view_hpr.pitch = window->StateStorage.GetFloat(ImGui::GetID(st_id_pitch), vl_rad(45));
			view_hpr.roll = window->StateStorage.GetFloat(ImGui::GetID(st_id_roll), vl_rad(0));
			
			vl_rot(&vl3d_view.rot[0][0], view_hpr);
			
			vl3d_view.scale = 0.75;
			vl3d_view.tbar_en = 0x00;
			vl_vzero(vl3d_view.pos);
			
			vl3d_eng_init(&vl3d_eng, (s_vl3d_eng_init) { .obj_list = vl3d_obj_list });
			
			vl3d_eng_draw_arrow(&vl3d_eng, vl3d_col_l,
								(float64_t[]) { -mat[0*3+0], -mat[1*3+0], -mat[2*3+0] },
								(float64_t[]) { +mat[0*3+0], +mat[1*3+0], +mat[2*3+0] }
			);
			
			vl3d_eng_draw_arrow(&vl3d_eng, vl3d_col_l,
								(float64_t[]) { -mat[0*3+1], -mat[1*3+1], -mat[2*3+1] },
								(float64_t[]) { +mat[0*3+1], +mat[1*3+1], +mat[2*3+1] }
			);
			
			vl3d_eng_draw_arrow(&vl3d_eng, vl3d_col_l,
								(float64_t[]) { -mat[0*3+2], -mat[1*3+2], -mat[2*3+2] },
								(float64_t[]) { +mat[0*3+2], +mat[1*3+2], +mat[2*3+2] }
			);

//			vl3d_eng_draw_arrow(&vl3d_eng,
//								(float64_t[]) { 0, 0, 0 },
//								(float64_t[]) { +mat[0*3+0], +mat[1*3+0], +mat[2*3+0] }
//			);
//
//			vl3d_eng_draw_arrow(&vl3d_eng,
//								(float64_t[]) { 0, 0, 0 },
//								(float64_t[]) { +mat[0*3+1], +mat[1*3+1], +mat[2*3+1] }
//			);
//
//			vl3d_eng_draw_arrow(&vl3d_eng,
//								(float64_t[]) { 0, 0, 0 },
//								(float64_t[]) { +mat[0*3+2], +mat[1*3+2], +mat[2*3+2] }
//			);
//
			vl3d_eng_draw_arrow(&vl3d_eng, vl3d_col_l, (float64_t[]) { -1.0, +0.0, +0.0 }, (float64_t[]) { +1.0, +0.0, +0.0 } );
			vl3d_eng_draw_arrow(&vl3d_eng, vl3d_col_l, (float64_t[]) { +0.0, -1.0, +0.0 }, (float64_t[]) { +0.0, +1.0, +0.0 } );
			vl3d_eng_draw_arrow(&vl3d_eng, vl3d_col_l, (float64_t[]) { +0.0, +0.0, -1.0 }, (float64_t[]) { +0.0, +0.0, +1.0 } );
			
			vl3d_eng_add_text(&vl3d_eng, (s_vl3d_text) { .color = vl3d_col_l, .p0 = { +1.0, +0.0, +0.0 }, .data = "X0" } );
			vl3d_eng_add_text(&vl3d_eng, (s_vl3d_text) { .color = vl3d_col_l, .p0 = { +0.0, +1.0, +0.0 }, .data = "Y0" } );
			vl3d_eng_add_text(&vl3d_eng, (s_vl3d_text) { .color = vl3d_col_l, .p0 = { +0.0, +0.0, +1.0 }, .data = "Z0" } );
			
			vl3d_eng_add_text(&vl3d_eng, (s_vl3d_text) { .color = vl3d_col_l, .p0 = { mat[0*3+0], mat[1*3+0], mat[2*3+0] }, .data = "X" } );
			vl3d_eng_add_text(&vl3d_eng, (s_vl3d_text) { .color = vl3d_col_l, .p0 = { mat[0*3+1], mat[1*3+1], mat[2*3+1] }, .data = "Y" } );
			vl3d_eng_add_text(&vl3d_eng, (s_vl3d_text) { .color = vl3d_col_l, .p0 = { mat[0*3+2], mat[1*3+2], mat[2*3+2] }, .data = "Z" } );
			
			vl3d_eng_add_line(&vl3d_eng, (s_vl3d_line) { .color = vl3d_col_l, .p0 = { +1.0, +0.0, +0.0 }, .p1 = { mat[0*3+0], mat[1*3+0], mat[2*3+0] } } );
			vl3d_eng_add_line(&vl3d_eng, (s_vl3d_line) { .color = vl3d_col_l, .p0 = { +0.0, +1.0, +0.0 }, .p1 = { mat[0*3+1], mat[1*3+1], mat[2*3+1] } } );
			vl3d_eng_add_line(&vl3d_eng, (s_vl3d_line) { .color = vl3d_col_l, .p0 = { +0.0, +0.0, +1.0 }, .p1 = { mat[0*3+2], mat[1*3+2], mat[2*3+2] } } );
			
			vl3d_eng_render(&vl3d_eng, &vl3d_view, "##mat_view",
							ImVec2(ImGui::GetContentRegionAvailWidth(),
								   ImGui::GetContentRegionAvailWidth()));
			
			vl_hpr(&view_hpr, &vl3d_view.rot[0][0]);
			
			window->StateStorage.SetFloat(ImGui::GetID(st_id_heading), (float) view_hpr.heading);
			window->StateStorage.SetFloat(ImGui::GetID(st_id_pitch), (float) view_hpr.pitch);
			window->StateStorage.SetFloat(ImGui::GetID(st_id_roll), (float) view_hpr.roll);
			window->StateStorage.SetFloat(ImGui::GetID(st_id_scale), (float) vl3d_view.scale);
			
			s_vl_hpr rot_hpr;
			
			vl_hpr(&rot_hpr, mat);
			
			imgui_hpr("##hpr", &rot_hpr, 1.0, "%.0f");
			
			vl_rot(mat, rot_hpr);
			
			break;
		}
		
		case 0x01:
		{
			static vlf_t min = -1;
			static vlf_t max = +1;
			
			imgui_mat(label, mat, 0.001, &min, &max, "%.001f");
			
			break;
		}
	}
	
	ImGui::EndGroup();
	
	if (ImGui::BeginPopupContextItem("item context menu"))
	{
		if (ImGui::Selectable("3d")) mode = 0x00;
		if (ImGui::Selectable("mat")) mode = 0x01;
		
		ImGui::EndPopup();
	}
	
	window->StateStorage.SetInt(ImGui::GetID(st_id_mode), mode);
	
	ImGui::PopID();
	
	return;
}

//------------------------------------------------------------------------------

inline bool imgui_hash(char *label, uint32_t hash)
{
	ImGui::PushID(label);
	
	ImGuiStyle style = ImGui::GetStyle();
	bool res = false;

	ImVec2 p = ImGui::GetCursorScreenPos();
	ImDrawList* draw_list = ImGui::GetWindowDrawList();
	
	float dy = ImGui::GetTextLineHeightWithSpacing() * 0.5;
	float dx = ImGui::GetContentRegionAvailWidth() / 16.0;
	
	float height = 2 * dy;
	float width  = 16 * dx;
	
	if (ImGui::InvisibleButton(label, ImVec2(width, height)))
	{ res = true; }
	
	uint32_t col_table[2] = {
			ImGui::ColorConvertFloat4ToU32(style.Colors[ImGuiCol_FrameBg]), vl3d_col_l
	};
	
	for (int y = 0; y < 2; ++y)
	{
		for (int x = 0; x < 16; ++x)
		{
			draw_list->AddRectFilled(
					ImVec2(p.x+x*dx, p.y+y*dy),
					ImVec2(p.x+(x+1)*dx, p.y+(y+1)*dy),
					col_table[(hash & (1 << (y*16+x))) > 0x00]
					);
		}
	}
	
	ImGui::PopID();
	
	return res;
}

//------------------------------------------------------------------------------

inline void imgui_bool(char *label, ImVec2 size, uint8_t *data)
{
//	ImGuiStyle& style = ImGui::GetStyle();
//	ImU32 col_text_u32 = ImGui::ColorConvertFloat4ToU32(style.Colors[ImGuiCol_Text]);
//	ImU32 col_textdis_u32 = ImGui::ColorConvertFloat4ToU32(style.Colors[ImGuiCol_TextDisabled]);
//	ImU32 col_button_u32 = ImGui::ColorConvertFloat4ToU32(style.Colors[ImGuiCol_Button]);
//
//	ImVec2 p = ImGui::GetCursorScreenPos();
//	ImDrawList* draw_list = ImGui::GetWindowDrawList();
//
//	float height = ImGui::GetFrameHeight();
//	float width = height * 2.0f;
//
//	if (ImGui::InvisibleButton(label, ImVec2(width, height)))
//	{
//		*data = (!*data) & 0x01;
//	}
//
//	switch (*data)
//	{
//		case 0x00:
//		{
//			draw_list->AddRect(ImVec2(p.x, p.y), ImVec2(p.x+width, p.y+height), col_text_u32);
//			draw_list->AddText(ImVec2(p.x, p.y), col_text_u32, "OFF");
//
//			break;
//		}
//
//		default:
//		{
//			draw_list->AddRectFilled(ImVec2(p.x, p.y), ImVec2(p.x+width, p.y+height), col_button_u32);
//			draw_list->AddText(ImVec2(p.x, p.y), col_text_u32, "ON");
//
//			break;
//		}
//	}
//
	ImGui::PushID(label);
	
	if (ImGui::Button((*data == 0x00) ? "OFF" : "ON", size))
	{ *data = (!*data) & 0x01; }
	
	ImGui::PopID();

//	ImU32 col_bg;
//
//	if (ImGui::IsItemHovered())
//		col_bg = *data ? IM_COL32(145+20, 211, 68+20, 255) : IM_COL32(218-20, 218-20, 218-20, 255);
//	else
//		col_bg = *data ? IM_COL32(145, 211, 68, 255) : IM_COL32(218, 218, 218, 255);
//
//	draw_list->AddRectFilled(p, ImVec2(p.x + width, p.y + height), col_bg);
//	draw_list->AddRectFilled(ImVec2(*data ? (p.x + width) : (p.x), p.y), ImVec2(*data ? (p.x + width) : (p.x), p.y), IM_COL32(255, 255, 255, 255));
	
	return;
}

//------------------------------------------------------------------------------

inline void imgui_switch(char *label, char **labels, ImVec2 size, uint8_t *data, int flags = ImGuiButtonFlags_None)
{
	ImGui::PushID(label);
	
	if (*data != 0x00) { *data = 0x01; }
	
	if (ImGui::ButtonEx(labels[*data], size, flags))
	{ *data = (*data != 0x00) ? 0x00 : 0x01; }
	
	if (*data != 0x00) { *data = 0x01; }
	
	ImGui::PopID();
	
	return;
}

inline void imgui_switchbox(char *label, char **labels, ImVec2 size, uint8_t *data)
{
	ImGui::PushID(label);

	ImGuiStyle& style = ImGui::GetStyle();
	ImU32 col_text_u32 = ImGui::ColorConvertFloat4ToU32(style.Colors[ImGuiCol_Text]);
	ImU32 col_textdis_u32 = ImGui::ColorConvertFloat4ToU32(style.Colors[ImGuiCol_TextDisabled]);
	ImU32 col_button_u32 = ImGui::ColorConvertFloat4ToU32(style.Colors[ImGuiCol_Button]);

	ImVec2 p = ImGui::GetCursorScreenPos();
	ImDrawList* draw_list = ImGui::GetWindowDrawList();

	float height = ImGui::GetFrameHeight();
	float width = size.x;

//	if (ImGui::InvisibleButton(label, ImVec2(width, height)))
//	{
//		*data = (!*data) & 0x01;
//	}

	switch (*data)
	{
		case 0x00:
		{
			draw_list->AddRect(ImVec2(p.x, p.y), ImVec2(p.x+width, p.y+height), col_button_u32);

			float font_size = ImGui::GetFontSize() * strlen(labels[0x00]) / 2;
			ImGui::SameLine(width / 2 - font_size + (font_size / 2));
			ImGui::Text(labels[0x00]);

//			draw_list->AddText(ImVec2(p.x, p.y), col_text_u32, "OFF");
			
			break;
		}

		default:
		{
			draw_list->AddRectFilled(ImVec2(p.x, p.y), ImVec2(p.x+width, p.y+height), col_button_u32);
			
			float font_size = ImGui::GetFontSize() * strlen(labels[0x00]) / 2;
			ImGui::SameLine(width / 2 - font_size + (font_size / 2));
			ImGui::Text(labels[0x01]);
			
//			draw_list->AddText(ImVec2(p.x, p.y), col_text_u32, "ON");

			break;
		}
	}
	
	ImGui::SetCursorScreenPos(p + ImVec2(width, 0));
	
	ImGui::PopID();

	return;
}

//------------------------------------------------------------------------------

inline void __imgui_lla_elem__ (char *label, char **nswe, float64_t *range, float64_t *value, float width)
{
    ImGuiWindow *window = ImGui::GetCurrentWindow();

    char mode_str[64];
    sprintf(mode_str, "##%s_%s", label, "mode");
    ImGuiID mode_id = ImGui::GetID(mode_str);

    uint8_t mode = window->StateStorage.GetInt(mode_id, 0x00);

    // remove sign life is easier that way
    // we MUST AND WILL resore it in the end
    uint8_t nswe_sign = (*value < 0.0) ? 0x00 : 0x01;
    *value = fabs(*value);

    float64_t value_deg = vl_deg(*value);
    int dms_d = (int) value_deg;
    int dms_m = (int) ((value_deg - dms_d) * 60);
    float64_t dms_s = (value_deg - dms_d - dms_m / (float64_t) 60.0) * 3600;

    ImGui::PushID(label);
    ImGui::BeginGroup();

//    vl_gui_switch("##d/dms", "Г", "ГМС", ImVec2(50, 0), &mode);
//    ImGui::SameLine();

    ImGui::SetNextItemWidth(width-40);

    switch (mode)
    {
        case 0x00:
        {
            char dms_str[32];
            sprintf(dms_str, "% 3d°%02d'%06.3f\"", dms_d, dms_m, dms_s);
            ImGui::InputText("##dms", dms_str, sizeof(dms_str));
            sscanf(dms_str, "%d°%d'%lf\"", &dms_d, &dms_m, &dms_s);

            dms_s = (dms_s <   0.0) ?  0.0 : dms_s;
            dms_s = (dms_s >= 60.0) ? 60.0 : dms_s;

            dms_m = (dms_m <   0.0) ?  0.0 : dms_m;
            dms_m = (dms_m >= 60.0) ? 60.0 : dms_m;

            float64_t value_new = vl_rad(
                    (float64_t) dms_d + (float64_t) dms_m / 60.0 + (float64_t) dms_s / 3600.0);

            *value = value_new;

            break;
        }

        default:
        {
            float64_t value_new;

            char deg_str[32];
            sprintf(deg_str, "% 10.6f°", value_deg);
            ImGui::InputText("##deg", deg_str, sizeof(deg_str));
            sscanf(deg_str, "%lf°", &value_new);

            value_new = vl_rad(value_new);

            *value = value_new;

            break;
        }
    }

    ImGui::SameLine(0,0);
	imgui_switch("##nswe", nswe, ImVec2(40, 0), &nswe_sign);

    *value = (*value < range[0]) ? range[0] : *value;
    *value = (*value > range[1]) ? range[1] : *value;

    *value *= (nswe_sign == 0x00) ? -1.0 : +1.0;

    ImGui::EndGroup();

    if (ImGui::BeginPopupContextItem("##mode"))
    {
        if (ImGui::Selectable("DEG°MIN'SEC\"")) { mode = 0x00; }
        if (ImGui::Selectable("DEG°")) { mode = 0x01; }

        ImGui::EndPopup();
    }

    ImGui::PopID();

    window->StateStorage.SetInt(mode_id, mode);

    return;
}

inline void imgui_lat(char *label, float64_t *lat, float width = 160)
{
    __imgui_lla_elem__(label, (char*[2]){ "S", "N" },
                     (float64_t[2]) { vl_rad(-90), vl_rad(+90) }, lat, width);

    return;
}

inline void imgui_lon(char *label, float64_t *lon, float width = 160)
{
    __imgui_lla_elem__(label, (char*[2]){ "W", "E" },
                     (float64_t[2]) { vl_rad(-180), vl_rad(+180) }, lon, width);

    return;
}

//------------------------------------------------------------------------------

typedef struct
{
	int day;
	int month;
	int year;
	
	int hour;
	int min;
	int sec;
	
}	s_imgui_datetime;

inline void imgui_datetime(char *label,
		int *day, int *month, int *year,
		int *hour, int *min, int *sec)
{
	ImGui::PushID(label);
	
	char datetime_str[64];
	
	sprintf(datetime_str, "%02d/%02d/%04d %02d:%02d:%02d",
			*day, *month, *year, *hour, *min, *sec);
	
	ImGui::InputText("##datetime", datetime_str, sizeof(datetime_str));
	
	sscanf(datetime_str, "%02d/%02d/%04d %02d:%02d:%02d",
		   day , month, year, hour, min  , sec);
	
	*day   = (*day   <    1) ?    1 : *day;
	*day   = (*day   >   30) ?   30 : *day;
	
	*month = (*month <    1) ?    1 : *month;
	*month = (*month >   12) ?   12 : *month;
	
	*year  = (*year  < 1990) ? 1990 : *year;
	*year  = (*year  > 3000) ? 3000 : *year;
	
	*hour  = (*hour  <    0) ?    0 : *hour;
	*hour  = (*hour  >   23) ?   23 : *hour;
	
	*min   = (*min   <    0) ?    0 : *min;
	*min   = (*min   >   59) ?   59 : *min;
	
	*sec   = (*sec   <    0) ?    0 : *sec;
	*sec   = (*sec   >   59) ?   59 : *sec;
	
	ImGui::PopID();
	
	return;
}

//------------------------------------------------------------------------------

inline void 	 gui_save_uint8  (char *label, uint8_t   data) { ImGui::GetCurrentWindow()->StateStorage.SetInt    (ImGui::GetID(label), (int) data); return; }
inline void 	 gui_save_uint16 (char *label, uint16_t  data) { ImGui::GetCurrentWindow()->StateStorage.SetInt    (ImGui::GetID(label), (int) data); return; }
inline void 	 gui_save_uint32 (char *label, uint32_t  data) { ImGui::GetCurrentWindow()->StateStorage.SetInt    (ImGui::GetID(label), (int) data); return; }
inline void 	 gui_save_int8   (char *label, int8_t    data) { ImGui::GetCurrentWindow()->StateStorage.SetInt    (ImGui::GetID(label), (int) data); return; }
inline void 	 gui_save_int16  (char *label, int16_t   data) { ImGui::GetCurrentWindow()->StateStorage.SetInt    (ImGui::GetID(label), (int) data); return; }
inline void 	 gui_save_int32  (char *label, int32_t   data) { ImGui::GetCurrentWindow()->StateStorage.SetInt    (ImGui::GetID(label), (int) data); return; }
inline void 	 gui_save_float32(char *label, float32_t data) { ImGui::GetCurrentWindow()->StateStorage.SetFloat  (ImGui::GetID(label),       data); return; }
inline void 	 gui_save_void   (char *label, void     *data) { ImGui::GetCurrentWindow()->StateStorage.SetVoidPtr(ImGui::GetID(label),       data); return; }

inline uint8_t   gui_load_uint8  (char *label, uint8_t   def  ) { return (uint8_t  ) ImGui::GetCurrentWindow()->StateStorage.GetInt    (ImGui::GetID(label), (int) def); }
inline uint16_t  gui_load_uint16 (char *label, uint16_t  def  ) { return (uint16_t ) ImGui::GetCurrentWindow()->StateStorage.GetInt    (ImGui::GetID(label), (int) def); }
inline uint32_t  gui_load_uint32 (char *label, uint32_t  def  ) { return (uint32_t ) ImGui::GetCurrentWindow()->StateStorage.GetInt    (ImGui::GetID(label), (int) def); }
inline int8_t    gui_load_int8   (char *label, int8_t    def  ) { return (int8_t   ) ImGui::GetCurrentWindow()->StateStorage.GetInt    (ImGui::GetID(label), (int) def); }
inline int16_t   gui_load_int16  (char *label, int16_t   def  ) { return (int16_t  ) ImGui::GetCurrentWindow()->StateStorage.GetInt    (ImGui::GetID(label), (int) def); }
inline int32_t   gui_load_int32  (char *label, int32_t   def  ) { return (int32_t  ) ImGui::GetCurrentWindow()->StateStorage.GetInt    (ImGui::GetID(label), (int) def); }
inline float32_t gui_load_float32(char *label, float32_t def  ) { return (float32_t) ImGui::GetCurrentWindow()->StateStorage.GetFloat  (ImGui::GetID(label),       def); }
inline void*  	 gui_load_void   (char *label                 ) { return             ImGui::GetCurrentWindow()->StateStorage.GetVoidPtr(ImGui::GetID(label)           ); }

//------------------------------------------------------------------------------

inline void gui_autowidth(void)
{
	ImGui::SetNextItemWidth(ImGui::GetContentRegionAvailWidth());
	return;
}

//inline void gui_hint(const char* fmt, ...)
//{
//	va_list args;
//	va_start (args, fmt);
//
//	if (ImGui::IsItemHovered()) { ImGui::SetTooltip(fmt, args); }
//
//	va_end (args);
//
//	return;
//}

#define gui_hint(fmt, ...) if (ImGui::IsItemHovered()) { ImGui::SetTooltip(fmt, ##__VA_ARGS__); }

//------------------------------------------------------------------------------

#endif /* __IMGUI_W__ */


