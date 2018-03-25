#pragma once
#include <Windows.h>
#include <stdexcept>
#include <string>
#include <functional>
#include <assert.h>

namespace async
{
    class Thread final
    {
    public:
        using Job = std::function<void()>;

        Thread(Job && proc)
        {
            mJob = move(proc);
            mHandle = CreateThread(nullptr, 0, jobRunner, &mJob, 0, nullptr);
            if (mHandle == nullptr)
            {
                throw std::runtime_error("Run thread failed: " + std::to_string(GetLastError()));
            }
        }

        void wait()
        {
            WaitForSingleObject(mHandle, INFINITE);
        }

        ~Thread()
        {
            auto isClosed = CloseHandle(mHandle);
            assert(isClosed);
        }

    private:
        static DWORD WINAPI jobRunner(LPVOID pThis)
        {
            (*reinterpret_cast<Job*>(pThis))();
            return 0;
        }

        explicit Thread(Thread const&) = delete;
        Thread& operator=(Thread const&) = delete;

        HANDLE mHandle;
        Job mJob;
    };

    template <template<typename, typename> typename T>
    void joinAll(T<std::shared_ptr<Thread>, std::allocator<std::shared_ptr<Thread>>> & threads)
    {
        for (auto & t : threads)
        {
            t->wait();
        }
    }
}