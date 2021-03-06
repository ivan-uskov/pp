#include "stdafx.h"
#include <iostream>

#include "ThreadedPiMonteCarlo.h"
#include "ArgumentsParser.h"

using namespace std;

void printHelp(ostream & out)
{
    out << endl
        << "PI Calculator, evaluates PI number with Monte Carlo Method" << endl
        << "Options:" << endl
        << "  --threads=<threadsCount> : <optional default 1> threads count, should be between 1 and 64" << endl
        << "  --tests=<testsCount>     : <required> tests count (monte carlo iterations) should be between 1 and " + to_string(ArgumentsParser::MAX_TESTS_COUNT) << endl;
}

int main(int argc, char** argv)
{
    try
    {
        auto args = ArgumentsParser().parseArguments(vector<string>(argv, argv + argc));
        if (args.hasHelpArgument)
        {
            printHelp(cout);
        }
        else
        {
            cout << ThreadedPIMonteCarlo::calculate(args.numberOfTests, args.numberOfThreads) << endl;
        }
    }
    catch (ArgumentsParser::BadCliArgumentException e)
    {
        cout << e.what() << endl;
        printHelp(cout);
    }
    catch (exception e)
    {
        cout << e.what() << endl;
        return 1;
    }
}