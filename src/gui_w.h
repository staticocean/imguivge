
#ifndef __GUI_W__
#define __GUI_W__

//------------------------------------------------------------------------------

#define IMGUI_DEFINE_MATH_OPERATORS
#include <lib/imgui/imgui.h>
#include <lib/imgui/imgui_internal.h>

//#include <lib/imfilebrowser/imfilebrowser.h>

#include <softael_lib/vl.h>
#include <softael_lib/vl3d.h>
#include <softael_lib/imgui_w.h>

//------------------------------------------------------------------------------

//inline ImGui::FileBrowser __file_browser_open__ = ImGui::FileBrowser();
//inline ImGui::FileBrowser __file_browser_save__ = ImGui::FileBrowser(ImGuiFileBrowserFlags_EnterNewFilename);
//
//inline void gui_fileopen(char *file_path, float width = -1)
//{
//    ImGui::PushID(file_path);
//    
//    if (width < 0) width = ImGui::GetContentRegionAvailWidth();
//    
//    ImGui::SetNextItemWidth(width-40);
//    char *file_name = file_path + strlen(file_path);
//    while (file_name > file_path && *(file_name-1) != '\\' && *(file_name-1) != '/')
//    { --file_name; }
//    
//    ImGui::InputText("##file_path", file_name, 512, ImGuiInputTextFlags_ReadOnly);
//    if (ImGui::IsItemHovered())
//    { ImGui::SetTooltip(file_path); }
//
//    ImGui::SameLine(0.0, 0.0);
//    
//    if(ImGui::Button("SEL", ImVec2(40,0)))
//    { __file_browser_open__.Open(); }
//    
//    __file_browser_open__.Display();
//    
//    if(__file_browser_open__.HasSelected())
//    {
//        strcpy(file_path, __file_browser_open__.GetSelected().string().c_str());
//        __file_browser_open__.ClearSelected();
//    }
//    
//    ImGui::PopID();
//    
//    return;
//}
//
//inline void gui_filesave(char *file_path)
//{
//    ImGui::PushID(file_path);
//    
//    ImGui::SetNextItemWidth(-40);
//    char *file_name = file_path + strlen(file_path);
//    while (file_name > file_path && *(file_name-1) != '\\' && *(file_name-1) != '/')
//    { --file_name; }
//    
//    ImGui::InputText("##file_path", file_name, 512, ImGuiInputTextFlags_ReadOnly);
//    if (ImGui::IsItemHovered())
//    { ImGui::SetTooltip(file_path); }
//    
//    ImGui::SameLine(0.0, 0.0);
//    
//    if(ImGui::Button("SEL", ImVec2(ImGui::GetContentRegionAvailWidth(),0)))
//    { __file_browser_save__.Open(); }
//    
//    __file_browser_save__.Display();
//    
//    if(__file_browser_save__.HasSelected())
//    {
//        strcpy(file_path, __file_browser_save__.GetSelected().string().c_str());
//        __file_browser_save__.ClearSelected();
//    }
//    
//    ImGui::PopID();
//    
//    return;
//}

//------------------------------------------------------------------------------

namespace ImGui
{
	bool BufferingBar(const char* label, float value,  const ImVec2& size_arg,
			const ImU32& bg_col, const ImU32& fg_col)
	{
		ImGuiWindow* window = GetCurrentWindow();
		if (window->SkipItems)
			return false;
		
		ImGuiContext& g = *GImGui;
		const ImGuiStyle& style = g.Style;
		const ImGuiID id = window->GetID(label);
		
		ImVec2 pos = window->DC.CursorPos;
		ImVec2 size = size_arg;
		
		if (size.x < 0) size.x = ImGui::GetContentRegionAvailWidth();
		
		const ImRect bb(pos, ImVec2(pos.x + size.x, pos.y + size.y));
		ItemSize(bb, style.FramePadding.y);
		if (!ItemAdd(bb, id))
			return false;
		
		// Render
		window->DrawList->AddRectFilled(bb.Min, ImVec2(pos.x + size.x, bb.Max.y), bg_col);
		window->DrawList->AddRectFilled(bb.Min, ImVec2(pos.x + size.x*value, bb.Max.y), fg_col);
		
		return true;
	}
	
	bool Spinner(const char* label, float radius, int thickness, const ImU32& color)
	{
		ImGuiWindow* window = GetCurrentWindow();
		if (window->SkipItems)
			return false;
		
		ImGuiContext& g = *GImGui;
		const ImGuiStyle& style = g.Style;
		const ImGuiID id = window->GetID(label);
		
		ImVec2 pos = window->DC.CursorPos;
		ImVec2 size((radius )*2, (radius + style.FramePadding.y)*2);
		
		const ImRect bb(pos, ImVec2(pos.x + size.x, pos.y + size.y));
		ItemSize(bb, style.FramePadding.y);
		if (!ItemAdd(bb, id))
			return false;
		
		// Render
		window->DrawList->PathClear();
		
		int num_segments = 30;
		int start = ImAbs(ImSin(g.Time*1.8f)*(num_segments-5));
		
		const float a_min = IM_PI*2.0f * ((float)start) / (float)num_segments;
		const float a_max = IM_PI*2.0f * ((float)num_segments-3) / (float)num_segments;
		
		const ImVec2 centre = ImVec2(pos.x+radius, pos.y+radius+style.FramePadding.y);
		
		for (int i = 0; i < num_segments; i++) {
			const float a = a_min + ((float)i / (float)num_segments) * (a_max - a_min);
			window->DrawList->PathLineTo(ImVec2(centre.x + ImCos(a+g.Time*8) * radius,
												centre.y + ImSin(a+g.Time*8) * radius));
		}
		
		window->DrawList->PathStroke(color, false, thickness);
		
		return true;
	}
}

//------------------------------------------------------------------------------

#endif /* __GUI_W__ */


