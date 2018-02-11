#include "stdafx.h"
#include <math.h>
#include <string>
#include <random>
#include <iostream>

using namespace std;

template<typename T>
bool equals(T a, T b)
{
    return std::abs(a - b) < std::numeric_limits<T>::epsilon();
}

int main()
{
    mt19937 randomEngine((random_device()()));
    std::uniform_real_distribution<float> pointDistribution(-1.f, 1.f);

    int numberOfIterations = 0;

    cout << "Enter iterations count: ";
    cin >> numberOfIterations;
    cout << endl;

    int count = 0;
    for (int i = 0; i < numberOfIterations; ++i)
    {
        auto x = pointDistribution(randomEngine);
        auto y = pointDistribution(randomEngine);
        auto z = std::sqrt(x * x + y * y);

        if (z < 1 || equals(z, decltype(z)(1)))
        {
            ++count;
        }
    }

    auto pi = double(count) / numberOfIterations * 4;
    cout << "Iterations: " << numberOfIterations << ", pi: " << pi << endl;
}