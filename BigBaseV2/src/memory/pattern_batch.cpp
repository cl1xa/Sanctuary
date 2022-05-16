#include "../common.hpp"
#include "../logger.hpp"
#include "pattern_batch.hpp"
#include "range.hpp"

namespace memory
{
	void pattern_batch::add(std::string name, pattern pattern, std::function<void(handle)> callback)
	{
		m_entries.emplace_back(std::move(name), std::move(pattern), std::move(callback));
	}

	void pattern_batch::run(range region)
	{
		bool all_found = true;
		for (auto& entry : m_entries)
		{
			if (auto result = region.scan(entry.m_pattern))
			{
				if (entry.m_callback)
				{
					std::invoke(std::move(entry.m_callback), result);
					LOG(INFO) << xorstr_("Found '") << entry.m_name << xorstr_("' GTA5.exe+") << HEX_TO_UPPER(result.as<DWORD64>() - region.begin().as<DWORD64>());
				}
				else
				{
					all_found = false;
					LOG(WARNING) << xorstr_("Failed to find '") << entry.m_name << xorstr_("'.");
				}
			}
			else
			{
				all_found = false;
				LOG(WARNING) << xorstr_("Failed to find '") << entry.m_name << xorstr_("'.");
			}
		}

		m_entries.clear();

		if (!all_found)
			throw std::runtime_error(xorstr_("Failed to find some patterns."));
	}
}
