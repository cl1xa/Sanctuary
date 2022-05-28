#include "config/config.hpp"

namespace big
{
	bool config::save(const string file_name)
	{
		const size_t class_size = sizeof(config);
		void* buffer = malloc(class_size);

		memcpy(buffer, &g_config, class_size);

		string path = getenv(xorstr_("appdata"));
		path += xorstr_("\\Sanctuary\\") + file_name;

		ofstream output(path, ios::binary);

		if (!output.is_open())
			return false;

		output.write(static_cast<const char*>(buffer), class_size);
		output.flush();
		output.close();

		free(buffer);
		buffer = NULL;

		return true;
	}

	bool config::load(const string file_name)
	{
		string path = getenv(xorstr_("appdata"));
		path += xorstr_("\\Sanctuary\\") + file_name;

		ifstream input(path, ios::binary);

		if (!input.is_open())
			return false;

		const size_t class_size = sizeof(config);
		void* buffer = malloc(class_size);

		input.read(static_cast<char*>(buffer), class_size);
		input.close();

		memcpy(&g_config, buffer, class_size);
		free(buffer);
		buffer = NULL;

		return true;
	}
}