#include "python_api.h"
#include <array>
#include <memory>
#include <stdexcept>
#include <cstdio>

std::vector<std::string> execute_python_script(const std::string filename, const std::string arguments) {
    std::array<char, 256> buffer;
    std::vector<std::string> result;
    std::string command = "python3 " + filename + " " + arguments;

    std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(command.c_str(), "r"), pclose);

    if (!pipe) {
        throw std::runtime_error("popen() failed!");
    }

    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
        result.push_back(buffer.data());
    }

    return result;
}