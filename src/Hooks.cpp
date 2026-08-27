#include "Hooks.h"
#include "Language.h"

namespace Hooks
{
	static void do_replacement(std::string& text, const boost::regex& pattern, RE::TESBoundObject* a_this, RE::TESObjectREFR* a_activator, RE::BSString& a_dst)
	{
		boost::smatch match;

		std::istringstream iss(text);
		std::ostringstream oss;
		std::string        line;
		while (std::getline(iss, line)) {
			if (boost::regex_search(line, match, pattern)) {
				oss << Language::GetOutput(a_activator, a_this, match);
			} else {
				oss << line;
			}
			oss << '\n';
		}

		a_dst = REX::STR::TRIM_COPY(oss.str());
	}

	namespace TESObjectCONT
	{
		struct GetActivateText
		{
			static bool thunk(RE::TESObjectCONT* a_this, RE::TESObjectREFR* a_activator, RE::BSString& a_dst)
			{
				auto result = func(a_this, a_activator, a_dst);

				if (Language::gameLanguageHash == "ENGLISH"_h) {
					return result;
				}

				if (std::string text = a_dst.c_str(); text.contains("'s ")) {				
					static boost::regex pattern(R"(([\S\s]+?)'s\s([\S\s]+))");
					do_replacement(text, pattern, a_this, a_activator, a_dst);
				}

				return result;
			}
			static inline REL::Relocation<decltype(thunk)> func;

			static inline constexpr std::size_t size = 0x4C;
		};
	}

	namespace TESNPC
	{
		struct GetActivateText
		{
			static bool thunk(RE::TESNPC* a_this, RE::TESObjectREFR* a_activator, RE::BSString& a_dst)
			{
				auto result = func(a_this, a_activator, a_dst);

				if (Language::gameLanguageHash == "ENGLISH"_h) {
					return result;
				}

				if (std::string text = a_dst.c_str(); text.contains("'s ") || text.contains(" - ")) {				
					boost::regex pattern(
						text.contains("'s ") ?
							R"(([\S\s]+?)'s\s([\S\s]+))" :
							R"(([\S\s]+?)\s-\s([\S\s]+))");
					do_replacement(text, pattern, a_this, a_activator, a_dst);
				}

				return result;
			}
			static inline REL::Relocation<decltype(thunk)> func;

			static inline constexpr std::size_t size = 0x4C;
		};
	}

	void Install()
	{
		REX::INFO("{:*^30}", "HOOKS");

		stl::write_vfunc<RE::TESObjectCONT, TESObjectCONT::GetActivateText>();
		REX::INFO("Installed container name hook");
		
		if (Language::doNPCReplacement) {
			stl::write_vfunc<RE::TESNPC, TESNPC::GetActivateText>();
			REX::INFO("Installed NPC name hook");
		}
	}
}
