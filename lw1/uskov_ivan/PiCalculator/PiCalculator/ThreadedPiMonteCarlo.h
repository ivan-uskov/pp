#pragma once
#include <assert.h>
#include <vector>
#include <memory>

#include "PiMonteCarlo.h"
#include "Thread.h"

namespace // private
{
    struct EstimationPart
    {
        unsigned testsCount;
        unsigned successTestCount;
    };

    std::vector<EstimationPart> getParts(unsigned testsCount, unsigned threadsCount)
    {
        std::vector<EstimationPart> parts;
        parts.reserve(threadsCount);
        auto partSize = testsCount / threadsCount;

        while (threadsCount-- > 0)
        {
            if (testsCount > partSize)
            {
                testsCount -= partSize;
            }
            else
            {
                partSize = testsCount;
                testsCount = 0;
            }

            parts.push_back({partSize, 0});
        }

        return parts;
    }
}

namespace ThreadedPIMonteCarlo
{
    double calculate(unsigned testsCount, unsigned threadsCount)
    {
        assert(testsCount > 1);
        if (threadsCount <= 1 || testsCount < threadsCount)
        {
            return PIMonteCarlo::calculate(testsCount);
        }

        auto parts = getParts(testsCount, threadsCount);
        std::vector<std::unique_ptr<Thread>> threads;
        threads.reserve(threadsCount);

        for (auto & part : parts)
        {
            threads.emplace_back(new Thread([&part] {
                PIMonteCarlo::Estimator e(part.testsCount);
                part.successTestCount = e.estimate();
            }));
        }

        unsigned successCount = 0;
        for (size_t i = 0; i < threads.size(); ++i)
        {
            threads[i]->wait();
            successCount += parts[i].successTestCount;
        }

        return calculate(successCount, testsCount);
    }
}