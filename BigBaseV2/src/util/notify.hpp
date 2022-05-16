#pragma once
#include "CNetGamePlayer.hpp"
#include "natives.hpp"
#include "script.hpp"

namespace big::notify
{
	inline void above_map(std::string_view text)
	{
		std::string in = text.data();
		const auto out = (fmt::format(xorstr_("{}"), in)).c_str();

		HUD::SET_TEXT_OUTLINE();
		HUD::BEGIN_TEXT_COMMAND_THEFEED_POST("STRING");

		HUD::ADD_TEXT_COMPONENT_SUBSTRING_PLAYER_NAME(out);
		HUD::END_TEXT_COMMAND_THEFEED_POST_TICKER(false, false);
	}

	// Shows a busy spinner till the value at the address equals the value passed or if timeout is hit
	inline void busy_spinner(std::string_view text, int* address, int value, int timeout = 15)
	{
		std::string in = text.data();
		const auto out = (fmt::format(xorstr_("{}"), in)).c_str();

		HUD::BEGIN_TEXT_COMMAND_BUSYSPINNER_ON("STRING");
		HUD::ADD_TEXT_COMPONENT_SUBSTRING_PLAYER_NAME(out);
		HUD::END_TEXT_COMMAND_BUSYSPINNER_ON(3);

		for (size_t i = 0; *address != value && i < (size_t)timeout * 100; i++)
			script::get_current()->yield(10ms);
		
		HUD::BUSYSPINNER_OFF();
	}

	inline void display_help_text(std::string_view text)
	{
		std::string in = text.data();
		const auto out = (fmt::format(xorstr_("{}"), in)).c_str();

		HUD::BEGIN_TEXT_COMMAND_DISPLAY_HELP("STRING");
		HUD::ADD_TEXT_COMPONENT_SUBSTRING_PLAYER_NAME(out);
		HUD::END_TEXT_COMMAND_DISPLAY_HELP(0, 0, 1, -1);
	}
}