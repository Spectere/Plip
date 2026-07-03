// ResultWriter.h
//
// Dumps the results of a test run.

#pragma once

#include <filesystem>
#include <fstream>
#include <map>
#include <set>

#include "Models/TestResults.h"

class ResultWriter {
public:
    static void WriteHtml(const std::filesystem::path& filename, const std::set<TestResult>& results);

private:
    static void WriteDetailedResult(std::ofstream& out, const TestResult& result);
    static void WriteImageResult(std::ofstream& out, const std::string& key, bool onlyActual = false);
    static void WriteRegisterResults(std::ofstream& out, const std::map<std::string, uint64_t>& expected, const std::map<std::string, uint64_t>& actual);
};
