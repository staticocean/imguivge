
#ifndef __GUI__
#define __GUI__

//------------------------------------------------------------------------------

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define IMGUI_DEFINE_MATH_OPERATORS
#include <lib/imgui/imgui.h>

#include <softael_lib/vl.h>
#include <softael_lib/softael.h>

#include "gui_w.h"
#include "gui_menu.h"
#include "gui_tbar.h"

//------------------------------------------------------------------------------

typedef struct gui
{
	float32_t   w_height;
	float32_t   w_width;
	
	s_gui_menu  gui_menu;
	s_gui_tbar  gui_tbar;
	
} 	s_gui;

typedef struct gui_init
{
    uint32_t    temp;
    
}	s_gui_init;

//------------------------------------------------------------------------------

inline uint8_t gui_init(s_gui *self, s_gui_init attr)
{
	self->w_height = 720;
	self->w_width  = 1024;
	
	ImGui::StyleColorsDark();
	ImGui::StyleColorsLight();
	
	ImGuiStyle& style_ref = ImGui::GetStyle();
	
	style_ref.ScrollbarRounding = 0.0;
	style_ref.ChildRounding     = 0.0;
	style_ref.WindowRounding    = 0.0;
	style_ref.FrameRounding     = 0.0;
	style_ref.GrabRounding      = 0.0;
	style_ref.PopupRounding     = 0.0;
	style_ref.TabRounding       = 0.0;
	
	style_ref.ChildBorderSize   = 0.0;
	style_ref.FrameBorderSize   = 0.0;
	style_ref.PopupBorderSize   = 1.0;
	style_ref.TabBorderSize     = 0.0;
	style_ref.WindowBorderSize  = 1.0;
 
	self->gui_tbar.height = 0;
	self->gui_menu.height = 0;
    
	return 0x00;
}

//------------------------------------------------------------------------------

inline uint8_t gui_main(s_gui *self)
{
    const int pwidth = 250;
    
//	static s_dev *sel = NULL;
	
	int static_flags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoBringToFrontOnFocus
					   | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar;

	#ifdef NDEBUG
	#else
		bool show_demo_window = true;
		if (show_demo_window) ImGui::ShowDemoWindow(&show_demo_window);
	#endif
	
    // Disable imgui menu for macOS compat.
    // because OSX has its own menu above
    
//	gui_menu_main(&self->gui_menu);
	
//	{
//		// Toolbar
//		ImGui::SetNextWindowPos((ImVec2) { 0, (float) self->gui_menu.height }, ImGuiCond_Always);
//		ImGui::SetNextWindowSize((ImVec2) { self->w_width, (float) self->gui_tbar.height }, ImGuiCond_Always);
//		ImGui::Begin("toolbar", NULL, static_flags | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoDecoration);
//		gui_tbar_main(&self->gui_tbar);
//		ImGui::End();
//	}
	
	{
		// progress popup
		// must be after toolbar
//		gui_eng_updategui(&self->gui_eng, &self->eng);
	}
	
	{
		// Object list
		ImGui::SetNextWindowPos((ImVec2) {0, (float) self->gui_menu.height + (float) self->gui_tbar.height }, ImGuiCond_Always);
		ImGui::SetNextWindowSize((ImVec2) {pwidth, self->w_height - self->gui_menu.height - self->gui_tbar.height }, ImGuiCond_Always);
		ImGui::Begin("lpanel", NULL, static_flags);
//		gui_eng_objlist(&self->gui_eng, &self->eng);

//		sel = (s_dev*) gui_load_void("devm_sel");
		
		ImGui::End();
	}
	
	{
		// Object edit
		ImGui::SetNextWindowPos((ImVec2) {pwidth, (float) self->gui_menu.height + (float) self->gui_tbar.height }, ImGuiCond_Always);
		ImGui::SetNextWindowSize((ImVec2) {self->w_width - pwidth*2, self->w_height - self->gui_menu.height - self->gui_tbar.height }, ImGuiCond_Always);
		ImGui::Begin("mpanel", NULL, static_flags);
//
//		if (sel != NULL && sel->state != 0x00)
//		{ sel->gui_edit(sel); }
		
		ImGui::End();
	}
	
	{
		// Main view
		ImGui::SetNextWindowPos ((ImVec2) {self->w_width - pwidth, (float) self->gui_menu.height + (float) self->gui_tbar.height }, ImGuiCond_Always);
		ImGui::SetNextWindowSize((ImVec2) {pwidth, self->w_height - self->gui_menu.height - self->gui_tbar.height }, ImGuiCond_Always);
		ImGui::Begin("rpanel", NULL, static_flags);
		
//		if (sel != NULL && sel->state != 0x00)
//		{ sel->gui_view(sel); }
		
//		ImVec2 c = ImGui::GetCursorPos();
//		ImGui::SetCursorPos(ImGui::GetContentRegionMax() - ImVec2(48, 120));
//////		.bg   = IM_COL32(224, 0, 0, 128),
//		gui_softael_ldraw(
//				IM_COL32(0, 0, 0, 255-90),
//				IM_COL32_WHITE, 48);
//		
//		ImGui::SetCursorPos(c);
		
		ImGui::End();
	}

	#ifdef NDEBUG
	#else
		bool mw = true;
		ImGui::ShowMetricsWindow(&mw);
	#endif

//	self->w_width = 700;
	
	return 0x00;
}

//------------------------------------------------------------------------------

#endif /* __GUI__ */
