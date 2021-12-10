#pragma once
#include "pch.h"
#include "../data.h"
#include "../menu/menu.h"
#include "../fasta/fasta.h"

namespace dgn
{
	class Permutator
	{
	private:
		typedef uint_least32_t least;

	public:
		static void LazyPermutation(least min = 0, least max = Data::cartesianSize);

	private:
		static unsigned char LazyCartesian(least j, least i);

		static void SimpleBaseInsertion();

		static void Preparation();

	private:
		static void InsertChunk(std::vector<std::string>&& chunkResults, size_t& chunksWritten)
		{
			high_resolution_clock::time_point begin = high_resolution_clock::now();
			Fasta::Write(std::move(chunkResults));
			high_resolution_clock::time_point end = high_resolution_clock::now();

			Data::writeFastaTime += duration_cast<nanoseconds>(high_resolution_clock::now() - begin).count() / 1000000;

			if (Settings::ToBool(Settings::Get("debug", "enable")))
				WriteChunk(chunksWritten);

			chunksWritten++;
			chunkResults.clear();
		}

		static std::string CountFiles()
		{
			auto dirIter = std::filesystem::directory_iterator("results");

			int fileCount = std::count_if(begin(dirIter), end(dirIter),
				[](auto& entry) { return entry.is_regular_file(); }
			);

			return std::to_string(fileCount);
		}

	private:
		static inline std::vector<std::vector<unsigned char>> s_Sets = {};

		static inline std::vector<size_t> s_Moduli = {};

		static inline std::vector<int> s_Divisions = {};
	};
}