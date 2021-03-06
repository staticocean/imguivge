
#ifndef __GUI__
#define __GUI__

//------------------------------------------------------------------------------

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <lib/imgui/imgui.h>

#include <lib_internal/vl.h>
#include <lib_internal/softael.h>
#include <lib_internal/imgui_theme.h>

#include "gui_w.h"
#include "gui_tbar.h"
#include "gui_vge.h"

//------------------------------------------------------------------------------

typedef struct gui
{
	float32_t   w_height;
	float32_t   w_width;
	
	s_gui_tbar  gui_tbar;
	
	s_vge 		vge;
	
} 	s_gui;

typedef struct gui_init
{
    uint32_t    temp;
    
}	s_gui_init;

//------------------------------------------------------------------------------

inline uint8_t gui_init(s_gui *gui, s_gui_init attr)
{
	NFD_Init();
	
    imgui_theme_set();
    
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    
    ImFontConfig font_config;
    font_config.OversampleH = 4;
    font_config.OversampleV = 4;
    
    io.FontDefault = io.Fonts->AddFontFromMemoryCompressedBase85TTF(default_font_compressed_data_base85,
       18, &font_config, io.Fonts->GetGlyphRangesCyrillic());
    
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
	style_ref.PopupBorderSize   = 0.0;
	style_ref.TabBorderSize     = 0.0;
	style_ref.WindowBorderSize  = 0.0;
	
	gui->w_height = 720;
	gui->w_width  = 1024;
	
	gui->gui_tbar.height = 40;
 
	vge_init(&gui->vge, (s_vge_attr) {});
	
	return 0x00;
}

//------------------------------------------------------------------------------

inline uint8_t gui_main(s_gui *gui)
{
    const int pwidth = 300;
    
//	static s_dev *sel = NULL;
	
	int static_flags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoBringToFrontOnFocus
					   | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar;

	#ifdef NDEBUG
	#else
		bool show_demo_window = true;
		if (show_demo_window) ImGui::ShowDemoWindow(&show_demo_window);
	#endif
	
	{
		// Toolbar
		ImGui::SetNextWindowPos(ImVec2(0,0), ImGuiCond_Always);
		ImGui::SetNextWindowSize(ImVec2(gui->w_width, (float) gui->gui_tbar.height), ImGuiCond_Always);
		ImGui::Begin("toolbar", NULL, static_flags
			| ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoBackground);
		gui_tbar_main(&gui->gui_tbar);
		ImGui::End();
	}
	
	{
		// Left panel
		ImGui::SetNextWindowPos(ImVec2(0, (float) gui->gui_tbar.height), ImGuiCond_Always);
		ImGui::SetNextWindowSize(ImVec2(pwidth, gui->w_height - gui->gui_tbar.height), ImGuiCond_Always);
		ImGui::Begin("lpanel", NULL, static_flags);
		
		gui_vge_objlist(&gui->vge);
		
		ImGui::End();
	}
	
	{
		// Main view
		ImGui::SetNextWindowPos(ImVec2(pwidth, (float) gui->gui_tbar.height), ImGuiCond_Always);
		ImGui::SetNextWindowSize(ImVec2(gui->w_width - pwidth*2, gui->w_height - gui->gui_tbar.height), ImGuiCond_Always);
		ImGui::Begin("mpanel", NULL, static_flags);
//
//		if (sel != NULL && sel->state != 0x00)
//		{ sel->gui_edit(sel); }
		
		gui_vge_canvas(&gui->vge);

		ImGui::End();
	}
	
	{
		// Right panel
		ImGui::SetNextWindowPos (ImVec2(gui->w_width - pwidth, (float) gui->gui_tbar.height), ImGuiCond_Always);
		ImGui::SetNextWindowSize(ImVec2(pwidth, gui->w_height - gui->gui_tbar.height), ImGuiCond_Always);
		ImGui::Begin("rpanel", NULL, static_flags);
		
		gui_vge_varlist(&gui->vge);
		
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
