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

			if (input.empty())
			{
				std::cout << "The sequence is empty. Please try again." << '\n';
				std::cin.clear();
				std::cout << '\n';
				dgn::GetInput();
				continue;
			}

			else 
				break;
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

	void HandleExecution()
	{
		using namespace dgn;

		// SBI:
		if (!Data::anyDegenerate)
			return Permutator::SimpleBaseInsertion();

		// Preparing for lazy cartesian:
		Permutator::Prepare();

		// Fasta file:
		auto dir = Settings::Get("results", "directory") + Settings::Get("results", "prefix") + Permutator::CountFiles() + "." + Settings::Get("results", "format");
		Fasta::Open(dir);

		// Execution & Thread handling::
		high_resolution_clock::time_point begin = high_resolution_clock::now();

		int amountThreads = std::stoi(Settings::Get("multithreading", "threads"));
		if (amountThreads == 0 || Data::cartesianSize < (2 << 4))
			Permutator::LazyPermutation();
		else
		{
			if (amountThreads == -1)
				amountThreads = std::thread::hardware_concurrency();

			std::vector<std::thread> threads;

			const size_t step = Data::cartesianSize / amountThreads;
			for (size_t i = 0; i < amountThreads; i++)
				threads.push_back(std::thread(Permutator::LazyPermutation, i * step, (i + 1) * step));

			for (auto& thread : threads)
			{
				if (thread.joinable())
					thread.join();
			}
		}

		Data::permutationTime = duration_cast<nanoseconds>(high_resolution_clock::now() - begin).count() / 1000000;

		Fasta::Close();

		std::cout << std::dec << '\n';
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
		Settings::ReadSettings();

		// Headers:
		Header();
		ShowInformation();

		// Input & Command:
		GetInput();
		Data::sequence = RetrieveInput();
		if (ParseCommands())
			continue;

		Input::SanitizeInput();

		if (Data::invalidSequence)
		{
			InvalidSequence();
			continue;
		}

		// Warning:
		if (Data::sequence.size() > Data::warningCartesianSize)
			CartesianSizeWarning();

		// Permutations:
		HandleExecution();

		// Post:
		DebugInfo();

		// Unmatched Error:
		if (Data::cartesianSize != Data::outcomes)
			UnmatchedBasesErrors();

		PostExecution();

		Data::Clear();

		if (!RepeatOperation())
			break;
	}
	
	Finalize();
	
	std::cin.get();
	return 0;
}
