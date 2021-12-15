#pragma once
#include "pch.h"
#include "../data.h"

namespace dgn
{
	class Input
	{
	public:
		static void SanitizeInput()
		{
			EmptySequence();

			if (Data::invalidSequence)
				return;

			Capitalize();

			PurgeExcess();

			CountBases();

			AnyDegenerate();

			ComputeCartesianSize();
		}

	private:
		static void EmptySequence() { Data::invalidSequence = Data::sequence.empty(); }

		static void AnyDegenerate()
		{
			Data::anyDegenerate = ( Data::baseSizes[4] > 0 ||  Data::baseSizes[3] > 0 || Data::baseSizes[2] > 0 );
		}

		static void Capitalize() { std::transform(Data::sequence.begin(), Data::sequence.end(), Data::sequence.begin(), ::toupper); }

		static void PurgeExcess()
		{
			using namespace std;

			// Getting only the bases:
			std::vector<char> bases;
			for (const auto& base : Data::validBases)
				bases.push_back(base.first);

			// Removing invalid bases:
			Data::sequence.erase(remove_if(Data::sequence.begin(), Data::sequence.end(),
				[&bases](char& c) {
					return !(find(bases.begin(), bases.end(), c) != bases.end());
				}),
			Data::sequence.end());

			if (Data::sequence.empty())
				Data::invalidSequence = true;
		}

		static void CountBases()
		{
			for (const auto& character : Data::sequence)
			{
				for (const auto& base : Data::validBases)
				{
					if (character == base.first)
						Data::baseSizes[base.second] += 1;
				}
			}
		}

		static void ComputeCartesianSize()
		{
			size_t size = 1;

			if (Data::baseSizes[4] > 0) size *= (size_t)pow(4, Data::baseSizes[4]);
			if (Data::baseSizes[3] > 0)	size *= (size_t)pow(3, Data::baseSizes[3]);
			if (Data::baseSizes[2] > 0)	size *= (size_t)pow(2, Data::baseSizes[2]);

			Data::cartesianSize = size;
		}
	};
}