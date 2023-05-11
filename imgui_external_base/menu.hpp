#pragma once

#include <imgui.h>
#include <imgui_internal.h>
#include <vector>
#include <string>
#include <xorstr.hpp>

namespace Menu {

	typedef struct Variables {
		std::vector<std::string> products = {
			xorstr_("FiveM"),
			xorstr_("Overwatch"),
			xorstr_("Warzone")
		};

		int selected = 0;
	};

	inline Variables* Data = nullptr;

	inline bool Initialize() {

		Data = new Variables{};

		ImGuiIO& io = ImGui::GetIO();
		ImGuiStyle& style = ImGui::GetStyle();

		const auto size = sizeof(style.Colors) / sizeof(style.Colors[0]);
		ImVec4(&colors)[size] = style.Colors;

		style.WindowTitleAlign = ImVec2(0.5f, 0.5f);
		
		colors[ImGuiCol_TitleBg] = ImColor(40, 40, 40);
		colors[ImGuiCol_TitleBgActive] = ImColor(40, 40, 40);
		colors[ImGuiCol_TitleBgCollapsed] = ImColor(40, 40, 40);

		colors[ImGuiCol_FrameBg] = ImColor(24, 24, 24);
		colors[ImGuiCol_FrameBgActive] = ImColor(26, 26, 26);
		colors[ImGuiCol_FrameBgHovered] = ImColor(26, 26, 26);

		colors[ImGuiCol_Header] = ImColor(50, 50, 50);
		colors[ImGuiCol_HeaderActive] = ImColor(50, 50, 50);
		colors[ImGuiCol_HeaderHovered] = ImColor(50, 50, 50);

		colors[ImGuiCol_Button] = ImColor(26, 26, 26);
		colors[ImGuiCol_ButtonActive] = ImColor(26, 26, 26);
		colors[ImGuiCol_ButtonHovered] = ImColor(26, 26, 26);

		return true;
	}

	inline void Render() {

		const int flags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize;

		ImGui::SetNextWindowSize(ImVec2(250, 300));

		ImGui::Begin(xorstr_("Example"), 0, flags);

		const ImVec2 avail = ImGui::GetContentRegionAvail();
		const float padding = 16.0f;

		ImGui::SetCursorPos(ImVec2(ImGui::GetCursorPosX() + padding, ImGui::GetCursorPosY() + padding));

		ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 4.0f);
		ImGui::ListBoxHeader(xorstr_("##listbox_header"), ImVec2(avail.x - (padding * 2.0f), avail.y - (padding * 4.0f)));
		
		for (int index = 0; 
			index < Data->products.size(); index++) {

			bool selected = (index == Data->selected);

			std::string& name = Data->products[index];
			if (ImGui::Selectable(name.c_str(), &selected))
				Data->selected = index;

			if (selected)
				ImGui::SetItemDefaultFocus();
		}

		ImGui::ListBoxFooter();

		ImVec2 button_size = { avail.x - (padding * 2.0f), 24.0f };

		ImGui::SetCursorPosX(ImGui::GetCursorPosX() + padding);
		if (ImGui::Button("Launch", button_size)) {

			/*
			
			Do your launch functionality here . . .

			*/

		}

		ImGui::PopStyleVar();
		ImGui::End();

	}
}