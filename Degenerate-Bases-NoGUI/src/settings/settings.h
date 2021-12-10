#pragma once
#include "pch.h"

namespace dgn
{
	class Settings
	{
	public:
		static const std::string Get(std::string&& section, std::string&& key)
		{
			if (Settings::s_Structure.has(section))
			{
				auto& collection = Settings::s_Structure[section];
				if (collection.has(key))
					return collection[key];
			}

			return std::string();
		}

	public:
		static void CreateDefault()
		{
			if (Settings::ExistsSettings())
				return;

			mINI::INIFile iniFile(Settings::s_SettingsPath);
			mINI::INIStructure iniStruct;

			iniStruct["fasta"].set({
				{ "header_character ", ">" },
				{ "header_template", "seq" }
				});

			iniStruct["results"].set({
				{ "prefix ", "Result_" },
				{ "format", "fas" },
				{ "directory", "./results/" }
				});

			iniStruct["chunks"].set({ { "chunk_size", "1024" } });

			iniStruct["debug"].set({
				{ "enable", "false" },
				{ "write_debug", "false" },
				});

			iniFile.generate(iniStruct, true);
		}

		static void ReadSettings()
		{
			mINI::INIFile file(Settings::s_SettingsPath);

			file.read(Settings::s_Structure);
		}

	public:
		static bool ToBool(const std::string& str)
		{
			for (auto& ch : str) 
				std::tolower(ch);

			return (str == "true" || str == "1");
		}

	private:
		static bool ExistsSettings() { return std::filesystem::exists(s_SettingsPath); }

	private:
		static const std::string s_SettingsPath;
		static mINI::INIStructure s_Structure;
	};

	const inline std::string dgn::Settings::s_SettingsPath = "settings.ini";

	inline mINI::INIStructure dgn::Settings::s_Structure = mINI::INIStructure();
}
