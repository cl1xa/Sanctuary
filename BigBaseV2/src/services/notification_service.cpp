#include "notification_service.hpp"

namespace big
{
	notification_service::notification_service()
	{
		g_notification_service = this;
	}

	notification_service::~notification_service()
	{
		g_notification_service = nullptr;
	}

	void notification_service::push(notification n)
	{
		this->notifications.emplace(std::hash<std::string>{}(n.message + n.title), n);
	}

	void notification_service::push(string title, string message, bool debug)
	{
		this->push({ NotificationType::INFO, title, message, chrono::system_clock::now(), 3000.f , 1.f});

		if (debug)
			LOG(G3LOG_DEBUG) << fmt::format(xorstr_("{}"), message);
		else
			LOG(INFO) << fmt::format(xorstr_("{}"), message);
	}

	void notification_service::push_warning(string title, string message)
	{
		this->push({ NotificationType::WARNING, title, message, chrono::system_clock::now(), 3000.f , 1.f });

		LOG(WARNING) << fmt::format(xorstr_("{}"), message);
	}

	void notification_service::push_error(string title, string message)
	{
		this->push({ NotificationType::DANGER, title, message, chrono::system_clock::now(), 3000.f , 1.f });

		LOG(FATAL) << fmt::format(xorstr_("{}"), message);
	}

	vector<notification> notification_service::get()
	{
		vector<notification> notifications_to_sent;
		vector<size_t> to_remove;

		for (auto& n : this->notifications) 
		{
			chrono::time_point<chrono::system_clock> curTime = chrono::system_clock::now();
			const float time_diff = (float)chrono::duration_cast<chrono::milliseconds>(curTime - n.second.created_on).count();
			n.second.alpha = 1;

			if (n.second.destroy_in <= time_diff) 
			{
				n.second.alpha = 1.f - ((time_diff - n.second.destroy_in) / 600);
				n.second.alpha = n.second.alpha < 0.f ? 0.f : n.second.alpha;
			}

			if (n.second.alpha > 0.f)
				notifications_to_sent.push_back(n.second);
			else to_remove.push_back(n.first);
		}

		for (size_t k : to_remove)
			this->notifications.erase(k);

		return notifications_to_sent;
	}

}