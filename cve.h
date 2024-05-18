#ifndef CVE_LIST_H
#define CVE_LIST_H

#include "apt_package.h"
#include <string>

class Cve {
    private:
        std::string id;
        std::string summary;
        std::string cvss4;
        std::string cvss3;
        std::string cvss2;
    
    public:
        Cve(std::string id, std::string summary, std::string cvss4, std::string cvss3, std::string cvss2);
        std::string get_id();
        std::string get_summary();
        std::string get_cvss4();
        std::string get_cvss3();
        std::string get_cvss2();
};

#endif 