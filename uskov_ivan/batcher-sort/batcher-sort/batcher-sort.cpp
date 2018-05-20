#include "stdafx.h"

#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>

#include "Measure.h"
#include "BatcherSort.h"
#include "ThreadedBatcherSort.h"

using namespace std;

static const int ARGUMENTS_COUNT = 2;

struct Data
{
	vector<int> dataForSingleThreadedSort;
	vector<int> dataForMultiThreadedSort;
};

struct Stats
{
	chrono::nanoseconds singleThreadedDuration;
	chrono::nanoseconds multiThreadedDuration;
};

Data readData(int argc, char ** argv)
{
	if (argc != ARGUMENTS_COUNT)
	{
		throw invalid_argument("Expected argument, input file name");
	}

	vector<int> data;
	ifstream in(argv[1]);
	copy(istream_iterator<int>(in), istream_iterator<int>(), back_inserter(data));

	vector<int> dataForThreadedSort(data);
	return { data, dataForThreadedSort };
}

Stats sort(Data & data)
{
	return {
		Measure::duration([&] { Batcher::sort(data.dataForSingleThreadedSort); }), 
		Measure::duration([&] { Batcher::sortThreaded(data.dataForMultiThreadedSort); })
	};
}

void printResult(Data const& data, Stats const& stats)
{
	cout << "Single threaded time: " << Measure::format(stats.singleThreadedDuration) << endl;
	cout << "Multi threaded time: " << Measure::format(stats.multiThreadedDuration) << endl;

	copy(data.dataForSingleThreadedSort.begin(), data.dataForSingleThreadedSort.end(), ostream_iterator<int>(cout, " "));
	cout << endl;
}

int main(int argc, char ** argv)
{
	try
	{
		auto data = readData(argc, argv);
		auto stats = sort(data);

		printResult(data, stats);
	}
	catch (exception const& e)
	{
		cout << e.what() << endl;
	}

    return 0;
}

