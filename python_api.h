#ifndef PYTHON_API_H
#define PYTHON_API_H

#include <string>
#include <vector>

std::vector<std::string> execute_python_script(const std::string filename, const std::string arguments = "");

#endif 