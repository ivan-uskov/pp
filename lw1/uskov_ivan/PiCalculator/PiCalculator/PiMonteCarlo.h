#pragma once
#include <type_traits>
#include <random>

namespace PIMonteCarlo
{
    class Estimator final
    {
    public:
        float const MAX_DISTANCE = 1.f;
        float const MAX_POINT_VALUE = 1.f;
        float const MIN_POINT_VALUE = -1.f;

        Estimator(unsigned numberOfIterations)
            : mRandomEngine((std::random_device()()))
            , mPointDistribution(MIN_POINT_VALUE, MAX_POINT_VALUE)
            , mNumberOfIterations(numberOfIterations)
        {}

        unsigned estimate()
        {
            unsigned count = 0;
            while (mNumberOfIterations-- > 0)
            {
                auto x = mPointDistribution(mRandomEngine);
                auto y = mPointDistribution(mRandomEngine);
                auto z = std::sqrt(x * x + y * y);

                if (z < MAX_DISTANCE || equals(z, decltype(z)(MAX_DISTANCE)))
                {
                    ++count;
                }
            }

            return count;
        }

    private:
        template<typename T, typename = std::enable_if_t<std::is_arithmetic_v<T>, T>>
        bool equals(T a, T b)
        {
            return std::abs(a - b) < std::numeric_limits<T>::epsilon();
        }

        unsigned mNumberOfIterations;
        std::mt19937 mRandomEngine;
        std::uniform_real_distribution<float> mPointDistribution;
    };

    double calculate(unsigned successTestsCount, unsigned totalTestsCount)
    {
        return double(successTestsCount) / totalTestsCount * 4;
    }

    double calculate(unsigned testsCount)
    {
        Estimator estimator(testsCount);
        return calculate(estimator.estimate(), testsCount);
    }
};