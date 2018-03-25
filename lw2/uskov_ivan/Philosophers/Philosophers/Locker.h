#pragma once

namespace async
{
    class Locker
    {
    public:
        virtual ~Locker() = default;
        virtual void lock() = 0;
        virtual void unlock() = 0;
    };
}