#pragma once

#include <ostream>
#include "Mutex.h"
#include "Guard.h"

namespace async
{
    class Writer
    {
    public:
        Writer(std::ostream & out)
            : mOut(out)
        {}
    private:
        template <typename T>
        friend Writer & operator << (Writer & writer, T val);

        std::ostream & mOut;
        Mutex mMutex;
    };

    template <typename T>
    Writer & operator << (Writer & writer, T val)
    {
        Guard g(writer.mMutex);
        writer.mOut << val;
        return writer;
    }
}