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

		static void SimpleBaseInsertion();

		static void Prepare();

		static std::string CountFiles();

	private:
		static unsigned char LazyCartesian(least j, least i);

	private:
		static inline std::vector<std::vector<unsigned char>> s_Sets = {};

		static inline std::vector<size_t> s_Moduli = {};

		static inline std::vector<int> s_Divisions = {};

	private:
		static inline std::mutex mPermutatorMutex;
	};
}