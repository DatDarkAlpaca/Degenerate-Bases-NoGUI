#include "pch.h"
#include "permutator.h"

#include "../data.h"
#include "../menu/menu.h"
#include "../fasta/fasta.h"
#include "../settings/settings.h"

void dgn::Permutator::Preparation()
{
	size_t inputSize = Data::sequence.size();

	s_Sets.clear();
	s_Sets.reserve(inputSize);

	for (size_t i = 0; i < inputSize; i++)
	{
		for (auto& unit : Data::units)
		{
			if (Data::sequence[i] == unit.first)
			{
				switch (unit.second.size())
				{
				case 1:
					s_Sets.push_back({ 0 });
					break;

				case 2:
					s_Sets.push_back({ 0, 1 });
					break;

				case 3:
					s_Sets.push_back({ 0, 1, 2 });
					break;

				case 4:
					s_Sets.push_back({ 0, 1, 2, 3 });
					break;
				}
			}
		}
	}

	size_t setsSize = s_Sets.size();

	s_Divisions.resize(setsSize);
	s_Moduli.resize(setsSize);

	int factor = 1;
	for (int i = setsSize - 1; i >= 0; --i)
	{
		size_t item = s_Sets[i].size();

		s_Divisions.insert(s_Divisions.begin(), factor);
		s_Moduli.insert(s_Moduli.begin(), item);

		factor *= item;
	}
}

unsigned char dgn::Permutator::LazyCartesian(least j, least i)
{
	if ((size_t)j >= Data::cartesianSize)
		throw std::exception("An error has occurred while permutating through the sequence.");

	std::vector<unsigned char> combinations;
	combinations.reserve(Data::sequence.size());

	for (size_t r = 0; r < s_Sets.size(); r++)
	{
		unsigned int a = int(floor(j / s_Divisions[r])) % s_Moduli[r];
		combinations.push_back(s_Sets[r][a]);
	}

	return combinations[i];
}

void dgn::Permutator::SimpleBaseInsertion()
{
	Fasta::Open(Settings::Get("results", "directory") + Settings::Get("results", "prefix") + Permutator::CountFiles() + "." + Settings::Get("results", "format"));
	
	Fasta::Write(Data::sequence);
	
	Fasta::Close();

	SBIUsed();
}

void dgn::Permutator::LazyPermutation(least min, least max)
{
	// SBI:
	if (!Data::anyDegenerate)
		return SimpleBaseInsertion();

	// Lazy algorithm preparation:
	Preparation();

	// Fasta file:
	auto dir = Settings::Get("results", "directory") + Settings::Get("results", "prefix") + Permutator::CountFiles() + "." + Settings::Get("results", "format");
	Fasta::Open(dir);

	// Chunks preparation:
	size_t chunksWritten = 0;
	std::vector<std::string> chunkResults = {};
	size_t chunkSize = std::stoul(Settings::Get("chunks", "chunk_size"));

	if (Settings::ToBool(Settings::Get("debug", "chunk_debug")))
		WriteChunkSize();

	// Reserving the result string:
	std::string result;
	result.reserve(Data::sequence.size());

	high_resolution_clock::time_point begin = high_resolution_clock::now();

	for (size_t j = min; j < max; ++j)
	{
		for (size_t i = 0; i < Data::sequence.size(); ++i)
		{
			auto cartesian = LazyCartesian((least)j, (least)i);

			for (auto& unit : Data::units)
			{
				if (Data::sequence[i] == unit.first)
				{
					result += unit.second[cartesian];
					break;
				}
			}

			Data::iterations++;
		}

		// Writting:
		if (chunkResults.size() == chunkSize)
			InsertChunk(std::move(chunkResults), chunksWritten);

		chunkResults.push_back(result);
		result.clear();

		Data::outcomes++;
	}

	// Chunk finalization:
	if (!chunkResults.empty())
		InsertChunk(std::move(chunkResults), chunksWritten);

	if (Data::cartesianSize != Data::outcomes)
		UnmatchedBasesErrors();

	std::cout << std::dec << '\n';

	Fasta::Close();

	high_resolution_clock::time_point end = high_resolution_clock::now();

	Data::permutationTime = duration_cast<nanoseconds>(high_resolution_clock::now() - begin).count() / 1000000;
}
