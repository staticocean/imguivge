
#ifndef __GUI_TBAR__
#define __GUI_TBAR__

//----------------------------------------------------------------

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <lib_internal/vl.h>
#include <lib/imgui/imgui.h>
#include <lib/clip/clip.h>
#include <lib/nfde/nfd.h>

//----------------------------------------------------------------

typedef struct gui_tbar
{
	int height;
	
	char file_path[512];
	
} 	s_gui_tbar;

//----------------------------------------------------------------

inline void gui_tbar_menu_file(s_gui_tbar *tbar)
{
	if (ImGui::MenuItem("Open", "Ctrl+O"))
	{
		nfdchar_t *user_path;
		nfdfilteritem_t filterItem[1] = { { "Trajectory Project", "trj" } };
		nfdresult_t result = NFD_OpenDialog(&user_path, filterItem, 1, tbar->file_path);
		
		if (result == NFD_OKAY)
		{
			strcpy(tbar->file_path, user_path);
//			trj_eng_load(tbar->eng, tbar->file_path);
			
			NFD_FreePath(user_path);
		}
		
//		else if (result == NFD_CANCEL)
//		{ puts("User pressed cancel."); }

//		else
//		{ printf("Error: %s\n", NFD_GetError() ); }
	}

//	if (ImGui::MenuItem("Save", "Ctrl+S"))
//	{ trj_eng_save(tbar->eng, tbar->file_path); }

	if (ImGui::MenuItem("Save As...", ""))
	{
		nfdchar_t *user_path;
		nfdfilteritem_t filterItem[1] = { { "Trajectory Project", "trj" } };
		nfdresult_t result = NFD_SaveDialog(&user_path, filterItem, 1, tbar->file_path, "project.trj");
		
		if (result == NFD_OKAY)
		{
			strcpy(tbar->file_path, user_path);
//			trj_eng_save(tbar->eng, tbar->file_path);
			
			NFD_FreePath(user_path);
		}

//		else if (result == NFD_CANCEL)
//		{ puts("User pressed cancel."); }

//		else
//		{ printf("Error: %s\n", NFD_GetError() ); }
	}
	
	ImGui::EndMenu();
	
	return;
}

inline void gui_tbar_menu_version(s_gui_tbar *tbar)
{
	const char *pn = "SAE-PN-RS0000-00-0000-0000";
	const char *rn = "SAE-RN-0000-0000-0000-0000";
	const char *mn = "SAE-MN-0000-0000-0000-0000";
	const char *sn = "SAE-SN-0000000000000000   ";
	
	if (ImGui::MenuItem("Product Number     ", pn, false, true)) { clip::set_text(pn); }
	if (ImGui::MenuItem("Release Number     ", rn, false, true)) { clip::set_text(rn); }
	if (ImGui::MenuItem("Manufacturer Number", mn, false, true)) { clip::set_text(mn); }
	if (ImGui::MenuItem("Serial Number      ", sn, false, true)) { clip::set_text(sn); }
	
	ImGui::Separator();
	
	ImGui::MenuItem("Click to copy", NULL, false, false);
	
	ImGui::EndMenu();
	
	return;
}

inline uint8_t gui_tbar_main(s_gui_tbar *tbar)
{
	static float64_t time_limit_min = 0.0;
	static float64_t time_step_min = 0.001;
	static float64_t time_step_max = 100.0;
	static uint32_t  time_iter_min = 0x00;
	
	{
		ImVec2 popup_pos = ImGui::GetCursorScreenPos() + ImVec2(0, ImGui::GetTextLineHeightWithSpacing());
		
		if(ImGui::Button("MENU", ImVec2(80,0)))
		{ ImGui::OpenPopup("gui_tbar_menu"); }
		
		ImGui::SetNextWindowPos(popup_pos, ImGuiCond_Always);
		ImGui::SetNextWindowSize(ImVec2(200,0), ImGuiCond_Always);
		
		ImGui::GetStyle().DisplayWindowPadding = ImVec2(0,0);
		ImGui::GetStyle().DisplaySafeAreaPadding = ImVec2(0,0);
		
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(16, 8));
			
		if (ImGui::BeginPopup("gui_tbar_menu"))
		{
			if (ImGui::BeginMenu("File      ")) { gui_tbar_menu_file     (tbar); }
			
			ImGui::Separator();
			
			if (ImGui::BeginMenu("Version   ")) { gui_tbar_menu_version  (tbar); }
			
			ImGui::EndPopup();
		}
		
		ImGui::PopStyleVar(1);
		
		ImGui::SameLine();
	}
	
	ImGui::SameLine(0,0);
	{
		char *file_name = tbar->file_path + strlen(tbar->file_path);
		while (file_name > tbar->file_path && *(file_name - 1) != '\\' && *(file_name - 1) != '/')
		{ --file_name; }
		
		char file_preview[256];
		sprintf(file_preview, " FILE: %s", file_name);
		
		ImGui::SetNextItemWidth(160);
		ImGui::InputText("##file_path", file_preview, 256, ImGuiInputTextFlags_ReadOnly);
		gui_hint(tbar->file_path);
	}
	
	return 0x00;
}

//----------------------------------------------------------------

#endif /* __GUI_TBAR__ */




