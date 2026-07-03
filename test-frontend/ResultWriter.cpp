// ResultWriter.cpp
//
// Dumps the results of a test run.
//
// This sucks and should probably be reworked. :^(

#include "ResultWriter.h"

void ResultWriter::WriteHtml(const std::filesystem::path& filename, const std::set<TestResult>& results) {
    std::ofstream out(filename, std::ios_base::trunc);

    const auto testTotal = results.size();
    auto testFailed = 0;
    for(const auto& result : results) {
        if(!result.Success) ++testFailed;
    }
    const auto testSuccess = testTotal - testFailed;

    // Header
    out << R"==(<!DOCTYPE html>
<html>
<head>
    <style>
        body {
            background-color: #111;
            color: #eee;
            font-family: sans-serif;
            box-sizing: border-box;
        }
        table { border-collapse: collapse; }
        table.test { width: 100%; border: 1px solid #333; }
        table.test, table.summary { margin-bottom: 1.25em; }
        table.test > thead tr th, table.test > thead tr td { width: 12.5%; }
        thead { background-color: #333; }
        table.success thead, tr.success { background-color: #373; }
        table.failure thead, tr.failure { background-color: #933; }
        thead th { padding-bottom: 0; text-align: left; }
        thead td { padding-top: 0; text-align: left; }
        .result.expected { text-align: right; }
        .result.expected > table { margin-left: auto; margin-right: 0; }
        .reg-pass th { background-color: #393; }
        .reg-pass td { background-color: #363; }
        .reg-fail th { background-color: #933; }
        .reg-fail td { background-color: #633; }
        th, td { padding: 4px 8px; }
        tbody th { background-color: #333; }
        tbody td, tbody th { border: 1px solid #333; }
        a { color: white; }
        img { image-rendering: pixelated; }
        .big-warning { background-color: #211; text-align: center; padding: 1em; color: #FFF; font-size: 24px; font-weight: bold; }
    </style>
</head>

<body>
    <h1>plip-test-frontend Results</h1>
    <table class="summary">
        <tr>
            <th>Total Tests</th>
            <td>)==" << testTotal << R"==(</td>
        </tr>
        <tr class="reg-pass">
            <th>Passed</th>
            <td>)==" << testSuccess << R"==(</td>
        </tr>
        <tr class="reg-fail">
            <th>Failed</th>
            <td>)==" << testFailed << R"==(</td>
        </tr>
    </table>

    <h2>Summary</h2>
    <table class="summary">
        <thead>
            <tr>
                <th>Core</th>
                <th>Suite</th>
                <th>Name</th>
                <th>Subtest</th>
            </tr>
        </thead>)==";

    // Summary
    for(const auto& result : results) {
        const std::string cl = result.Success ? "success" : "failure";
        out << R"==(
        <tr class=")==" << cl << R"==(">
            <td>)==" << result.Test.Core << R"==(</td>
            <td>)==" << result.Test.Suite << R"==(</td>
            <td><a href="#)==" << result.Key << "\">" << result.Test.Name << R"==(</a></td>
            <td>)==" << result.GetSubtest().Name << R"==(</td>
        </tr>)==";
    }
    out << R"==(
    </table>

    <h2>Failures</h2>)==";

    // Failures (detailed)
    for(const auto& result : results) {
        if(result.Success) continue;
        WriteDetailedResult(out, result);
    }

    // Connecting nonsense. :p
    out << R"==(
    <h2>Successes</h2>)==";

    // Successes (detailed)
    for(const auto& result : results) {
        if(!result.Success) continue;
        WriteDetailedResult(out, result);
    }

    // Footer
    out << R"==(</body>
</html>
)==";

    // fin
    out.close();
}

void ResultWriter::WriteDetailedResult(std::ofstream& out, const TestResult& result) {
    const std::string cl = result.Success ? "success" : "failure";

    out << R"==(
    <table class="test )==" << cl << "\" id=\"" << result.Key << R"==(">
        <thead>
            <tr>
                <th colspan="2">Core</th>
                <th colspan="2">Path</th>
                <th colspan="2">Name</th>
                <th colspan="2">Subtest</th>
            </tr>
            <tr>
                <td colspan="2">)==" << result.Test.Core << R"==(</td>
                <td colspan="2">)==" << result.Test.Filename << R"==(</td>
                <td colspan="2">)==" << result.Test.Name << R"==(</td>
                <td colspan="2">)==" << result.GetSubtest().Name << R"==(</td>
            </tr>
        </thead>
        <tbody>
            <tr>
                <th colspan="4">Expected</th>
                <th colspan="4">Actual</th>
            </tr>
            <tr>)==";

    if(result.TimedOut) {
        out << R"==(
                <td colspan="8" class="big-warning">Timed out!</td>)==";
    } else if(result.ErrorOccurred()) {
        out << R"==(
                <td colspan="8" class="big-warning">
)==";

        if(!result.InitializationError.empty()) out << "<div>" << result.InitializationError << "</div>\n";
        if(!result.ComparisonError.empty()) out << "<div>" << result.ComparisonError << "</div>\n";
        if(!result.RunnerError.empty()) out << "<div>" << result.RunnerError << "</div>\n";

        out << R"==(
                </td>)==";
    } else {
        switch(result.Expected().Type) {
            case ResultType::Image:
                WriteImageResult(out, result.Key);
                break;
            case ResultType::Registers:
                WriteRegisterResults(out, result.Expected().Value.ValueRegs, result.Expected().Value.ValueRegs);
                break;
            case ResultType::None:
            default:
                // Just in case it somehow managed to get through the first few checks...
                out << R"(<td colspan="8" class="big-warning">BUG: UNEXPECTED RESULT TYPE</td>)";
                break;
        }
    }

    out << R"==(
            </tr>
        </tbody>
    </table>
)==";
}

void ResultWriter::WriteImageResult(std::ofstream& out, const std::string& key) {
    out << R"==(
                <td colspan="4" class="result expected">
                    <img src="images/)==" << key << R"==(-expected.png" />
                </td>
                <td colspan="4" class="result actual">
                    <img src="images/)==" << key << R"==(-actual.png" />
                </td>)==";
}

void ResultWriter::WriteRegisterResults(std::ofstream& out, const std::map<std::string, uint64_t>& expected, const std::map<std::string, uint64_t>& actual) {
    out << R"==(
                <td colspan="4" class="result expected">
                    <table class="reg-result">)==";

    for(const auto& [ reg,  val ] : expected) {
        out << R"==(
                        <tr>
                            <th>)==" << reg << R"==(</th>
                            <td>)==" << val << R"==(</td>
                        </tr>)==";
    }

    out << R"==(
                    </table>
                </td>
                <td colspan="4" class="result actual">
                    <table class="reg-result">)==";

    for(const auto& [ reg,  val ] : actual) {
        // Only print it if there's an equivalent in the expected list.
        if(!expected.contains(reg)) continue;

        const auto expectedVal(expected.find(reg));
        const std::string cl = (expectedVal->second != val) ? "reg-fail" : "reg-pass";

        out << R"==(
                        <tr class=")==" << cl << R"==(">
                            <th>)==" << reg << R"==(</th>
                            <td>)==" << val << R"==(</td>
                        </tr>)==";
    }

    out << R"==(
                    </table>
                </td>)==";
}
