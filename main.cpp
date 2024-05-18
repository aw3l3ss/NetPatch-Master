#include <iostream>
#include <vector>
#include <sstream>

#include "server.h"
#include "cve.h"
#include "apt_package.h"

std::vector<AptPackage> get_packages(const char *filename);

int main() {
    Server server("92.38.255.48", "root", "2guGa0&D%1w3", 22);
    server.connect();
    server.execute_command("apt list --installed", "test.bin");
    server.disconnect();

    std::vector<AptPackage> apt_packages = get_packages("test.bin");

    

    return 0;
}

std::vector<AptPackage> get_packages(const char *filename) {
    FILE *file = fopen(filename, "rb");

    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    fseek(file, 0, SEEK_SET);

    char *buffer = new char[size + 1];
    fread(buffer, 1, size, file);
    fclose(file);

    buffer[size] = '\0';

    std::vector<std::string> lines;
    std::string line;
    std::istringstream iss(buffer);

    while (std::getline(iss, line)) {
        lines.push_back(line);
    }

    delete[] buffer;

    std::vector<AptPackage> apt_packages;

    for (int i = 1; i < lines.size(); i++) {
        std::string name;
        std::string tmp;
        std::string version;

        std::istringstream iss(lines[i]);
        std::getline(iss, name, '/');
        std::getline(iss, tmp, ' ');
        std::getline(iss, version, ' ');

        AptPackage apt_package(name, version);
        apt_packages.push_back(apt_package);
    }

    return apt_packages;
}