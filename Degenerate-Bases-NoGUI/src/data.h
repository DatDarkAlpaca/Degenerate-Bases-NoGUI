#pragma once
#include "pch.h"

namespace dgn
{
	struct Data
	{
	public:
		static void Clear()
		{
			for (auto& base : baseSizes)
				base.second = 0;

			iterations = 0;
			outcomes = 0;

			permutationTime = 0;
			writeFastaTime = 0;
			sequence = "";

			anyDegenerate = false;
			invalidSequence = false;
		}

	public:
		static const std::unordered_map<char, unsigned short> validBases;
		static std::unordered_map<unsigned short, unsigned int> baseSizes;
		static std::unordered_map<char, std::vector<char>> units;

		static inline size_t cartesianSize = 0;
		static inline std::string sequence = "";

		static inline bool invalidSequence = false;
		static inline bool anyDegenerate = false;

		static inline std::atomic<uint32_t> iterations{ 0 }, outcomes{ 0 };

		static inline std::atomic<long long> permutationTime = 0, writeFastaTime = 0;

		static inline uint32_t warningCartesianSize = (1 << 24);
	};
}