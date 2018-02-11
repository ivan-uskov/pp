#pragma once
#include <vector>
#include <regex>
#include <stdexcept>

class ArgumentsParser
{
public:
    static const unsigned MAX_TESTS_COUNT = 100000000;
    static const unsigned MAX_THREADS_COUNT = 64;

    struct Arguments
    {
        bool hasHelpArgument = false;
        unsigned numberOfTests = 0;
        unsigned numberOfThreads = 0;
    };

    struct BadCliArgumentException : std::invalid_argument
    {
        BadCliArgumentException(std::string && s)
            : invalid_argument(s)
        {}
    };

    Arguments parseArguments(const std::vector<std::string> & args)
    {
        ensureArgumentsCountCorrect(args.size());
        auto parsedArguments = fetchArguments(args);
        ensureArgumentsValid(parsedArguments);

        return parsedArguments;
    }

private:
    void ensureArgumentsValid(const Arguments & args)
    {
        if (!args.hasHelpArgument && args.numberOfTests == 0)
        {
            throw BadCliArgumentException("expected tests count");
        }
    }

    Arguments fetchArguments(const std::vector<std::string> & args)
    {
        Arguments parsedArgs;
        for (size_t i = 1; i < args.size(); ++i)
        {
            auto arg = args[i];
            if (arg == "--help")
            {
                parsedArgs.hasHelpArgument = true;
                break;
            }

            auto result = parseThreadsCount(arg);
            if (result.second)
            {
                parsedArgs.numberOfThreads = result.first;
                continue;
            }

            result = parseTestsCount(arg);
            if (result.second)
            {
                parsedArgs.numberOfTests = result.first;
                continue;
            }

            throw BadCliArgumentException("Unexpected argument: " + arg);
        }

        return parsedArgs;
    }

    void ensureArgumentsCountCorrect(size_t argc)
    {
        if (argc < 2 || argc > 3)
        {
            throw BadCliArgumentException("bad argument count: " + std::to_string(argc));
        }
    }

    std::pair<unsigned, bool> parseThreadsCount(std::string arg)
    {
        std::smatch sm;
        if (!std::regex_match(arg, sm, mThreadsCountRegex))
        {
            return { 0, false };
        }

        auto threadsCount = std::stoul(sm[1]);
        if (threadsCount == 0 || threadsCount > MAX_THREADS_COUNT)
        {
            throw BadCliArgumentException("Invalid threads count: " + std::to_string(threadsCount));
        }

        return { threadsCount, true };
    }

    std::pair<unsigned, bool> parseTestsCount(std::string arg)
    {
        std::smatch sm;
        if (!std::regex_match(arg, sm, mTestsCountRegex))
        {
            return { 0, false };
        }

        unsigned long testsCount = std::stoul(sm[1]);
        if (testsCount == 0 || testsCount > MAX_TESTS_COUNT)
        {
            throw BadCliArgumentException("Invalid tests count: " + std::to_string(testsCount));
        }

        return { testsCount, true };
    }

private:
    std::regex mTestsCountRegex = std::regex("^--tests=([0-9]+)$");
    std::regex mThreadsCountRegex = std::regex("^--threads=([0-9]+)$");
};