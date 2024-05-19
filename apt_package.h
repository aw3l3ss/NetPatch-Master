#ifndef APT_PACKAGE_H
#define APT_PACKAGE_H

#include <string>
#include <vector>

class AptPackage {
    private:
        std::string name;
        std::string version;
    
    public:
        AptPackage(std::string name, std::string version);
        std::string get_name();
        std::string get_version();
};

std::vector<AptPackage> get_packages(const char *filename);

#endif 