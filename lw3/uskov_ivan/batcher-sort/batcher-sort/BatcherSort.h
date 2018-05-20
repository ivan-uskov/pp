#pragma once

#include "BaseBatcherSort.h"

namespace Batcher
{
	namespace Impl
	{
		template <typename Collection>
		class Sorter : public BaseSorter<Collection>
		{
		public:
			using Comparator = BaseSorter<Collection>::Comparator;
			Sorter(Collection & collection, Comparator && comparator)
				: BaseSorter<Collection>(collection, std::forward<Comparator>(comparator))
			{}

		protected:
			void WalkAndExchange(size_t size, std::function<void(size_t)> && exchanger) override
			{
				for (size_t i = 0; i < size; ++i)
				{
					exchanger(i);
				}
			}
		};
	};

	template <typename Collection, typename Comparator = std::less<>>
	void sort(Collection& array, Comparator comparator = std::less<>())
	{
		Impl::Sorter<Collection>(array, comparator).sort();
	}
}