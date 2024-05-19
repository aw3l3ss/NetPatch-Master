#include <iostream>

#include "server.h"
#include "cve.h"
#include "apt_package.h"

int main() {
    Server server("92.38.255.48", "root", "2guGa0&D%1w3", 22);
    server.connect();
    server.execute_command("apt list --installed", "test.bin");
    server.disconnect();

    std::vector<AptPackage> apt_packages = get_packages("test.bin");

    int cve_counter = 0;
    
    for (int i = 0; i < apt_packages.size(); i++) {
        std::vector<Cve> cves = parse_cve(apt_packages[i]);

        if (cves.size()) {
            std::cout << apt_packages[i].get_name() << " " << apt_packages[i].get_version() << std::endl;
            for (int j = 0; j < cves.size(); j++) {
                std::cout << cves[j].get_id() << std::endl 
                          << " CVSS 4.0: " << cves[j].get_cvss4() << std::endl
                          << " CVSS 3.1: " << cves[j].get_cvss3() << std::endl
                          << " CVSS 2.0: " << cves[j].get_cvss2() << std::endl << std::endl;
            }

            cve_counter += cves.size();

            std::cout << std::endl << std::endl;
        }
    }

    std::cout << "Total count of CVEs: " << cve_counter << std::endl;

    return 0;
}