#pragma once

#include <chrono>
#include <tuple>
#include <type_traits>
#include <sstream>

namespace Measure
{
	struct MoreThanOnce;

	template <typename Job>
	std::chrono::nanoseconds duration(Job && job);

	template<typename ...Durations, typename DurationIn>
	std::tuple<Durations...> explodeDurations(DurationIn d);

	template<typename ...Durations, typename DurationIn>
	std::string format(DurationIn time, char separator = ':');

	std::string format(std::chrono::nanoseconds time)
	{
		return format<std::chrono::hours, std::chrono::minutes, std::chrono::seconds, std::chrono::milliseconds, std::chrono::microseconds>(time);
	}

	template <typename Job>
	std::chrono::nanoseconds duration(Job && job)
	{
		const auto start = std::chrono::steady_clock::now();
		job();
		return std::chrono::steady_clock::now() - start;
	}

	template<typename ...Durations, typename DurationIn>
	std::string format(DurationIn time, char separator)
	{
		auto durations = explodeDurations<Durations...>(time);
		MoreThanOnce hasFirst;
		auto withDelim = [&](auto val) {
			return hasFirst ? val + separator : val;
		};
		auto sum = [&](auto const&... e)->decltype(auto) {
			return (withDelim(std::to_string(e.count())) + ...);
		};
		return std::apply(sum, durations);
	}

	template<typename ...Durations, typename DurationIn>
	std::tuple<Durations...> explodeDurations(DurationIn d) {
		std::tuple<Durations...> retval;
		using discard = int[];
		(void)discard {
			0, (void((
				(std::get<Durations>(retval) = std::chrono::duration_cast<Durations>(d)),
				(d -= std::chrono::duration_cast<DurationIn>(std::get<Durations>(retval)))
			)), 0)...
		};
		return retval;
	}

	struct MoreThanOnce
	{
		operator bool()
		{
			if (!val)
			{
				val = true;
				return false;
			}

			return true;
		}
	private:
		bool val = false;
	};
}