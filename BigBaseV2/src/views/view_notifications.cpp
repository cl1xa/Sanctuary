#include "views/view.hpp"
#include "services/notification_service.hpp"
#include "renderer.hpp"

namespace big
{
	float draw_notification(float start_pos, ImDrawList* dl, string title, string message, ImVec4 color)
	{
		ImColor textCol = ImGui::ColorConvertFloat4ToU32({ 1.f, 1.f, 1.f, 1.f });
		color.w = 0.5f;
		ImColor fadeBegin = ImGui::ColorConvertFloat4ToU32(color);
		color.w = 0.f;
		ImColor fadeEnd = ImGui::ColorConvertFloat4ToU32(color);

		int j = 0;
		int prevSpace = 0;
		float total_size = 0.f;
		vector<string> split_points;

		for (int i = 0; i <= message.size(); i++)
		{
			string current_message = message.substr(j, i - j);

			if (message.substr(i, 1) == " ")
				prevSpace = i;

			ImVec2 size = ImGui::CalcTextSize(current_message.c_str());

			if (i == message.size())
			{
				total_size = total_size + size.y;
				split_points.push_back(message.substr(j, i));
			}
			else if (size.x >= 330.f)
			{
				total_size = total_size + size.y;
				split_points.push_back(message.substr(j, prevSpace - j));
				j = prevSpace + 1;
				i = prevSpace;
			}
		}

		dl->AddRectFilled({ (float)*g_pointers->m_resolution_x - 360.f, 10.f + start_pos }, { (float)*g_pointers->m_resolution_x - 10.f, start_pos + 45.f + total_size }, (ImU32)3357612055);
		dl->AddRectFilledMultiColor({ (float)*g_pointers->m_resolution_x - 360.f, 10.f + start_pos }, { (float)*g_pointers->m_resolution_x - 255.f, start_pos + 45.f + total_size }, fadeBegin, fadeEnd, fadeEnd, fadeBegin);

		dl->AddText(g_renderer->m_font, 22.f, { (float)*g_pointers->m_resolution_x - 350.f, 15.f + start_pos }, textCol, title.c_str());

		int i = 0;

		for (string txt : split_points)
		{
			dl->AddText({ (float)*g_pointers->m_resolution_x - 350.f, 40.f + (i * 20.f) + start_pos }, textCol, txt.c_str());
			i++;
		}

		return start_pos + 45.f + total_size;
	}

	void view_main::notifications()
	{
		ImDrawList* draw_list = ImGui::GetBackgroundDrawList();

		vector<notification> notifications = g_notification_service->get();

		float prev_pos = 0.f;

		for (int i = 0; i < notifications.size(); i++)
		{
			notification& n = notifications[i];

			prev_pos = draw_notification(prev_pos, draw_list, n.title, n.message, g_notification_service->notification_colors.at(n.type));
		}
	}
}