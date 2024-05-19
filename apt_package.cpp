#include "apt_package.h"
#include <stdio.h>
#include <sstream>

AptPackage::AptPackage(std::string name, std::string version) {
    this->name = name;
    this->version = version;
}

std::string AptPackage::get_name() {
    return name;
}

std::string AptPackage::get_version() {
    return version;
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
        std::string version_tmp;
        std::string version = "";

        std::istringstream iss(lines[i]);
        std::getline(iss, name, '/');
        std::getline(iss, tmp, ' ');
        std::getline(iss, version_tmp, ' ');

        for (int j = 0; j < version_tmp.length(); j++) {
            if (version_tmp[j] == ':') {
                version = "";
            }

            else if (version_tmp[j] == '+' || version_tmp[j] == '-' || version_tmp[j] == '~') {
                break;
            }

            else {
                version += version_tmp[j];
            }
        }

        AptPackage apt_package(name, version);
        apt_packages.push_back(apt_package);
    }

    return apt_packages;
}