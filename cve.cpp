#include "cve.h"

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