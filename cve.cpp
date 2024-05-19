#include "cve.h"
#include "python_api.h"
#include <sstream>

#include <iostream>

Cve::Cve(std::string id, std::string summary, std::string cvss4, std::string cvss3, std::string cvss2) {
    this->id = id;
    this->summary = summary;
    this->cvss4 = cvss4;
    this->cvss3 = cvss3;
    this->cvss2 = cvss2;
}

std::string Cve::get_id() {
    return id;
}

std::string Cve::get_summary() {
    return summary;
}

std::string Cve::get_cvss4() {
    return cvss4;
}
std::string Cve::get_cvss3() {
    return cvss3;
}

std::string Cve::get_cvss2() {
    return cvss2;
}

std::vector<Cve> parse_cve(AptPackage apt_package) {
    std::vector<Cve> cves;

    std::vector<std::string> lines = execute_python_script("cve_parser.py", "-c " + apt_package.get_name() + " -v " + apt_package.get_version());

    int i = 0;
    int k = 0;

    std::string id;
    std::string summary;
    std::string cvss4;
    std::string cvss3;
    std::string cvss2;

    for (int j = 1; j < lines.size(); j++) {
        std::string line = lines[j];

        if (line == "\n") {
            continue;
        }

        if (i == 0) {
            id = line.substr(9, line.length() - 10);
        }

        else if (i == 1) {
            if (k == 0) {
                summary = line.substr(9, line.length() - 10);
                k = 1;
            }

            else {
                summary += line;
            }
        }

        else if (i == 2 && line.substr(0, 14) != "CVSS Severity:") {
            cvss4 = line.substr(6, line.length() - 7);
        }

        else if (i == 3) {
            cvss3 = line.substr(6, line.length() - 7);
        }

        else {
            cvss2 = line.substr(6, line.length() - 7);
        }

        if (i == 4) {
            cves.push_back(Cve(id, summary, cvss4, cvss3, cvss2));
            k = 0;
            i = 0;
        }

        else if (i == 1) {
            line = lines[j + 1];

            if (line.substr(0, 14) == "CVSS Severity:") {
                ++i;
            }
        }

        else {
            if (line.substr(0, 14) != "CVSS Severity:") {
                ++i;
            }
        }
    }

    return cves;
}