#include "pch.h"

#include "menu/menu.h"
#include "fasta/fasta.h"
#include "input/input.h"
#include "settings/settings.h"
#include "permutator/permutator.h"

namespace
{
	std::string RetrieveInput()
	{
		std::string input;
		while (true)
		{
			std::getline(std::cin, input);

			if (input.length() >= 80)
			{
				std::cout << "[!] The sequence is longer than 80 bases. This might cause writing errors." << '\n';
				std::cin.clear();
				break;
			}

			if (input.empty())
			{
				std::cout << "The sequence is empty. Please try again." << '\n';
				std::cin.clear();
				std::cout << '\n';
				dgn::GetInput();
				continue;
			}
			else break;
		}

		return input;
	}

	bool RepeatOperation()
	{
		std::string choice;
		std::getline(std::cin, choice);

		for (char& c : choice)
			c = toupper(c);

		if (!choice.find("N") || !choice.find("NAO") || !choice.find("NO"))
			return false;
		else
			return true;
	}

	bool ParseCommands()
	{
		using namespace dgn;

		auto to_lower = [](std::string& str) { for (char& c : str) c = std::tolower(c); return str; };

		if (to_lower(Data::sequence) == "help")
		{
			HelpMenu();
			return true;
		}
		else if (to_lower(Data::sequence) == "bases" || to_lower(Data::sequence) == "base")
		{
			BaseList();
			return true;
		}
		else
			return false;
	}
}

int main()
{
	using namespace dgn;

	Settings::CreateDefault();
	Settings::ReadSettings();

	Fasta::CreateDefault();

	while (true)
	{
		Header();
		ShowInformation();

		GetInput();

		Data::sequence = RetrieveInput();

		if (ParseCommands())
			continue;

		Input::SanitizeInput();

		Permutator::LazyPermutation();

		DebugInfo();

		PostExecution();

		Data::Clear();

		if (!RepeatOperation())
			break;
	}
	
	Finalize();
	
	std::cin.get();
	return 0;
}
