#ifndef __ivge_image__ 
#define __ivge_image__ 
typedef struct ivge_image { 
} s_ivge_image; 
inline void ivge_image_draw(s_ivge_image *ivge_image) { 
ImGuiWindow* window = ImGui::GetCurrentWindow(); 
ImRect window_rect = window->InnerRect; 
ImVec2 window_size = window_rect.Max - window_rect.Min; 
float  scale       = window_size.x / 764.000000; 
window->DrawList->AddLine(window_rect.Min+ImVec2(299.000000,290.500000)*scale, window_rect.Min+ImVec2(465.000000,373.500000)*scale, -16777029); 
window->DrawList->AddText(window_rect.Min+ImVec2(945.872803,17.206497)*scale, -16777216, "SADASADDSD"); 
} 
#endif 
