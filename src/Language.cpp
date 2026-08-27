#include "Language.h"

namespace Language
{
	std::string GetOutput(RE::TESObjectREFR* a_owner, RE::TESBoundObject* a_object, const boost::smatch& a_match)
	{
		switch (gameLanguageHash) {
		case "CHINESE"_h:
			return chinese(a_owner, a_object, a_match).output();
		case "FRENCH"_h:
			return french(a_owner, a_object, a_match).output();
		case "GERMAN"_h:
			return german(a_owner, a_object, a_match).output();
		case "JAPANESE"_h:
			return japanese(a_owner, a_object, a_match).output();
		case "KOREAN"_h:
			return korean(a_owner, a_object, a_match).output();
		case "POLISH"_h:
			return polish(a_owner, a_object, a_match).output();
		case "PORTUGUESE"_h:
			return portuguese(a_owner, a_object, a_match).output();
		case "SPANISH"_h:
			return spanish(a_owner, a_object, a_match).output();
		default:
			return generic(a_owner, a_object, a_match).output();
		}
	}

	void LoadSettings()
	{
		const auto store = REX::FIniSettingStore::GetSingleton();
		store->Init("Data/SKSE/Plugins/po3_LanguageFixes.ini", "");

		store->Load();
		store->Save();
	}

	void GetGameLanguageHash()
	{
		std::string gameLanguage = REX::STR::TO_UPPER(*"sLanguage:General"_ini);

		auto& languageOverrideStr = stl::get_setting_ref(languageOverride);
		
		if (languageOverrideStr.empty()) {
			REX::INFO("Language override set to AUTODETECT ({})", gameLanguage);
			gameLanguageHash = REX::STR::CONST_HASH(gameLanguage);
		} else {
			REX::STR::TRIM(languageOverrideStr);
			languageOverrideStr = REX::STR::TO_UPPER(languageOverrideStr);

			REX::INFO("Language override set to {}", languageOverrideStr);
			gameLanguageHash = REX::STR::CONST_HASH(languageOverrideStr);
		}
	}
}
