#pragma once

#include "BaseBatcherSort.h"

namespace Batcher
{
	namespace Impl
	{
		template <typename Collection>
		class ThreadedSorter : public BaseSorter<Collection>
		{
		public:
			using Comparator = BaseSorter<Collection>::Comparator;
			ThreadedSorter(Collection & collection, Comparator && comparator)
				: BaseSorter<Collection>(collection, std::forward<Comparator>(comparator))
			{}

		protected:
			void WalkAndExchange(size_t size, std::function<void(size_t)> && exchanger) override
			{
#pragma omp parallel for
				for (long i = 0; i < size; ++i)
				{
					exchanger(i);
				}
			}
		};
	};

	template <typename Collection, typename Comparator = std::less<>>
	void sortThreaded(Collection& array, Comparator comparator = std::less<>())
	{
		Impl::ThreadedSorter<Collection>(array, comparator).sort();
	}
}