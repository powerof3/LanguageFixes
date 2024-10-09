#pragma once

namespace Language
{
	class language
	{
	public:
		language(RE::TESObjectREFR* a_owner, RE::TESBoundObject* a_object, const srell::smatch& a_match) :
			owner(a_owner, a_match[1].str()),
			object(a_object, a_match[2].str())
		{}
		virtual ~language(){};

		virtual std::string output() { return {}; };

		// members
		template <class T>
		struct info
		{
			info(T* a_ptr, std::string a_text) :
				ptr(a_ptr),
				text(std::move(a_text))
			{}

			T*          ptr{};
			std::string text{};
		};

		info<RE::TESObjectREFR>  owner;
		info<RE::TESBoundObject> object;
	};

	class chinese : public language
	{
	public:
		using language::language;
		~chinese() override = default;

		std::string output() override
		{
			return owner.text.append("的").append(object.text);
		}
	};

	class french : public language
	{
	public:
		using language::language;
		~french() override = default;

		std::string output() override
		{
			auto possessive = starts_with_d(owner.text) ? " d'" : " de ";
			return object.text.append(possessive).append(owner.text);
		}

	private:
		bool is_vowel(char a_char)
		{
			switch (a_char) {
			case 'a':
			case 'e':
			case 'i':
			case 'o':
			case 'u':
				return true;
			default:
				return false;
			}
		}
		bool starts_with_d(std::string_view a_str)
		{
			if (a_str.empty()) {
				return false;
			}
			auto str = string::tolower(a_str);
			if (str.front() == 'h' && str.size() > 1) {
				return is_vowel(str[1]);
			} else {
				return is_vowel(str[0]);
			}
		}
	};

	class german : public language
	{
	public:
		using language::language;
		~german() override = default;

		std::string output() override
		{
			auto possessive = ends_with_s(owner.text) ? "' " : "s ";
			return owner.text.append(possessive).append(object.text);
		}

	private:
		bool ends_with_s(std::string_view a_str)
		{
			if (a_str.empty()) {
				return false;
			}
			auto str = string::tolower(a_str);
			return str.back() == 's';
		}
	};

	class japanese : public language
	{
	public:
		using language::language;
		~japanese() override = default;

		std::string output() override
		{
			return owner.text.append("の").append(object.text);
		}
	};

	class polish : public language
	{
	public:
		using language::language;
		~polish() override = default;

		std::string output() override
		{
			return object.text.append(" ").append(get_suffixed_name());
		}

	private:
		RE::SEX get_sex()
		{
			if (auto actor = owner.ptr->As<RE::Actor>()) {
				if (auto npc = actor->GetActorBase()) {
					return npc->GetSex();
				}
			}
			return RE::SEX::kNone;
		}

		const std::string& get_suffixed_name()
		{
			auto sex = get_sex();
			auto str = string::tolower(owner.text);

			switch (str.back()) {
			case 'a':
				owner.text.back() = 'y';
				break;
			case 'y':
				if (sex == RE::SEX::kMale) {
					owner.text.pop_back();
					owner.text.append("'ego");
				}
				break;
			default:
				if (sex != RE::SEX::kFemale) {
					owner.text.append("a");
				}
				break;
			}

			return owner.text;
		}
	};

	class portuguese : public language
	{
	public:
		using language::language;
		~portuguese() override = default;

		std::string output() override
		{
			return object.text.append(" de ").append(owner.text);
		}
	};

	class spanish : public language
	{
	public:
		using language::language;
		~spanish() override = default;

		std::string output() override
		{
			return object.text.append(" de ").append(owner.text);
		}
	};

	class generic : public language
	{
	public:
		using language::language;
		~generic() override = default;

		std::string output() override
		{
			return object.text.append(" - ").append(owner.text);
		}
	};

	inline std::uint64_t gameLanguageHash;

	void        GetGameLanguageHash();
	std::string GetOutput(RE::TESObjectREFR* a_owner, RE::TESBoundObject* a_object, const srell::smatch& a_match);
}
