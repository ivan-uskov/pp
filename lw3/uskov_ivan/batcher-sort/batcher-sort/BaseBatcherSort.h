#pragma once

#include <type_traits>
#include <cassert>
#include <iterator>
#include <stdexcept>
#include <utility>
#include <functional>

namespace Batcher
{
	namespace Impl
	{
		bool IsPowerOfTwo(size_t x)
		{
			return (x > 0) && (x & (x - 1)) == 0;
		}

		template <typename Collection>
		class BaseSorter
		{
		public:
			using ElementType = typename Collection::value_type;
			using Comparator = std::function<bool(ElementType, ElementType)>;

			BaseSorter(Collection & collection, Comparator && comparator)
				: mCollection(collection)
				, mComparator(comparator)
			{
				if (!IsPowerOfTwo(collection.size()))
				{
					throw std::logic_error("Collection size should be power of 2 and not empty");
				}
			}
			virtual ~BaseSorter() = default;

			void sort()
			{
				auto size = mCollection.size();
				for (size_t p = size; p > 0; p >>= 1)
				{
					size_t q = size;
					size_t r = 0;
					size_t d = p;

					bool sorting = true;
					while (sorting)
					{
						size_t subRangeSize = size - d;
						WalkAndExchange(subRangeSize, [=](auto i) {
							if ((i & p) == r)
							{
								CompareExchange(i, i + d);
							}
						});

						sorting = (q != p);
						if (sorting)
						{
							d = q - p;
							q >>= 1;
							r = p;
						}
					}
				}
			}

		protected:
			virtual void WalkAndExchange(size_t size, std::function<void(size_t)> && exchanger) = 0;

			Collection & mCollection;

		private:
			void CompareExchange(size_t li, size_t ri)
			{
				if (mComparator(mCollection[ri], mCollection[li]))
				{
					std::swap(mCollection[li], mCollection[ri]);
				}
			}

			Comparator  mComparator;
		};
	};
}