#include "apt_package.h"
#include <stdio.h>

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