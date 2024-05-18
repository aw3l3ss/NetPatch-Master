#ifndef APT_PACKAGE_H
#define APT_PACKAGE_H

#include <string>

class AptPackage {
    private:
        std::string name;
        std::string version;
    
    public:
        AptPackage(std::string name, std::string version);
        std::string get_name();
        std::string get_version();
};

#endif 