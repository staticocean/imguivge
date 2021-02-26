
#ifndef __SOFTAEL__
#define __SOFTAEL__

//------------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#define IMGUI_DEFINE_MATH_OPERATORS
#include <lib/imgui/imgui.h>
#include <lib/imgui/imgui_internal.h>

#include <vl.h>

//------------------------------------------------------------------------------

typedef struct softael_lconfig
{
	uint32_t bg;
	uint32_t main;
	
	void *context;
	
	void (*rect) (void *context, ImVec2 x0, ImVec2 x1, ImVec2 x2, ImVec2 x3, uint32_t color);
	void (*text) (void *context, ImVec2 x0, uint32_t size, uint32_t color, char *text);
	
}	s_softael_lconfig;

inline void softael_ldrawtop(s_softael_lconfig *config)
{
	// top rect
	config->rect(
			config->context,
			ImVec2(0, 0), ImVec2(512, 0),
			ImVec2(512, 256), ImVec2(0, 256),
			config->bg);
	
	config->text(
			config->context, ImVec2(40, 64), 140,
			config->main, "SOFTAEL");
	
	return;
}

inline void softael_ldrawbot(s_softael_lconfig *config)
{
	ImVec2 offset = ImVec2(0, 256+32);
	
	// top rect
	config->rect(
			config->context,
			ImVec2(0, 0) + offset,
			ImVec2(512, 0) + offset,
			ImVec2(512, 1024) + offset,
			ImVec2(0, 1024) + offset,
			config->bg);
	
	for (int i = 0; i < 6; ++i)
	{
		config->rect(
				config->context,
				ImVec2(64*i, 0) + offset,
				ImVec2(64*i+128, 0) + offset,
				ImVec2(64*i+256, 1024 / vl_pow(2, i)) + offset,
				ImVec2(64*i+128, 1024 / vl_pow(2, i)) + offset,
				(config->main & (~IM_COL32_A_MASK)) | (178 << IM_COL32_A_SHIFT));
	}
	
	return;
}

inline void softael_ldraw(s_softael_lconfig *config)
{
	softael_ldrawtop(config);
	softael_ldrawbot(config);
	
	return;
};

//------------------------------------------------------------------------------

typedef struct gui_softael_ldraw_context
{
	vlf_t scale;
	ImDrawList* draw_list;
	ImVec2 offset;
	
}	s_gui_softael_ldraw_context;

inline void __gui_softael_ldraw_rect__(void *context, ImVec2 x0, ImVec2 x1, ImVec2 x2, ImVec2 x3, uint32_t color)
{
	s_gui_softael_ldraw_context *config = (s_gui_softael_ldraw_context*) context;
	ImDrawList* dl = config->draw_list;
	
	ImVec2 x0_ = (ImVec2(x0[0], x0[1])*config->scale + config->offset);
	ImVec2 x1_ = (ImVec2(x1[0], x1[1])*config->scale + config->offset);
	ImVec2 x2_ = (ImVec2(x2[0], x2[1])*config->scale + config->offset);
	ImVec2 x3_ = (ImVec2(x3[0], x3[1])*config->scale + config->offset);
	
	dl->AddTriangleFilled(x0_, x1_, x2_, color);
	dl->AddTriangleFilled(x2_, x3_, x0_, color);
	
	return;
}

inline void __gui_softael_ldraw_text__(void *context, ImVec2 x0, uint32_t size, uint32_t color, char *text)
{
	s_gui_softael_ldraw_context *config = (s_gui_softael_ldraw_context*) context;
	ImDrawList* dl = config->draw_list;
	
	dl->AddText(ImGui::GetIO().FontDefault, size*config->scale,
			x0*config->scale + config->offset, color, text);
	
	return;
}

inline void gui_softael_ldraw(uint32_t bg, uint32_t main, vlf_t width)
{
	s_gui_softael_ldraw_context context =
	{
			.scale = width / 512,
	};
	
	s_softael_lconfig config =
	{
		.bg = bg,
		.main = main,
		
		.context = &context,
		
		.rect = __gui_softael_ldraw_rect__,
		.text = __gui_softael_ldraw_text__,
	};
	
//	ImVec2 size = ImVec2(512, 1024+246+32) * context.scale;
	
	context.draw_list = ImGui::GetWindowDrawList();
	context.offset = ImGui::GetCursorScreenPos();
	
	softael_ldrawtop(&config);
	softael_ldrawbot(&config);
	
//	context.draw_list->AddRectFilled(context.offset, context.offset + ImVec2(100, 100), );
//	ImGui::Dummy(size);
	
	return;
}

//------------------------------------------------------------------------------

#endif /* __SOFTAEL__ */






