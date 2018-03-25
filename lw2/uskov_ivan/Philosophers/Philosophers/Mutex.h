#pragma once

#include <Windows.h>
#include <stdexcept>
#include <string>

#include "Locker.h"

namespace async
{
    class Mutex final : public Locker
    {
    public:
        Mutex()
        {
            mMutex = CreateMutex(NULL, FALSE, NULL);
            if (mMutex == NULL)
            {
                throw std::runtime_error("Create mutex failed, err: " + std::to_string(GetLastError()));
            }
        }

        void lock() override
        {
            auto res = WaitForSingleObject(mMutex, INFINITE);
            if (res != WAIT_OBJECT_0)
            {
                throw std::runtime_error("lock mutex error");
            }
        }

        void unlock() override
        {
            if (!ReleaseMutex(mMutex))
            {
                throw std::runtime_error("unlock mutex error");
            }
        }

        ~Mutex()
        {
            CloseHandle(mMutex);
        }

    private:
        Mutex(const Mutex&) = delete;
        Mutex& operator=(const Mutex&) = delete;

        HANDLE mMutex;
    };
}