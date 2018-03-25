#pragma once

#include <ostream>
#include <vector>
#include <memory>
#include "Thread.h"
#include "Mutex.h"
#include "Writer.h"

#include "Philosopher.h"

class Table final
{
public:
    Table(std::ostream & logStream)
        : mLogger(logStream)
    {
        mForks.reserve(5);
        for (int i = 0; i < 5; ++i)
        {
            mForks.emplace_back(new async::Mutex());
        }
    }

    void plantPhilosophers()
    {
        mPhilosophers.emplace_back(new Philosopher("Albert Einstein", { *mForks[0], *mForks[1] }, mLogger));
        mPhilosophers.emplace_back(new Philosopher("Tomas Edison", { *mForks[1], *mForks[2] }, mLogger));
        mPhilosophers.emplace_back(new Philosopher("Nikola Tesla", { *mForks[2], *mForks[3] }, mLogger));
        mPhilosophers.emplace_back(new Philosopher("Max Plank", { *mForks[3], *mForks[4] }, mLogger));
        mPhilosophers.emplace_back(new Philosopher("Andre Amper", { *mForks[0], *mForks[4] }, mLogger));
    }

    void start()
    {
        std::vector<std::shared_ptr<async::Thread>> threads;
        for (auto & p : mPhilosophers)
        {
            threads.push_back(std::make_shared<async::Thread>([&p] {
                p->run();
            }));
        }

        async::joinAll(threads);
    }

private:
    async::Writer mLogger;
    std::vector<std::shared_ptr<async::Mutex>> mForks;
    std::vector<std::shared_ptr<Philosopher>> mPhilosophers;
    std::vector<std::shared_ptr<async::Thread>> mThreads;
};