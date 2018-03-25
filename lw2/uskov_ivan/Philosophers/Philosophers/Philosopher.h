#pragma once
#include <string>
#include "Writer.h"

namespace
{
    constexpr DWORD EATING_TIME = 1000;
    constexpr DWORD THINKING_TIME = 2000;
}

class Philosopher final
{
public:
    Philosopher(std::string name, std::pair<async::Locker&, async::Locker&> forks, async::Writer & w)
        : name(name)
        , mLogWriter(w)
        , mForks(forks)
    {}

    void run()
    {
        log("sat at the table");
        while (1)
        {
            {
                mForks.first.lock();
                log("get left fork");
                mForks.second.lock();
                log("get right fork");
                log("eating");
                Sleep(EATING_TIME);
                log("stop eating");
                mForks.second.unlock();
                mForks.first.unlock();
            }
            log("thinking");
            Sleep(THINKING_TIME);
        }
    }

private:
    Philosopher(const Philosopher&) = delete;
    Philosopher& operator=(const Philosopher&) = delete;

    void log(std::string str)
    {
        mLogWriter << (name + " " + str + "\n");
    }

    std::string name;
    async::Writer & mLogWriter;
    std::pair<async::Locker&, async::Locker&> mForks;
};