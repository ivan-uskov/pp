#pragma once

#include "Locker.h"

namespace async
{
    class Guard final
    {
    public:
        Guard(Locker & l)
            : mLocker(l)
        {
            mLocker.lock();
        }

        ~Guard()
        {
            mLocker.unlock();
        }

    private:
        Locker & mLocker;
    };
}