#include "Language.h"

namespace Language
{
	std::string get_output(RE::TESObjectREFR* a_owner, RE::TESBoundObject* a_object, const srell::smatch& a_match)
	{	
		switch (gameLanguageHash) {
		case "CHINESE"_h:
			return chinese(a_owner, a_object, a_match).output();
		case "FRENCH"_h:
			return french(a_owner, a_object, a_match).output();
		case "GERMAN"_h:
			return german(a_owner, a_object, a_match).output();
		case "POLISH"_h:
			return polish(a_owner, a_object, a_match).output();
		case "SPANISH"_h:
			return spanish(a_owner, a_object, a_match).output();
		default:
			return generic(a_owner, a_object, a_match).output();
		}
	}
}
