#pragma once
#include "pch.h"
#include "../data.h"
#include "../settings/settings.h"

namespace dgn
{
	inline void Header()
	{
		std::cout << "-= Degenerate Bases : v4.6f = -" << '\n';
		std::cout << "Author: Paulo C." << '\n' << '\n';
	}
	
	inline void ShowInformation()
	{
		std::cout << "[!] For more information, write 'help' and press Enter. If you need a list of all available bases, write 'bases'"
			<< '\n' << '\n';
	}

	inline void HelpMenu()
	{
		std::cout << '\n';

		std::cout << "Please input a sequence containing degenerate bases. It can have spaces, uppercase, and lowercase letters." << '\n';
		std::cout << "If any base is invalid, the program will inform you." << '\n';
		std::cout << "To list all possible bases, type 'bases' then press Enter." << '\n';

		std::cout << '\n';
	}

	inline void BaseList()
	{
		std::cout << '\n';
		std::cout << "The following list contains all available degenerate bases." << '\n' << '\n';

		std::cout << "N - A, C, T, G" << '\n' << '\n';

		std::cout << "D - A, G, T" << '\n';
		std::cout << "V - A, C, G" << '\n';
		std::cout << "B - C, T, G" << '\n';
		std::cout << "H - A, C, T" << '\n' << '\n';

		std::cout << "W - A, T" << '\n';
		std::cout << "S - C, G" << '\n';
		std::cout << "K - G, T" << '\n';
		std::cout << "M - A, C" << '\n';
		std::cout << "Y - C, T" << '\n';
		std::cout << "R - A, G" << '\n' << '\n';

		std::cout << "Normal Bases: A, C, G, T" << '\n' << '\n';

		std::cout << "Gaps: -, ." << '\n';
		std::cout << "Additional: *" << '\n';

		std::cout << '\n';
	}

	inline void GetInput()
	{
		std::cout << "Insert a sequence: ";
	}

	inline void SBIUsed()
	{
		std::cout << "[!] No degenerate bases were detected. Using SBI." << '\n';
		std::cout << "\n";
	}

	inline void DebugInfo()
	{
		if (!Settings::ToBool(Settings::Get("debug", "enable")))
			return;

		else
		{
			std::cout << "-=-=-=-= Debug Info. =-=-=-=-" << '\n';
			std::cout << "Base sequence's length  :   " << Data::sequence.size() << '\n';
			std::cout << "Amount of outcomes      :   " << Data::outcomes << '\n';
			std::cout << "Amount of iterations    :   " << Data::iterations << '\n';
			std::cout << '\n';
			std::cout << "Total Iteration time    :   " << Data::permutationTime << "ms" << '\n';
			std::cout << "File writing time       :   " << Data::writeFastaTime << "ms" << '\n';
			std::cout << '\n';
		}
	}

	inline void PostExecution()
	{
		std::cout << "[!] The sequence has been successfully computed and written to a fasta file" << '\n' << '\n';

		std::cout << "Would you like to enter another base? (N/Y): ";
	}

	inline void Finalize()
	{
		std::cout << "Bye bye!" << '\n';
	}

	inline void UnmatchedBasesErrors()
	{
		std::cout << "[!] A lethal error has ocurred. \
			 The estimated amount of outcomes does not match the real amount of outcomes." << '\n';
	}

	inline void CartesianSizeWarning()
	{
		std::cout << "[!] The input sequence will result in a considerable amount of outcomes. (" << Data::cartesianSize << ")" << '\n';
		std::cout << "    It may take a long time to compute this sequence." << '\n';
	}
}
